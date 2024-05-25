/*
 * edroom_can_buffers.cpp
 *
 *  Created on: Nov 21, 2021
 *      Author: opolo70
 */

#include <stdio.h>

#include <public/basic_types.h>

	 //declare CAN_driver functions
#include "public/leon3_occan_drv.h"
#include "public/pi_free_can_drv.h"

#include "public/edroombp.h"

Pr_Mutex mutex;

void Lock(){
	mutex.Wait();
}
void Unlock(){
	mutex.Signal();
}


void edroom_can_drv_config(){

	leon3_occan_drv_config_prologue(NEXYS);
	leon3_occan_drv_rx_irq_enable();
	leon3_occan_drv_tx_irq_enable();
	leon3_occan_drv_config_epilogue();
	pi_free_can_drv_config();

}


void edroom_can_drv_read_status(){

	return;
}
uint32_t edroom_can_drv_get_id(uint8_t *prio)
{
	uint8_t prio_aux;
	uint32_t ID = pi_free_get_canmsg_id(&prio_aux);
	*prio = prio_aux;
	return ID;
}

uint16_t edroom_can_drv_pending_msg()
{
	//todo
	return pi_free_rx_status();
}
//int read_message(uint8_t priority, uint16_t Mlength, uint8_t Msg[], uint16_t* DLC, uint32_t* ID, void (pLock()), void (pUnlock()));

int32_t edroom_can_drv_read_edroom_message(uint8_t priority, uint8_t *p_data, uint32_t Mlength, uint8_t flush)
{
	uint16_t DLC;
	uint32_t ID;
	//todo
	int32_t read=pi_free_can_drv_read_message(priority, Mlength, p_data, &DLC, &ID, flush, Lock, Unlock);
	return read;
}

int32_t edroom_can_drv_send_edroom_message(uint8_t signal, uint8_t senderPortID, uint8_t senderCmpID, uint8_t recCmpPriority, uint16_t element_size, void *p_data){

	uint32_t ID;

		   //CAN MSG
	ID = (uint32_t)signal;
	ID |= senderPortID << 10;
	ID |= senderCmpID << 16;
	   //ID[24:23] << lo controla la función send_message
	ID |= recCmpPriority << 25;
	   //DLC = EDROOMPoolCDTCDescriptor.ElementSize;
	   //ID[28:25] =aux->mp_ReceiverCmp->m_NomPriority;
	   //ID[24:23] tipo de mensaje
	   //ID[22:18] 7 bits aux->mp_SenderCmp->m_ComponentID;
	   //ID[17:13] 6 bits aux->mp_SenderPort->m_IdInterface;
	   //ID[12:0] 10 bits SHK_FDIR_TC
	//printf("FFwdHK_FDIRTC: ID: %d\n", ID);
	//printf("FFwdHK_FDIRTC: DLC: %d\n", element_size);
	   //printf("FFwdHK_FDIRTC: Msg: %d\n", *(byte_t *) p_data);


	   //pSHK_FDIR_TC_Data es un puntero a los datos;
	   //EDROOMPoolCDTCDescriptor.ElementSize te sirve para saber cuantos bytes tienes de datos;
	   //uint8_t sent = send_message(ID, DLC, Msg, Lock, Unlock);


	//todo
	//int32_t sent = send_message(ID, element_size, (byte_t *) p_data, DumiLock, DumiUnlock);
	int32_t sent = pi_free_can_drv_send_message(ID, element_size, (byte_t *) p_data, Lock, Unlock);
	return sent;
	//printf("FFwdHK_FDIRTC: Sent: %d\n", sent);


}

