#ifndef _VI_IBC_CONFIG_H_
#define _VI_IBC_CONFIG_H_

#include "vmSystemConfig.h"
#include "vmCRCConfig.h"
#include "vmDigitalIOConfig.h"
//#include "ctype.h"
#include "vmPINConfig.h"
#include "vmIBCGlobal.h"

#define IBC_MSG_BYTE_COUNT                       5
#define IBC_DATA_BITS_LENGTH                     10
#define IBC_DATA_BYTES_LENGTH                    10
#define IBC_BOARD_ID                             1

#define IBC_BAUD_RATE_9600                       20
#define IBC_BAUD_RATE_19200                      21

#define T1_DIV_PRESCALE                          T1_DIV_BY_8
#define T3_DIV_PRESCALE                          T3_DIV_BY_8

#define IBC_TX_1_BT_LOAD_VALUE_9600              (0xFFFF - 0x0041 + 1)
#define IBC_TX_1_BT_LOAD_VALUE_19200             (0xFFFF - 0x0021 + 1)

#define IBC_RX_BIT_TIME_1_5_T1LOAD_VAL_9600       (0xFFFF - 0x0062 + 1) // For 9600
#define IBC_RX_BIT_TIME_T1LOAD_VAL_9600           (0xFFFF - 0x0041 + 1) // For 9600

#ifdef M1_COMM_MODULE
    #define IBC_RX_1_BT_CUSTOMISED_VAL_9600        (0xFFFF - 0x0039 + 1)  // 78us Instead of 104us we load 91.2 for per bit time.
#else
    #define IBC_RX_1_BT_CUSTOMISED_VAL_9600        (0xFFFF - 0x0039 + 1)//   IBC_RX_BIT_TIME_T1LOAD_VAL_9600  // 104 us
#endif
//#define IBC_RX_1_BT_CUSTOMISED_VAL_9600           (0xFFFF - 0x0031 + 1)  // Instead of 104us we load 78 us for per bit time.
#define IBC_RX_BIT_TIME_1_2_T1LOAD_VAL_9600       (0xFFFF - 0x004E + 1)
#define IBC_RX_BIT_TIME_1_3_T1LOAD_VAL_9600       (0xFFFF - 0x0051 + 1)
#define IBC_RX_BIT_TIME_1_4_T1LOAD_VAL_9600       (0xFFFF - 0x005B + 1)


#define IBC_RX_BIT_TIME_1_5_T1LOAD_VAL_19200     (0xFFFF - 0x0031 + 1) // For 19200
#define IBC_RX_BIT_TIME_T1LOAD_VAL_19200         (0xFFFF - 0x0041 + 1) // For 19200

#define SIZE_IBC_SEND_BYTE_SEGMENT                15  //1+3+8+3, 1 Start Bit, 3 Sequence Bits, 8 bits of Data and 3 Stop Bits; Total 15bits in one segment/ custised byte

#define IBC_TIMER_0_CRITICAL_ON                   0
#define IBC_TIMER_0_CRITICAL_OFF                  1

//*****************************************************************************
// Below -> Data Structure for Interboard Communication ***********************
//*****************************************************************************



typedef struct _ST_INT_BOARD_ALL_BITS
{
    unsigned int8 m_ui1IBCRcvbit_0: 1;
    unsigned int8 m_ui1IBCRcvbit_1: 1;
    unsigned int8 m_ui1IBCRcvbit_2: 1;
    unsigned int8 m_ui1IBCRcvbit_3: 1;
    unsigned int8 m_ui1IBCRcvbit_4: 1;
    unsigned int8 m_ui1IBCRcvbit_5: 1;
    unsigned int8 m_ui1IBCRcvbit_6: 1;
    unsigned int8 m_ui1IBCRcvbit_7: 1;
} ST_INT_BOARD_ALL_BITS;

//typedef union _UN_INT_BOARD_BYTE_TYPE
//{
//    unsigned int8          m_ui8Byte;
//    ST_INT_BOARD_ALL_BITS  m_stByte;
//    char				   m_chData;	
//} UN_INT_BOARD_BYTE_TYPE;

typedef struct _ST_INT_BOARD_DATA
{
    char 			m_boardData[IBC_DATA_BITS_LENGTH];
    unsigned int8  	m_boardID;
} ST_INT_BOARD_DATA;


//*****************************************************************************
// Above -> Data Structure for Interboard Communication ***********************
//*****************************************************************************





//*****************************************************************************
// Below -> Interboard Communication  RX #  From M1_COMM **********************
//*****************************************************************************

// Below -> Multiple Baud Rate Settings.

#ifdef IBC_BOUD_RATE_1000
    #define IBC_RX_BIT_TIME_1_5_T1LOAD_VAL   (0xFFFF - 0x0EA6 + 1)
    #define IBC_RX_BIT_TIME_T1LOAD_VAL       (0xFFFF - 0x09C4 + 1)
#endif

#ifdef IBC_BOUD_RATE_9600
    #define IBC_RX_TIME_DELAY_1_5_BIT           156 
    #define IBC_RX_TIME_DELAY_1_BIT             104
    #define IBC_RX_BIT_TIME_1_5_T1LOAD_VAL   (0xFFFF - 0x0062 + 1)
    #define IBC_RX_BIT_TIME_T1LOAD_VAL       (0xFFFF - 0x0041 + 1)
#endif

#ifdef IBC_BOUD_RATE_19200
    #define IBC_RX_TIME_DELAY_1_5_VAL           78 
    #define IBC_RX_TIME_DELAY_1_VAL             52
    #define IBC_RX_BIT_TIME_1_5_T1LOAD_VAL   (0xFFFF - 0x0031 + 1)
    #define IBC_RX_BIT_TIME_T1LOAD_VAL       (0xFFFF - 0x0021 + 1)
#endif

// Above -> Multiple Baud Rate Settings.

// Initialize IBC Data Set, Should be called from main())
//void InitIBCDataSet()
//{
//    g_unTempRxData1.m_uiByte = 0;
//}

