/*
 * leon3_can.c
 *
 *  Created on: Oct 17, 2017
 *      Author: user
 */

#include "public/leon3_occan_drv.h"

#define Acceptance_Code0 Frame_Information
#define Acceptance_Code1 ID[0]
#define Acceptance_Code2 ID[1]
#define Acceptance_Code3 ID[2]
#define Acceptance_Mask0 ID[3]
#define Acceptance_Mask1 DATA[0]
#define Acceptance_Mask2 DATA[1]
#define Acceptance_Mask3 DATA[2]


struct CAN_regs
{

	volatile uint8_t Mode;

	volatile uint8_t Command;

	volatile uint8_t Status;

	volatile uint8_t Interrupt;

	volatile uint8_t Interrupt_Enable;

	volatile uint8_t Reserved1;

	volatile uint8_t BusTiming0;

	volatile uint8_t BusTiming1;

	volatile uint8_t Reserved2;

	volatile uint8_t Reserved3;

	volatile uint8_t Reserved4;

	volatile uint8_t Arbitration_Lost_Capture;

	volatile uint8_t Error_Code_Capture;

	volatile uint8_t Error_Warning_Limit;

	volatile uint8_t Rx_Error_Counter;

	volatile uint8_t Tx_Error_Counter;

	volatile uint8_t Frame_Information;

	volatile uint8_t ID[4];

	volatile uint8_t DATA[8];

	volatile uint8_t Rx_Msg_Counter;

	volatile uint8_t Reserved5;

	volatile uint8_t Clock_Divider;

};

struct CAN_regs * pLEON3_CAN_REGS= (struct CAN_regs *) 0xFFFC0000;


void leon3_can_config_prologue(board_t placa)
{
	pLEON3_CAN_REGS->Mode |=(1<<0);//Enter Reset Mode
	pLEON3_CAN_REGS->Mode &=~(0xE);//Dual filter and disabled another mode

	//Configurar velocidad a 1 Mbps
	pLEON3_CAN_REGS->Clock_Divider |= (1<<7);//PeliCAN mode

	 //cambiar a switch case
	if(placa == NEXYS){
		pLEON3_CAN_REGS->BusTiming0 |= 6;//BRP=6
	}
	if(placa == A3P){
		pLEON3_CAN_REGS->BusTiming0 |= 1;//BRP=1
	}

	pLEON3_CAN_REGS->BusTiming0 &=(0xC6);
	pLEON3_CAN_REGS->BusTiming1 |=(1<<0)|(1<<4);//TSEG1=TSEG2=1
	pLEON3_CAN_REGS->BusTiming1 &= (0x91);

	pLEON3_CAN_REGS->Acceptance_Code0=0xFF;//Configure filter
	pLEON3_CAN_REGS->Acceptance_Code1=0xFF;
	pLEON3_CAN_REGS->Acceptance_Code2=0xFF;
	pLEON3_CAN_REGS->Acceptance_Code3=0xFF;
	pLEON3_CAN_REGS->Acceptance_Mask0=0xFF;
	pLEON3_CAN_REGS->Acceptance_Mask1=0xFF;
	pLEON3_CAN_REGS->Acceptance_Mask2=0xFF;
	pLEON3_CAN_REGS->Acceptance_Mask3=0xFF;
	pLEON3_CAN_REGS->Interrupt_Enable &= ~(0xEF); //Disable another interrupt
}

uint8_t leon3_can_get_irq_status(void){
	return (pLEON3_CAN_REGS->Interrupt);
}

void leon3_can_tx_irq_enable(void){
	pLEON3_CAN_REGS->Interrupt_Enable |= (1<<1);  // (1<<1) CAN TX interrupt enable -> bit 1
}

void leon3_can_tx_irq_disable(void){
	pLEON3_CAN_REGS->Interrupt_Enable &= (0xED);  // (1<<1) CAN TX interrupt disable
}

void leon3_can_irq_mask(void){
	leon3_mask_irq(CAN_IRQ_LVL);
}

void leon3_can_irq_unmask(void){
	leon3_unmask_irq(CAN_IRQ_LVL);
}

void leon3_can_rx_irq_enable(void){
	pLEON3_CAN_REGS->Interrupt_Enable |= 1; // CAN RX interrupt enable -> bit 0
}

void leon3_can_rx_irq_disable(void){
	pLEON3_CAN_REGS->Interrupt_Enable &= (0xEE);  // (1<<1) CAN RX interrupt disable
}

void leon3_can_config_epilogue(void){
	pLEON3_CAN_REGS->Mode &= ~(1<<0);//Operating mode
	pLEON3_CAN_REGS->Command |=(1<<2)|(1<<3);//Free received buffer for new reception and clear OV bit
}

void leon3_can_clear_overrun(void){
	pLEON3_CAN_REGS->Command |=(1<<3);//clear OV bit
}

void leon3_can_abort_transmision(void){
	pLEON3_CAN_REGS->Command |= (1<<1);//abort transmision bit
}

uint8_t leon3_can_status_transmitting_msg(void){
	return ((pLEON3_CAN_REGS->Status >> 5) & 0x01);//transmitting msg bit
}

uint8_t leon3_can_status_last_msg_transferred(void){
	return ((pLEON3_CAN_REGS->Status >> 3) & 0x01);//last msg successfullyt ransferred
}


