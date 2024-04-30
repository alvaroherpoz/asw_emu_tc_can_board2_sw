

#include <public/ricuasw_iface_v1.h>





// ******************************* CONSTRUCTOR ************************************



RICUASW::RICUASW(TEDROOMComponentID id,
		TEDROOMUInt32 roomNumMaxMens,
		TEDROOMPriority roomtaskPrio,
		TEDROOMStackSizeType roomStack,
		CEDROOMMemory *pActorMemory ) : 

		CEDROOMComponent(id,EDROOMprioMINIMUM+1,roomNumMaxMens,
				roomtaskPrio,roomStack, pActorMemory )

{


}


//**************************************************************************
//**************************   PUBLIC METHODS  *****************************



//************************** EDROOMConfig **********************************


int RICUASW::EDROOMConfig()
{


 return 0; 
}



//************************** EDROOMStart **********************************

int RICUASW::EDROOMStart()
{



	//***************** CEDROOMComponent::EDROOMStart*********

	CEDROOMComponent::EDROOMStart(); 	// Call to EDROOMStart method of CEDROOMComponent

	return 0;
}


//**************************************************************************
//***************************    PROTECTED METHODS *************************



//*****************************  EDROOMBehaviour ***************************



void RICUASW::EDROOMBehaviour()
{


	do{
	 m_DestroyMutex.Wait();
	}while(1);

}




//********************* ComponentIsFinished **********************************


#ifdef _EDROOM_SYSTEM_CLOSE

bool RICUASW::EDROOMIsComponentFinished()
{


	return ( EPDManager.EDROOMIsComponentFinished() && TM_ChannelCtrl.EDROOMIsComponentFinished() && HK_FDIRMng.EDROOMIsComponentFinished() && BKGTCExec.EDROOMIsComponentFinished() && CEDROOMComponent::EDROOMIsComponentFinished());

}

#endif


//****************** EDROOMMemory::SetMemory *******************************

void RICUASW::CEDROOMMemory::SetMemory(TEDROOMUInt32 numMessages_ ,
		CEDROOMMessage * MessagesMem_,
		bool * MessagesMemMarks_,
		TEDROOMUInt32 numberOfNodes_,
		CEDROOMQueue::CQueueNode * QueueNodesMem_,
		bool * QueueNodesMemMarks_)
{

		CEDROOMComponentMemory::SetMemory( numMessages_,MessagesMem_, MessagesMemMarks_,
			numberOfNodes_,QueueNodesMem_, QueueNodesMemMarks_, QueueHeads);



}

