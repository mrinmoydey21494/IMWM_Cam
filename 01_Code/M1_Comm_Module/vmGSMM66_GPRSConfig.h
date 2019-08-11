/* 
 * File:   vmGSMM66_GPRSConfig.h
 * Author: mrinmoy
 *
 * Created on January 22, 2019, 6:50 PM
 */

#ifndef VM_GSMM66_GPRS_CONFIG_H
#define	VM_GSMM66_GPRS_CONFIG_H

#include "vmSIM900_GPRSConfig.h"
#include "viRS485Config.h"

// Stuff Machine ID and Variety type in GPRS Message header.
// Amta plant(Line 2) -> Machine ID - 2 and Marie Gold variety Type - 3.
// Delta Foods Machine ID - 4
// Super Snacks Machine ID - 5
#define  MACHINE_ID                2  // Delta
    
#define   SIZE_GSMM66_RX_BUFFER                 500
#define   SIZE_GPRS_DATA_NODE_QUEUE             100

#define   GPRS_DATA_SENDING_SEQUENCE_1          1
#define   GPRS_DATA_SENDING_SEQUENCE_2          2
#define   GPRS_DATA_SENDING_SEQUENCE_3          3
#define   GPRS_DATA_SENDING_SEQUENCE_4          4
#define   GPRS_DATA_SENDING_SEQUENCE_5          5
#define   GPRS_DATA_SENDING_SEQUENCE_6          6
#define   GPRS_DATA_SENDING_SEQUENCE_7          7
#define   GPRS_DATA_SENDING_SEQUENCE_8          8


typedef struct _ST_GPRS_DATA_NODE
{
    unsigned int32 m_ui32TimeStamp;
    unsigned int16 m_ui16SequenceNo;
    unsigned int16 m_ui16ItemWeight;
    unsigned int8  m_ui8RejectionStatus;
} ST_GPRS_DATA_NODE;

typedef struct _ST_GPRS_MSG_SEND_QUEUE
{
    ST_GPRS_DATA_NODE m_stArrGPRSDataNode[SIZE_GPRS_DATA_NODE_QUEUE];
    int8              m_i8Front;
    int8              m_i8Rear;
} ST_GPRS_MSG_SEND_QUEUE;

typedef struct _ST_PROD_M_CURR_BOX_DATA
{
    int16             m_i16BoxWeight;
    unsigned int16    m_ui16SequenceNo;
    unsigned int16    m_ui16RejectionStatus;
    unsigned int32    m_ui32TimeStamp;
} ST_PROD_M_CURR_BOX_DATA;

// *****************************************************************************
// ***************** Below -> Global Variable Declarations *********************
// *****************************************************************************
unsigned int8       g_ui8M66RxBuffer[SIZE_GSMM66_RX_BUFFER]               = {0};
unsigned int16      g_ui16M66RxByteCount                                  = 0;
unsigned int8       g_ui8URLInputByteLength                               = 0;
unsigned int8       g_ui8URLInputTime                                     = 0;
//unsigned int8       g_ui8DataToSend                                       = 0;
unsigned int16      g_ui16Itm1SequenceNumber                              = 1;
unsigned int16      g_ui16Itm2SequenceNumber                              = 2;
unsigned int16      g_ui16ItemWeight                                      = 1999;
unsigned int8       g_ui8DataQueueLen                                     = 0;
unsigned int8       g_fGPRSSequence                                       = 0;
unsigned int8       g_fGPRSFailureBit                                     = 0;

ST_GPRS_MSG_SEND_QUEUE   g_stGPRSMsgSendQueue;
ST_PROD_M_CURR_BOX_DATA  g_stProdModeCurrBoxData;
// *****************************************************************************
// ***************** Above -> Global Variable Declarations *********************
// *****************************************************************************

void ResetGSMM66RxBuffer(void)
{
    // Reset whole buffer.
    memset(g_ui8M66RxBuffer, 0, SIZE_GSMM66_RX_BUFFER);
    
    // Reset byte count
    g_ui16M66RxByteCount = 0;
}

unsigned int8 CountDataSize(unsigned int32 ui32Data)
{
    unsigned int8 ui8DataLen = 0;
    
    if(ui32Data <= 9)
    {
        ui8DataLen = 1;
    }
    else if(ui32Data <= 99)
    {
        ui8DataLen = 2;
    }
    else if(ui32Data <= 999)
    {
        ui8DataLen = 3;
    }
    else if(ui32Data <= 9999)
    {
        ui8DataLen = 4;
    }
    else if(ui32Data <= 99999)
    {
        ui8DataLen = 5;
    }
    else if(ui32Data <= 999999)
    {
        ui8DataLen = 6;
    }
    else if(ui32Data <= 9999999)
    {
        ui8DataLen = 7;
    }
    else if(ui32Data <= 99999999)
    {
        ui8DataLen = 8;
    }
    else if(ui32Data <= 999999999)
    {
        ui8DataLen = 9;
    }
    else if(ui32Data <= 9999999999)
    {
        ui8DataLen = 10;
    }
    else if(ui32Data <= 99999999999)
    {
        ui8DataLen = 11;
    }
    else if(ui32Data <= 999999999999)
    {
        ui8DataLen = 12;
    }
    
    return ui8DataLen;
}

