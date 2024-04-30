#ifndef RCCEPDMANAGER_H_
#define RCCEPDMANAGER_H_

//******************************************************************************
// EDROOM Service Library

#define _EDROOM_IS_EMBEDDED_

#include <public/edroomsl_iface_v1.h>

//******************************************************************************
// Data Classes

#include <public/cdrecovaction_iface_v1.h>
#include <public/cdtmlist_iface_v1.h>
#include <public/cdeventlist_iface_v1.h>
#include <public/cdtcdescriptor_iface_v1.h>


//******************************************************************************
// Required software interfaces

#include <public/icuasw_pus_services_iface_v1.h>
#include <public/sc_channel_drv_iface_v1.h>


/**
 * \class   CCEPDManager
 *
 */
class RCCEPDManager: public CEDROOMComponent {

public:

	/**
	 * \enum TEDROOMCCEPDManagerSignal
	 * \brief Component Signal Enum Type
	 *
	 */
	 enum TEDROOMCCEPDManagerSignal { EDROOMSignalTimeout, 
							EDROOMSignalDestroy, 
							SBKGTC, 
							SHK_FDIR_TC, 
							SRecAction, 
							EDROOMIRQsignal, 
							STxTM, 
							STMQueued };

	/**
	 * \class RCCEPDManager::CEDROOMMemory
	 * \brief Component Memory
	 *
	 */
	class CEDROOMMemory: public CEDROOMComponentMemory{

		private:

			//!Array of Message Queue Heads, one for each priority
			CEDROOMQueue::CQueueHead QueueHeads[EDROOMprioMINIMUM+1];

	// ********************************************************************
	// ******************* Component Message Data Pools *******************
	// ********************************************************************

		public:

			//! CDTMList Data Pool Memory
			CDTMList	poolCDTMList[10+1];
			//! CDTMList Data Pool Marks Memory
			bool	poolMarkCDTMList[10];
			//! CDTCDescriptor Data Pool Memory
			CDTCDescriptor	poolCDTCDescriptor[10+1];
			//! CDTCDescriptor Data Pool Marks Memory
			bool	poolMarkCDTCDescriptor[10];


			/** \brief This function is used for setting the Component Memory
			 * 
			 * \param numberOfMsgs number of messages that the component can store
			 * \param msgsMemory memory for the messages that the component can store
			 * \param msgsMemoryMarks memory marks for the messages that the component can store
			 * \param numberOfNodes number of nodes that the component needs
			 * \param queueNodesMemory memory for the component message queues 
			 * \param queueNodesMemoryMarks memory marks for the component message queues 
			 */
			void SetMemory(TEDROOMUInt32 numberOfMsgs
						, CEDROOMMessage * msgsMemory
						, bool * msgsMemoryMarks
						, TEDROOMUInt32 numberOfNodes
						, CEDROOMQueue::CQueueNode * queueNodesMemory
						, bool * queueNodesMemoryMarks);

	};


	// ********************************************************************
	//******************  Component Communication Ports *******************
	// ********************************************************************

	//! BKGExecCtrl Component Port
	CEDROOMInterface	BKGExecCtrl;
	//! HK_FDIRCtrl Component Port
	CEDROOMInterface	HK_FDIRCtrl;
	//! TMChannelCtrl Component Port
	CEDROOMInterface	TMChannelCtrl;




	// *************************************************************************
	// **************************** Frame Service Methods	********************
	// *************************************************************************



	//! Constructor
	RCCEPDManager(TEDROOMComponentID id, TEDROOMUInt32 roomNumMaxMens, TEDROOMPriority roomtaskPrio,
		TEDROOMStackSizeType roomStack, CEDROOMMemory * pActorMemory );


	//! Component Configuration 
	 int EDROOMConfig();


	//! Component Start 
	 int EDROOMStart();



	//!Semaphore used for controlling the component destroy
	Pr_SemaphoreBin        m_DestroyMutex;

	//! Component behaviour
	virtual void EDROOMBehaviour();


	#ifdef _EDROOM_SYSTEM_CLOSE

	//! Method that returns true if component is finished 
	bool EDROOMIsComponentFinished();


	#endif




		// Pools *************************************************

		class CEDROOMPOOLCDTMList:public CEDROOMProtectedMemoryPool {
			public:
			CEDROOMPOOLCDTMList(TEDROOMUInt32 elemCount,CDTMList *pMem, bool *pMarks);
			CDTMList	* AllocData();
		};
		CEDROOMPOOLCDTMList	EDROOMPoolCDTMList;

		class CEDROOMPOOLCDTCDescriptor:public CEDROOMProtectedMemoryPool {
			public:
			CEDROOMPOOLCDTCDescriptor(TEDROOMUInt32 elemCount,CDTCDescriptor *pMem, bool *pMarks);
			CDTCDescriptor	* AllocData();
		};
		CEDROOMPOOLCDTCDescriptor	EDROOMPoolCDTCDescriptor;


		/**
		 * \brief Get and Clear the Error Flags 
		 * \return the error flags  

		 */
		 TEDROOMUInt32 GetAndClearErrorFlags(){ return GetAndClearErrorFlags();}




};
#endif
