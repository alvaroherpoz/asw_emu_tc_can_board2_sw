//##############################################################################
//###############     This file has been generated by EDROOM     ###############
//##############################################################################
//broker2
 
#include <edroom_glue/edroomdeployment.h>


//*****************************************************************************
//Main Wait
 
void MainWait(RICUASW   &ricuasw,
		RCCEPDManager   &rccepdmanager,
		RCCTM_ChannelCtrl   &rcctm_channelctrl,
		CCHK_FDIRMng   &cchk_fdirmng,
		CCBKGTCExec   &ccbkgtcexec){
 
	Pr_Time waitTime(3, 0);
 
#ifdef _EDROOM_SYSTEM_CLOSE
	while(!ricuasw.EDROOMIsComponentFinished()
				||!rccepdmanager.EDROOMIsComponentFinished()
				||!rcctm_channelctrl.EDROOMIsComponentFinished()
				||!cchk_fdirmng.EDROOMIsComponentFinished()
				||!ccbkgtcexec.EDROOMIsComponentFinished())
#else
	while(true)
#endif

 
		Pr_DelayIn(waitTime);
}
 
//*****************************************************************************
//SetMemory

void CEDROOMSystemMemory::SetMemory(){
 
	ricuaswMemory.SetMemory(10, ricuaswMessages, &ricuaswMessagesMarks[0]
					,13,ricuaswQueueNodes, &ricuaswQueueNodesMarks[0]);
	rccepdmanagerMemory.SetMemory(10, rccepdmanagerMessages, &rccepdmanagerMessagesMarks[0]
					,11,rccepdmanagerQueueNodes, &rccepdmanagerQueueNodesMarks[0]);
	rcctm_channelctrlMemory.SetMemory(10, rcctm_channelctrlMessages, &rcctm_channelctrlMessagesMarks[0]
					,10,rcctm_channelctrlQueueNodes, &rcctm_channelctrlQueueNodesMarks[0]);
	cchk_fdirmngMemory.SetMemory(10, cchk_fdirmngMessages, &cchk_fdirmngMessagesMarks[0]
					,14,cchk_fdirmngQueueNodes, &cchk_fdirmngQueueNodesMarks[0]);
	ccbkgtcexecMemory.SetMemory(10, ccbkgtcexecMessages, &ccbkgtcexecMessagesMarks[0]
					,11,ccbkgtcexecQueueNodes, &ccbkgtcexecQueueNodesMarks[0]);
}
 

//*****************************************************************************
//CEDROOMSystemCommSAP
//*****************************************************************************

//Components

RICUASW	* CEDROOMSystemCommSAP::mp_ricuasw=NULL;
RCCEPDManager	* CEDROOMSystemCommSAP::mp_rccepdmanager=NULL;
RCCTM_ChannelCtrl	* CEDROOMSystemCommSAP::mp_rcctm_channelctrl=NULL;
CCHK_FDIRMng	* CEDROOMSystemCommSAP::mp_cchk_fdirmng=NULL;
CCBKGTCExec	* CEDROOMSystemCommSAP::mp_ccbkgtcexec=NULL;

//! Event for trigger the bottom half associated to the IRQ
Pr_IRQEvent		CEDROOMSystemCommSAP::RemoteCommEventIRQ(0x1C);

//!Binary Semaphore for signal the end of the bottom half of the IRQ
Pr_SemaphoreBin CEDROOMSystemCommSAP::RemoteCommSemEndIRQ(0);

//**************************************************************************************************
//**************Function to get the information of the msg with the most priority*******************
void GetMaxPrioMsgInfo(uint8_t* msgPrio, uint8_t* msgSenderCmp, uint8_t* msgSenderCmpInterface, uint8_t* msgSignal){
	uint8_t aux_prio;
	uint32_t ID_CANMSG = edroom_can_drv_get_id(&aux_prio);
	*msgPrio = aux_prio;
	*msgSenderCmp = (ID_CANMSG>>16) & 0x0000007F;
	*msgSenderCmpInterface = (ID_CANMSG>>10) & 0x0000003F;
	*msgSignal = ID_CANMSG & 0x000003FF;
}


//! IRQ Handler for the IRQ
Pr_IRQHandler_RetType	CEDROOMSystemCommSAP::RemoteCommIRQHandler(void)
{
	//IRQ Handler
	
	RemoteCommEventIRQ.Signal();
	
}