void CheckURLInputByteLength(void)
{
    unsigned int8 ui8DataByte = 0;
    
    // Check remaining data in the GPRS queue is greater equals than five or not.
    if(g_ui8DataQueueLen >= 5)
    {
        // Five accumulated data node has to send.
        // Set number of messages to send is five.
        g_stGPRSMsgHeader.m_ui8NoOfMessages = 5;
        
        // Basic input byte has to send is 69 bytes. add up additional bytes per no of messages.
        // Add data node separator "|" for input byte length.
        g_ui8URLInputByteLength = g_ui8URLInputByteLength + (g_stGPRSMsgHeader.m_ui8NoOfMessages - 1);
        // Add data separator "$" for each data node.
        g_ui8URLInputByteLength = g_ui8URLInputByteLength + (g_stGPRSMsgHeader.m_ui8NoOfMessages * 3);
        // Now loop through the data queue to count input byte length.
        for(unsigned int8 ui8LoopCount = 0; ui8LoopCount < g_stGPRSMsgHeader.m_ui8NoOfMessages; ui8LoopCount++)
        {
            // Check for TimeStamp.
            ui8DataByte = CountDataSize(g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front + ui8LoopCount].m_ui32TimeStamp);
            // Add up data byte.
            g_ui8URLInputByteLength = g_ui8URLInputByteLength + ui8DataByte;
            // Check for Sequence number.
            ui8DataByte = CountDataSize(g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front + ui8LoopCount].m_ui16SequenceNo);
            // Add up data byte.
            g_ui8URLInputByteLength = g_ui8URLInputByteLength + ui8DataByte;
            // Check for Item weight.
            ui8DataByte = CountDataSize(g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front + ui8LoopCount].m_ui16ItemWeight);
            // Add up data byte.
            g_ui8URLInputByteLength = g_ui8URLInputByteLength + ui8DataByte;
            // Check for Rejection status.
            ui8DataByte = CountDataSize(g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front + ui8LoopCount].m_ui8RejectionStatus);
            // Add up data byte.
            g_ui8URLInputByteLength = g_ui8URLInputByteLength + ui8DataByte;
                
        }
    }
    else
    {
        // Single data node has to send.
        // Set number of messages to send is one.
        g_stGPRSMsgHeader.m_ui8NoOfMessages = 1;
        
        // Basic input byte has to send is 69 bytes. add up additional bytes per no of messages.
        // No data node separator needed. Only data separator for each node will be counted.
        // Add data separator "$" for each data node.
        g_ui8URLInputByteLength = g_ui8URLInputByteLength + (g_stGPRSMsgHeader.m_ui8NoOfMessages * 3);
        
        // Now count data size of each data for single data node.
        // Check for TimeStamp.
        ui8DataByte = CountDataSize(g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front].m_ui32TimeStamp);
        // Add up data byte.
        g_ui8URLInputByteLength = g_ui8URLInputByteLength + ui8DataByte;
        // Check for Sequence number.
        ui8DataByte = CountDataSize(g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front ].m_ui16SequenceNo);
        // Add up data byte.
        g_ui8URLInputByteLength = g_ui8URLInputByteLength + ui8DataByte;
        // Check for Item weight.
        ui8DataByte = CountDataSize(g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front].m_ui16ItemWeight);
        // Add up data byte.
        g_ui8URLInputByteLength = g_ui8URLInputByteLength + ui8DataByte;
        // Check for Rejection status.
        ui8DataByte = CountDataSize(g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front].m_ui8RejectionStatus);
        // Add up data byte.
        g_ui8URLInputByteLength = g_ui8URLInputByteLength + ui8DataByte;
        
    }
}

void StuffDummyTimeStamp(void)
{
    unsigned int16 ui16DummyTimeStamp = 11111;
    unsigned int8  ui8LoopCount       = 0;
    
    for(ui8LoopCount = 0; ui8LoopCount < SIZE_GPRS_DATA_NODE_QUEUE; ui8LoopCount++)
    {
        // Stuff Data.
        g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[ui8LoopCount].m_ui32TimeStamp = ui16DummyTimeStamp++;
    }
}


void InitializeGPRSMsgQueue(void)
{
    // Initialize front and rear of the queue
    g_stGPRSMsgSendQueue.m_i8Front = -1;
    g_stGPRSMsgSendQueue.m_i8Rear = -1;
    
    // Initialize each queue element
    for(unsigned int8 ui8LoopCount = 0; ui8LoopCount < SIZE_GPRS_DATA_NODE_QUEUE; ui8LoopCount++)
    {
        g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[ui8LoopCount].m_ui16ItemWeight = 0;
        g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[ui8LoopCount].m_ui16SequenceNo = 0;
        g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[ui8LoopCount].m_ui32TimeStamp  = 0;
        g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[ui8LoopCount].m_ui8RejectionStatus = 0;
    }

}

unsigned int8 IsGPRSMsgQueueEmpty(void)
{
    unsigned int8 ui8QueueStatus = 0;
    
    if(g_stGPRSMsgSendQueue.m_i8Rear == -1) /* Queue at initial state; queue is empty */
    {
        ui8QueueStatus = 1;
    }
    else if(g_stGPRSMsgSendQueue.m_i8Front == g_stGPRSMsgSendQueue.m_i8Rear) /* All data sent; queue is empty */
    {
        // Update flag that last element present in the queue.
        g_fIsLastElementInGPRSQueue = 1;
        
        // Reset GPRS message queue
        //InitializeGPRSMsgQueue();
        
        //ui8QueueStatus = 1;
    }
    else if(g_stGPRSMsgSendQueue.m_i8Front == g_stGPRSMsgSendQueue.m_i8Rear + 1)
    {
        // Reset GPRS message queue
        InitializeGPRSMsgQueue();
        // Stuff dummy time stamp.
        StuffDummyTimeStamp();
        
        // Update data availability status in queue.
        ui8QueueStatus = 1;
    }
    
    return ui8QueueStatus;
}