typedef struct _ST_IBC_RCV_BYTE_NODE
{

    unsigned int8 m_ui8SeqBit0 :1;
    unsigned int8 m_ui8SeqBit1 :1;
    unsigned int8 m_ui8SeqBit2 :1;
    unsigned int8 m_ui8Padding :5;
    
    unsigned int8 m_ui8DataBit0 :1;
    unsigned int8 m_ui8DataBit1 :1;
    unsigned int8 m_ui8DataBit2 :1;
    unsigned int8 m_ui8DataBit3 :1;
    unsigned int8 m_ui8DataBit4 :1;
    unsigned int8 m_ui8DataBit5 :1;
    unsigned int8 m_ui8DataBit6 :1;
    unsigned int8 m_ui8DataBit7 :1;
} ST_IBC_RCV_BYTE_NODE;

typedef union _UN_IBC_RCV_BYTE_NODE
{
    ST_IBC_RCV_BYTE_NODE m_stIBCNode;
    unsigned int8        m_ui8Arr[2];
} UN_IBC_RCV_BYTE_NODE;



typedef struct _ST_IBC_DATA_NODE
{
    unsigned int8 g_ui8ArrIBCData[IBC_MSG_BYTE_COUNT];
} ST_IBC_DATA_NODE;

#define SIZE_IBC_DATA_SEND_QUEUE 15

//typedef struct _ST_IBC_DATA_SEND_QUEUE
//{
//    ST_IBC_DATA_NODE m_stArrIBCDataNode[SIZE_IBC_DATA_SEND_QUEUE];
//    int8             m_i8Front;
//    int8             m_i8Rear;
//} ST_IBC_DATA_SEND_QUEUE;

typedef struct _ST_IBC_MSG_QUEUE
{
    ST_IBC_DATA_NODE m_stArrIBCDataNode[SIZE_IBC_DATA_SEND_QUEUE];
    int8             m_i8Front;
    int8             m_i8Rear;
} ST_IBC_MSG_QUEUE;
//*****************************************************************************
// Below -> Global Variable Declaration ***************************************
//*****************************************************************************

//UN_INT_BOARD_BYTE_TYPE    g_unIBCRcvData;
unsigned int8             g_ui8IBCRcvBitCount = 0;
unsigned int8             g_ui8IBCTxBitCount  = 0;

// TESTING
unsigned int8             g_ui8IBCDataToSend = 0xA5;
unsigned int8             g_fIBCCommStatus   = 0;

ST_IBC_DATA_NODE          g_stIBCRcvData;
ST_IBC_DATA_NODE          g_stIBCSendData;
unsigned int8             g_ui8ArrSendSingleMSG[IBC_MSG_BYTE_COUNT*(SIZE_IBC_SEND_BYTE_SEGMENT)] = {0}; //
unsigned int8             g_fIBCDataReceived = 0;

//ST_IBC_DATA_SEND_QUEUE    g_stIBCDataSendQueue;
unsigned int8             g_fIBCSendBusy = 0;
UN_IBC_RCV_BYTE_NODE      g_unIBCRcvByteNode;

ST_IBC_MSG_QUEUE          g_stIBCSendQueue;
ST_IBC_MSG_QUEUE          g_stIBCRcvQueue;

unsigned int8             g_ui8fTimer0State   = IBC_TIMER_0_CRITICAL_OFF;
//*****************************************************************************
// Above -> Global Variable Declaration ***************************************
//*****************************************************************************

//////////
//////////void InitializeDataSendQueue()
//////////{
//////////    int iLoopCount1 = 0;
//////////    int iLoopCount2 = 0;
//////////    g_stIBCDataSendQueue.m_i8Front = g_stIBCDataSendQueue.m_i8Rear = -1;
//////////    
//////////    for (iLoopCount1 = 0; iLoopCount1 < SIZE_IBC_DATA_SEND_QUEUE; iLoopCount1++)
//////////    {
//////////        for (iLoopCount2 = 0; iLoopCount2 < IBC_MSG_BYTE_COUNT; iLoopCount2++)
//////////        {
//////////           g_stIBCDataSendQueue.m_stArrIBCDataNode[iLoopCount1].g_ui8ArrIBCData[iLoopCount2] = 0;
//////////        }
//////////    }
//////////}
//////////
//////////void CopySendData(int8 i8Location, ST_IBC_DATA_NODE* pstIBCData)
//////////{
//////////    int8 i8LoopCount = 0;
//////////    
//////////    for (i8LoopCount = 0; i8LoopCount < IBC_MSG_BYTE_COUNT; i8LoopCount++)
//////////    {
//////////        g_stIBCDataSendQueue.m_stArrIBCDataNode[i8Location].g_ui8ArrIBCData[i8LoopCount] = pstIBCData->g_ui8ArrIBCData[i8LoopCount];
//////////    }
//////////}
//////////
//////////void CopyFromSendQueue(int8 i8Location, ST_IBC_DATA_NODE** ppstIBCData)
//////////{
//////////    int8 i8LoopCount = 0;
//////////    
//////////    for (i8LoopCount = 0; i8LoopCount < IBC_MSG_BYTE_COUNT; i8LoopCount++)
//////////    {
//////////        *ppstIBCData->g_ui8ArrIBCData[i8LoopCount] = g_stIBCDataSendQueue.m_stArrIBCDataNode[i8Location].g_ui8ArrIBCData[i8LoopCount];
//////////    }
//////////}
/* Function to create Circular queue */
//////////void InsertSendQueue(ST_IBC_DATA_NODE* pstIBCData) 
//////////{
//////////    if (!pstIBCData)
//////////    {
//////////        return;
//////////    }
//////////    
//////////    if ((g_stIBCDataSendQueue.m_i8Front == 0 && g_stIBCDataSendQueue.m_i8Rear == SIZE_IBC_DATA_SEND_QUEUE-1) || 
//////////            (g_stIBCDataSendQueue.m_i8Front == g_stIBCDataSendQueue.m_i8Rear + 1)) 
//////////    { 
//////////        //printf("\nQueue is Full"); 
//////////        return; 
//////////    }
//////////  
//////////    else if (g_stIBCDataSendQueue.m_i8Front == -1) /* Insert First Element */
//////////    { 
//////////        g_stIBCDataSendQueue.m_i8Front = g_stIBCDataSendQueue.m_i8Rear = 0;
//////////        
//////////        // Insert Data
//////////        //arr[g_stIBCDataSendQueue.m_i8Rear] = value;
//////////        CopySendData(g_stIBCDataSendQueue.m_i8Rear, pstIBCData);
//////////    } 
//////////  
//////////    else if (g_stIBCDataSendQueue.m_i8Rear == SIZE_IBC_DATA_SEND_QUEUE-1 && g_stIBCDataSendQueue.m_i8Front != 0) 
//////////    { 
//////////        g_stIBCDataSendQueue.m_i8Rear = 0; 
//////////        CopySendData(g_stIBCDataSendQueue.m_i8Rear, pstIBCData); 
//////////    } 
//////////  
//////////    else
//////////    { 
//////////        g_stIBCDataSendQueue.m_i8Rear++; 
//////////        CopySendData(g_stIBCDataSendQueue.m_i8Rear, pstIBCData);
//////////    } 
//////////} 

