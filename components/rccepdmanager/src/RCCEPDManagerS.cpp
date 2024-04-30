

#include <public/rccepdmanager_iface_v1.h>



// ******************************* CONSTRUCTOR ************************************



RCCEPDManager::RCCEPDManager(TEDROOMComponentID id,
		TEDROOMUInt32 roomNumMaxMens,
		TEDROOMPriority roomtaskPrio,
		TEDROOMStackSizeType roomStack,
		CEDROOMMemory *pActorMemory ) : 

		CEDROOMComponent(id,EDROOMprioMINIMUM+1,roomNumMaxMens,
				roomtaskPrio,roomStack, pActorMemory ),

		EDROOMPoolCDTMList(
							10, pActorMemory->poolCDTMList,
							pActorMemory->poolMarkCDTMList),
		EDROOMPoolCDTCDescriptor(
							10, pActorMemory->poolCDTCDescriptor,
							pActorMemory->poolMarkCDTCDescriptor)


{


}


//**************************************************************************
//**************************   PUBLIC METHODS  *****************************



//************************** EDROOMConfig **********************************


int RCCEPDManager::EDROOMConfig()
{


 return 0; 
}



//************************** EDROOMStart **********************************

int RCCEPDManager::EDROOMStart()
{


	//***************** CEDROOMComponent::EDROOMStart*********

	CEDROOMComponent::EDROOMStart(); 	// Call to EDROOMStart method of CEDROOMComponent

	return 0;
}



//**************************************************************************
//***************************    PROTECTED METHODS *************************

//! Component behaviour
void RCCEPDManager::EDROOMBehaviour(){

	do{
	 m_DestroyMutex.Wait();
	}while(1);

}




//********************* ComponentIsFinished **********************************


#ifdef _EDROOM_SYSTEM_CLOSE

bool RCCEPDManager::EDROOMIsComponentFinished()
{


	return ( CEDROOMComponent::EDROOMIsComponentFinished());

}

#endif


//****************** EDROOMMemory::SetMemory *******************************

void RCCEPDManager::CEDROOMMemory::SetMemory(TEDROOMUInt32 numMessages_ ,
		CEDROOMMessage * MessagesMem_,
		bool * MessagesMemMarks_,
		TEDROOMUInt32 numberOfNodes_,
		CEDROOMQueue::CQueueNode * QueueNodesMem_,
		bool * QueueNodesMemMarks_)
{

		CEDROOMComponentMemory::SetMemory( numMessages_,MessagesMem_, MessagesMemMarks_,
			numberOfNodes_,QueueNodesMem_, QueueNodesMemMarks_, QueueHeads);


}

//********************************** Pools *************************************

//CEDROOMPOOLCDTMList

RCCEPDManager::CEDROOMPOOLCDTMList::CEDROOMPOOLCDTMList(
		TEDROOMUInt32 elemCount,CDTMList* pMem,bool * pMemMarks):
			CEDROOMProtectedMemoryPool(elemCount, pMem, pMemMarks,
				sizeof(CDTMList))
{
}

CDTMList *	RCCEPDManager::CEDROOMPOOLCDTMList::AllocData()
{
return(CDTMList*)CEDROOMProtectedMemoryPool::AllocData();
}

//CEDROOMPOOLCDTCDescriptor

RCCEPDManager::CEDROOMPOOLCDTCDescriptor::CEDROOMPOOLCDTCDescriptor(
		TEDROOMUInt32 elemCount,CDTCDescriptor* pMem,bool * pMemMarks):
			CEDROOMProtectedMemoryPool(elemCount, pMem, pMemMarks,
				sizeof(CDTCDescriptor))
{
}

CDTCDescriptor *	RCCEPDManager::CEDROOMPOOLCDTCDescriptor::AllocData()
{
return(CDTCDescriptor*)CEDROOMProtectedMemoryPool::AllocData();
}