void InsertInGPRSMsgQueue()
{
    if ((g_stGPRSMsgSendQueue.m_i8Front == 0 && g_stGPRSMsgSendQueue.m_i8Rear == SIZE_GPRS_DATA_NODE_QUEUE - 1) || 
            (g_stGPRSMsgSendQueue.m_i8Front == g_stGPRSMsgSendQueue.m_i8Rear + 1)) 
    { 
        //printf("\nQueue is Full"); 
        return; 
    }
    
    else if (g_stGPRSMsgSendQueue.m_i8Rear == -1) /* Insert First Element */
    { 
        // Initialize queue front and rear.
        g_stGPRSMsgSendQueue.m_i8Rear = 0;
        g_stGPRSMsgSendQueue.m_i8Front = 0;
        
        // Insert Data
        //arr[g_stIBCDataSendQueue.m_i8Rear] = value;
        g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Rear].m_ui16SequenceNo = g_stProdModeCurrBoxData.m_ui16SequenceNo;
        g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Rear].m_ui16ItemWeight = g_stProdModeCurrBoxData.m_i16BoxWeight;
        g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Rear].m_ui8RejectionStatus = (unsigned int8)g_stProdModeCurrBoxData.m_ui16RejectionStatus;
        //g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Rear].m_ui32TimeStamp = g_stProdModeCurrBoxData.m_ui32TimeStamp;
    } 
    
    else if (g_stGPRSMsgSendQueue.m_i8Rear == SIZE_GPRS_DATA_NODE_QUEUE - 1 && g_stGPRSMsgSendQueue.m_i8Front != 0) /* Rear reached at last position */
    { 
        // Make queue circular
        g_stGPRSMsgSendQueue.m_i8Rear = 0; 
        
        // Insert data
        g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Rear].m_ui16SequenceNo = g_stProdModeCurrBoxData.m_ui16SequenceNo;
        g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Rear].m_ui16ItemWeight = g_stProdModeCurrBoxData.m_i16BoxWeight;
        g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Rear].m_ui8RejectionStatus = (unsigned int8)g_stProdModeCurrBoxData.m_ui16RejectionStatus; 
        //g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Rear].m_ui32TimeStamp = g_stProdModeCurrBoxData.m_ui32TimeStamp;
    } 
    
    else
    { 
        // Normal data insertion..not in boundary condition
        g_stGPRSMsgSendQueue.m_i8Rear++; 
        
        // Insert data
        g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Rear].m_ui16SequenceNo = g_stProdModeCurrBoxData.m_ui16SequenceNo;
        g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Rear].m_ui16ItemWeight = g_stProdModeCurrBoxData.m_i16BoxWeight;
        g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Rear].m_ui8RejectionStatus = (unsigned int8)g_stProdModeCurrBoxData.m_ui16RejectionStatus;
        //g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Rear].m_ui32TimeStamp = g_stProdModeCurrBoxData.m_ui32TimeStamp;
    } 
}

void InitializeGPRSMsgHeader(void)
{
    // Stuff Machine ID and Variety type in GPRS Message header.
    // Amta plant(Line 2) -> Machine ID - 2 and Marie Gold variety Type - 3.
    // Delta Foods Machine ID - 4
    // Super Snacks Machine ID - 5
    g_stGPRSMsgHeader.m_ui16MachineID = MACHINE_ID;  // Delta
    g_stGPRSMsgHeader.m_ui8Variety = 4;
}

void StuffGPRSMSgQueueDummy(void)
{
    unsigned int8  ui8NoOfMessage     = 5;
    unsigned int16 ui16MachineID      = 1;
    unsigned int8  ui8Variety         = 6;
    
    unsigned int32 ui32TimeStamp       = 4545;
    unsigned int16 ui16SequenceNo      = 1;
    int16          i16Itemweight       = 7000;
    unsigned int16 ui16Rejectionstatus = 1;
    
    // Stuff dummy message header
    g_stGPRSMsgHeader.m_ui8NoOfMessages = ui8NoOfMessage;
    g_stGPRSMsgHeader.m_ui16MachineID   = ui16MachineID;
    g_stGPRSMsgHeader.m_ui8Variety      = ui8Variety;
    
    for(unsigned int8 ui8LoopCount = 0; ui8LoopCount < SIZE_GPRS_DATA_NODE_QUEUE; ui8LoopCount++)
    {
        g_stGPRSMsgSendQueue.m_i8Rear++;
        
        // Stuff data
        g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Rear].m_ui32TimeStamp = ui32TimeStamp++;
        g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Rear].m_ui16SequenceNo = ui16SequenceNo++;
        g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Rear].m_ui16ItemWeight = i16Itemweight++;
        g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Rear].m_ui8RejectionStatus = ui16Rejectionstatus;
    }
    
    //g_stGPRSMsgSendQueue.m_i8Front = 0;
}



// ################################################################# Below GPRS Connection Driver #############################################################