//////////void DeleteQueue(ST_IBC_DATA_NODE* pstIBCData) 
//////////{ 
//////////    int8 i8LoopCount = 0;
//////////
//////////    if (g_stIBCDataSendQueue.m_i8Front == -1)
//////////    { 
//////////        //printf("\nQueue is Empty"); 
//////////        return; 
//////////    } 
//////////  
//////////    //int data = arr[g_stIBCDataSendQueue.m_i8Front];
//////////    //CopyFromSendQueue(g_stIBCDataSendQueue.m_i8Front, &pstIBCData);
//////////    for (i8LoopCount = 0; i8LoopCount < IBC_MSG_BYTE_COUNT; i8LoopCount++)
//////////    {
//////////        pstIBCData->g_ui8ArrIBCData[i8LoopCount] = g_stIBCDataSendQueue.m_stArrIBCDataNode[g_stIBCDataSendQueue.m_i8Front].g_ui8ArrIBCData[i8LoopCount];
//////////    }
//////////    
//////////    //arr[g_stIBCDataSendQueue.m_i8Front] = -1; 
//////////    if (g_stIBCDataSendQueue.m_i8Front == g_stIBCDataSendQueue.m_i8Rear) 
//////////    { 
//////////        g_stIBCDataSendQueue.m_i8Front = -1; 
//////////        g_stIBCDataSendQueue.m_i8Rear = -1; 
//////////    } 
//////////    else if (g_stIBCDataSendQueue.m_i8Front == SIZE_IBC_DATA_SEND_QUEUE-1) 
//////////        g_stIBCDataSendQueue.m_i8Front = 0; 
//////////    else
//////////        g_stIBCDataSendQueue.m_i8Front++; 
//////////  
//////////    return;
//////////} 
//////////
//////////int8 IsMSGAvailableInIBCSendQueue()
//////////{
//////////    if (g_stIBCDataSendQueue.m_i8Front == -1)
//////////    {
//////////        return 0;
//////////    }
//////////    
//////////    return 1;
//////////}
//////////
//////////void SendIBCMSG()
//////////{
//////////    int8          i8LoopCount    = 0;
//////////    unsigned int8 ui8Sequence    = 0;
//////////    unsigned int8 ui8TempData    = 0;
//////////    
//////////    // Initialize the Send Array
//////////    for (i8LoopCount = 0; i8LoopCount < IBC_MSG_BYTE_COUNT; i8LoopCount++)
//////////    {
//////////        g_stIBCSendData.g_ui8ArrIBCData[i8LoopCount] = 0;
//////////    }
//////////
//////////    // Get Node from Queue
//////////    DeleteQueue(&g_stIBCSendData);
//////////    for (i8LoopCount = 0; i8LoopCount < IBC_MSG_BYTE_COUNT; i8LoopCount++)
//////////    {
//////////        // Step 1: First Stuff 1 Start Bit
//////////        g_ui8ArrSendSingleMSG[(SIZE_IBC_SEND_BYTE_SEGMENT*i8LoopCount) + 0] = 0;   // Start Bit Always 0
//////////        
//////////        // Step 2: Now Stuff 3 bit Message Byte Sequence Number
//////////        
//////////        // Stuff the 1st Sequence Bit
//////////        ui8Sequence = i8LoopCount;
//////////        ui8Sequence = ui8Sequence << 7;
//////////        ui8Sequence = ui8Sequence >> 7;
//////////        g_ui8ArrSendSingleMSG[(SIZE_IBC_SEND_BYTE_SEGMENT*i8LoopCount) + 1] = ui8Sequence;
//////////        
//////////        // Stuff the 2nd Sequence Bit 
//////////        ui8Sequence = i8LoopCount;
//////////        ui8Sequence = ui8Sequence << 6;
//////////        ui8Sequence = ui8Sequence >> 7;
//////////        g_ui8ArrSendSingleMSG[(SIZE_IBC_SEND_BYTE_SEGMENT*i8LoopCount) + 2] = ui8Sequence;
//////////        
//////////        // Stuff the 3rd Bit
//////////        ui8Sequence = i8LoopCount;
//////////        ui8Sequence = ui8Sequence << 5;
//////////        ui8Sequence = ui8Sequence >> 7;
//////////        g_ui8ArrSendSingleMSG[(SIZE_IBC_SEND_BYTE_SEGMENT*i8LoopCount) + 3] = ui8Sequence;
//////////        
//////////        // Step 3: Now Stuff 8 bit Data
//////////        ui8TempData = g_stIBCSendData.g_ui8ArrIBCData[i8LoopCount];
//////////        ui8TempData = ui8TempData << 7;
//////////        ui8TempData = ui8TempData >> 7;
//////////        g_ui8ArrSendSingleMSG[(SIZE_IBC_SEND_BYTE_SEGMENT*i8LoopCount) + 4] = ui8TempData;
//////////        
//////////        ui8TempData = g_stIBCSendData.g_ui8ArrIBCData[i8LoopCount];
//////////        ui8TempData = ui8TempData << 6;
//////////        ui8TempData = ui8TempData >> 7;
//////////        g_ui8ArrSendSingleMSG[(SIZE_IBC_SEND_BYTE_SEGMENT*i8LoopCount) + 5] = ui8TempData;
//////////        
//////////        ui8TempData = g_stIBCSendData.g_ui8ArrIBCData[i8LoopCount];
//////////        ui8TempData = ui8TempData << 5;
//////////        ui8TempData = ui8TempData >> 7;
//////////        g_ui8ArrSendSingleMSG[(SIZE_IBC_SEND_BYTE_SEGMENT*i8LoopCount) + 6] = ui8TempData;
//////////        
//////////        ui8TempData = g_stIBCSendData.g_ui8ArrIBCData[i8LoopCount];
//////////        ui8TempData = ui8TempData << 4;
//////////        ui8TempData = ui8TempData >> 7;
//////////        g_ui8ArrSendSingleMSG[(SIZE_IBC_SEND_BYTE_SEGMENT*i8LoopCount) + 7] = ui8TempData;
//////////        
//////////        ui8TempData = g_stIBCSendData.g_ui8ArrIBCData[i8LoopCount];
//////////        ui8TempData = ui8TempData << 3;
//////////        ui8TempData = ui8TempData >> 7;
//////////        g_ui8ArrSendSingleMSG[(SIZE_IBC_SEND_BYTE_SEGMENT*i8LoopCount) + 8] = ui8TempData;
//////////        
//////////        ui8TempData = g_stIBCSendData.g_ui8ArrIBCData[i8LoopCount];
//////////        ui8TempData = ui8TempData << 2;
//////////        ui8TempData = ui8TempData >> 7;
//////////        g_ui8ArrSendSingleMSG[(SIZE_IBC_SEND_BYTE_SEGMENT*i8LoopCount) + 9] = ui8TempData;
//////////        
//////////        ui8TempData = g_stIBCSendData.g_ui8ArrIBCData[i8LoopCount];
//////////        ui8TempData = ui8TempData << 1;
//////////        ui8TempData = ui8TempData >> 7;
//////////        g_ui8ArrSendSingleMSG[(SIZE_IBC_SEND_BYTE_SEGMENT*i8LoopCount) + 10] = ui8TempData;
//////////        
//////////        ui8TempData = g_stIBCSendData.g_ui8ArrIBCData[i8LoopCount];
//////////        ui8TempData = ui8TempData >> 7;
//////////        g_ui8ArrSendSingleMSG[(SIZE_IBC_SEND_BYTE_SEGMENT*i8LoopCount) + 11] = ui8TempData;
//////////        
//////////        // Step 4: Stuff three Stop Bits
//////////        g_ui8ArrSendSingleMSG[(SIZE_IBC_SEND_BYTE_SEGMENT*i8LoopCount) + 12] = 1;  // Stop bit is 1
//////////        g_ui8ArrSendSingleMSG[(SIZE_IBC_SEND_BYTE_SEGMENT*i8LoopCount) + 13] = 1;
//////////        g_ui8ArrSendSingleMSG[(SIZE_IBC_SEND_BYTE_SEGMENT*i8LoopCount) + 14] = 1;
//////////    }
//////////    
//////////    // So message stuffing is done now start the send timer
//////////    // Initialize send counter.
//////////    g_ui8IBCTxBitCount = 0;
//////////    
//////////    // TESTING Start
////////////    g_ui8ArrSendSingleMSG[0] = 0;
////////////    g_ui8ArrSendSingleMSG[1] = 1;
////////////    g_ui8ArrSendSingleMSG[2] = 0;
////////////    g_ui8ArrSendSingleMSG[3] = 1;
////////////    g_ui8ArrSendSingleMSG[4] = 0;
////////////    g_ui8ArrSendSingleMSG[5] = 1;
////////////    g_ui8ArrSendSingleMSG[6] = 0;
////////////    g_ui8ArrSendSingleMSG[7] = 1;
////////////    g_ui8ArrSendSingleMSG[8] = 0;
////////////    g_ui8ArrSendSingleMSG[9] = 1;
////////////    g_ui8ArrSendSingleMSG[10] = 0;
////////////    g_ui8ArrSendSingleMSG[11] = 1;
////////////    g_ui8ArrSendSingleMSG[12] = 1;
////////////    g_ui8ArrSendSingleMSG[13] = 1;
////////////    g_ui8ArrSendSingleMSG[14] = 1;    
//////////    // TESTING End
//////////    
//////////    // Start Timer3 that handle bit by bit sending.
//////////    // Set Timer3 as per Bit Time Value.
//////////    setup_timer_3(T3_INTERNAL | T3_DIV_PRESCALE);
//////////    set_timer3(IBC_TX_1_BT_LOAD_VALUE_9600);
//////////    
//////////    clear_interrupt(INT_TIMER3);
//////////    enable_interrupts(INT_TIMER3);
//////////}