//!Bottom Half Task Function for the IRQ //ToDo
Pr_TaskRV_t		CEDROOMSystemCommSAP::RemoteCommIRQBottomHalfTask(Pr_TaskP_t)
{
	
	bool endTask=false;

	do
	{
		RemoteCommEventIRQ.Wait();

		if(!RemoteCommSemEndIRQ.WaitCond())
		{
			
			uint8_t msgPrio;
			uint8_t msgSenderCmp;
			uint8_t msgSenderCmpInterface;
			uint8_t msgSignal;
			uint8_t flush_edroom_can_drv = 0; //=0 in case of error in msg, it wont be flushed

			//DEBUG
			//todo edroom_can_drv_read_status();

			//ID 28-25 aux->mp_ReceiverCmp->m_NomPriority; //4 bits
			//ID 24-23 2 bits tipo de mensaje
			//ID 22 16 7 bits aux->mp_SenderCmp->m_ComponentID;
			//ID 15-10 6 bits aux->mp_SenderPort->m_IdInterface;
			//ID 9-0 10 bits SHK_FDIR_TC
			//*****************
			//Handling CAN IRQ cector 0x1C

			GetMaxPrioMsgInfo(&msgPrio, &msgSenderCmp, &msgSenderCmpInterface, &msgSignal);

			/* EL siguiente código se genera en función del identificador del sender remoto, que lo tenemos fijado en Config (más abajo)
			 * void CEDROOMSystemDeployment_Node1::Config(RICUASW	*p_ricuasw,
			 * 												RCCEPDManager	*p_rccepdmanager,
			 * 												RCCTM_ChannelCtrl *p_rcctm_channelctrl,
			 * 												CCHK_FDIRMng *p_cchk_fdirmng,
			 * 												CCBKGTCExec	*p_ccbkgtcexec){
			 */
			switch (msgSenderCmp)//Who sent the message?
			{
			case(2): //EPDManager Sent it
				switch(msgSenderCmpInterface)// What interface was used?
				{
				case(1)://Interface EPDManager-BKGTCExec (Asynchronous)
					switch(msgSignal)//What signal was sent?
					{
					case(RCCEPDManager::SBKGTC):
						{
						//Read the data of the message
						uint32_t elementsize = mp_rccepdmanager->EDROOMPoolCDTCDescriptor.GetElementSize();
						CDTCDescriptor * pData=mp_rccepdmanager->EDROOMPoolCDTCDescriptor.AllocData();
						int32_t msgSize = edroom_can_drv_read_edroom_message(msgPrio, (uint8_t *) pData, elementsize, flush_edroom_can_drv);
						if(msgSize <=0 || (uint32_t)msgSize!=elementsize){
							//Error: Wrong Msg Size
							printf("Error:Wrong Msg Size\n");
							mp_rccepdmanager->EDROOMPoolCDTCDescriptor.FreeData(pData);

						}else{
						//Send the message and the data. (send)
							mp_rccepdmanager->BKGExecCtrl.send(msgSignal, pData, &mp_rccepdmanager->EDROOMPoolCDTCDescriptor);
						}
						//DEBUG
						//todo edroom_can_drv_read_status();

						}
						break;
					}
					break;
				case(2)://Interface EPDManager-HKFDIRMng (Asynchronous)
					switch(msgSignal)//What signal was sent?
					{
					case(RCCEPDManager::SHK_FDIR_TC):
						{
						uint32_t elementsize= mp_rccepdmanager->EDROOMPoolCDTCDescriptor.GetElementSize();
						//Read the data of the message
						CDTCDescriptor * pData=mp_rccepdmanager->EDROOMPoolCDTCDescriptor.AllocData();
						int32_t msgSize = edroom_can_drv_read_edroom_message(msgPrio,(uint8_t *) pData, elementsize, flush_edroom_can_drv);
						if(msgSize<=0 || (uint32_t)msgSize != elementsize)
						{
							//Error: Wrong Msg Size
							printf("Error:Wrong Msg Size\n");
							mp_rccepdmanager->EDROOMPoolCDTCDescriptor.FreeData(pData);
						}else{
						//Send the message and the data. (send)
						mp_rccepdmanager->HK_FDIRCtrl.send(msgSignal, pData, &mp_rccepdmanager->EDROOMPoolCDTCDescriptor);
						}
						//DEBUG
						//todo edroom_can_drv_read_status();

						}
						break;
					}
					break;
				}
				//case(3)://N/A
				//case(4)://N/A THis interface is CURRENTLY not remote
				break;
			case(3)://TMChannelCtrl Sent it
				switch(msgSenderCmpInterface)//What interface was used?
				{
					
					//*********Pensing review******************
					
				case(1)://Interface BKGTCExec-TM_ChannelCtrl (Synchronous reply)
					switch(msgSignal)//What is the signal?
					{
					case(RCCTM_ChannelCtrl::STMQueued):
						{
						//Read the NULL data of the message (neccesary to discard the message from the buffer)
						uint8_t * pDummy;//se puede hacer si el tamaño permitido es 0
						int32_t msgSize = edroom_can_drv_read_edroom_message(msgPrio, pDummy, 0, flush_edroom_can_drv);
						if(msgSize!=0)
						{
							//Error: Wrong Msg Size
							printf("Error:Wrong Msg Size\n");
						}else{
						//Send the message but no data. (Reply from remote)
						mp_rcctm_channelctrl->TMChannelCtrl3.reply_from_remote(msgSignal);
						}

						//DEBUG
						//todo edroom_can_drv_read_status();
						

						}
						break;
					}
					break;
					
					//*********Pensing review******************
					
				case(2)://Interface HKFDIRMng-TM_ChannelCtrl (Synchronous reply)
					switch(msgSignal)//What is the signal?
					{
					case(RCCTM_ChannelCtrl::STMQueued):
						{
						//Read the NULL data of the message (neccesary to discard the message from the buffer)
						uint8_t * pDummy;//se puede hacer si el tamaño permitido es 0
						int32_t msgSize = edroom_can_drv_read_edroom_message(msgPrio, pDummy, 0, flush_edroom_can_drv);
						if(msgSize!=0)
						{
							//Error: Wrong Msg Size
							printf("Error:Wrong Msg Size\n");
						}else{
						//Send the message but no data. (Reply from remote)
						mp_rcctm_channelctrl->TMChannelCtrl2.reply_from_remote(msgSignal);
						}
						//DEBUG
						//todo edroom_can_drv_read_status();

						}
						break;
					//STMQueued
					//Synchronous Reply
					}
					break;
				//case(3):N/A
				}
				break;
			}
			Pr_IRQManager::EnableIRQ(0x1C);
			if (edroom_can_drv_pending_msg() != 0)
				RemoteCommEventIRQ.Signal();
		}else endTask = 1;
	}while(!endTask);
}
//*****************************************************************************
//SetComponents
 