unsigned int8 SetPDPContext(void)
{
    unsigned int8 ui8IsOKFound = 1;
    g_fIsTimedOut = 0;
    
    // Reset Rx buffer.
    ResetGSMM66RxBuffer();
    // Set AT command for define PDP Context.
    fputs("AT+CGDCONT=1,\"IP\",\"www\"\r", GSM_M66);
    // Set timeout.
    SetTimeout(2);
    // Wait for response.
    while(g_fIsTimedOut == 0 && ui8IsOKFound == 1)
    {
        // Loop through entire receive buffer.
        for(unsigned int16 ui16ByteCount = 0; ui16ByteCount < g_ui16M66RxByteCount; ui16ByteCount++)
        {
            // Check for "OK".
            if(g_ui8M66RxBuffer[(ui16ByteCount - 0)] == 0x0A &&
               g_ui8M66RxBuffer[(ui16ByteCount - 1)] == 0x0D &&
               g_ui8M66RxBuffer[(ui16ByteCount - 2)] == 'K' &&
               g_ui8M66RxBuffer[(ui16ByteCount - 3)] == 'O')
               {
                   // Set flag denoting 'OK' is found.
                   ui8IsOKFound = 0;
                   break;
               }
        }
    }
    
    return ui8IsOKFound;
}

unsigned int8 ActivatePDP(void)
{
    unsigned int8 ui8IsOKFound = 1;
    g_fIsTimedOut = 0;
    
    // Reset Rx buffer.
    ResetGSMM66RxBuffer();
    // Set AT command to activate PDP context.
    fputs("AT+CGACT=1,1\r", GSM_M66);
    // Set timeout.
    SetTimeout(2);
    // Wait for response.
    while(g_fIsTimedOut == 0 && ui8IsOKFound == 1)
    {
        // Loop through entire receive buffer.
        for(unsigned int16 ui16ByteCount = 0; ui16ByteCount < g_ui16M66RxByteCount; ui16ByteCount++)
        {
            // Check for "OK".
            if(g_ui8M66RxBuffer[(ui16ByteCount - 0)] == 0x0A &&
               g_ui8M66RxBuffer[(ui16ByteCount - 1)] == 0x0D &&
               g_ui8M66RxBuffer[(ui16ByteCount - 2)] == 'K' &&
               g_ui8M66RxBuffer[(ui16ByteCount - 3)] == 'O')
               {
                   // Set flag denoting 'OK' is found.
                   ui8IsOKFound = 0;
                   break;
               }
        }
    }
    
    return ui8IsOKFound;
} 

unsigned int8 ShowActivatedAddress(void)
{
    unsigned int8 ui8IsResponseFound            = 1;
    unsigned int8 ui8FirstCharSequenceFound     = 0;
    unsigned int8 ui8FirstCharSeqIndex          = 0;
    unsigned int8 ui8SecondCharSequenceFound    = 0;
    unsigned int8 ui8DoubleQuoteCount           = 0;
   
    
    // Reset Rx buffer.
    ResetGSMM66RxBuffer();
    // Set AT command to Show activated PDP address.
    fputs("AT+CGPADDR=1\r",GSM_M66);
    delay_ms(2000);
    // Set timeout.
    SetTimeout(2);
    // Reset timeout flag.
     g_fIsTimedOut = 0;
    // Wait for response.
    while(g_fIsTimedOut == 0 && ui8IsResponseFound == 1)
    {
        // Reset WDT
        //restart_wdt();
        // Reset variables.
        ui8FirstCharSequenceFound  = 0;
        ui8FirstCharSeqIndex       = 0;
        ui8SecondCharSequenceFound = 0;
        
        // Loop through entire receive buffer.
        for(unsigned int16 ui16ByteCount = 0; ui16ByteCount <= g_ui16M66RxByteCount; ui16ByteCount++)
        {
            // Check for first character sequence.
            if(ui8FirstCharSequenceFound == 0 && ui8SecondCharSequenceFound == 0)
            {
                if(g_ui8M66RxBuffer[(ui16ByteCount - 0)] == ':' &&
                   g_ui8M66RxBuffer[(ui16ByteCount - 1)] == 'R' &&
                   g_ui8M66RxBuffer[(ui16ByteCount - 2)] == 'D' &&
                   g_ui8M66RxBuffer[(ui16ByteCount - 3)] == 'D' &&
                   g_ui8M66RxBuffer[(ui16ByteCount - 4)] == 'A' &&
                   g_ui8M66RxBuffer[(ui16ByteCount - 5)] == 'P' &&
                   g_ui8M66RxBuffer[(ui16ByteCount - 6)] == 'G' &&
                   g_ui8M66RxBuffer[(ui16ByteCount - 7)] == 'C' &&
                   g_ui8M66RxBuffer[(ui16ByteCount - 8)] == '+')
                {
                    // Set flag denoting first character sequence has found.
                    ui8FirstCharSequenceFound = 1;
                    // Copy the first character sequence index.
                    ui8FirstCharSeqIndex = (ui16ByteCount - 0);
                }
            }
            
            // Check for second character sequence set.
            else if(ui8FirstCharSequenceFound == 1 && ui8SecondCharSequenceFound == 0)
            {
                if(g_ui8M66RxBuffer[(ui16ByteCount - 0)] == '"')
                {
                    // Increment double quote count.
                    ui8DoubleQuoteCount++;
                    // Check double quote count.
                    if(2 == ui8DoubleQuoteCount)
                    {
                        // Reset double quote count.
                        ui8DoubleQuoteCount = 0;
                        // loop through to check second quote count.
                        for(unsigned int8 ui8LoopCount = ui8FirstCharSeqIndex; ui8LoopCount < (ui16ByteCount - 0); ui8LoopCount++)
                        {
                            // Check for stop count.
                            if(g_ui8M66RxBuffer[(ui8LoopCount - 0)] == '.')
                            {
                                // Increase stop.
                                ui8DoubleQuoteCount++;
                                // Check limiting stop count.
                                if(3 == ui8DoubleQuoteCount)
                                {
                                    // Set flag denoting second character sequence has found.
                                    ui8SecondCharSequenceFound = 1;
                                    break;
                                }
                            }    
                        }
                        
                        
                    }
                }
            }
            
            // Check for OK.
            else if(ui8FirstCharSequenceFound == 1 && ui8SecondCharSequenceFound == 1)
            {
                if(g_ui8M66RxBuffer[(ui16ByteCount - 0)] == 0x0A &&
                   g_ui8M66RxBuffer[(ui16ByteCount - 1)] == 0x0D &&
                   g_ui8M66RxBuffer[(ui16ByteCount - 2)] == 'K' &&
                   g_ui8M66RxBuffer[(ui16ByteCount - 3)] == 'O')
               {
                   // Set flag denoting 'OK' is found.
                   ui8IsResponseFound = 0;
                   break;
               }
            }
        }
        
    }
    
    return ui8IsResponseFound;
}