////////////// Common QUEUE for Data Sending and Receiving START ///////////////

void InitializeDataQueue(ST_IBC_MSG_QUEUE *pstIBCMSGQueue)
{
    int iLoopCount1 = 0;
    int iLoopCount2 = 0;
    pstIBCMSGQueue->m_i8Front = pstIBCMSGQueue->m_i8Rear = -1;
    
    for (iLoopCount1 = 0; iLoopCount1 < SIZE_IBC_DATA_SEND_QUEUE; iLoopCount1++)
    {
        for (iLoopCount2 = 0; iLoopCount2 < IBC_MSG_BYTE_COUNT; iLoopCount2++)
        {
           pstIBCMSGQueue->m_stArrIBCDataNode[iLoopCount1].g_ui8ArrIBCData[iLoopCount2] = 0;
        }
    }
}

void CopyIBCMsg(ST_IBC_DATA_NODE* pstIBCSource, ST_IBC_DATA_NODE* pstIBCDDest)
{
    int8 i8LoopCount = 0;
    
    for (i8LoopCount = 0; i8LoopCount < IBC_MSG_BYTE_COUNT; i8LoopCount++)
    {
        pstIBCDDest->g_ui8ArrIBCData[i8LoopCount] = pstIBCSource->g_ui8ArrIBCData[i8LoopCount];
    }
}

