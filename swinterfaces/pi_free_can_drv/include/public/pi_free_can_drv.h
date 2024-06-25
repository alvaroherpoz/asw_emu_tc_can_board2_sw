/*
 * leon3_can.c
 *
 *  Created on: Mar 12, 2024
 *      Author: Alvaro
 */
#ifndef PI_FREE_CAN_DRV_H_
#define PI_FREE_CAN_DRV_H_

#include "public/basic_types.h"
#include "public/can_queues.h"

#ifdef __cplusplus

extern "C" {

#endif


#define FIRST_PRIO 0

typedef struct {
	//previous and next node
	uint8_t prev;
	uint8_t next;
}dl_list_node;

uint8_t pi_free_can_drv_insert_tx_prio(uint8_t prio);
void pi_free_can_drv_extract_tx_prio(uint8_t prio);
uint8_t pi_free_can_drv_insert_rx_prio(uint8_t prio);
void pi_free_can_drv_extract_rx_prio(uint8_t prio);

uint16_t pi_free_can_drv_tx_queue_free_bytes(can_queue_t * p_queue);
int32_t pi_free_can_drv_send_message(uint32_t ID, uint16_t DLC, uint8_t Msg[], void (pLock()), void (pUnlock()));

void pi_free_can_drv_config(void);

int32_t pi_free_send_message(uint32_t ID, uint16_t DLC, uint8_t Msg[], void (pLock()), void (pUnlock()));

int32_t pi_free_can_drv_read_message(uint8_t priority, uint16_t Mlength, uint8_t* p_data, uint16_t* DLC, uint32_t* ID, uint8_t flush, void (pLock()), void (pUnlock()));

uint32_t pi_free_get_canmsg_id(uint8_t* prio);

uint16_t pi_free_rx_status(void);

uint8_t pi_free_can_irq_handler(void);

#ifdef __cplusplus

}

#endif

#endif