void CEDROOMSystemCommSAP::SetComponents(RICUASW   *p_ricuasw,
										RCCEPDManager   *p_rccepdmanager,
										RCCTM_ChannelCtrl   *p_rcctm_channelctrl,
										CCHK_FDIRMng   *p_cchk_fdirmng,
										CCBKGTCExec   *p_ccbkgtcexec){
	mp_ricuasw=p_ricuasw;
	mp_rccepdmanager=p_rccepdmanager;
	mp_rcctm_channelctrl=p_rcctm_channelctrl;
	mp_cchk_fdirmng=p_cchk_fdirmng;
	mp_ccbkgtcexec=p_ccbkgtcexec;
}


//*****************************************************************************
//*****************************************************************************
//Signal Translation Functions
//*****************************************************************************
//*****************************************************************************


TEDROOMSignal CEDROOMSystemCommSAP::C5BKGTCExec_PTMChannelCtrl__C3TM_ChannelCtrl_PTMChannelCtrl3(TEDROOMSignal signalOut){
 
	TEDROOMSignal signalIn;
 
	switch(signalOut){
 
		case( CCBKGTCExec::STxTM):	 signalIn=RCCTM_ChannelCtrl::STxTM; break;
 
		default: signalIn=(TEDROOMSignal)(-1); break;
 
	}
	return signalIn;
 
}
 
TEDROOMSignal CEDROOMSystemCommSAP::C3TM_ChannelCtrl_PTMChannelCtrl3__C5BKGTCExec_PTMChannelCtrl(TEDROOMSignal signalOut){
 
	TEDROOMSignal signalIn;
 
	switch(signalOut){
 
		case( RCCTM_ChannelCtrl::STMQueued):	 signalIn=CCBKGTCExec::STMQueued; break;
 
		default: signalIn=(TEDROOMSignal)(-1); break;
 
	}
	return signalIn;
 
}
 
TEDROOMSignal CEDROOMSystemCommSAP::C2EPDManager_PBKGExecCtrl__C5BKGTCExec_PBKGExecCtrl(TEDROOMSignal signalOut){
 
	TEDROOMSignal signalIn;
 
	switch(signalOut){
 
		case( RCCEPDManager::SBKGTC):	 signalIn=CCBKGTCExec::SBKGTC; break;
 
		default: signalIn=(TEDROOMSignal)(-1); break;
 
	}
	return signalIn;
 
}
 
TEDROOMSignal CEDROOMSystemCommSAP::C5BKGTCExec_PBKGExecCtrl__C2EPDManager_PBKGExecCtrl(TEDROOMSignal signalOut){
 
	TEDROOMSignal signalIn;
 
	switch(signalOut){
 
		default: signalIn=(TEDROOMSignal)(-1); break;
 
	}
	return signalIn;
 
}
 
