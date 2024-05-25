
#include <public/can_queues.h>
	//Funciones de colas

void queue_init(can_queue_t *p_queue) {
	if (p_queue) { //Check pointer is valid (!=0)
		p_queue->head = 0;
		p_queue->queued_elements = 0;
	}
}

void msg_comleted_queue_init(rx_msg_completed_queue_t *p_queue) {
	if (p_queue) { //Check pointer is valid (!=0)
		p_queue->head = 0;
		p_queue->queued_elements = 0;
	}
}

uint8_t queue_is_full(can_queue_t *p_queue) {
	uint8_t is_full = 0;
	if (p_queue) //Check pointer is valid (!=0)
		is_full = (p_queue->queued_elements == QUEUE_MAX_SIZE);
	return is_full;
}

uint8_t queue_is_empty(can_queue_t *p_queue) {
	uint8_t is_empty = 0;
	if (p_queue) //Check pointer is valid (!=0)
		is_empty = (p_queue->queued_elements == 0);
	return is_empty;
}

uint8_t queue_is_empty_rx_msg_completed_queue(rx_msg_completed_queue_t *p_queue) {
	uint8_t is_empty = 0;
	if (p_queue) //Check pointer is valid (!=0)
		is_empty = (p_queue->queued_elements == 0);
	return is_empty;
}

uint8_t queue_is_full_rx_msg_completed_queue(rx_msg_completed_queue_t *p_queue) {
	uint8_t is_full = 0;
	if (p_queue) //Check pointer is valid (!=0)
		is_full = (p_queue->queued_elements == QUEUE_MAX_SIZE);
	return is_full;
}

uint8_t queue_insert_element(msg_can_t* can_msg, can_queue_t *p_queue) {
	uint8_t error = 1;

		if (p_queue && (!queue_is_full(p_queue))) {
			int i;
			uint8_t next_tail = (p_queue->head + p_queue->queued_elements) % QUEUE_MAX_SIZE;

			//Guardamos el ID que son 29 bits
			p_queue->data[next_tail+1].id[0] = can_msg->id[0];
			p_queue->data[next_tail+1].id[1] = can_msg->id[1];
			p_queue->data[next_tail+1].id[2] = can_msg->id[2];
			p_queue->data[next_tail+1].id[3] = can_msg->id[3];

			//Guardamos los datos del mensaje
			for (i=0; i < can_msg->DLC; i++){
				p_queue->data[next_tail+1].msg[i] = can_msg->msg[i];
			}
			//Guardamos el tamaño del mensaje en bytes
			p_queue->data[next_tail+1].DLC  = can_msg->DLC;

			p_queue->data[next_tail+1].RTR = 0;

			p_queue->queued_elements += 1;
			error = 0; //No error
		}
		return error;
}

void update_queued_elements(can_queue_t *p_queue, uint16_t num){
	p_queue->queued_elements += num;
}

uint8_t queue_insert_elements_without_update_queued_elements(uint32_t can_msg_id, can_queue_t *p_queue, uint8_t *p_data, uint16_t DLC, uint8_t pos) {
	uint8_t error = 1;

	if (p_queue && (!queue_is_full(p_queue))) {
		int i;
		uint8_t next_tail = (p_queue->head + p_queue->queued_elements + pos) % QUEUE_MAX_SIZE;

		//Guardamos el ID que son 29 bits
		p_queue->data[next_tail+1].id[0] = (can_msg_id >> 21) & 0xFF;
		p_queue->data[next_tail+1].id[1] = (can_msg_id >> 13) & 0xFF;
		p_queue->data[next_tail+1].id[2] = (can_msg_id >> 5) & 0xFF;
		p_queue->data[next_tail+1].id[3] = ((can_msg_id & 0x1F) << 3);

		//Guardamos los datos del mensaje
		for (i=0; i < DLC; i++){
			p_queue->data[next_tail+1].msg[i] = p_data[i+8*pos];
		}
		//Guardamos el tamaño del mensaje en bytes
		p_queue->data[next_tail+1].DLC  = DLC;

		p_queue->data[next_tail+1].RTR = 0;

		error = 0; //No error
	}
	return error;
}

