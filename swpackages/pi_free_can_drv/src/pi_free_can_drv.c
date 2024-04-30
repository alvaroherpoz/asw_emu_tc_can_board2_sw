/*
 * leon3_can.c
 *
 *  Created on: Mar 12, 2024
 *      Author: Alvaro
 */

#include <public/leon3_hw_irqs.h>
#include <public/can_queues.h>
#include <public/pi_free_can_drv.h>
#include <public/leon3_occan_drv.h>

can_queue_t tx_prio_queues[NUM_PRIORITIES];
rx_can_queue_t rx_prio_queues[NUM_PRIORITIES];
rx_msg_completed_queue_t rx_msg_completed[NUM_PRIORITIES];

dl_list_node next_tx_queue[17]; // El primer elemento es el que indica cual es la cola con mas prioridad que hay para transmitir, el resto contienen la cola siguiente con menor prioridad a transmitir
dl_list_node next_rx_queue[17]; // El primer elemento es el que indica cual es la cola con mas prioridad que hay para recibir, el resto contienen la cola siguiente con menor prioridad a recibir

void pi_free_can_drv_config(void) {
	int i;
	//Inicializamos la lista doblemente enlazada de "siguientes colas", el primer elemento corresponderá siempre a la cola con mayor prioridad
	next_rx_queue[0].next=NULL;
	next_rx_queue[0].prev=NULL;

	next_tx_queue[0].next=NULL;
	next_tx_queue[0].prev=NULL;

	//Inicializamos las colas
	for (i = 0; i < 16; i++) {
		int j;
		//Inicializamos la lista doblemente enlazada de "siguientes colas"
		next_rx_queue[i+1].next=NULL;
		next_rx_queue[i+1].prev=NULL;

		next_tx_queue[i+1].next=NULL;
		next_tx_queue[i+1].prev=NULL;

		//Colas tx
		queue_init(&tx_prio_queues[i]);

		//Colas rx de mensajes completos
		msg_comleted_queue_init(&rx_msg_completed[i]);

		//Colas rx
		for (j = 0; j < NUM_NODES; j++) {
			queue_init(&rx_prio_queues[i].rx_node_queue[j]);
		}
	}

}

	//Funciones Listas doblemente enlazadas

uint8_t pi_free_can_drv_insert_tx_prio(uint8_t prio){
	uint8_t same_prio = 0;
	if(next_tx_queue[FIRST_PRIO].next == 0){
		next_tx_queue[FIRST_PRIO].next = prio;
	}
	else{
		uint8_t next;
		next = next_tx_queue[FIRST_PRIO].next;
		while(prio > next){
			next = next_tx_queue[next].next;
		}
		if(prio == next){
			same_prio = 1;
		}
		else{
			next_tx_queue[prio].next = next; // El siguiente de la prioridad es el siguiente que hemos encontrado
			next_tx_queue[prio].prev = next_tx_queue[next].prev; // El anterior de la prioridad es el anterior del siguiente
			next_tx_queue[next_tx_queue[prio].prev].next = prio; // El siguiente del anterior ahora es la nueva prioridad
			next_tx_queue[next].prev = prio; //el anterior del siguiente ahora es la nueva prioridad
		}

	}
	return same_prio;
}

void pi_free_can_drv_extract_tx_prio(uint8_t prio){
	next_tx_queue[next_tx_queue[prio].prev].next = next_tx_queue[prio].next;
	next_tx_queue[next_tx_queue[prio].next].prev = next_tx_queue[prio].prev;
	next_tx_queue[prio].next = 0;
	next_tx_queue[prio].prev = 0;
}

uint8_t pi_free_can_drv_insert_rx_prio(uint8_t prio){
	uint8_t same_prio = 0;
	if(next_rx_queue[FIRST_PRIO].next == 0){
		next_rx_queue[FIRST_PRIO].next = prio;
	}
	else{
		uint8_t next;
		next = next_rx_queue[FIRST_PRIO].next;
		while(prio > next){
			next = next_rx_queue[next].next;
		}
		if(prio == next){
			same_prio = 1;
		}
		else{
			next_rx_queue[prio].next = next; // El siguiente de la prioridad es el siguiente que hemos encontrado
			next_rx_queue[prio].prev = next_rx_queue[next].prev; // El anterior de la prioridad es el anterior del siguiente
			next_rx_queue[next_rx_queue[prio].prev].next= prio; // El siguiente del anterior ahora es la nueva prioridad
			next_rx_queue[next].prev = prio; //el anterior del siguiente ahora es la nueva prioridad
		}

	}
	return same_prio;
}

