/*
 * icuaasw_pus_service5.cpp
 *
 *  Created on: Dec 22, 2016
 *      Author: user
 */



#ifndef PUBLIC__ICUASW_PUS_SERVICE5_H
#define PUBLIC__ICUASW_PUS_SERVICE5_H

union RIDSerial_t{
    uint16_t RID;
    uint8_t  bytes[2];
};


struct RIDEnableConfig_t {
	bool_t enabled;
	uint16_t RID;
};

#define PUS_5_RIDNumber 16

class PUSService5:public PUSServices{

public:
  static RIDEnableConfig_t RIDEnableConfig[PUS_5_RIDNumber];
  static bool GetRIDEnableIndex(uint16_t RID, uint8_t &index);
  static void HandleEvents( CDEventList & eventList, CDTMList &List);
  static void ExecTC( CDTCDescriptor &TC, CDTMList &List);
  static void Exec5_5TC( CDTCDescriptor &TC, CDTMList &List);
  static void Exec5_6TC( CDTCDescriptor &TC, CDTMList &List);

};
#endif

