/*
 * icuaasw_pus_service5.cpp
 *
 *  Created on: Dec 22, 2016
 *      Author: user
 */



#include <public/icuasw_pus_services_iface_v1.h>


RIDEnableConfig_t PUSService5::RIDEnableConfig[PUS_5_RIDNumber]={
		{true,0x2001},
		{true,0x2002},
		{true,0x3001},
		{true,0x3002},
		{true,0x4001},
		{true,0x4002},
		{true,0x4003},
		{true,0x4004},
		{true,0x4005},
		{true,0x4006},
		{true,0x4007},
		{true,0x4008},
		{true,0x4009},
		{true,0x400A},
		{true,0x400B},
		{true,0x400C}
};


bool PUSService5::GetRIDEnableIndex(uint16_t RID, uint8_t &index){
  bool encontrado=false;
  for (int j=0; ((j<PUS_5_RIDNumber)&&!encontrado) ; j++ ){
           if(RID==RIDEnableConfig[j].RID)
             {
               index=j;
               encontrado=true;
           }
  }
  return encontrado;
}

void  PUSService5::HandleEvents( CDEventList & eventList, CDTMList &List){

	//RIDEnableConfig_t
	CDEventList auxEvList;
	while(eventList.GetNumberOfEvents()){
		auxEvList.AddEvent(eventList.ExtractFirstEvent());
	}

	while(auxEvList.GetNumberOfEvents()){
		uint8_t index;
		union RIDSerial_t RIDSerial;
		RIDSerial.RID=auxEvList.ExtractFirstEvent();
		if(GetRIDEnableIndex(RIDSerial.RID,index)){
			if(RIDEnableConfig[index].enabled){
				uint8_t subservice;
				uint8_t aux[256];
				byte_t *auxP=aux;

				#ifdef BIG_ENDIAN
					subservice=RIDSerial.bytes[0]>>4;
				#else
					subservice=RIDSerial.bytes[1]>>4;
				#endif
				SetUInt16(RIDSerial.RID,auxP);
				List.Add(5,subservice,aux);
				eventList.AddEvent(RIDSerial.RID);
			}

		}
	}

}

//TO_DO
void PUSService5::Exec5_5TC( CDTCDescriptor &TC, CDTMList &List){

  uint8_t index;
  byte_t *aux=TC.appData;
  uint16_t RID=GetUInt16(aux);


  if(GetRIDEnableIndex(RID, index)){
  	  List.Add(1,7);
  }
  else{
	  List.Add(1,8);
  }
}

//TO_DO
void PUSService5::Exec5_6TC( CDTCDescriptor &TC, CDTMList &List){

	  uint8_t index;
	  byte_t *aux=TC.appData;
	  uint16_t RID=GetUInt16(aux);

	  if(GetRIDEnableIndex(RID, index)){
	  	  List.Add(1,7);
	  }
	  else{
		  List.Add(1,8);
	  }
}

void  PUSService5::ExecTC( CDTCDescriptor &TC, CDTMList &List){

  switch(TC.dataFieldHeader.subservice){

  	  	  	  	case(5):

				Exec5_5TC(TC,List);
                	break;
                case(6):

					Exec5_6TC(TC,List);
                	break;
                default:
                  List.Add(1,8);
  }
}




