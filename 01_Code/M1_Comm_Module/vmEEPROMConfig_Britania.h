#ifndef _VI_EEPROM_CONFIG_BRITANIA_H_
#define _VI_EEPROM_CONFIG_BRITANIA_H_

#include "viEEPROMConfig.h"

#define  EEPROM_ADDR_GD_BB_35GM_60PCKTS_CBB    0
#define  EEPROM_ADDR_GD_BB_35GM_120PCKTS_CBB   3
#define  EEPROM_ADDR_GD_CB_29GM_168PCKTS_CBB   6
#define  EEPROM_ADDR_GD_BB_40GM_168PCKTS_CBB   9

#define  STNRD_CARTON_WEIGHT_GD_BB_35GM_60PCKTS_CBB     2100
#define  STNRD_CARTON_WEIGHT_GD_BB_35GM_120PCKTS_CBB    4200
#define  STNRD_CARTON_WEIGHT_GD_CB_29GM_168PCKTS_CBB    4870
#define  STNRD_CARTON_WEIGHT_GD_BB_40GM_168PCKTS_CBB    6720

#define  CARTON_TOLERANCE_GD_BB_35GM_60PCKTS_CBB      35
#define  CARTON_TOLERANCE_GD_BB_35GM_120PCKTS_CBB     35
#define  CARTON_TOLERANCE_GD_CB_29GM_168PCKTS_CBB     29
#define  CARTON_TOLERANCE_GD_BB_40GM_168PCKTS_CBB     40

void StandardCartonWeightandToleranceWritetoEEPROM(void)
{
	
}


//typedef struct _ST_TOLE






#endif /* _VI_EEPROM_CONFIG_BRITANIA_H_ */