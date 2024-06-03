/*
 * can_queues.h
 *
 *  Created on: Apr 16, 2024
 *      Author: atcsol
 */

#ifndef CAN_QUEUES_H_
#define CAN_QUEUES_H_

#define QUEUE_MAX_SIZE 40
#define NUM_NODES 8
#define NUM_PRIORITIES 16

#include "public/leon3_occan_drv.h"

typedef struct {
	uint8_t head;
	uint8_t queued_elements;
	msg_can_t data[QUEUE_MAX_SIZE];
}can_queue_t;

typedef struct {
	can_queue_t rx_node_queue[NUM_NODES];
}rx_can_queue_t;

typedef struct {
	uint8_t head;
	uint8_t queued_elements;
	uint32_t node_id[QUEUE_MAX_SIZE];
}rx_msg_completed_queue_t;

void queue_init(can_queue_t *p_queue);
void msg_comleted_queue_init(rx_msg_completed_queue_t *p_queue);

uint8_t queue_is_full(can_queue_t *p_queue);
uint8_t queue_is_empty(can_queue_t *p_queue);
uint8_t queue_is_full_rx_msg_completed_queue(rx_msg_completed_queue_t *p_queue);
uint8_t queue_is_empty_rx_msg_completed_queue(rx_msg_completed_queue_t *p_queue);

uint8_t queue_insert_element(msg_can_t* can_msg, can_queue_t *p_queue);
uint8_t queue_extract_element(msg_can_t *can_msg, can_queue_t *p_queue);
uint8_t queue_insert_msg_completed_element(rx_msg_completed_queue_t *p_queue, uint32_t data);
uint8_t queue_extract_msg_completed_element(rx_msg_completed_queue_t *p_queue, uint32_t *p_data);

void update_queued_elements(can_queue_t *p_queue, uint16_t num);
uint8_t queue_insert_elements_without_update_queued_elements(uint32_t can_msg_id, can_queue_t *p_queue, uint8_t *p_data, uint16_t num_of_bytes, uint8_t pos);

uint8_t queue_extract_without_update_element(msg_can_t *can_msg, can_queue_t *p_queue, uint16_t pos);
void update_dequeued_elements(can_queue_t *p_queue, uint16_t num);

uint8_t queue_extract_msg_completed_without_update_elements(rx_msg_completed_queue_t *p_queue, uint32_t *p_data, uint8_t position);
void queue_msg_completed_update_elements(rx_msg_completed_queue_t *p_queue, uint16_t num);

#endif /* CAN_QUEUES_H_ */
