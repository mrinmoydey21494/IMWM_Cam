
#ifndef _VI_SYSTEM_CONFIG_H_
#define _VI_SYSTEM_CONFIG_H_

//****************** Inclusion Files ***************************
#include "18F46K80.h"
#include "vmPINConfig.h"

//****************** Component Configuration ********************

//optimization level.
#pragma opt 8
 
//********* CPU Settings *******************************************

#pragma device HIGH_INTS=TRUE
#pragma fuses NOPLLEN,ECH,WDT1024,NOPROTECT,NOBROWNOUT,NOFCMEN,NOIESO,NOPUT /* WDT1024 => 1024 * 4 ms = 4096 mS Watchdog Timer */ /* For Field Mode */

#pragma use delay(clock=20000000,restart_wdt)


//******** Serial Interface ************
//#pragma use rs232(stream=RS485, xmit=PIN_RS485_XMIT, rcv=PIN_RS485_RCV, baud=9600, bits=8, parity=N, stop=1, restart_wdt)
#pragma use rs232(stream=ARD_RS232, xmit=PIN_LAN_COMM_XMIT, rcv=PIN_LAN_COMM_RCV, baud=9600, bits=8, parity=N, stop=1, restart_wdt)

//#define unsigned int8 BYTE
#ifndef M0_ADC_MODULE
#define M0_ADC_MODULE 1
#endif

#endif /* _VI_SYSTEM_CONFIG_H_ */