void SendGPRSConnectionQuery(void)
{
    g_fIsTimedOut = 0;
    
    
    // Reset Rx buffer.
    ResetGSMM66RxBuffer();
    // Check GPRS Connection.
    fputs("AT+CGACT?\r", GSM_M66);
    // Set timeout.
    //SetTimeout(3);

}

//unsigned int8 CheckGPRSConnection(void)
//{
//    unsigned int8 ui8IsResponseFound = 5;
//    unsigned int8 ui8IsFirstCharSequenceFound = 0;
//    unsigned int8 ui8FirstCharSequenceIndex = 0;
//    g_fIsTimedOut = 0;
//    
//    // Reset Rx buffer.
//    ResetGSMM66RxBuffer();
//    // Check GPRS Connection.
//    fputs("AT+CGACT?\r", GSM_M66);
//    // Set timeout.
//    SetTimeout(2);
//    // Wait for response.
//    while(g_fIsTimedOut == 0 && ui8IsResponseFound == 5)
//    {
//        // Reset variables.
//        ui8IsFirstCharSequenceFound = 0;
//        ui8FirstCharSequenceIndex = 0;
//        
//        // Loop through entire receive buffer.
//        for(unsigned int16 ui16ByteCount = 0; ui16ByteCount < g_ui16M66RxByteCount; ui16ByteCount++)
//        {
//            // Check for first character sequence.
//            if(ui8IsFirstCharSequenceFound == 0)
//            {
//                if(g_ui8M66RxBuffer[(ui16ByteCount - 0)] == ':'&&
//                   g_ui8M66RxBuffer[(ui16ByteCount - 1)] == 'T'&&
//                   g_ui8M66RxBuffer[(ui16ByteCount-  2)] == 'C'&&
//                   g_ui8M66RxBuffer[(ui16ByteCount - 3)] == 'A'&&
//                   g_ui8M66RxBuffer[(ui16ByteCount - 4)] == 'G'&&
//                   g_ui8M66RxBuffer[(ui16ByteCount - 5)] == 'C'&&
//                   g_ui8M66RxBuffer[(ui16ByteCount - 6)] == '+')
//                {
//                    // Set flag denoting first character sequence found.
//                    ui8IsFirstCharSequenceFound = 1;
//                    // Copy the index of the first character of the sequence.
//                    ui8FirstCharSequenceIndex = (ui16ByteCount - 0);
//                }
//            }
//            // Check for second character sequence.
//            else if(ui8IsFirstCharSequenceFound == 1)
//            {
//                if(g_ui8M66RxBuffer[(ui16ByteCount - 0)] == '0'&&
//                   g_ui8M66RxBuffer[(ui16ByteCount - 1)] == ','&&
//                   g_ui8M66RxBuffer[(ui16ByteCount - 2)] == '1')
//                {
//                    // Set flag denoting particular response found.
//                    // and the response says GPRS not connected.
//                    ui8IsResponseFound = 0;
//                    break;
//                }
//                else if(g_ui8M66RxBuffer[(ui16ByteCount - 0)] == '1'&&
//                        g_ui8M66RxBuffer[(ui16ByteCount - 1)] == ','&&
//                        g_ui8M66RxBuffer[(ui16ByteCount - 2)] == '1')
//                {
//                    // Set flag denoting particular response found.
//                    // and the response says GPRS connected.
//                    ui8IsResponseFound = 1;
//                    break;
//                }
//            }
//        }
//    }
//    
//    return ui8IsResponseFound;
//}