TEDROOMSignal CEDROOMSystemCommSAP::C2EPDManager_PHK_FDIRCtrl__C4HK_FDIRMng_PHK_FDIRCtrl(TEDROOMSignal signalOut){
 
	TEDROOMSignal signalIn;
 
	switch(signalOut){
 
		case( RCCEPDManager::SHK_FDIR_TC):	 signalIn=CCHK_FDIRMng::SHK_FDIR_TC; break;
 
		default: signalIn=(TEDROOMSignal)(-1); break;
 
	}
	return signalIn;
 
}
 
TEDROOMSignal CEDROOMSystemCommSAP::C4HK_FDIRMng_PHK_FDIRCtrl__C2EPDManager_PHK_FDIRCtrl(TEDROOMSignal signalOut){
 
	TEDROOMSignal signalIn;
 
	switch(signalOut){
 
		case( CCHK_FDIRMng::SRecAction):	 signalIn=RCCEPDManager::SRecAction; break;
 
		default: signalIn=(TEDROOMSignal)(-1); break;
 
	}
	return signalIn;
 
}
 
TEDROOMSignal CEDROOMSystemCommSAP::C3TM_ChannelCtrl_PTMChannelCtrl2__C4HK_FDIRMng_PTMChannelCtrl(TEDROOMSignal signalOut){
 
	TEDROOMSignal signalIn;
 
	switch(signalOut){
 
		case( RCCTM_ChannelCtrl::STMQueued):	 signalIn=CCHK_FDIRMng::STMQueued; break;
 
		default: signalIn=(TEDROOMSignal)(-1); break;
 
	}
	return signalIn;
 
}
 
TEDROOMSignal CEDROOMSystemCommSAP::C4HK_FDIRMng_PTMChannelCtrl__C3TM_ChannelCtrl_PTMChannelCtrl2(TEDROOMSignal signalOut){
 
	TEDROOMSignal signalIn;
 
	switch(signalOut){
 
		case( CCHK_FDIRMng::STxTM):	 signalIn=RCCTM_ChannelCtrl::STxTM; break;
 
		default: signalIn=(TEDROOMSignal)(-1); break;
 
	}
	return signalIn;
 
}
 
TEDROOMSignal CEDROOMSystemCommSAP::C2EPDManager_PTMChannelCtrl__C3TM_ChannelCtrl_PTMChannelCtrl(TEDROOMSignal signalOut){
 
	TEDROOMSignal signalIn;
 
	switch(signalOut){
 
		case( RCCEPDManager::STxTM):	 signalIn=RCCTM_ChannelCtrl::STxTM; break;
 
		default: signalIn=(TEDROOMSignal)(-1); break;
 
	}
	return signalIn;
 
}
 
TEDROOMSignal CEDROOMSystemCommSAP::C3TM_ChannelCtrl_PTMChannelCtrl__C2EPDManager_PTMChannelCtrl(TEDROOMSignal signalOut){
 
	TEDROOMSignal signalIn;
 
	switch(signalOut){
 
		case( RCCTM_ChannelCtrl::STMQueued):	 signalIn=RCCEPDManager::STMQueued; break;
 
		default: signalIn=(TEDROOMSignal)(-1); break;
 
	}
	return signalIn;
 
}
 
 
 