void pi_free_can_drv_extract_rx_prio(uint8_t prio){
	next_rx_queue[next_rx_queue[prio].prev].next = next_rx_queue[prio].next;
	next_rx_queue[next_rx_queue[prio].next].prev = next_rx_queue[prio].prev;
	next_rx_queue[prio].next = 0;
	next_rx_queue[prio].prev = 0;
}

	//Fin listas doblemente enlazadas


uint16_t pi_free_can_drv_tx_queue_free_bytes(can_queue_t * p_queue){
	return (QUEUE_MAX_SIZE - p_queue->queued_elements)*4;
}

int32_t pi_free_can_drv_send_message(uint32_t ID, uint16_t DLC, uint8_t Msg[], void (pLock()), void (pUnlock()))
{
	uint32_t to_return = 2;
	uint32_t aux_ID = ID & 0x007FFFFF;
	uint32_t ID_msg;
	uint32_t DLC_msg;
	uint16_t pending_Bytes = DLC;
	//Cogemos la prioridad
	uint8_t priority = (ID & 0x1E000000)>>25;
	uint8_t flag = 1;
	uint16_t num_msg_queued = 0;

	//Miramos si hay hueco en la cola para el mensaje
	uint32_t Out_FreeBytes = pi_free_can_drv_tx_queue_free_bytes(&tx_prio_queues[priority]);

	if ((pLock != NULL) && (pUnlock != NULL)) {
		//Mirar si hay hueco en la cola de la prioridad y si hay metemos en la cola el mensaje en bloques de 8 bytes
		if (Out_FreeBytes < DLC) {
			to_return = 0;
		} else {
			if (pending_Bytes > 8) // Primer mensaje
					{
				ID_msg = aux_ID | 0x11800000;
				DLC_msg = 8; //8 bytes de DLC

				queue_insert_elements_without_update_queued_elements(ID_msg,
						&tx_prio_queues[priority], Msg, DLC_msg);

				num_msg_queued += 1;

				pending_Bytes = pending_Bytes - 8;

				flag = 0;
			}
			while (pending_Bytes > 8) {
				ID_msg = aux_ID | 1 << 28 | 1 << 23; //Mensajes intermedios

				queue_insert_elements_without_update_queued_elements(ID_msg,
						&tx_prio_queues[priority], Msg, DLC_msg);

				num_msg_queued += 1;

				pending_Bytes = pending_Bytes - 8;
			}

			ID_msg = aux_ID | flag << 24 | pending_Bytes << 25; // Ultimo mensaje

			queue_insert_elements_without_update_queued_elements(ID_msg,
					&tx_prio_queues[priority], Msg, pending_Bytes);

			num_msg_queued += 1;

			//ENmascaramos la riq del can para actualizar los elementos de la cola
			leon3_can_irq_mask();

			update_queued_elements(&tx_prio_queues[priority], num_msg_queued);

			//Actualizamos la estructura global
			pLock();
			pi_free_can_drv_insert_tx_prio(priority);
			pUnlock();

			//desenmascaramos interrupcion del can
			leon3_can_irq_unmask();

			to_return = 1;

		}
	} else {
		to_return = 0;
	}

	//Forzar interrupcion para enviar ??

	return to_return;
}

