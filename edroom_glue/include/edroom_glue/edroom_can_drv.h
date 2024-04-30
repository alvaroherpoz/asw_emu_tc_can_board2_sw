/*
 * edroom_can_drv_config.h
 *
 *  Created on: Nov 21, 2021
 *      Author: opolo70
 */

#ifndef EDROOM_CAN_DRV_H_
#define EDROOM_CAN_DRV_H_

#include <public/config.h>
#include <public/basic_types.h>


void edroom_can_drv_config();

void edroom_can_read_status();

uint32_t edroom_can_drv_get_id(uint8_t *prio);

uint16_t edroom_can_drv_pending_msg();

int32_t edroom_can_drv_send_edroom_message(uint8_t signal, uint8_t senderPortID, uint8_t senderCmpID, uint8_t recCmpPriority, uint16_t element_size, void *p_data);

int32_t edroom_can_drv_read_edroom_message(uint8_t priority, uint8_t *p_data, uint32_t Mlength, uint8_t flush);

#endif /* edroom_can_DRV_CONFIG_H_ */
