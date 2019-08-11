#ifndef _VI_IMW_MSG_CONFIG_H_
#define _VI_IMW_MSG_CONFIG_H_

#include "viSystemConfig.h"
#include "viCommon.h"
//#include "viIMWIOManager.h"



#define SIZE_UART2_MSG_BUFFER 255

#define SIZE_MSG_HEADER 6    // STX STX STX DATA_LENGTH CMD STATUS_BYTE

#define STX  0xA5
#define ETX  0xD5

#define COUNT_ETX 3

#define CMD_MCU_PC_DATA_LOGGING_KL_MODE   20  // For Data Logging, Keep Alive(KL) in Production and Calibration Mode.
#define CMD_PC_MCU_SET_TOLERANCE_TABLE    30
#define CMD_PC_MCU_REQ_CALIB_TABLE        41
#define CMD_PC_MCU_SET_CALIB_POINT        42
#define CMD_PC_MCU_SET_CALIB_TABLE        43

#define MSG_STATUS_DATA_LOGG_PASSED        0
#define MSG_STATUS_DATA_LOGG_REJECTED      1
#define MSG_STATUS_DATA_LOGG_KEEP_ALIVE    99
#define MSG_STATUS_DATA_LOGG_KA_CALIB_MODE 100

#define MSG_STATUS_DATA_LOGG_REJECTION_OFSET 10


#define CRC_REQUIRED                       1
#define CRC_NOT_REQUIRED                   0

#define MSG_QUEUE_EMPTY                    0
#define MSG_QUEUE_NOT_EMPTY                1

//****************************************************************
//*********** Below Data Structure Implementation ****************
//****************************************************************
//typedef struct _ST_UART2_RX_BUFFER_MEMBER
//{
//	unsigned int8 m_ui8RxQueueFront;
//	unsigned int8 m_ui8RxQueueRear;
//	unsigned int8 m_ui8DataPacketETXFound;
//
//} ST_UART2_RX_BUFFER_MEMBER;

typedef struct _ST_UART2_RCV_BUFFER_QUEUE
{
	int16 m_i16Front;
	int16 m_i16Rear;
	BYTE  m_bCountETX;
	BYTE  m_bCountSTX;
	//BYTE  m_fMSGCurStatus;
	//BYTE  m_bDataLength;
    
    BYTE m_bArr[SIZE_UART2_MSG_BUFFER];
} ST_UART2_RCV_BUFFER_QUEUE;

//***************************************************************
//*********** Above Data Structure Implementation ***************
//***************************************************************


//***************************************************************
//********** Below Global Variable Declaration ******************
//***************************************************************
ST_UART2_RCV_BUFFER_QUEUE g_stUART2RcvBuff;

int16 g_i16CountETX = 0;

BYTE g_bArrSendBufferUART2[SIZE_UART2_MSG_BUFFER] = {0};

//***************************************************************
//******** Above Global Variable Declaration ********************
//***************************************************************


void InitUART2RxInterrupt(void)
{
	clear_interrupt(INT_RDA2);
	enable_interrupts(INT_RDA2);
    
    // Initialize message Queue
    memset(g_stUART2RcvBuff.m_bArr, 0, SIZE_UART2_MSG_BUFFER);
    g_stUART2RcvBuff.m_i16Front = -1;
    g_stUART2RcvBuff.m_i16Rear  = -1;
    g_stUART2RcvBuff.m_bCountETX = 0;
    g_stUART2RcvBuff.m_bCountSTX = 0;
}


