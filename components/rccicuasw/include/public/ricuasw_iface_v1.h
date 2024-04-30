#ifndef RICUASW_H_
#define RICUASW_H_

//******************************************************************************
// EDROOM Service Library

#define _EDROOM_IS_EMBEDDED_

#include <public/edroomsl_iface_v1.h>

//******************************************************************************
// Required software interfaces

#include <public/emu_sc_channel_drv_slib_v1.h>
#include <public/icuasw_pus_services_iface_v1.h>
#include <public/datapool_mng_iface_v1.h>


/**
 * \class   ICUASW
 *
 */
class RICUASW: public CEDROOMComponent {

public:

	/**
	 * \enum TEDROOMICUASWSignal
	 * \brief Component Signal Enum Type
	 *
	 */
	 enum TEDROOMICUASWSignal { EDROOMSignalTimeout, 
							EDROOMSignalDestroy };

	/**
	 * \class ICUASW::CEDROOMMemory
	 * \brief Component Memory
	 *
	 */
	class CEDROOMMemory: public CEDROOMComponentMemory{

		private:

			//!Array of Message Queue Heads, one for each priority
			CEDROOMQueue::CQueueHead QueueHeads[EDROOMprioMINIMUM+1];

			//************ Component Timing Service Memory************

			//!Component Timing Service Timer Info Memory
			CEDROOMTimerInfo TimerInf[3];
			//!Component Timing Service Timer Info Marks Memory
			bool TimerInfMarks[3];
			//!Component Timing Service TimeOut Messages Memory
			CEDROOMTimeOutMessage TimeOutMsgs[3];
			//!Component Timing Service TimeOut Messages Marks Memory
			bool TimeOutMsgsMarks[3];

		public: 

			//!Component Timing Service Memory Object
			CEDROOMTimingMemory TimingMemory;



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



	// *************************************************************************
	// **************************** Frame Service Methods	********************
	// *************************************************************************



	//! Constructor
	RICUASW(TEDROOMComponentID id, TEDROOMUInt32 roomNumMaxMens, TEDROOMPriority roomtaskPrio,
		TEDROOMStackSizeType roomStack, CEDROOMMemory * pActorMemory );



	//! Component Configuration 
	 int EDROOMConfig();


	//! Component Start 
	 int EDROOMStart();



	#ifdef _EDROOM_SYSTEM_CLOSE

	//! Method that returns true if component is finished 
	bool EDROOMIsComponentFinished();


	#endif



	//!Semaphore used for controlling the component destroy
	Pr_SemaphoreBin        m_DestroyMutex;


	//! Component behaviour 
	virtual void EDROOMBehaviour();


};
#endif