/* Function to create Circular queue */
void InsertIBCMsgQueue(ST_IBC_MSG_QUEUE *pstIBCMSGQueue, ST_IBC_DATA_NODE* pstIBCData) 
{
    if (!pstIBCData)
    {
        return;
    }
    
    if ((pstIBCMSGQueue->m_i8Front == 0 && pstIBCMSGQueue->m_i8Rear == SIZE_IBC_DATA_SEND_QUEUE-1) || 
            (pstIBCMSGQueue->m_i8Rear == (pstIBCMSGQueue->m_i8Front-1)%(SIZE_IBC_DATA_SEND_QUEUE-1))) 
    { 
        //printf("\nQueue is Full"); 
        return;
    }
  
    else if (pstIBCMSGQueue->m_i8Front == -1) /* Insert First Element */
    { 
        pstIBCMSGQueue->m_i8Front = pstIBCMSGQueue->m_i8Rear = 0;
        
        // Insert Data
        //arr[g_stIBCDataSendQueue.m_i8Rear] = value;
        CopyIBCMsg(pstIBCData, &(pstIBCMSGQueue->m_stArrIBCDataNode[pstIBCMSGQueue->m_i8Rear]));
    } 
  
    else if (pstIBCMSGQueue->m_i8Rear == SIZE_IBC_DATA_SEND_QUEUE-1 && pstIBCMSGQueue->m_i8Front != 0) 
    { 
        pstIBCMSGQueue->m_i8Rear = 0; 
        CopyIBCMsg(pstIBCData, &(pstIBCMSGQueue->m_stArrIBCDataNode[pstIBCMSGQueue->m_i8Rear]));
    } 
  
    else
    { 
        pstIBCMSGQueue->m_i8Rear++; 
        CopyIBCMsg(pstIBCData, &(pstIBCMSGQueue->m_stArrIBCDataNode[pstIBCMSGQueue->m_i8Rear]));
    } 
} 

void DeleteIBCMsgQueue(ST_IBC_MSG_QUEUE *pstIBCMSGQueue, ST_IBC_DATA_NODE* pstIBCData) 
{
    if (pstIBCMSGQueue->m_i8Front == -1)
    { 
        //printf("\nQueue is Empty"); 
        return; 
    } 

    // Copy Message Node from Front
    CopyIBCMsg(&(pstIBCMSGQueue->m_stArrIBCDataNode[pstIBCMSGQueue->m_i8Front]), pstIBCData);

    if (pstIBCMSGQueue->m_i8Front == pstIBCMSGQueue->m_i8Rear) 
    { 
        pstIBCMSGQueue->m_i8Front = -1; 
        pstIBCMSGQueue->m_i8Rear = -1; 
    } 
    else if (pstIBCMSGQueue->m_i8Front == SIZE_IBC_DATA_SEND_QUEUE-1) 
        pstIBCMSGQueue->m_i8Front = 0; 
    else
        pstIBCMSGQueue->m_i8Front++; 
  
    return;
}


int8 IsMSGInIBCQueue(ST_IBC_MSG_QUEUE *pstIBCMSGQueue)
{
    if (pstIBCMSGQueue->m_i8Front == -1)
    {
        return 0;
    }
    
    return 1;
}