unsigned int8 CheckGPRSConnectionQueryResponse(void)
{
    unsigned int8 ui8IsResponseFound = 5;
    unsigned int8 ui8IsFirstCharSequenceFound = 0;
    unsigned int8 ui8FirstCharSequenceIndex = 0;
    
    
    // Loop through entire receive buffer.
    for(unsigned int16 ui16ByteCount = 0; ui16ByteCount < g_ui16M66RxByteCount; ui16ByteCount++)
    {
        // Check for first character sequence.
        if(ui8IsFirstCharSequenceFound == 0)
        {
            if(g_ui8M66RxBuffer[(ui16ByteCount - 0)] == ':'&&
               g_ui8M66RxBuffer[(ui16ByteCount - 1)] == 'T'&&
               g_ui8M66RxBuffer[(ui16ByteCount-  2)] == 'C'&&
               g_ui8M66RxBuffer[(ui16ByteCount - 3)] == 'A'&&
               g_ui8M66RxBuffer[(ui16ByteCount - 4)] == 'G'&&
               g_ui8M66RxBuffer[(ui16ByteCount - 5)] == 'C'&&
               g_ui8M66RxBuffer[(ui16ByteCount - 6)] == '+')
            {
                // Set flag denoting first character sequence found.
                ui8IsFirstCharSequenceFound = 1;
                // Copy the index of the first character of the sequence.
                ui8FirstCharSequenceIndex = (ui16ByteCount - 0);
            }
        }
        // Check for second character sequence.
        else if(ui8IsFirstCharSequenceFound == 1)
        {
            if(g_ui8M66RxBuffer[(ui16ByteCount - 0)] == '0'&&
               g_ui8M66RxBuffer[(ui16ByteCount - 1)] == ','&&
               g_ui8M66RxBuffer[(ui16ByteCount - 2)] == '1')
            {
                // Set flag denoting particular response found.
                // and the response says GPRS not connected.
                ui8IsResponseFound = 0;
                break;
            }
            else if(g_ui8M66RxBuffer[(ui16ByteCount - 0)] == '1'&&
                    g_ui8M66RxBuffer[(ui16ByteCount - 1)] == ','&&
                    g_ui8M66RxBuffer[(ui16ByteCount - 2)] == '1')
            {
                // Set flag denoting particular response found.
                // and the response says GPRS connected.
                ui8IsResponseFound = 1;
                break;
            }
        }
    }
    
    return ui8IsResponseFound;
}

// ########################################################## Above GPRS Connection Driver ###############################################################

// ########################################################## Below HTTP Application Driver ##############################################################

unsigned int8 SetHTTPURL(void)
{
    unsigned int8 ui8IsSetURLOk = 1;
    unsigned int8 ui8IsConnectionDone = 0;
    g_ui8URLInputTime           = 60;
    g_ui8URLInputByteLength     = 93;
    g_fIsTimedOut               = 0;
    
    // Reset Rx buffer.
    ResetGSMM66RxBuffer();
    // Increment variables.
    g_ui16Itm1SequenceNumber += 2;
    g_ui16Itm2SequenceNumber += 2;
    g_ui16ItemWeight++;
    // Check URL input byte length.
    CheckURLInputByteLength();
    // Set AT Commands to define web service URL.
    fprintf(GSM_M66, "AT+QHTTPURL=%u,%u\r", g_ui8URLInputByteLength, g_ui8URLInputTime);
    // Set timeout.
    SetTimeout(2);
    // Wait for response.
    while(g_fIsTimedOut == 0 && ui8IsSetURLOk == 1)
    {
        // Loop through entire receive buffer.
        for(unsigned int16 ui16ByteCount = 0; ui16ByteCount < g_ui16M66RxByteCount; ui16ByteCount++)
        {
            if(g_ui8M66RxBuffer[(ui16ByteCount - 0)] == 'T' &&
               g_ui8M66RxBuffer[(ui16ByteCount - 1)] == 'C' &&
               g_ui8M66RxBuffer[(ui16ByteCount - 2)] == 'E' &&
               g_ui8M66RxBuffer[(ui16ByteCount - 3)] == 'N' &&
               g_ui8M66RxBuffer[(ui16ByteCount - 4)] == 'N' &&
               g_ui8M66RxBuffer[(ui16ByteCount - 5)] == 'O' &&
               g_ui8M66RxBuffer[(ui16ByteCount - 6)] == 'C')
            {
                // Set flag denote connection is established.
                ui8IsConnectionDone = 1;
                break;
            }       
        }
        
        // set URL in buffer.
        if(ui8IsConnectionDone)
        {
            // Set web service URL.
            fprintf(GSM_M66, "http://mwm.itsinindia.com/api/Test/GetStringGetMethod?a=2&b=1&c=5&m=1000$%Lu$%Lu$1|1500$%Lu$%Lu$1", g_ui16Itm1SequenceNumber, g_ui16ItemWeight, g_ui16Itm2SequenceNumber, ++g_ui16ItemWeight);
            delay_ms(300);
            // Check for "OK".
            for(unsigned int16 ui16ByteCount = 0; ui16ByteCount < g_ui16M66RxByteCount; ui16ByteCount++)
            {
                if(g_ui8M66RxBuffer[(ui16ByteCount - 0)] == 0x0A &&
                   g_ui8M66RxBuffer[(ui16ByteCount - 1)] == 0x0D &&
                   g_ui8M66RxBuffer[(ui16ByteCount - 2)] == 'K' &&
                   g_ui8M66RxBuffer[(ui16ByteCount - 3)] == 'O')
                {
                    // Set flag denoting Set URL successfully.
                    ui8IsSetURLOk = 0;
                    break;
                }
            }    
        }
        
       
    }
    
    return ui8IsSetURLOk;
    
}