//*****************************************************************************
//RegisterInterfaces
void CEDROOMSystemCommSAP::RegisterInterfaces(){
	// Register Interface for remote Component 1 //ICUASW
	//m_localCommSAP.RegisterInterface(1, mp_ricuasw->Timer, mp_ricuasw);
 
	// Register Interface for remote Component 2 //EPDManager
	m_localCommSAP.RegisterInterface(1, mp_rccepdmanager->BKGExecCtrl, mp_rccepdmanager);
	m_localCommSAP.RegisterInterface(2, mp_rccepdmanager->HK_FDIRCtrl, mp_rccepdmanager);
	//m_localCommSAP.RegisterInterface(3, mp_rccepdmanager->RxTC, mp_rccepdmanager);
	//m_localCommSAP.RegisterInterface(4, mp_rccepdmanager->TMChannelCtrl, mp_rccepdmanager);
 
	// Register Interface for remote Component 3 //TM_ChannelCtrl
	m_localCommSAP.RegisterInterface(1, mp_rcctm_channelctrl->TMChannelCtrl3, mp_rcctm_channelctrl);
	m_localCommSAP.RegisterInterface(2, mp_rcctm_channelctrl->TMChannelCtrl2, mp_rcctm_channelctrl);
	//m_localCommSAP.RegisterInterface(3, mp_rcctm_channelctrl->TMChannelCtrl, mp_rcctm_channelctrl);
 
	// Register Interface for Component 4//HK_FDIR
	m_localCommSAP.RegisterInterface(1, mp_cchk_fdirmng->HK_FDIRCtrl, mp_cchk_fdirmng);
	m_localCommSAP.RegisterInterface(2, mp_cchk_fdirmng->TMChannelCtrl, mp_cchk_fdirmng);
	m_localCommSAP.RegisterInterface(3, mp_cchk_fdirmng->HK_FDIRTimer, mp_cchk_fdirmng);//Local
 
	// Register Interface for Component 5//BKGExexCtrl
	m_localCommSAP.RegisterInterface(1, mp_ccbkgtcexec->BKGExecCtrl, mp_ccbkgtcexec);
	m_localCommSAP.RegisterInterface(2, mp_ccbkgtcexec->TMChannelCtrl, mp_ccbkgtcexec);
 
}
 
 
//*****************************************************************************
////SetLocalConnections
 /*
void CEDROOMSystemCommSAP::SetLocalConnections(){
 
	m_localCommSAP.Connect(mp_ccbkgtcexec->TMChannelCtrl, mp_rcctm_channelctrl->TMChannelCtrl3, connections[0],
					C5BKGTCExec_PTMChannelCtrl__C3TM_ChannelCtrl_PTMChannelCtrl3, 
					C3TM_ChannelCtrl_PTMChannelCtrl3__C5BKGTCExec_PTMChannelCtrl);
 
	m_localCommSAP.Connect(mp_rccepdmanager->BKGExecCtrl, mp_ccbkgtcexec->BKGExecCtrl, connections[1],
					C2EPDManager_PBKGExecCtrl__C5BKGTCExec_PBKGExecCtrl, 
					C5BKGTCExec_PBKGExecCtrl__C2EPDManager_PBKGExecCtrl);
 
	m_localCommSAP.Connect(mp_rccepdmanager->HK_FDIRCtrl, mp_cchk_fdirmng->HK_FDIRCtrl, connections[2],
					C2EPDManager_PHK_FDIRCtrl__C4HK_FDIRMng_PHK_FDIRCtrl, 
					C4HK_FDIRMng_PHK_FDIRCtrl__C2EPDManager_PHK_FDIRCtrl);
 
	m_localCommSAP.Connect(mp_rcctm_channelctrl->TMChannelCtrl2, mp_cchk_fdirmng->TMChannelCtrl, connections[3],
					C3TM_ChannelCtrl_PTMChannelCtrl2__C4HK_FDIRMng_PTMChannelCtrl, 
					C4HK_FDIRMng_PTMChannelCtrl__C3TM_ChannelCtrl_PTMChannelCtrl2);
 
	m_localCommSAP.Connect(mp_rccepdmanager->TMChannelCtrl, mp_rcctm_channelctrl->TMChannelCtrl, connections[4],
					C2EPDManager_PTMChannelCtrl__C3TM_ChannelCtrl_PTMChannelCtrl, 
					C3TM_ChannelCtrl_PTMChannelCtrl__C2EPDManager_PTMChannelCtrl);
 
}
 
*/
//*************************************************************************
//			CEDROOMRemoteTXChannel PUBLIC METHODS
//*************************************************************************

//*************************************************************************
//Constructor

CEDROOMRemoteTXChannel::CEDROOMRemoteTXChannel()
{

}

//*************************************************************************
//send
void CEDROOMRemoteTXChannel::send(TEDROOMSignal signal,
		void *p_data,
		CEDROOMMemoryPool * p_DataPool)
{
	//Check DataPool
	if(p_DataPool && p_data)
	{
		if(p_DataPool->IsEmpty())
		{
			mp_SenderCmp->m_Mutex.Wait();

			//Data Pool Is Empty
			mp_SenderCmp->SetErrorFlag(EDROOMSendDataPoolEmpty);

			mp_SenderCmp->m_Mutex.Signal();
		}

		//DEBUG
		//todo edroom_can_drv_read_status();

		edroom_can_drv_send_edroom_message((uint8_t)signal,
				(uint8_t)mp_SenderPort->m_IdInterface,
				(uint8_t)mp_SenderCmp->m_ComponentID,
				(uint8_t)mp_ReceiverCmp->m_NomPriority,
				p_DataPool->ElementSize,
				(byte_t *) p_data);

		//DEBUG
		//todo edroom_can_drv_read_status();

		if(0!=p_DataPool->FreeData(p_data))
		{
			//Data id out of Pool!!
			mp_SenderCmp->SetErrorFlag(EDROOMDataOutOfPool);
		}
	}
	else
	{

		//DEBUG
		//todo edroom_can_drv_read_status();

		edroom_can_drv_send_edroom_message((uint8_t)signal,
						(uint8_t)mp_SenderPort->m_IdInterface,
						(uint8_t)mp_SenderCmp->m_ComponentID,
						(uint8_t)mp_ReceiverCmp->m_NomPriority,
						0, NULL);

		//DEBUG
		//todo edroom_can_drv_read_status();

	}
}