BYTE GetQueue(BYTE* pbIsQueueEmpty )
{
    BYTE bData = 0;
    
    *pbIsQueueEmpty = MSG_QUEUE_NOT_EMPTY;

    if (-1 == g_stUART2RcvBuff.m_i16Front)
    {
        // Queue is empty
        *pbIsQueueEmpty = MSG_QUEUE_EMPTY;
        return 0;
    }
    
    bData = g_stUART2RcvBuff.m_bArr[g_stUART2RcvBuff.m_i16Front];
    g_stUART2RcvBuff.m_bArr[g_stUART2RcvBuff.m_i16Front] = 0;  // Fill the Queue by 0
    
    if (g_stUART2RcvBuff.m_i16Front == g_stUART2RcvBuff.m_i16Rear)
    {
        g_stUART2RcvBuff.m_i16Front = g_stUART2RcvBuff.m_i16Rear = -1;
    }
    else if ((SIZE_UART2_MSG_BUFFER - 1) == g_stUART2RcvBuff.m_i16Front)
    {
        g_stUART2RcvBuff.m_i16Front = 0;
    }
    else
    {
        g_stUART2RcvBuff.m_i16Front++;
    }
    
    // Return Data
    return bData;
}

//TESTING
unsigned int16 g_ui16Testing = 0;
BYTE g_bArrTest[SIZE_UART2_MSG_BUFFER] = {0};

//TESTING

#INT_RDA2
void UART2_ReceivedByte_ISR(void)
{
	
	// PC Communication Receive LED Status ON.
	if(PC_COMM_RX_LED_OFF == g_fPCCommRxLEDStatus)
	{
		g_fPCCommRxLEDStatus = PC_COMM_RX_LED_ON;
	}
	
	g_fPCCommRxLEDOffTimeCountStart = PC_RX_LED_OFF_START_TIME_COUNT;
	
    // We have received one Character 
    // Increment Front. 
    if ((0 == g_stUART2RcvBuff.m_i16Front && (SIZE_UART2_MSG_BUFFER - 1) == g_stUART2RcvBuff.m_i16Rear) ||
        (g_stUART2RcvBuff.m_i16Rear == (g_stUART2RcvBuff.m_i16Front - 1)))
    {
        // Queue is Full
    }
    else if (-1 == g_stUART2RcvBuff.m_i16Front ) // Insert First Element 
    {
        g_stUART2RcvBuff.m_i16Front = g_stUART2RcvBuff.m_i16Rear = 0;
        // Insert into Buffer Queue
        g_stUART2RcvBuff.m_bArr[g_stUART2RcvBuff.m_i16Rear] = fgetc(ARD_RS232); 
        
        //g_bArrTest[g_ui16Testing++] = g_stUART2RcvBuff.m_bArr[g_stUART2RcvBuff.m_i16Rear];  //TESTING
    }
    else if (((SIZE_UART2_MSG_BUFFER - 1) == g_stUART2RcvBuff.m_i16Rear) &&
             (0 != g_stUART2RcvBuff.m_i16Front))
    {
        g_stUART2RcvBuff.m_i16Rear = 0;
        g_stUART2RcvBuff.m_bArr[g_stUART2RcvBuff.m_i16Rear++] = fgetc(ARD_RS232); 
        
       //g_bArrTest[g_ui16Testing++] = g_stUART2RcvBuff.m_bArr[g_stUART2RcvBuff.m_i16Rear];  //TESTING
    }
    else
    {
        g_stUART2RcvBuff.m_i16Rear++;
        g_stUART2RcvBuff.m_bArr[g_stUART2RcvBuff.m_i16Rear] = fgetc(ARD_RS232);
        
       // g_bArrTest[g_ui16Testing++] = g_stUART2RcvBuff.m_bArr[g_stUART2RcvBuff.m_i16Rear];  //TESTING
    }
    
	if (ETX == g_stUART2RcvBuff.m_bArr[g_stUART2RcvBuff.m_i16Rear])
    {
        g_stUART2RcvBuff.m_bCountETX++;
    }

    //g_bArrTest[g_ui16Testing++] = fgetc(ARD_RS232);
//    if(++g_ui16Testing == 0x5A)
//    {
//    	g_ui16Testing = 0;
//    }
    
	clear_interrupt(INT_RDA2);
}