void SetHTTPURLParameter(void)
{
    g_ui8URLInputTime           = 60;
    g_ui8URLInputByteLength     = 66;
    g_fIsTimedOut               = 0;
    
    // Reset Rx buffer.
    ResetGSMM66RxBuffer();
    
//    //Increase GPRS send msg queue front.
//    if(g_stGPRSMsgSendQueue.m_i8Front != 0)
//    {
//        g_stGPRSMsgSendQueue.m_i8Front++;
//    }  
    
    // Check remaining data length of queue.
    // Count data length according to queue front and rear position.
    if(g_stGPRSMsgSendQueue.m_i8Rear > g_stGPRSMsgSendQueue.m_i8Front)
    {
        g_ui8DataQueueLen = (g_stGPRSMsgSendQueue.m_i8Rear - g_stGPRSMsgSendQueue.m_i8Front + 1);
    }
    else if(g_stGPRSMsgSendQueue.m_i8Rear == g_stGPRSMsgSendQueue.m_i8Front)
    {
        g_ui8DataQueueLen = 1;
    }
    else
    {
        g_ui8DataQueueLen = ((SIZE_GPRS_DATA_NODE_QUEUE - g_stGPRSMsgSendQueue.m_i8Front) + (g_stGPRSMsgSendQueue.m_i8Rear + 1));
    }
    
    // If, queue front is very near to queue size i.e. less than 5 data though data length is
    // more than 5 then to properly circulate queue front we need to send data one by one at this situation until it reaches queue end.
    if((SIZE_GPRS_DATA_NODE_QUEUE - g_stGPRSMsgSendQueue.m_i8Front) < 5)
    {
        g_ui8DataQueueLen = 1;
    }
    
    // Check URL input byte length.
    CheckURLInputByteLength();
    
    // Set AT Commands to define web service URL.
    fprintf(GSM_M66, "AT+QHTTPURL=%u,%u\r", g_ui8URLInputByteLength, g_ui8URLInputTime);
    // Set timeout.
    //SetTimeout(2);
}

unsigned int8 SetHTTPURLL(void)
{
    unsigned int8 ui8IsSetURLOk = 1;
    unsigned int8 ui8IsConnectionDone = 0;
    
    // Loop through entire receive buffer.
    for(unsigned int16 ui16ByteCount = 0; ui16ByteCount < g_ui16M66RxByteCount; ui16ByteCount++)
    {
        if(g_ui8M66RxBuffer[(ui16ByteCount - 0)] == 'T' &&
           g_ui8M66RxBuffer[(ui16ByteCount - 1)] == 'C' &&
           g_ui8M66RxBuffer[(ui16ByteCount - 2)] == 'E' &&
           g_ui8M66RxBuffer[(ui16ByteCount - 3)] == 'N' &&
           g_ui8M66RxBuffer[(ui16ByteCount - 4)] == 'N' &&
           g_ui8M66RxBuffer[(ui16ByteCount - 5)] == 'O' &&
           g_ui8M66RxBuffer[(ui16ByteCount - 6)] == 'C')
        {
            // Set flag denote connection is established.
            ui8IsConnectionDone = 1;
            break;
        }       
    }
    
    // set URL in buffer.
    if(ui8IsConnectionDone)
    {
        if(g_ui8DataQueueLen > 5)
        {
            // Set No of messages to send.
            g_stGPRSMsgHeader.m_ui8NoOfMessages = 5;
            // Set web service URL.
            fprintf(GSM_M66, "http://trackweigher.com/api/Test/GetStringGetMethod?a=%u&b=%Lu&c=%u&m=%Lu$%Lu$%Lu$%u|%Lu$%Lu$%Lu$%u|%Lu$%Lu$%Lu$%u|%Lu$%Lu$%Lu$%u|%Lu$%Lu$%Lu$%u", g_stGPRSMsgHeader.m_ui8NoOfMessages, g_stGPRSMsgHeader.m_ui16MachineID, g_stGPRSMsgHeader.m_ui8Variety,
                                                                                                                                                                                   g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front].m_ui32TimeStamp, g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front].m_ui16SequenceNo, g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front].m_ui16ItemWeight, g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front++].m_ui8RejectionStatus, 
                                                                                                                                                                                   g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front].m_ui32TimeStamp, g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front].m_ui16SequenceNo, g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front].m_ui16ItemWeight, g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front++].m_ui8RejectionStatus,
                                                                                                                                                                                   g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front].m_ui32TimeStamp, g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front].m_ui16SequenceNo, g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front].m_ui16ItemWeight, g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front++].m_ui8RejectionStatus,
                                                                                                                                                                                   g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front].m_ui32TimeStamp, g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front].m_ui16SequenceNo, g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front].m_ui16ItemWeight, g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front++].m_ui8RejectionStatus,
                                                                                                                                                                                   g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front].m_ui32TimeStamp, g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front].m_ui16SequenceNo, g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front].m_ui16ItemWeight, g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front].m_ui8RejectionStatus);
                                                                                                                                    
        }
        else
        {
            // Set No of messages to send.
            g_stGPRSMsgHeader.m_ui8NoOfMessages = 1;
            // Set web service URL.
            fprintf(GSM_M66, "http://trackweigher.com/api/Test/GetStringGetMethod?a=%u&b=%Lu&c=%u&m=%Lu$%Lu$%Lu$%u", g_stGPRSMsgHeader.m_ui8NoOfMessages, g_stGPRSMsgHeader.m_ui16MachineID, g_stGPRSMsgHeader.m_ui8Variety,
                                                                                                                     g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front].m_ui32TimeStamp, g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front].m_ui16SequenceNo, g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front].m_ui16ItemWeight, g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front].m_ui8RejectionStatus);
        }
        delay_ms(300);
        // Check for "OK".
        for(unsigned int16 ui16ByteCount = 0; ui16ByteCount < g_ui16M66RxByteCount; ui16ByteCount++)
        {
            if(g_ui8M66RxBuffer[(ui16ByteCount - 0)] == 0x0A &&
               g_ui8M66RxBuffer[(ui16ByteCount - 1)] == 0x0D &&
               g_ui8M66RxBuffer[(ui16ByteCount - 2)] == 'K' &&
               g_ui8M66RxBuffer[(ui16ByteCount - 3)] == 'O')
            {
                // Set flag denoting Set URL successfully.
                ui8IsSetURLOk = 0;
                break;
            }
        }    
    }
    
    // Check if queue front is at zero-th position..then increment front.
    // This is after sending data of zero-th position.
