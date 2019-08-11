#ifndef _VM_PIN_CONFIG_H_
#define _VM_PIN_CONFIG_H_

//#include "18F46K80.h"

// RS485 Communication Pin Mapping.
#define PIN_RS485_XMIT              PIN_D6
#define PIN_RS485_RCV               PIN_D7
#define PIN_RS485_RTS               PIN_D4			

// GPSR Communication PIN Mapping; SIM900/SIM800/Quectel.
#define PIN_GPRS_COMM_XMIT          PIN_C6
#define PIN_GPRS_COMM_RCV           PIN_C7

// IBC Communication PIN Mapping.
#define PIN_IBC_RCV                 PIN_B1
#define PIN_IBC_XMIT                PIN_D5

// UART To UART Communication PIN Mapping.
#define PIN_SW_UART_COMM_XMIT       PIN_B2
#define PIN_SW_UART_COMM_RCV        PIN_B3

// Software UART PIN Mapping.
#define PIN_SW_UART_XMIT            PIN_B4
#define PIN_SW_UART_RCV             PIN_B0

// DI PIN Mapping;In 5V
#define PIN_IN_EXT_DI_00            PIN_C0
#define PIN_IN_EXT_DI_01            PIN_C1
#define PIN_IN_EXT_DI_02            PIN_C2
#define PIN_IN_EXT_DI_03            PIN_C3


#endif /* _VM_PIN_CONFIG_H_ */