////////////// Common QUEUE for Data Sending and Receiving END /////////////////
void SendIBCMSGFromQ()
{
    int8          i8LoopCount    = 0;
    unsigned int8 ui8Sequence    = 0;
    unsigned int8 ui8TempData    = 0;

    // Initialize the Send Array
    for (i8LoopCount = 0; i8LoopCount < IBC_MSG_BYTE_COUNT; i8LoopCount++)
    {
        g_stIBCSendData.g_ui8ArrIBCData[i8LoopCount] = 0;
    }

    // Get Node from Queue
    DeleteIBCMsgQueue(&g_stIBCSendQueue, &g_stIBCSendData);
    for (i8LoopCount = 0; i8LoopCount < IBC_MSG_BYTE_COUNT; i8LoopCount++)
    {
        // Step 1: First Stuff 1 Start Bit
        g_ui8ArrSendSingleMSG[(SIZE_IBC_SEND_BYTE_SEGMENT*i8LoopCount) + 0] = 0;   // Start Bit Always 0
        
        // Step 2: Now Stuff 3 bit Message Byte Sequence Number
        
        // Stuff the 1st Sequence Bit
        ui8Sequence = i8LoopCount;
        ui8Sequence = ui8Sequence << 7;
        ui8Sequence = ui8Sequence >> 7;
        g_ui8ArrSendSingleMSG[(SIZE_IBC_SEND_BYTE_SEGMENT*i8LoopCount) + 1] = ui8Sequence;
        
        // Stuff the 2nd Sequence Bit 
        ui8Sequence = i8LoopCount;
        ui8Sequence = ui8Sequence << 6;
        ui8Sequence = ui8Sequence >> 7;
        g_ui8ArrSendSingleMSG[(SIZE_IBC_SEND_BYTE_SEGMENT*i8LoopCount) + 2] = ui8Sequence;
        
        // Stuff the 3rd Bit
        ui8Sequence = i8LoopCount;
        ui8Sequence = ui8Sequence << 5;
        ui8Sequence = ui8Sequence >> 7;
        g_ui8ArrSendSingleMSG[(SIZE_IBC_SEND_BYTE_SEGMENT*i8LoopCount) + 3] = ui8Sequence;
        
        // Step 3: Now Stuff 8 bit Data
        ui8TempData = g_stIBCSendData.g_ui8ArrIBCData[i8LoopCount];
        ui8TempData = ui8TempData << 7;
        ui8TempData = ui8TempData >> 7;
        g_ui8ArrSendSingleMSG[(SIZE_IBC_SEND_BYTE_SEGMENT*i8LoopCount) + 4] = ui8TempData;
        
        ui8TempData = g_stIBCSendData.g_ui8ArrIBCData[i8LoopCount];
        ui8TempData = ui8TempData << 6;
        ui8TempData = ui8TempData >> 7;
        g_ui8ArrSendSingleMSG[(SIZE_IBC_SEND_BYTE_SEGMENT*i8LoopCount) + 5] = ui8TempData;
        
        ui8TempData = g_stIBCSendData.g_ui8ArrIBCData[i8LoopCount];
        ui8TempData = ui8TempData << 5;
        ui8TempData = ui8TempData >> 7;
        g_ui8ArrSendSingleMSG[(SIZE_IBC_SEND_BYTE_SEGMENT*i8LoopCount) + 6] = ui8TempData;
        
        ui8TempData = g_stIBCSendData.g_ui8ArrIBCData[i8LoopCount];
        ui8TempData = ui8TempData << 4;
        ui8TempData = ui8TempData >> 7;
        g_ui8ArrSendSingleMSG[(SIZE_IBC_SEND_BYTE_SEGMENT*i8LoopCount) + 7] = ui8TempData;
        
        ui8TempData = g_stIBCSendData.g_ui8ArrIBCData[i8LoopCount];
        ui8TempData = ui8TempData << 3;
        ui8TempData = ui8TempData >> 7;
        g_ui8ArrSendSingleMSG[(SIZE_IBC_SEND_BYTE_SEGMENT*i8LoopCount) + 8] = ui8TempData;
        
        ui8TempData = g_stIBCSendData.g_ui8ArrIBCData[i8LoopCount];
        ui8TempData = ui8TempData << 2;
        ui8TempData = ui8TempData >> 7;
        g_ui8ArrSendSingleMSG[(SIZE_IBC_SEND_BYTE_SEGMENT*i8LoopCount) + 9] = ui8TempData;
        
        ui8TempData = g_stIBCSendData.g_ui8ArrIBCData[i8LoopCount];
        ui8TempData = ui8TempData << 1;
        ui8TempData = ui8TempData >> 7;
        g_ui8ArrSendSingleMSG[(SIZE_IBC_SEND_BYTE_SEGMENT*i8LoopCount) + 10] = ui8TempData;
        
        ui8TempData = g_stIBCSendData.g_ui8ArrIBCData[i8LoopCount];
        ui8TempData = ui8TempData >> 7;
        g_ui8ArrSendSingleMSG[(SIZE_IBC_SEND_BYTE_SEGMENT*i8LoopCount) + 11] = ui8TempData;
        
        // Step 4: Stuff three Stop Bits
        g_ui8ArrSendSingleMSG[(SIZE_IBC_SEND_BYTE_SEGMENT*i8LoopCount) + 12] = 1;  // Stop bit is 1
        g_ui8ArrSendSingleMSG[(SIZE_IBC_SEND_BYTE_SEGMENT*i8LoopCount) + 13] = 1;
        g_ui8ArrSendSingleMSG[(SIZE_IBC_SEND_BYTE_SEGMENT*i8LoopCount) + 14] = 1;
        
////////        // TESTING Start
////////        g_ui8ArrSendSingleMSG[(SIZE_IBC_SEND_BYTE_SEGMENT*i8LoopCount) + 0] = 0;  
////////        g_ui8ArrSendSingleMSG[(SIZE_IBC_SEND_BYTE_SEGMENT*i8LoopCount) + 1] = 1;
////////        g_ui8ArrSendSingleMSG[(SIZE_IBC_SEND_BYTE_SEGMENT*i8LoopCount) + 2] = 0;
////////        g_ui8ArrSendSingleMSG[(SIZE_IBC_SEND_BYTE_SEGMENT*i8LoopCount) + 3] = 1; 
////////        g_ui8ArrSendSingleMSG[(SIZE_IBC_SEND_BYTE_SEGMENT*i8LoopCount) + 4] = 0;
////////        g_ui8ArrSendSingleMSG[(SIZE_IBC_SEND_BYTE_SEGMENT*i8LoopCount) + 5] = 1;
////////        g_ui8ArrSendSingleMSG[(SIZE_IBC_SEND_BYTE_SEGMENT*i8LoopCount) + 6] = 0;
////////        g_ui8ArrSendSingleMSG[(SIZE_IBC_SEND_BYTE_SEGMENT*i8LoopCount) + 7] = 1;
////////        g_ui8ArrSendSingleMSG[(SIZE_IBC_SEND_BYTE_SEGMENT*i8LoopCount) + 8] = 0;
////////        g_ui8ArrSendSingleMSG[(SIZE_IBC_SEND_BYTE_SEGMENT*i8LoopCount) + 9] = 1;
////////        g_ui8ArrSendSingleMSG[(SIZE_IBC_SEND_BYTE_SEGMENT*i8LoopCount) + 10] = 0;
////////        g_ui8ArrSendSingleMSG[(SIZE_IBC_SEND_BYTE_SEGMENT*i8LoopCount) + 11] = 1;
////////        g_ui8ArrSendSingleMSG[(SIZE_IBC_SEND_BYTE_SEGMENT*i8LoopCount) + 12] = 1;
////////        g_ui8ArrSendSingleMSG[(SIZE_IBC_SEND_BYTE_SEGMENT*i8LoopCount) + 13] = 1;
////////        g_ui8ArrSendSingleMSG[(SIZE_IBC_SEND_BYTE_SEGMENT*i8LoopCount) + 14] = 1;
////////        // TESTING End
    }
    
    
////    // TESTING Start
////    g_ui8ArrSendSingleMSG[0] = 0;
////    g_ui8ArrSendSingleMSG[1] = 1;
////    g_ui8ArrSendSingleMSG[2] = 0;
////    g_ui8ArrSendSingleMSG[3] = 1;
////    g_ui8ArrSendSingleMSG[4] = 0;
////    g_ui8ArrSendSingleMSG[5] = 1;
////    g_ui8ArrSendSingleMSG[6] = 0;
////    g_ui8ArrSendSingleMSG[7] = 1;
////    g_ui8ArrSendSingleMSG[8] = 0;
////    g_ui8ArrSendSingleMSG[9] = 1;
////    g_ui8ArrSendSingleMSG[10] = 0;
////    g_ui8ArrSendSingleMSG[11] = 1;
////    g_ui8ArrSendSingleMSG[12] = 1;
////    g_ui8ArrSendSingleMSG[13] = 1;
////    g_ui8ArrSendSingleMSG[14] = 1;    
////    // TESTING End
        
    // So message stuffing is done now start the send timer
    // Initialize send counter.
    g_ui8IBCTxBitCount = 0;
    
    // Hold the IBC Send Lock
    g_fIBCSendLockStatus = IBC_SEND_LOCK;
    // Start Timer3 that handle bit by bit sending.
    // Set Timer3 as per Bit Time Value.
    setup_timer_3(T3_INTERNAL | T3_DIV_PRESCALE);
    set_timer3(IBC_TX_1_BT_LOAD_VALUE_9600);
    
    // Before Sending Data in IBC Run Timer0 in Time Critical. This we are doing
    // just because we have seen the IBC Send Receive is affecting while Timer 0
    // is executing, hence the IBC data send receive is not sampled at proper time
    g_ui8fTimer0State   = IBC_TIMER_0_CRITICAL_ON;
    
    clear_interrupt(INT_TIMER3);
    enable_interrupts(INT_TIMER3);
}

