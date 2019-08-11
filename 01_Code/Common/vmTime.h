#ifndef _VM_TIME_CONFIG_H_
#define _VM_TIME_CONFIG_H_

//#include "18F46K80.h"
#include "vmModbusGlobal.h"
#include <time.h>
#include <time.c>
//#include "vmHMIBoardModbusSetup.h"

#define SYSTEM_BASE_DATE 		1900
#define SYSTEM_HH_TO_SS 		3600
#define SYSTEM_MM_TO_SS 		60


unsigned int32 GetRTC(void)
{
    struct_tm lDate;    

    unsigned int16 ui16SSDiff = 0;
    unsigned int16 ui16MMDiff = 0;
    unsigned int16 ui16HHDiff = g_ui16RegisterTime_Diff / SYSTEM_HH_TO_SS;
    ui16SSDiff = g_ui16RegisterTime_Diff % SYSTEM_HH_TO_SS;
    ui16MMDiff = ui16SSDiff / SYSTEM_MM_TO_SS;
    ui16SSDiff = ui16SSDiff % SYSTEM_MM_TO_SS;
    
    lDate.tm_sec = (unsigned int8) g_ui16RegisterTime_ss + ui16SSDiff;  
    lDate.tm_min = (unsigned int8)g_ui16RegisterTime_mm + ui16MMDiff;  
    lDate.tm_hour = (unsigned int8)g_ui16RegisterTime_HH + ui16HHDiff;  
    lDate.tm_mday = (unsigned int8) g_ui16RegisterDateDD;  
    lDate.tm_mon = (Month) g_ui16RegisterDateMM;  
    lDate.tm_year = (unsigned int16) g_ui16RegisterDateYY - SYSTEM_BASE_DATE;  

    time_t lTimeEpoch = mktime(&lDate);
    return lTimeEpoch;
//lTimeEpoch
}

#endif /* _VM_TIME_CONFIG_H_ */