//*************************************************************************
//invoke
//(to remote)
CEDROOMMessage * CEDROOMRemoteTXChannel::invoke(TEDROOMSignal signal,
		void *p_data,
		CEDROOMMemoryPool * p_DataPool)
{

	//DEBUG
	//todo edroom_can_drv_read_status();

	CEDROOMSynchrMessage *p_Msg;
	//TEDROOMBool queuefull= false;

	Pr_SemaphoreBin *apSemSincr;

	//LocalTXChannel of the remote component port
	CEDROOMLocalTXChannel * remote_cmp_port_localtxchannel=(CEDROOMLocalTXChannel * )mp_ReceiverPort->mp_Channel;

	//Alloc Synchr Msg. It waits until the message is free, so synchr Msg Pool
	//is never empty

	p_Msg = mp_SenderCmp->m_SynchrMsgPool.AllocData();

	//Sender here is not remote because the reply is done using the local connection
	p_Msg->m_IsSenderRemote=false;


	//TODO?Complete attributes;


	//NO signal conversion here. Conversion is done in the Remote Receiver
	p_Msg->signal=(*mp_SenderToReceiverSignalTranslator)(signal);

	//Msg Pool
	p_Msg->mp_MsgPool=&mp_SenderCmp->m_SynchrMsgPool;

	//Msg Data
	p_Msg->data=p_data;

	//Msg Data Pool
	p_Msg->mp_DataPool=p_DataPool;

	//Check If Data Pool is Empty
	if(p_DataPool)
		if(p_DataPool->IsEmpty())
			mp_SenderCmp->SetErrorFlag(EDROOMInvokeDataPoolEmpty);

	//Priority
	p_Msg->priority=mp_ReceiverCmp->GetNomPriority();

	//Interface
	p_Msg->mp_Interface=mp_ReceiverPort;

	//Synchr Sem
	apSemSincr=&(p_Msg->m_SynchrSem);

	//Free Component->MsgBack, if it is not NULL
	mp_SenderCmp->EDROOMFreeMsgBackData();

	//Pointer to Msg Back;
	p_Msg->mp_MsgBack=&(mp_SenderCmp->m_MessageBack);

	//sender is waiting
	p_Msg->m_IsSenderWaiting=1;

	//****************************************************************************
	//*********************** REMOTE *********************************************
	//Set LocalTXChannel pointer to Synchronize Msg. This pointer is used replu_from_remote
	remote_cmp_port_localtxchannel->mp_SynchrMsgFromRemote=p_Msg;

	if(p_data && p_DataPool)
	{
		edroom_can_drv_send_edroom_message((uint8_t)signal, (uint8_t)mp_SenderPort->m_IdInterface, (uint8_t)mp_SenderCmp->m_ComponentID, (uint8_t)mp_ReceiverCmp->m_NomPriority
				, p_DataPool->ElementSize, (byte_t *) p_data);
	}
	else
	{
		edroom_can_drv_send_edroom_message((uint8_t)signal, (uint8_t)mp_SenderPort->m_IdInterface, (uint8_t)mp_SenderCmp->m_ComponentID, (uint8_t)mp_ReceiverCmp->m_NomPriority
						, 0, NULL);
	}

	//DEBUG
	//todo edroom_can_drv_read_status();

	//Put msg in queue
	//mp_ReceiverCmp->m_Mutex.Wait();
/*
	if(mp_ReceiverCmp->m_MsgQueue.IsFull())
	{
		CEDROOMMessage* auxMsg;
		queuefull=true;
		//Msg Pool Is Empty
		mp_ReceiverCmp->SetErrorFlag(EDROOMMsgQueueFull);
		p_Msg->mp_MsgBack->signal=0;//timeout
		p_Msg->mp_MsgBack->data=NULL;
		p_Msg->mp_MsgBack->mp_DataPool=NULL;
		auxMsg=(CEDROOMMessage*)p_Msg;
		mp_ReceiverCmp->EDROOMFreeMessage(auxMsg);

	}
	else
		mp_ReceiverCmp->m_MsgQueue.In(p_Msg);

	mp_ReceiverCmp->m_Mutex.Signal();
*/
	//if(!queuefull)
	//{
	//Wait reply if message has been sent (queue is not full)
	apSemSincr->Wait();
	//}

	CEDROOMMessage* auxMsg;
	auxMsg=(CEDROOMMessage*)p_Msg;
	mp_ReceiverCmp->EDROOMFreeMessage(auxMsg);

	//return the pointer
	return(&(mp_SenderCmp->m_MessageBack));

}