void IBCProcessRcvMsg()
{
    // I
}

#INT_TIMER3    // Used for IBC Send
void IBC_Tx_ISR(void)
{
    // Send IBC bits
    output_bit(PIN_IBC_XMIT, g_ui8ArrSendSingleMSG[g_ui8IBCTxBitCount++]);
    // Reset Timer3 at 1Bit Time value.
    set_timer3(IBC_TX_1_BT_LOAD_VALUE_9600);
    clear_interrupt( INT_TIMER3 );

    // Check if IBC Sending completed
    if ((IBC_MSG_BYTE_COUNT*SIZE_IBC_SEND_BYTE_SEGMENT) == g_ui8IBCTxBitCount)
    {
        // First release Timer0 Critical run lock
        g_ui8fTimer0State   = IBC_TIMER_0_CRITICAL_OFF;
        
        // Release Send Busy status
        g_fIBCSendBusy     = 0;
        
        // Send Stop bit forever
        output_bit(PIN_IBC_XMIT, 1);

        // Reset Tx counter
        g_ui8IBCTxBitCount = 0;
        // Reset IBC Send checking status flag.
        //g_fIBCCommStatus = 0;
        // Disable Timer
        setup_timer_3 (T3_DISABLED);
        disable_interrupts(INT_TIMER3);
        // Clear Timer1 interrupt
        clear_interrupt(INT_TIMER3);
        
        // Release IBC Send Lock
        g_fIBCSendLockStatus = IBC_SEND_UNLOCK;
    }
    
    // TESTING
    //output_toggle(PIN_IN_EXT_DI_00);
 
}



/*
    Timer1 is used to handle IBC Received bytes.
 and Timer3 is used to handle IBC sent bytes.
 */
#ifdef M1_COMM_MODULE
#INT_EXT1 
#else
#INT_EXT3
#endif
void IBC_Rx_ISR(void)
{
  
    // Initialize Rx Bit Counter.
   	g_ui8IBCRcvBitCount = 0;
    g_unIBCRcvByteNode.m_ui8Arr[0] = 0;
    g_unIBCRcvByteNode.m_ui8Arr[1] = 0;
   	
    // Setup Timer1 at 1.5 Bit Time.                   // Here we are using DIV_BY_8.
    setup_timer_1( T1_INTERNAL | T1_DIV_PRESCALE ); // CPU Ext Clock is 20MHz, 20/4 => 5MHz is Peripheral Clock
                                                // => T1_DIV_BY_2 => 5/2 = 2.5MHz => (1/2.5)us = 0.4us
                                                // To Get 1Ms quanta = (1000us/0.4) = 2500 => 0x09C4
                                                // So, The timer will start counting from (0xFFFF - 0x09C4 + 1)
                                                // To Get 1.5Ms quanta = (1500us/0.4) = 3750 => 0x0EA6
                                                // So, The timer will start counting from (0xFFFF - 0x0EA6 + 1) 
    
#ifdef M1_COMM_MODULE
    set_timer1( IBC_RX_BIT_TIME_1_4_T1LOAD_VAL_9600); 
#else
    set_timer1( IBC_RX_BIT_TIME_1_5_T1LOAD_VAL_9600);
#endif
    
    // Before Sending Data in IBC Run Timer0 in Time Critical. This we are doing
    // just because we have seen the IBC Send Receive is affecting while Timer 0
    // is executing, hence the IBC data send receive is not sampled at proper time
    g_ui8fTimer0State   = IBC_TIMER_0_CRITICAL_ON;
    
    clear_interrupt( INT_TIMER1 );
    enable_interrupts( INT_TIMER1 );
    
    // Clear and Disable External Interrupt 1 
#ifdef M1_COMM_MODULE
    clear_interrupt(INT_EXT1);
    disable_interrupts(INT_EXT1); 
#else
    clear_interrupt(INT_EXT3);
    disable_interrupts(INT_EXT3);
#endif
    
    // TESTING Start
#ifdef M1_COMM_MODULE
    //output_high(PIN_IN_EXT_DI_00);
#endif
    // TESTING End
}

// TESTING Start
int8 g_i8RcvByteCount = 0;
// TESTING End

