#ifndef _VI_PIN_CONFIG_H_
#define _VI_PIN_CONFIG_H_

//#include "18F46K80.h"

// DI PIN Mapping.
#define PIN_IN_DI_00               PIN_D4 
#define PIN_IN_DI_01               PIN_B0 
#define PIN_IN_DI_02               PIN_B1 
#define PIN_IN_DI_03               PIN_B2 
#define PIN_IN_DI_04               PIN_B4   
#define PIN_IN_DI_05               PIN_B5 
#define PIN_IN_DI_06               PIN_A0 
#define PIN_IN_DI_07               PIN_A1 

// D0 PIN Mapping.
#define PIN_OUT_DO_00              PIN_E0 
#define PIN_OUT_DO_01              PIN_E1 
#define PIN_OUT_DO_02              PIN_E2 
#define PIN_OUT_DO_03              PIN_C0 
#define PIN_OUT_DO_04              PIN_C1 
#define PIN_OUT_DO_05              PIN_D0
#define PIN_OUT_DO_06              PIN_D1
#define PIN_OUT_DO_07              PIN_D2


// LAN Communication Pin Mapping.
#define PIN_LAN_COMM_XMIT          PIN_C6
#define PIN_LAN_COMM_RCV    	   PIN_C7

// ADC Communication PIN Mapping; IC -> ADS1131(18-bit ADC).
#define PIN_ADC_DOUT_DRDY           PIN_C4
#define PIN_ADC_SCLK                PIN_C3
#define PIN_ADC_PDWN                PIN_C2


// IBC Communication PIN Mapping.
#define PIN_IBC_RCV                 PIN_B3
#define PIN_IBC_XMIT                PIN_D5

// UART To UART Communication PIN Mapping.
#define PIN_UART2_COMM_XMIT         PIN_D6
#define PIN_UART2_COMM_RCV          PIN_D7


#endif /* _VI_PIN_CONFIG_H_ */