//*****************************************************************************
//reply

void CEDROOMRemoteTXChannel::reply_to_remote(TEDROOMSignal signal, void * p_data
		, CEDROOMMemoryPool *p_DataPool)
{
	//Check DataPool
	if(p_data && p_DataPool)
	{
		if(p_DataPool->IsEmpty())
		{
			mp_SenderCmp->m_Mutex.Wait();

			//Data Pool is Empty
			mp_SenderCmp->SetErrorFlag(EDROOMReplyDataPoolEmpty);

			mp_SenderCmp->m_Mutex.Signal();
			//¿No debería terminar aquí la función debido a un error?
		}

		//DEBUG
		//todo edroom_can_drv_read_status();

		edroom_can_drv_send_edroom_message((uint8_t)signal, (uint8_t)mp_SenderPort->m_IdInterface
				, (uint8_t)mp_SenderCmp->m_ComponentID, (uint8_t)mp_ReceiverCmp->m_NomPriority
				, p_DataPool->ElementSize, (byte_t *) p_data);

		//DEBUG
		//todo edroom_can_drv_read_status();

		if(0!=p_DataPool->FreeData(p_data))
		{
			//Data is out of Pool!!!
			mp_SenderCmp->SetErrorFlag(EDROOMDataOutOfPool);
		}
	}
	else
	{
		//NO DATA in the reply
		edroom_can_drv_send_edroom_message((uint8_t)signal, (uint8_t)mp_SenderPort->m_IdInterface
				, (uint8_t)mp_SenderCmp->m_ComponentID, (uint8_t)mp_ReceiverCmp->m_NomPriority, 0, NULL);
	}

}
//*****************************************************************************
//**********************	Connect		***************************************

//*****************************************************************************
//**********************	RegisterInterface		***************************

TEDROOMInt32 CEDROOMRemoteCommSAP::RegisterInterface(TEDROOMInterfaceID id,
		CEDROOMInterface & interf,
		CEDROOMComponent * component)
{
	interf.m_IdInterface=id;//interface id

	interf.mp_Component=component;//component pointer

	return(0);
}

void CEDROOMRemoteCommSAP::Connect(CEDROOMInterface &inter1,
		CEDROOMInterface &inter2,
		CEDROOMRemoteConnection &connection,
		TEDROOMSignal (f1) (TEDROOMSignal ),
		TEDROOMSignal (f2) (TEDROOMSignal ))
{
	//Connect ports

	connection.m_Channel1.mp_SenderPort=&inter1;	//puertos que la forman ¿forman el qué?

	connection.m_Channel1.mp_ReceiverPort=&inter2;	//puertos que la forman ¿el qué?

	connection.m_Channel2.mp_SenderPort=&inter2;	//puertos que la forman ¿forman el qué?

	connection.m_Channel2.mp_ReceiverPort=&inter1;	//puertos que la forman ¿el qué?

	//pointers to channels

	inter1.mp_Channel=&connection.m_Channel1;
	inter2.mp_Channel=&connection.m_Channel2;

	//pointer to components

	connection.m_Channel1.mp_SenderCmp=inter1.mp_Component;
	connection.m_Channel1.mp_ReceiverCmp=inter2.mp_Component;
	connection.m_Channel2.mp_SenderCmp=inter2.mp_Component;
	connection.m_Channel2.mp_ReceiverCmp=inter1.mp_Component;

	//Signal conversion

	connection.m_Channel1.mp_SenderToReceiverSignalTranslator=f1;
	connection.m_Channel2.mp_SenderToReceiverSignalTranslator=f2;

}

//*****************************************************************************
////SetRemoteConnections
 