#INT_TIMER1           // This is for Receive                
void IBC_Rx_Sampling_ISR(void)
{ 
////////    // TESTING Start
////////#ifdef M1_COMM_MODULE
////////    //output_toggle(PIN_IN_EXT_DI_00);
////////#else
////////    output_toggle(PIN_A5);
////////#endif
////////    // TESTING End
    
    // Increment Rx Bit Counter for each received bit.
   	g_ui8IBCRcvBitCount++;
	
    // Check Rx Bit Counter and read Bits
    switch(g_ui8IBCRcvBitCount)
    {
        case 1: // Sequence Bit 0 
            g_unIBCRcvByteNode.m_stIBCNode.m_ui8SeqBit0 = input(PIN_IBC_RCV);
            // Setup Timer1 at 1 Bit Time.
            set_timer1(IBC_RX_BIT_TIME_T1LOAD_VAL_9600);
            clear_interrupt( INT_TIMER1 );
            break;
        case 2: // Sequence Bit 1
            g_unIBCRcvByteNode.m_stIBCNode.m_ui8SeqBit1 = input(PIN_IBC_RCV);
            // Setup Timer1 at 1 Bit Time.
            set_timer1(IBC_RX_BIT_TIME_T1LOAD_VAL_9600);
            clear_interrupt( INT_TIMER1 );
            break;
        case 3: // Sequence Bit 2
            g_unIBCRcvByteNode.m_stIBCNode.m_ui8SeqBit2 = input(PIN_IBC_RCV);
            // Setup Timer1 at 1 Bit Time.
            set_timer1(IBC_RX_BIT_TIME_T1LOAD_VAL_9600);
            clear_interrupt( INT_TIMER1 );
            break;
        case 4: // Data Bit Bit 0
            g_unIBCRcvByteNode.m_stIBCNode.m_ui8DataBit0 = input(PIN_IBC_RCV);
            // Setup Timer1 at 1 Bit Time.
            set_timer1(IBC_RX_BIT_TIME_T1LOAD_VAL_9600);
            clear_interrupt( INT_TIMER1 );
            break;
        case 5: // Bit 4
            g_unIBCRcvByteNode.m_stIBCNode.m_ui8DataBit1 = input(PIN_IBC_RCV);
            // Setup Timer1 at 1 Bit Time.
            set_timer1(IBC_RX_BIT_TIME_T1LOAD_VAL_9600);
            clear_interrupt( INT_TIMER1 );
            break;
        case 6: // Bit 5
            g_unIBCRcvByteNode.m_stIBCNode.m_ui8DataBit2 = input(PIN_IBC_RCV);
            // Setup Timer1 at 1 Bit Time.
            set_timer1(IBC_RX_BIT_TIME_T1LOAD_VAL_9600);
            clear_interrupt( INT_TIMER1 );
            break;
        case 7: // Bit 6
            g_unIBCRcvByteNode.m_stIBCNode.m_ui8DataBit3 = input(PIN_IBC_RCV);
            // Setup Timer1 at 1 Bit Time.
            set_timer1(IBC_RX_BIT_TIME_T1LOAD_VAL_9600);
            clear_interrupt( INT_TIMER1 );
            break;
        case 8: // Bit 7
            g_unIBCRcvByteNode.m_stIBCNode.m_ui8DataBit4 = input(PIN_IBC_RCV);
            // Setup Timer1 at 1 Bit Time.
         	set_timer1(IBC_RX_BIT_TIME_T1LOAD_VAL_9600);
         	clear_interrupt( INT_TIMER1 );
            break;
            
            
        case 9: // Bit 8
            g_unIBCRcvByteNode.m_stIBCNode.m_ui8DataBit5 = input(PIN_IBC_RCV);
            // Setup Timer1 at 1 Bit Time.
            set_timer1(IBC_RX_1_BT_CUSTOMISED_VAL_9600);
            clear_interrupt( INT_TIMER1 );
            break;
        case 10: // Bit 9
            g_unIBCRcvByteNode.m_stIBCNode.m_ui8DataBit6 = input(PIN_IBC_RCV);
            // Setup Timer1 at 1 Bit Time.
            set_timer1(IBC_RX_1_BT_CUSTOMISED_VAL_9600);
            clear_interrupt( INT_TIMER1 );
            break;
        case 11: // Bit 10
            g_unIBCRcvByteNode.m_stIBCNode.m_ui8DataBit7 = input(PIN_IBC_RCV);
            
            // We have received all data so now we can release Timer0 Critical Run
            g_ui8fTimer0State   = IBC_TIMER_0_CRITICAL_OFF;
            
            // Setup Timer1 at 1 Bit Time.
            set_timer1(IBC_RX_1_BT_CUSTOMISED_VAL_9600);
            clear_interrupt( INT_TIMER1 );
            break;
            
        case 12: // STOP Bit
            // Check if the Stop Bit is High.
            if(1 == input(PIN_IBC_RCV)) // Stop bit detected
            {
                // Byte received successfully
                // Save received byte to the Message Array 1
                //g_arruiIBCRxMsg1[g_uiIBCRxByteCount1 - 1] = g_unTempRxData1.m_uiByte;
            }
            else
            { 
                // Handle error.
                // Framing error.
                // Set Flag tracking Framing Error.
                //g_uiIBCRxFramingErr1 = 1;
                //g_uiIBCSTXReceived1  = 0;
            }
            // Disable Timer1.
            setup_timer_1 (T1_DISABLED);
            disable_interrupts(INT_TIMER1); 
            // Clear Timer1 interrupt.
            clear_interrupt(INT_TIMER1); 
 
            
            g_i8RcvByteCount++;
            if (5 == g_i8RcvByteCount)
            {
                g_i8RcvByteCount = 0;
            }
            // Now check the Byte Sequence
            g_stIBCRcvData.g_ui8ArrIBCData[g_unIBCRcvByteNode.m_ui8Arr[0]] = g_unIBCRcvByteNode.m_ui8Arr[1];
            
            if ((IBC_MSG_BYTE_COUNT - 1) == g_unIBCRcvByteNode.m_ui8Arr[0])
            {
                // The complete 5 byte data has been received
                // Now we can process the data
                g_fIBCDataReceived = 1;
                
                // Insert Message into IBC Receive Queue
////////                InsertIBCMsgQueue(&g_stIBCRcvQueue, &g_stIBCRcvData);  
            }
            // Reset Rx Bit Counter.
            g_ui8IBCRcvBitCount = 0;
            
            // Enable External Interrupt to detect next byte.
#ifdef M1_COMM_MODULE
            ext_int_edge(1, H_TO_L); 
            clear_interrupt(INT_EXT1);
            enable_interrupts(INT_EXT1);
#else
            ext_int_edge(3, H_TO_L); 
            clear_interrupt(INT_EXT3);
            enable_interrupts(INT_EXT3);
#endif
            break;

        default:
            // Framing error.
            // Set Flag tracking Framing Error.
            //g_uiIBCRxFramingErr1 = 1;
            
            // First release Timer0 Critical run lock
            g_ui8fTimer0State   = IBC_TIMER_0_CRITICAL_OFF;
            
            // Disable Timer1.
            setup_timer_1 (T1_DISABLED);
            disable_interrupts(INT_TIMER1); 
            // Clear Timer1 interrupt.
            clear_interrupt(INT_TIMER1); 
            
            // Reset Rx Bit Counter 1.
            g_ui8IBCRcvBitCount = 0;
            
            // Enable External Interrupt 0 to detect next start bit.
            ext_int_edge(1, H_TO_L); 
            clear_interrupt(INT_EXT1);
            enable_interrupts(INT_EXT1);
            break;
    }
    
}



#endif /* _VI_IBC_CONFIG_H_ */