//Revisar esto para las colas de transmision, se hará de diferente forma
uint8_t queue_extract_element(msg_can_t *can_msg, can_queue_t *p_queue) {
	uint8_t error = 1;
	//Check pointers are valid && queue is not empty
	if (can_msg && p_queue && (!queue_is_empty(p_queue))) {
		int i;
		for (i = 0; i <= 3; i++) {
			can_msg->id[i] = p_queue->data[p_queue->head].id[i];
		}
		for (i = 0; i <= 7; i++) {
			can_msg->msg[i] = p_queue->data[p_queue->head].msg[i];
		}
		can_msg->DLC = p_queue->data[p_queue->head].DLC;
		can_msg->RTR = p_queue->data[p_queue->head].RTR;

		p_queue->head = (p_queue->head + 1) % QUEUE_MAX_SIZE;
		p_queue->queued_elements--;
		error = 0; //No error
	}
	return error;
}

uint8_t queue_extract_without_update_element(msg_can_t *can_msg, can_queue_t *p_queue, uint16_t pos) {
	uint8_t error = 1;
	//Check pointers are valid && queue is not empty
	if (can_msg && p_queue && (!queue_is_empty(p_queue))) {
		int i;
		uint16_t position = p_queue->head + pos;
		for (i = 0; i <= 3; i++) {
			can_msg->id[i] = p_queue->data[position].id[i];
		}
		for (i = 0; i <= 7; i++) {
			can_msg->msg[i] = p_queue->data[position].msg[i];
		}
		can_msg->DLC = p_queue->data[position].DLC;
		can_msg->RTR = p_queue->data[position].RTR;

		error = 0; //No error
	}
	return error;
}

void update_dequeued_elements(can_queue_t *p_queue, uint16_t num){
	p_queue->head = (p_queue->head + num) % QUEUE_MAX_SIZE;
	p_queue->queued_elements -= num;
}

//Para msg de rx completos

uint8_t queue_insert_msg_completed_element(rx_msg_completed_queue_t *p_queue, uint8_t data) {
	uint8_t error = 1;
	//Check pointer is valid && queue is not full
	if (p_queue && (!queue_is_full_rx_msg_completed_queue(p_queue))) {
		uint8_t next_tail = (p_queue->head + p_queue->queued_elements) % QUEUE_MAX_SIZE;
		p_queue->node_id[next_tail] = data;
		p_queue->queued_elements++;
		error = 0; //No error
	}
	return error;
}

uint8_t queue_extract_msg_completed_element(rx_msg_completed_queue_t *p_queue, uint8_t *p_data) {
	uint8_t error = 1;
	//Check pointers are valid && queue is not empty
	if (p_data && p_queue && (!queue_is_empty_rx_msg_completed_queue(p_queue))) {
		*p_data = p_queue->node_id[p_queue->head];
		p_queue->head = (p_queue->head + 1) % QUEUE_MAX_SIZE;
		p_queue->queued_elements--;
		error = 0; //No error
	}
	return error;
}

//Esta funcion se utiliza para ver el tamaño del mensaje que vamos a leer, sin actualizar el tamaño de la cola
uint8_t queue_extract_msg_completed_without_update_elements(rx_msg_completed_queue_t *p_queue, uint32_t *p_data, uint8_t position) {
	uint8_t error = 1;
	uint8_t element_position = (p_queue->head + position) % QUEUE_MAX_SIZE;
	//Check pointers are valid && queue is not empty
	if (p_data && p_queue && (!queue_is_empty_rx_msg_completed_queue(p_queue))) {
		*p_data = p_queue->node_id[element_position];
		error = 0; //No error
	}
	return error;
}
void queue_msg_completed_update_elements(rx_msg_completed_queue_t *p_queue, uint16_t num){
	p_queue->head = (p_queue->head + num) % QUEUE_MAX_SIZE;
	p_queue->queued_elements-=num;
}