uint8_t leon3_send_message(uint8_t ID[4], uint8_t RTR, uint8_t DLC, uint8_t data[8])
{
	uint32_t write_timeout=0;
	uint8_t i;
	while(((pLEON3_CAN_REGS->Status & (1<<2))==0) && (write_timeout < 0xAAAAA)){
		write_timeout++;//Wait until bit Transmit buffer status set
	}
	if(write_timeout < 0xAAAAA)//Bit set
	{
		if(DLC>8) DLC=8;//DLCmax=8

		pLEON3_CAN_REGS->Frame_Information = (1<<7) + ((RTR & 0x1)<<6) + (DLC & 0xF);//EFF+RTR+DLC

		pLEON3_CAN_REGS->ID[0] = ID[0];//Frame ID
		pLEON3_CAN_REGS->ID[1] = ID[1];
		pLEON3_CAN_REGS->ID[2] = ID[2];
		pLEON3_CAN_REGS->ID[3] = ID[3];

		for(i=0; i<DLC; i++)
		{
			pLEON3_CAN_REGS->DATA[i]=data[i];//Frame DATA
		}
		pLEON3_CAN_REGS->Command |=(1<<0);//Transmit
	}
	return (write_timeout == 0xAAAAA);
}


uint8_t leon3_get_message(msg_can_t *Msg){
	uint32_t read_timeout=0, condition=0;
		uint8_t i, FI;
		do{
			read_timeout++;
			condition=pLEON3_CAN_REGS->Status & (1<<0);//Wait until Receive buffer status set
		}while((condition==0) && (read_timeout < 0xAAAAA));
		if(read_timeout < 0xAAAAA)//Bit set
		{

			FI = pLEON3_CAN_REGS->Frame_Information;//Frame information

			Msg->RTR = (FI>>6) & 0x1;//RTR
			Msg->DLC = FI & 0xF;//DLC

			if(((FI>>7) & 0x1))//Extended Frame Format
			{
				Msg->id[0] = pLEON3_CAN_REGS->ID[0];
				Msg->id[1] = pLEON3_CAN_REGS->ID[1];
				Msg->id[2] = pLEON3_CAN_REGS->ID[2];
				Msg->id[3] = (pLEON3_CAN_REGS->ID[3] & 0xF8);

				if(Msg->DLC>8) Msg->DLC=8;//DLCmax=8
				for(i=0; i<Msg->DLC; i++)
				{
					Msg->msg[i]=pLEON3_CAN_REGS->DATA[i];//Frame DATA
				}
			}
			else//Standard Frame Format
			{
				Msg->id[0] = pLEON3_CAN_REGS->ID[0];
				Msg->id[1] = (pLEON3_CAN_REGS->ID[1] &0x7);//Frame ID

				Msg->msg[0]=pLEON3_CAN_REGS->ID[2];//First data byte
				if(Msg->DLC>=2) Msg->msg[1]=pLEON3_CAN_REGS->ID[3];//Second data byte
				for(i=2; i<Msg->DLC; i++)//Rest of data bytes
				{
					Msg->msg[i]=pLEON3_CAN_REGS->DATA[i-2];
				}
			}
			pLEON3_CAN_REGS->Command |=(1<<2);//Free receive buffer

			return 0;
		}
		return 0xFF;

}
/*
uint8_t leon3_canGetMessage(Can_Message_t *Msg)
{
	uint32_t read_timeout=0, condition=0;
	uint8_t i, FI;
	do{
		read_timeout++;
		condition=pLEON3_CAN_REGS->Status & (1<<0);//Wait until Receive buffer status set
	}while((condition==0) && (read_timeout < 0xAAAAA));
	if(read_timeout < 0xAAAAA)//Bit set
	{
		FI = pLEON3_CAN_REGS->Frame_Information;//Frame information
		Msg->RTR = (FI>>6) & 0x1;//RTR
		Msg->DLC = FI & 0xF;//DLC
		if(((FI>>7) & 0x1))//Extended Frame Format
		{
			Msg->ID = (pLEON3_CAN_REGS->ID[0]<<21)+(pLEON3_CAN_REGS->ID[1]<<13)+(pLEON3_CAN_REGS->ID[2]<<5)+((pLEON3_CAN_REGS->ID[3] & 0xF8)>>3);//Frame ID
			if(Msg->DLC>8) Msg->DLC=8;//DLCmax=8
			for(i=0; i<Msg->DLC; i++)
			{
				Msg->Data[i]=pLEON3_CAN_REGS->DATA[i];//Frame DATA
			}
		}
		else//Standard Frame Format
		{
			Msg->ID = (pLEON3_CAN_REGS->ID[0]<<3)+((pLEON3_CAN_REGS->ID[1]>>5)&0x7);//Frame ID
			Msg->Data[0]=pLEON3_CAN_REGS->ID[2];//First data byte
			if(Msg->DLC>=2) Msg->Data[1]=pLEON3_CAN_REGS->ID[3];//Second data byte
			for(i=2; i<Msg->DLC; i++)//Rest of data bytes
			{
				Msg->Data[i]=pLEON3_CAN_REGS->DATA[i-2];
			}
		}
		pLEON3_CAN_REGS->Command |=(1<<2);//Free receive buffer

		return 0;
	}
	return 0xFF;
}*/
