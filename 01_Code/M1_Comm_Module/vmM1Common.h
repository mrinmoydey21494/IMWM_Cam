/* 
 * File:   vmM1Common.h
 * Author: mrinmoy
 *
 * Created on July 8, 2019, 3:52 PM
 */

#ifndef VMM1COMMON_H
#define	VMM1COMMON_H

#define RS485_TX_BUFF_SIZE 		20
#define RS485_RX_BUFF_SIZE 		50

#define FLAG_RS485_DATA_READY                   1
#define FLAG_RS485_DATA_NOT_READY               0

#define FLAG_RS485_DATA_SEND_ON                 1
#define FLAG_RS485_DATA_SEND_DONE               2
#define FLAG_RS485_DATA_SEND_OFF                3

#define DELAY_TIME_RTS_BEFORE                       1
#define DELAY_TIME_RTS_AFTER                        2


unsigned int8 g_RS485TxBuffer[RS485_TX_BUFF_SIZE] = {0};
unsigned int8 g_RS485RxBuffer[RS485_RX_BUFF_SIZE] = {0};

unsigned int8  g_fRS485DataReady                  = FLAG_RS485_DATA_NOT_READY;
unsigned int8  g_ui8ByteToSend                    = 0;
unsigned int8  g_ui8fRS485DataSend                = FLAG_RS485_DATA_SEND_OFF;

#endif	/* VMM1COMMON_H */