void CEDROOMSystemCommSAP::SetRemoteConnections(){

	edroom_can_drv_config();

	m_remoteCommSAP.Connect(mp_ccbkgtcexec->TMChannelCtrl, mp_rcctm_channelctrl->TMChannelCtrl3, remote_connections[0],
			C5BKGTCExec_PTMChannelCtrl__C3TM_ChannelCtrl_PTMChannelCtrl3,
			C3TM_ChannelCtrl_PTMChannelCtrl3__C5BKGTCExec_PTMChannelCtrl);

	m_remoteCommSAP.Connect(mp_ccbkgtcexec->BKGExecCtrl, mp_rccepdmanager->BKGExecCtrl, remote_connections[1],
			C5BKGTCExec_PBKGExecCtrl__C2EPDManager_PBKGExecCtrl,
			C2EPDManager_PBKGExecCtrl__C5BKGTCExec_PBKGExecCtrl);

	m_remoteCommSAP.Connect(mp_cchk_fdirmng->HK_FDIRCtrl, mp_rccepdmanager->HK_FDIRCtrl, remote_connections[2],
			C4HK_FDIRMng_PHK_FDIRCtrl__C2EPDManager_PHK_FDIRCtrl,
			C2EPDManager_PHK_FDIRCtrl__C4HK_FDIRMng_PHK_FDIRCtrl);

	m_remoteCommSAP.Connect(mp_cchk_fdirmng->TMChannelCtrl, mp_rcctm_channelctrl->TMChannelCtrl2, remote_connections[3],
			C4HK_FDIRMng_PTMChannelCtrl__C3TM_ChannelCtrl_PTMChannelCtrl2,
			C3TM_ChannelCtrl_PTMChannelCtrl2__C4HK_FDIRMng_PTMChannelCtrl);

}
 
//*****************************************************************************
////SetConnections
 
void CEDROOMSystemCommSAP::SetConnections(){
 
	//SetLocalConnections(); No local connections
	SetRemoteConnections();
 
}
 
//*****************************************************************************
////CEDROOMSystemDeployment Constructor
 
CEDROOMSystemDeployment::CEDROOMSystemDeployment(){
 
#ifdef CONFIG_EDROOMSL_ADD_TRACE
#endif
	systemMemory.SetMemory();
 
}
 
//*****************************************************************************
////Config
 
void CEDROOMSystemDeployment::Config(RICUASW   *p_ricuasw,
											RCCEPDManager   *p_rccepdmanager,
											RCCTM_ChannelCtrl   *p_rcctm_channelctrl,
											CCHK_FDIRMng   *p_cchk_fdirmng,
											CCBKGTCExec   *p_ccbkgtcexec){
 
	mp_ricuasw=p_ricuasw;
	mp_rccepdmanager=p_rccepdmanager;
	mp_rcctm_channelctrl=p_rcctm_channelctrl;
	mp_cchk_fdirmng=p_cchk_fdirmng;
	mp_ccbkgtcexec=p_ccbkgtcexec;
 
	systemCommSAP.SetComponents(	p_ricuasw,
									p_rccepdmanager,
									p_rcctm_channelctrl,
									p_cchk_fdirmng,
									p_ccbkgtcexec);
 
	systemCommSAP.RegisterInterfaces();
	systemCommSAP.SetConnections();
 
}
 
//*****************************************************************************
////StartComponents
 
void CEDROOMSystemDeployment::StartComponents(){
	mp_ricuasw->EDROOMStart();
	mp_rccepdmanager->EDROOMStart();
	mp_rcctm_channelctrl->EDROOMStart();
	mp_cchk_fdirmng->EDROOMStart();
	mp_ccbkgtcexec->EDROOMStart();
 
}
 
//*****************************************************************************
////Start
 
void CEDROOMSystemDeployment::Start(){
 
	//Install CAN IRQ HANDLER, Vector 0x1C;

	Pr_IRQManager::InstallIRQHandler(CEDROOMSystemCommSAP::RemoteCommIRQHandler, 1, 0x1C);

	Pr_Task RemoteComm(CEDROOMSystemCommSAP::RemoteCommIRQBottomHalfTask, EDROOMprioURGENT, 1024*2);

#ifdef CONFIG_EDROOMBP_DEPLOYMENT_NEED_TASK
 
	Pr_Task MainTask(CEDROOMSystemDeployment::main_task,EDROOMprioMINIMUM,1024*16);
 
	kernel.Start();
 
#else
 
StartComponents();
 
	kernel.Start();
 
	MainWait(*mp_ricuasw,
				*mp_rccepdmanager,
				*mp_rcctm_channelctrl,
				*mp_cchk_fdirmng,
				*mp_ccbkgtcexec);
 
 
#endif
 
}
 
#ifdef CONFIG_EDROOMBP_DEPLOYMENT_NEED_TASK
 
	extern CEDROOMSystemDeployment systemDeployment;
 
 
//*****************************************************************************
////main_task
 
 
Pr_TaskRV_t CEDROOMSystemDeployment::main_task(Pr_TaskP_t){
 
	systemDeployment.StartComponents();
	MainWait(*systemDeployment.mp_ricuasw,
				*systemDeployment.mp_rccepdmanager,
				*systemDeployment.mp_rcctm_channelctrl,
				*systemDeployment.mp_cchk_fdirmng,
				*systemDeployment.mp_ccbkgtcexec);
 
}
#endif