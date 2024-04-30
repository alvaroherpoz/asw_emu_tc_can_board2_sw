#ifndef LEON3_OCCAN_DRV_H_
#define LEON3_OCCAN_DRV_H_

#include "public/basic_types.h"

#ifdef __cplusplus

extern "C" {

#endif

/*
typedef struct
{
	volatile uint32_t ID;
	volatile uint8_t DLC;
	volatile uint8_t RTR;
	volatile uint8_t Data[8];
}Can_Message_t;
*/

//	Crear un tipo estructurado para msg_can que tenga id y msg
typedef struct {
	uint8_t RTR;
	uint8_t DLC;
	uint8_t id[4];
	uint8_t msg[8];
}msg_can_t;

typedef enum {A3P, NEXYS} board_t;

#define CAN_IRQ_LVL 13

void leon3_can_config_prologue(board_t);
void leon3_can_config_epilogue(void);

/*uint8_t leon3_canSendMessage(uint32_t ID, uint8_t RTR, uint8_t DLC, uint8_t *data);
uint8_t leon3_canGetMessage(Can_Message_t *Msg);*/

uint8_t leon3_send_message(uint8_t ID[4], uint8_t RTR, uint8_t DLC, uint8_t data[8]);
uint8_t leon3_get_message(msg_can_t *Msg);

uint8_t leon3_can_get_irq_status(void);
void leon3_can_clear_overrun(void);
void leon3_can_irq_mask(void);
void leon3_can_irq_unmask(void);
void leon3_can_tx_irq_enable(void);
void leon3_can_rx_irq_enable(void);
void leon3_can_abort_transmision(void);
void leon3_can_abort_transmision(void);
uint8_t leon3_can_status_transmitting_msg(void);
uint8_t leon3_can_status_last_msg_transferred(void);
#ifdef __cplusplus

}

#endif

#endif