int32_t pi_free_can_drv_read_message(uint8_t priority, uint16_t Mlength, uint8_t Msg[], uint16_t* DLC, uint32_t* ID, uint8_t flush, void (pLock()), void (pUnlock()))
{
	int32_t to_return = -3;

	if((pLock!=NULL) && (pUnlock!=NULL))
	{
		uint8_t type;
		uint32_t aux_ID;
		uint16_t aux_DLC=0;
		uint16_t num_can_msg_read=0;
		msg_can_t can_msg;

		//Extraemos ID del mensaje completo
		queue_extract_msg_completed_without_update_elements(&rx_msg_completed[priority], &aux_ID, 0);

		//Cogemos de la cabecera ID el id del componente
		uint8_t senderComponentID = (aux_ID >> 16) & 0xFF;

		//Ahora, cogemos el primer mensaje de esa prioridad
		//cogemos de la cola de mensajes RX del componente y su prioridad el ID
		queue_extract_without_update_element(&can_msg, &rx_prio_queues[priority].rx_node_queue[senderComponentID],num_can_msg_read);

		//Cogemos el ID del primer mensaje y el DLC
		//Miramos DLC
		aux_ID = (can_msg.id[0] << 21) & 0xFF;
		aux_ID = (can_msg.id[1] << 13) & 0xFF;
		aux_ID = (can_msg.id[2] << 5) & 0xFF;
		aux_ID = ((can_msg.id[3] << 3)& 0x1F);

		aux_DLC = can_msg.DLC;

		//Cogemos el tipo del mensaje(primero de muchos, o único)
		type = (aux_ID & 0x01800000)>>23;

		num_can_msg_read++;

		//Si es primero de muchos, miramos tamaño de mensaje
		while(type%2==1)
		{
			queue_extract_without_update_element(&can_msg, &rx_prio_queues[priority].rx_node_queue[senderComponentID],num_can_msg_read);
			aux_ID = (can_msg.id[0] << 21) & 0xFF;
			aux_ID = (can_msg.id[1] << 13) & 0xFF;
			aux_ID = (can_msg.id[2] << 5) & 0xFF;
			aux_ID = ((can_msg.id[3] << 3)& 0x1F);
			aux_DLC = aux_DLC + can_msg.DLC;
			type = (aux_ID & 0x01800000)>>23;
			num_can_msg_read++;
		}
		//Guardamos el DLC total y el ID del componente
		*DLC = aux_DLC;
		*ID = (aux_ID & 0x007FFFFF) | priority<<25;
		//Miramos si concuerdan el tamaño a leer y el tamaño del mensaje que debería tener
		if (aux_DLC != Mlength)
		{
			if (flush != 0)
			{
				//extract the erroneous message
				//Actualizar la colas eliminando mensaje corrupto
				//Borramos el ID del mensaje completo de las colas de siguiente mensaje
				queue_msg_completed_update_elements(&rx_msg_completed[priority], 1);

				//Eliminamos todos los mensajes de ese mensaje completo de las colas rx
				update_dequeued_elements(&rx_prio_queues[priority].rx_node_queue[senderComponentID],num_can_msg_read);

				//Mirar si está vacía la prioridad para quitar la prioridad como siguiente cola a manejar
				if(queue_is_empty_rx_msg_completed_queue(&rx_msg_completed[priority]))
				{
					pi_free_can_drv_extract_rx_prio(priority);
				}

			}
			if (aux_DLC>Mlength)
			//The length of the message is higher
				to_return = -1;
			else
			//The length of the message is lower
				to_return = -2;

		}
		else{
			uint16_t pending_Bytes = aux_DLC;
			uint16_t msg_position = 0;
			num_can_msg_read = 0;
			int i;
			while (pending_Bytes > 8)
			{
				queue_extract_without_update_element(&can_msg, &rx_prio_queues[priority].rx_node_queue[senderComponentID],num_can_msg_read);
				//Cogemos todas las partes del mensaje
				for(i = 0; i < can_msg.DLC ; i++){
					Msg[i+msg_position] = can_msg.msg[i];
				}
				num_can_msg_read++;
				msg_position += 8;
				pending_Bytes -= 8;

			}
			//extraccion del ultimo (o unico) mensaje
			queue_extract_without_update_element(&can_msg, &rx_prio_queues[priority].rx_node_queue[senderComponentID],num_can_msg_read);

			//Cogemos todas las partes del mensaje
			for(i = 0; i < can_msg.DLC ; i++){
				Msg[i+msg_position] = can_msg.msg[i];
			}
			num_can_msg_read++;

			//Actualizamos las colas
			//Borramos el ID del mensaje completo de las colas de siguiente mensaje
			queue_msg_completed_update_elements(&rx_msg_completed[priority], 1);

			//Eliminamos todos los mensajes de ese mensaje completo de las colas rx
			update_dequeued_elements(&rx_prio_queues[priority].rx_node_queue[senderComponentID], num_can_msg_read);
			//Mirar si está vacía la prioridad para quitar la prioridad como siguiente cola a manejar
			if(queue_is_empty_rx_msg_completed_queue(&rx_msg_completed[priority]))
			{
				pi_free_can_drv_extract_rx_prio(priority);
			}
			to_return = (int32_t) aux_DLC;
		}
	}
	return to_return;
}
uint32_t pi_free_get_canmsg_id(uint8_t *prio){
//gets the id of the most prioritary msg available
	//and the priority of the msg

	uint8_t i;
	uint32_t id;
	i = next_rx_queue[0].next; // cogemos la prioridad mas alta que tenga mensajes disponibles completos
	*prio = i;

	queue_extract_msg_completed_without_update_elements(&rx_msg_completed[i], &id, 0);
	return id;
}

uint16_t pi_free_rx_status(){

	uint16_t rx_status = 0;
	int j;

	for(j = 15; j>=0 ; j--){
		if(rx_msg_completed[j].queued_elements > 0){
			rx_status |= 1<<j;
		}
		else{
			rx_status |= 0<<j;
		}
	}
	return rx_status;
}