void SendUART2(BYTE bDataLength,
               BYTE bCMD,
               BYTE bStatus,
               BYTE fCRCRequired)
{
	
    BYTE bETXPosition = 0;
    BYTE bMSGLength = 0;
    unsigned int16 ui16CRCValue = 0;
    int i8Count = 0;
    
    // Stuff STX
    g_bArrSendBufferUART2[0] = g_bArrSendBufferUART2[1] = g_bArrSendBufferUART2[2] = STX;

	// Stuff protocol bytes.
    g_bArrSendBufferUART2[3] = bDataLength;
    g_bArrSendBufferUART2[4] = bCMD;
    g_bArrSendBufferUART2[5] = bStatus;
        
	// Stuff CRC.        
    if (CRC_REQUIRED == fCRCRequired)
    {
        // Calculate CRC   
///        ui16CRCValue = calculateCrc(g_bArrSendBufferUART2 + SIZE_MSG_HEADER, bDataLength); 
        g_bArrSendBufferUART2[SIZE_MSG_HEADER + bDataLength]   = (unsigned int8)((ui16CRCValue >> 0) & 0xFF);
        g_bArrSendBufferUART2[SIZE_MSG_HEADER + bDataLength + 1] = (unsigned int8)((ui16CRCValue >> 8) & 0xFF);
    }
    else
    {
        // No need to calculate CRC
        g_bArrSendBufferUART2[SIZE_MSG_HEADER + bDataLength] = 0;
        g_bArrSendBufferUART2[SIZE_MSG_HEADER + bDataLength + 1] = 0;
        //g_bArrSendBufferUART2[SIZE_MSG_HEADER + bDataLength] = (int8)(g_i16Mean);
        //g_bArrSendBufferUART2[SIZE_MSG_HEADER + bDataLength + 1] = (int8)(g_i16Mean >> 8);
    }
    
  
    // Stuff ETX
    bETXPosition = (3 + 3 + bDataLength + 2);
    g_bArrSendBufferUART2[bETXPosition] =
            g_bArrSendBufferUART2[bETXPosition + 1] = 
            g_bArrSendBufferUART2[bETXPosition + 2] = ETX;
    
    // Calculate total message length
    bMSGLength = bETXPosition + 3;
    
	// Set PC Communication  Tx LED On.
    SetSystemLedState(LED_PC_COMMUNICATION_TX,LOGICAL_ON);  
	g_fPCCommTxLEDOffTimeCountStart = PC_TX_LED_OFF_START_TIME_COUNT;
    
    // So Message Prepared:
    // Send message
    for (int8 i8LoopCount = 0; i8LoopCount < (bMSGLength + 1); i8LoopCount++)
    {
        fputc(g_bArrSendBufferUART2[i8LoopCount], ARD_RS232);
        
        if (i8Count++ > 15)
        {
            delay_ms(30);
            i8Count = 0;
        }    
    }
    
    //delay_ms(25);
    
    // Reset Keep Alive Counter.
    g_ui16KeepAliveCounter = KEEP_ALIVE_TIMEOUT;
    
    // Reset Buffer
    memset (g_bArrSendBufferUART2, 0, bMSGLength);
    
}


// This Function will Send PC Keep Alive through UART2
void SendPCKeepAlive()
{
//	UN_SHIPPER_WEIGHT unTemp;
//	unTemp.m_ui16ShipperWeight = 2300;
	
	if(MODE_PRODUCTION == g_bCurMode)
	{
		// Stuff 0 as data.
		g_bArrSendBufferUART2[SIZE_MSG_HEADER] = 0x00;
		g_bArrSendBufferUART2[SIZE_MSG_HEADER + 1] = 0x00;
		
		// Prepare the Message
		SendUART2(2, CMD_MCU_PC_DATA_LOGGING_KL_MODE , MSG_STATUS_DATA_LOGG_KEEP_ALIVE , CRC_NOT_REQUIRED);  
	}	
	else
	{
		// Stuff 0 as data.
		g_bArrSendBufferUART2[SIZE_MSG_HEADER] = 0x00;
		g_bArrSendBufferUART2[SIZE_MSG_HEADER + 1] = 0x00;
		// Prepare the Message.
		SendUART2(2, CMD_MCU_PC_DATA_LOGGING_KL_MODE , MSG_STATUS_DATA_LOGG_KA_CALIB_MODE , CRC_NOT_REQUIRED);
	}		
}
	
#endif /* _VI_IMW_MSG_CONFIG_H_ */