//    if(g_stGPRSMsgSendQueue.m_i8Front == 0)
//    {
//        g_stGPRSMsgSendQueue.m_i8Front++;
//    }
    
    return ui8IsSetURLOk;
}

void SendHTTPGetSessionRequest(void)
{
    g_fIsTimedOut = 0;
    
    // Reset Rx buffer.
    ResetGSMM66RxBuffer();
    // Set AT Command to start web service action.
    fputs("AT+QHTTPGET=120\r", GSM_M66);
    // Set timeout.
    //SetTimeout(10);
}

unsigned int8 CheckHTTPGETRequestResponse(void)
{
    unsigned int8 ui8IsOKFound = 1;
    unsigned int8 ui8IsFirstCharSeqFound  = 0;
    unsigned int8 ui8IsSecondCharSeqFound = 0;
    unsigned int8 ui8CharSeqIndex         = 0;
    unsigned int8 ui8CommaCount           = 0;
    
    // Loop through entire receive buffer
    for(unsigned int16 ui16ByteCount = 0; ui16ByteCount < g_ui16M66RxByteCount; ui16ByteCount++)
    {
        // Check for first character sequence.
        if(ui8IsFirstCharSeqFound == 0 && ui8IsSecondCharSeqFound == 0)
        {
            if(g_ui8M66RxBuffer[(ui16ByteCount - 0)] == 0x0A &&
               g_ui8M66RxBuffer[(ui16ByteCount - 1)] == 0x0D &&
               g_ui8M66RxBuffer[(ui16ByteCount - 2)] == 'K' &&
               g_ui8M66RxBuffer[(ui16ByteCount - 3)] == 'O')
            {
                // Set flag denoting that first character sequence is found.
                ui8IsFirstCharSeqFound = 1;
            }
        }

        // Check for second character sequence.
        else if(ui8IsFirstCharSeqFound == 1 && ui8IsSecondCharSeqFound == 0)
        {
            if(g_ui8M66RxBuffer[(ui16ByteCount - 0)] == ':' &&
               g_ui8M66RxBuffer[(ui16ByteCount - 1)] == 'T' &&
               g_ui8M66RxBuffer[(ui16ByteCount - 2)] == 'E' &&
               g_ui8M66RxBuffer[(ui16ByteCount - 3)] == 'G' &&
               g_ui8M66RxBuffer[(ui16ByteCount - 4)] == 'P' &&
               g_ui8M66RxBuffer[(ui16ByteCount - 5)] == 'T' &&
               g_ui8M66RxBuffer[(ui16ByteCount - 6)] == 'T' &&
               g_ui8M66RxBuffer[(ui16ByteCount - 7)] == 'H' &&
               g_ui8M66RxBuffer[(ui16ByteCount - 8)] == 'Q' &&
               g_ui8M66RxBuffer[(ui16ByteCount - 9)] == '+' )
            {
                // Set flag denoting that second character sequence has found.
                ui8IsSecondCharSeqFound = 1;
            } 

        }

        // Check for third character sequence.
        else if(ui8IsFirstCharSeqFound == 1 && ui8IsSecondCharSeqFound == 1)
        {
            if(g_ui8M66RxBuffer[(ui16ByteCount - 0)] == ',')
            {
                // Increase comma count
                ui8CommaCount++;
                // Check for comma count level
                if(ui8CommaCount == 2)
                {
                    // Check for success message code.
                    if(g_ui8M66RxBuffer[(ui16ByteCount - 1)] == '0' &&
                       g_ui8M66RxBuffer[(ui16ByteCount - 2)] == '0' &&
                       g_ui8M66RxBuffer[(ui16ByteCount - 3)] == '2')
                    {
                        // Set flag denoting that success message code found.
                        ui8IsOKFound = 0;
                        break;
                    }
                }
            }    
        }

    }
    
    return ui8IsOKFound;
}

//unsigned int8 StartHTTPGETSession(void)
//{
//    unsigned int8 ui8IsOKFound = 1;
//    g_fIsTimedOut = 0;
//    
//    // Reset Rx buffer.
//    ResetGSMM66RxBuffer();
//    // Set AT Command to start web service action.
//    fputs("AT+QHTTPGET=120\r", GSM_M66);
//    // Set timeout.
//    SetTimeout(20);
//    // Wait for response.
//    while(g_fIsTimedOut == 0 && ui8IsOKFound == 1)
//    {
//        // Reset WDT
//        restart_wdt();
//        // Loop through entire receive buffer
//        for(unsigned int16 ui16ByteCount = 0; ui16ByteCount < g_ui16M66RxByteCount; ui16ByteCount++)
//        {
//            if(g_ui8M66RxBuffer[(ui16ByteCount - 0)] == 0x0A &&
//               g_ui8M66RxBuffer[(ui16ByteCount - 1)] == 0x0D &&
//               g_ui8M66RxBuffer[(ui16ByteCount - 2)] == 'K' &&
//               g_ui8M66RxBuffer[(ui16ByteCount - 3)] == 'O')
//            {
//                // Set flag denoting Set URL successfully.
//                ui8IsOKFound = 0;
//                break;
//            }
//        }
//    }
//    
//    return ui8IsOKFound;
//}


// ############################################################# Above HTTP Application driver ##########################################################
#endif	/* VM_GSMM66_GPRS_CONFIG_H */

