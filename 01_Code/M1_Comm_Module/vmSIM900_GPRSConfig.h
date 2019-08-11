/* 
 * File:   vmSIM900_GPRSConfig.h
 * Author: mrinmoy
 *
 * Created on December 12, 2018, 4:05 PM
 */

#ifndef VM_SIM900_GPRS_CONFIG_H
#define	VM_SIM900_GPRS_CONFIG_H

#include "vmTimerConfig.h"
//#include "vmCalibrationConfig.h"
#include "viRS485Config.h"

#define   SIM900_RX_BUFF_SIZE                   500


#define   IN_MOTION_MACHINE_ID                  1
#define   GPRS_RQSTTYPE_SEND_2_SERVER           1

#define  GPRS_CONNECTION_FAILS                  30
#define  GPRS_HTTP_ENABLE_FAILS                 31
#define  GPRS_SET_BEARER_ID_FAILS               32
#define  GPRS_SET_WEB_SERVICE_URL_FAILS         33
#define  GPRS_START_HTTP_GET_SESSION_FAILS      34
#define  VYOMEC_250_LOO118_MACHINE_ID           1


//typedef struct _ST_GPRS_DATA_NODE
//{
//    unsigned int32 m_ui32TimeStamp;
//    unsigned int16 m_ui16SequenceNo;
//    unsigned int16 m_ui16ItemWeight;
//    unsigned int8  m_ui8RejectionStatus;
//} ST_GPRS_DATA_NODE;
//
//typedef struct _ST_GPRS_MSG_SEND_QUEUE
//{
//    ST_GPRS_DATA_NODE m_stArrGPRSDataNode[SIZE_GPRS_DATA_NODE_QUEUE];
//    int8              m_i8Front;
//    int8              m_i8Rear;
//} ST_GPRS_MSG_SEND_QUEUE;

//typedef struct _ST_PROD_M_CURR_BOX_DATA
//{
//    int16             m_i16BoxWeight;
//    unsigned int16    m_ui16SequenceNo;
//    unsigned int16    m_ui16RejectionStatus;
//    unsigned int32    m_ui32TimeStamp;
//} ST_PROD_M_CURR_BOX_DATA;

// *****************************************************************************
// ***************** Below -> Global Variable Declarations *********************
// *****************************************************************************

unsigned int8      g_ui8SIM900RxBuffer[SIM900_RX_BUFF_SIZE] = {0};
unsigned int16     g_ui16SIM900RxByteCount                  = 0;
//unsigned int16     g_ui16HTTPData                           = 0;
//unsigned int8      g_ui8GPRSRqstType                        = GPRS_RQSTTYPE_SEND_2_SERVER;
unsigned int8      g_fIsLastElementInGPRSQueue              = 0;
//unsigned int8      g_fGPRSSequence                          = 0;
//unsigned int8      g_fGPRSFailureBit                        = 0;
//unsigned int8      g_ui8DataQueueLen                        = 0;


//ST_GPRS_DATA_NODE  g_stGPRSDataNode;
//ST_GPRS_MSG_SEND_QUEUE   g_stGPRSMsgSendQueue;
//ST_PROD_M_CURR_BOX_DATA  g_stProdModeCurrBoxData;

// *****************************************************************************
// ***************** Above -> Global Variable Declarations *********************
// *****************************************************************************

void Delay_milisecond_(unsigned int16 ui16DelayTime)
{
	unsigned int16 ui16Counter = 0;
	while(ui16Counter < ui16DelayTime)
	{
		// Reset WDT
   		restart_wdt();
    
		delay_ms(1);
		ui16Counter++;
	}
}

void ResetSIM900RxBuffer(void)
{
    // Reset Rx Byte count.
    g_ui16SIM900RxByteCount = 0;
    // Flush all data in SIM900 Rx buffer
    memset(g_ui8SIM900RxBuffer, 0, SIM900_RX_BUFF_SIZE);
}



//void StuffGPRSMSgQueueDummy(void)
//{
//    unsigned int8  ui8NoOfMessage     = 2;
//    unsigned int16 ui16MachineID      = 1;
//    unsigned int8  ui8Variety         = 6;
//    
//    unsigned int32 ui32TimeStamp       = 454545;
//    unsigned int16 ui16SequenceNo      = 1;
//    int16          i16Itemweight       = 7000;
//    unsigned int16 ui16Rejectionstatus = 1;
//    
//    // Stuff dummy message header
//    g_stGPRSMsgHeader.m_ui8NoOfMessages = ui8NoOfMessage;
//    g_stGPRSMsgHeader.m_ui16MachineID   = ui16MachineID;
//    g_stGPRSMsgHeader.m_ui8Variety      = ui8Variety;
//    
//    for(unsigned int8 ui8LoopCount = 0; ui8LoopCount <= 30; ui8LoopCount++)
//    {
//        g_stGPRSMsgSendQueue.m_i8Rear++;
//        
//        // Stuff data
//        g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Rear].m_ui32TimeStamp = ui32TimeStamp++;
//        g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Rear].m_ui16SequenceNo = ui16SequenceNo++;
//        g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Rear].m_ui16ItemWeight = i16Itemweight++;
//        g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Rear].m_ui8RejectionStatus = ui16Rejectionstatus;
//    }
//    
//    g_stGPRSMsgSendQueue.m_i8Front = 0;
//}

//void StuffDummyTimeStamp(void)
//{
//    unsigned int32 ui32TimeStamp       = 8080;
//    int8           i8TempRear          = 0;
//    
//    i8TempRear = g_stGPRSMsgSendQueue.m_i8Rear;
//    for(unsigned int8 ui8LoopCount = 0; ui8LoopCount < SIZE_GPRS_DATA_NODE_QUEUE; ui8LoopCount++)
//    {
//        i8TempRear++;
//        
//        // Stuff data
//        g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[i8TempRear].m_ui32TimeStamp = ui32TimeStamp++;
//        
//    }
//    
//}

void StuffDummyHeader(void)
{
    unsigned int16 ui16MachineID      = VYOMEC_250_LOO118_MACHINE_ID;
    unsigned int8  ui8Variety         = 6;
    
    // Stuff dummy message header
    //g_stGPRSMsgHeader.m_ui8NoOfMessages = ui8NoOfMessage;
    g_stGPRSMsgHeader.m_ui16MachineID   = ui16MachineID;
    g_stGPRSMsgHeader.m_ui8Variety      = ui8Variety;
    
}






// ############################################################# GPRS Connection Driver ##################################################################

unsigned int8 SetConnectionType(void)
{
	unsigned int8 ui8fIsOKfound = 1;
	g_fIsTimedOut = 0;
	
	// Reset Rx Buffer.
	ResetSIM900RxBuffer();
	// Set connection type to GPRS.
	//fputs("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\r", SIM900);
	// Set timeout.
	SetTimeout(5);
	// Wait for response.
	while(ui8fIsOKfound == 1 && g_fIsTimedOut == 0)
	{
		// Restart WDT.
        restart_wdt();
        for(unsigned int16 ui16ByteCount = 0;  ui16ByteCount < g_ui16SIM900RxByteCount; ui16ByteCount++)
        {
        	// Check for "OK".
            if(g_ui8SIM900RxBuffer[(ui16ByteCount - 0)] == 0x0A &&
               g_ui8SIM900RxBuffer[(ui16ByteCount - 1)] == 0x0D &&
               g_ui8SIM900RxBuffer[(ui16ByteCount - 2)] == 'K' &&
               g_ui8SIM900RxBuffer[(ui16ByteCount - 3)] == 'O')
            {
                // Mark flag denoting "OK" was found.
                ui8fIsOKfound = 0;
                break;
            }
        }
        
	}
	
	return ui8fIsOKfound;
}

unsigned int8 SetAPN(void)
{
	unsigned int8 ui8fIsOKfound = 1;
	g_fIsTimedOut = 0;
	
	// Reset Rx Buffer.
	ResetSIM900RxBuffer();
	// Set APN.
    //fputs("AT+SAPBR=3,1,\"APN\",\"www\"\r",SIM900);
    // Set timeout.
	SetTimeout(5);
	// Wait for response.
	while(ui8fIsOKfound == 1 && g_fIsTimedOut == 0)
	{
		// Restart WDT.
        restart_wdt();
        for(unsigned int16 ui16ByteCount = 0;  ui16ByteCount < g_ui16SIM900RxByteCount; ui16ByteCount++)
        {
        	// Check for "OK".
            if(g_ui8SIM900RxBuffer[(ui16ByteCount - 0)] == 0x0A &&
               g_ui8SIM900RxBuffer[(ui16ByteCount - 1)] == 0x0D &&
               g_ui8SIM900RxBuffer[(ui16ByteCount - 2)] == 'K' &&
               g_ui8SIM900RxBuffer[(ui16ByteCount - 3)] == 'O')
            {
                // Mark flag denoting "OK" was found.
                ui8fIsOKfound = 0;
                break;
            }
        }
	}
	
	return ui8fIsOKfound;
	
}

unsigned int8 EnableGPRS(void)
{
	unsigned int8 ui8fIsOKfound = 1;
	g_fIsTimedOut = 0;
	
	// Reset Rx Buffer.
	ResetSIM900RxBuffer();
	// Enable GPRS.
    //fputs("AT+SAPBR=1,1\r", SIM900);
    // Set timeout.
	SetTimeout(5);
	// Wait for response.
	while(ui8fIsOKfound == 1 && g_fIsTimedOut == 0)
	{
		// Restart WDT.
        restart_wdt();
        for(unsigned int16 ui16ByteCount = 0;  ui16ByteCount < g_ui16SIM900RxByteCount; ui16ByteCount++)
        {
        	// Check for "OK".
            if(g_ui8SIM900RxBuffer[(ui16ByteCount - 0)] == 0x0A &&
               g_ui8SIM900RxBuffer[(ui16ByteCount - 1)] == 0x0D &&
               g_ui8SIM900RxBuffer[(ui16ByteCount - 2)] == 'K' &&
               g_ui8SIM900RxBuffer[(ui16ByteCount - 3)] == 'O')
            {
                // Mark flag denoting "OK" was found.
                ui8fIsOKfound = 0;
                break;
            }
        }
       
	}
	
	return ui8fIsOKfound;
}

unsigned int8 CheckGPRSConnection(void)
{
	unsigned int8 ui8fIsOKFound = 1;
    unsigned int8 ui8fIsFirstCharFound = 0;
    unsigned int8 ui8FirstCharIndex = 0;
    unsigned int8 ui8fIsSecondCharFound = 0;
    unsigned int8 ui8StopCount = 0;
    g_fIsTimedOut = 0;
    
    // Reset Rx Buffer.
	ResetSIM900RxBuffer();
    // Check GPRS Connection.
    //fputs("AT+SAPBR=2,1\r",SIM900);   
    // Set timeout.
    SetTimeout(5);
    // Wait for response.
    while(ui8fIsOKFound == 1 && g_fIsTimedOut == 0)
    {
    	// Restart WDT.
        restart_wdt();
        // Reset variables.
        ui8fIsFirstCharFound = 0;
        ui8FirstCharIndex = 0;
        ui8fIsSecondCharFound = 0;
        ui8StopCount = 0;
        
        // Loop Thorugh entire receive buffer.
        for(unsigned int16 ui16ByteCount = 0;  ui16ByteCount < g_ui16SIM900RxByteCount; ui16ByteCount++)
        {
        	// Check for first character sequence.
        	if(ui8fIsFirstCharFound == 0 && ui8fIsSecondCharFound == 0)
        	{
                // To check bearer after connect GPRS
        		if(g_ui8SIM900RxBuffer[(ui16ByteCount - 0)] == '"' &&
                   g_ui8SIM900RxBuffer[(ui16ByteCount - 1)] == ',' &&
                   g_ui8SIM900RxBuffer[(ui16ByteCount - 2)] == '1' &&
                   g_ui8SIM900RxBuffer[(ui16ByteCount - 3)] == ',' &&
                   g_ui8SIM900RxBuffer[(ui16ByteCount - 4)] == '1' &&
                   g_ui8SIM900RxBuffer[(ui16ByteCount - 5)] == 0x20 &&
                   g_ui8SIM900RxBuffer[(ui16ByteCount - 6)] == ':')
                 {
                 	// Set flag denoting first charcter sequence found.
                 	ui8fIsFirstCharFound = 1;
                 	// Copy the first character sequence index.
                 	ui8FirstCharIndex = ui16ByteCount;
                 }
                
        	}
        	// Check for second character sequence.
        	else if(ui8fIsFirstCharFound == 1 && ui8fIsSecondCharFound == 0)  // First character already found. 
        	{
        		if(g_ui8SIM900RxBuffer[(ui16ByteCount - 0)] == '"')
        		{
        			// Loop from first character to second character to check if IP was received.
                    for(unsigned int8 ui8LoopCount = ui8FirstCharIndex; ui8LoopCount <= ui16ByteCount; ui8LoopCount++)
                    {
                        // Check how many . is received.....should be 3.
                        if(g_ui8SIM900RxBuffer[ui8LoopCount] == '.')
                        {
                            ui8StopCount++;
                            // Check if all 3 stops have been received.
                            if(ui8StopCount == 3)
                            {
                                ui8fIsSecondCharFound = 1;
                            }
                        }
                    }
        		}
        	}
        	// First and second character both found........check for "OK".
        	else if(ui8fIsFirstCharFound == 1 && ui8fIsSecondCharFound == 1)
        	{
        		// Check for "OK".
                if(g_ui8SIM900RxBuffer[(ui16ByteCount - 0)] == 0x0A &&
                   g_ui8SIM900RxBuffer[(ui16ByteCount - 1)] == 0x0D &&
                   g_ui8SIM900RxBuffer[(ui16ByteCount - 2)] == 'K' &&
                   g_ui8SIM900RxBuffer[(ui16ByteCount - 3)] == 'O')
                {
                    // Mark flag denoting "OK" was found.
                    ui8fIsOKFound = 0;
                }
        	}
        }
    }

	return ui8fIsOKFound;
}

void SendCheckGPRSConnectionQuery(void)
{
    g_fIsTimedOut = 0;
    
    // Reset Rx Buffer.
	ResetSIM900RxBuffer();
    // Check GPRS Connection.
    //fputs("AT+SAPBR=2,1\r",SIM900);   
    // Set timeout.
    SetTimeout(3);
}

unsigned int8 GPRSConnectionCheckQueryResponse()
{
    unsigned int8 ui8fIsOKFound = 1;
    unsigned int8 ui8fIsFirstCharFound = 0;
    unsigned int8 ui8FirstCharIndex = 0;
    unsigned int8 ui8fIsSecondCharFound = 0;
    unsigned int8 ui8StopCount = 0;
    
    
    // Loop through entire SIM900 Rx buffer
    for(unsigned int16 ui16ByteCount = 0;  ui16ByteCount < g_ui16SIM900RxByteCount; ui16ByteCount++)
    {
        // Check for first character sequence.
        if(ui8fIsFirstCharFound == 0 && ui8fIsSecondCharFound == 0)
        {
            // To check bearer after connect GPRS
            if(g_ui8SIM900RxBuffer[(ui16ByteCount - 0)] == '"' &&
               g_ui8SIM900RxBuffer[(ui16ByteCount - 1)] == ',' &&
               g_ui8SIM900RxBuffer[(ui16ByteCount - 2)] == '1' &&
               g_ui8SIM900RxBuffer[(ui16ByteCount - 3)] == ',' &&
               g_ui8SIM900RxBuffer[(ui16ByteCount - 4)] == '1' &&
               g_ui8SIM900RxBuffer[(ui16ByteCount - 5)] == 0x20 &&
               g_ui8SIM900RxBuffer[(ui16ByteCount - 6)] == ':')
             {
                // Set flag denoting first charcter sequence found.
                ui8fIsFirstCharFound = 1;
                // Copy the first character sequence index.
                ui8FirstCharIndex = ui16ByteCount;
             }

        }
        // Check for second character sequence.
        else if(ui8fIsFirstCharFound == 1 && ui8fIsSecondCharFound == 0)  // First character already found. 
        {
            if(g_ui8SIM900RxBuffer[(ui16ByteCount - 0)] == '"')
            {
                // Loop from first character to second character to check if IP was received.
                for(unsigned int8 ui8LoopCount = ui8FirstCharIndex; ui8LoopCount <= ui16ByteCount; ui8LoopCount++)
                {
                    // Check how many . is received.....should be 3.
                    if(g_ui8SIM900RxBuffer[ui8LoopCount] == '.')
                    {
                        ui8StopCount++;
                        // Check if all 3 stops have been received.
                        if(ui8StopCount == 3)
                        {
                            ui8fIsSecondCharFound = 1;
                        }
                    }
                }
            }
        }
        // First and second character both found........check for "OK".
        else if(ui8fIsFirstCharFound == 1 && ui8fIsSecondCharFound == 1)
        {
            // Check for "OK".
            if(g_ui8SIM900RxBuffer[(ui16ByteCount - 0)] == 0x0A &&
               g_ui8SIM900RxBuffer[(ui16ByteCount - 1)] == 0x0D &&
               g_ui8SIM900RxBuffer[(ui16ByteCount - 2)] == 'K' &&
               g_ui8SIM900RxBuffer[(ui16ByteCount - 3)] == 'O')
            {
                // Mark flag denoting "OK" was found.
                ui8fIsOKFound = 0;
            }
        }
    }    
    
    return ui8fIsOKFound;
}

unsigned int8 DisconnectGPRS(void)
{
	unsigned int8 ui8fIsOKFound = 2;
	g_fIsTimedOut = 0;
	
	// Reset Rx Buffer.
	ResetSIM900RxBuffer();
	// Disconnect GPRS.
    //fputs("AT+SAPBR=0,1\r",SIM900);
    Delay_milisecond_(5000); 
    // Set timeout.
    SetTimeout(15);
    // Wait for Response.
    while(g_fIsTimedOut == 0 && ui8fIsOKFound == 2)
    {
    	// Restart WDT.
        restart_wdt();
        // Loop through entire SIM900 Rx buffer.
        for(unsigned int16 ui16ByteCount = 0;  ui16ByteCount < g_ui16SIM900RxByteCount; ui16ByteCount++)
        {
        	// Check for "OK".
            if(g_ui8SIM900RxBuffer[(ui16ByteCount - 0)] == 0x0A &&
               g_ui8SIM900RxBuffer[(ui16ByteCount - 1)] == 0x0D &&
               g_ui8SIM900RxBuffer[(ui16ByteCount - 2)] == 'K' &&
               g_ui8SIM900RxBuffer[(ui16ByteCount - 3)] == 'O')
            {
                // Mark flag denoting "OK" was found.
                ui8fIsOKFound = 3;
                break;
            }
        }
    }  
    
	return ui8fIsOKFound; 	
}

unsigned int8 CheckGPRSDisconnection(void)
{
	unsigned int8 ui8fIsOKFound = 1;
    unsigned int8 ui8fIsFirstCharFound = 0;
    unsigned int8 ui8FirstCharIndex = 0;
    unsigned int8 ui8fIsSecondCharFound = 0;
    unsigned int8 ui8StopCount = 0;
    g_fIsTimedOut = 0;
    
    // Reset Rx Buffer.
	ResetSIM900RxBuffer();
    // Check GPRS Connection.
    //fputs("AT+SAPBR=2,1\r",SIM900);   
    // Set timeout.
    SetTimeout(15);
    // Wait for response.
    while(ui8fIsOKFound == 1 && g_fIsTimedOut == 0)
    {
    	// Restart WDT.
        restart_wdt();
        // Reset variables.
        ui8fIsFirstCharFound = 0;
        ui8FirstCharIndex = 0;
        ui8fIsSecondCharFound = 0;
        ui8StopCount = 0;
        
        // Loop Thorugh entire receive buffer.
        for(unsigned int16 ui16ByteCount = 0;  ui16ByteCount < g_ui16SIM900RxByteCount; ui16ByteCount++)
        {
        	// Check for first character sequence.
        	if(ui8fIsFirstCharFound == 0 && ui8fIsSecondCharFound == 0)
        	{
                // To check bearer after connect GPRS
        		if(g_ui8SIM900RxBuffer[(ui16ByteCount - 0)] == '"' &&
                   g_ui8SIM900RxBuffer[(ui16ByteCount - 1)] == ',' &&
                   g_ui8SIM900RxBuffer[(ui16ByteCount - 2)] == '3' &&
                   g_ui8SIM900RxBuffer[(ui16ByteCount - 3)] == ',' &&
                   g_ui8SIM900RxBuffer[(ui16ByteCount - 4)] == '1' &&
                   g_ui8SIM900RxBuffer[(ui16ByteCount - 5)] == 0x20 &&
                   g_ui8SIM900RxBuffer[(ui16ByteCount - 6)] == ':')
                 {
                 	// Set flag denoting first charcter sequence found.
                 	ui8fIsFirstCharFound = 1;
                 	// Copy the first character sequence index.
                 	ui8FirstCharIndex = ui16ByteCount;
                 }
                
        	}
        	// Check for second character sequence.
        	else if(ui8fIsFirstCharFound == 1 && ui8fIsSecondCharFound == 0)  // First character already found. 
        	{
        		if(g_ui8SIM900RxBuffer[(ui16ByteCount - 0)] == '"')
        		{
        			// Loop from first character to second character to check if IP was received.
                    for(unsigned int8 ui8LoopCount = ui8FirstCharIndex; ui8LoopCount <= ui16ByteCount; ui8LoopCount++)
                    {
                        // Check how many . is received.....should be 3.
                        if(g_ui8SIM900RxBuffer[ui8LoopCount] == '.')
                        {
                            ui8StopCount++;
                            // Check if all 3 stops have been received.
                            if(ui8StopCount == 3)
                            {
                                ui8fIsSecondCharFound = 1;
                            }
                        }
                    }
        		}
        	}
        	// First and second character both found........check for "OK".
        	else if(ui8fIsFirstCharFound == 1 && ui8fIsSecondCharFound == 1)
        	{
        		// Check for "OK".
                if(g_ui8SIM900RxBuffer[(ui16ByteCount - 0)] == 0x0A &&
                   g_ui8SIM900RxBuffer[(ui16ByteCount - 1)] == 0x0D &&
                   g_ui8SIM900RxBuffer[(ui16ByteCount - 2)] == 'K' &&
                   g_ui8SIM900RxBuffer[(ui16ByteCount - 3)] == 'O')
                {
                    // Mark flag denoting "OK" was found.
                    ui8fIsOKFound = 0;
                }
        	}
        }
    }

	return ui8fIsOKFound;
}

// ############################################ End of GPRS Connection Driver ############################################################


// ############################################# HTTP Application Driver ####################################################################

unsigned int8 EnableHttpMode(void)
{
	unsigned int8 ui8fIsOKFound = 1;
	g_fIsTimedOut = 0;

	// Reset Rx Buffer.
	ResetSIM900RxBuffer();
    // Enable HTTP Mode.
    //fputs("AT+HTTPINIT\r",SIM900);
    // Set timeout.
    SetTimeout(5);
    // Wait for response.
    while(g_fIsTimedOut == 0 && ui8fIsOKFound == 1)
    {
        // Restart WDT.
        restart_wdt();
        // Loop through the entire SIM900 Rx buffer.
        for(unsigned int16 ui16ByteCount = 0; ui16ByteCount < g_ui16SIM900RxByteCount; ui16ByteCount++)
        {
            // Check for "OK".
            if(g_ui8SIM900RxBuffer[(ui16ByteCount - 0)] == 0x0A &&
               g_ui8SIM900RxBuffer[(ui16ByteCount - 1)] == 0x0D &&
               g_ui8SIM900RxBuffer[(ui16ByteCount - 2)] == 'K' &&
               g_ui8SIM900RxBuffer[(ui16ByteCount - 3)] == 'O')
            {
                // Mark flag denoting "OK" was found.
                ui8fIsOKFound = 0;
                break;
            }
        }
    }
   
    return ui8fIsOKFound;

} 

void SendEnableHTTPModeRequest(void)
{
    g_fIsTimedOut = 0;

	// Reset Rx Buffer.
	ResetSIM900RxBuffer();
    // Enable HTTP Mode.
    //fputs("AT+HTTPINIT\r",SIM900);
    // Set timeout.
    SetTimeout(2);
}

unsigned int8 CheckEnableHTTPModeReqResponse(void)
{
    unsigned int8 ui8fIsOKFound = 1;
    
    for(unsigned int16 ui16ByteCount = 0; ui16ByteCount < g_ui16SIM900RxByteCount; ui16ByteCount++)
    {
        // Check for "OK".
        if(g_ui8SIM900RxBuffer[(ui16ByteCount - 0)] == 0x0A &&
           g_ui8SIM900RxBuffer[(ui16ByteCount - 1)] == 0x0D &&
           g_ui8SIM900RxBuffer[(ui16ByteCount - 2)] == 'K' &&
           g_ui8SIM900RxBuffer[(ui16ByteCount - 3)] == 'O')
        {
            // Mark flag denoting "OK" was found.
            ui8fIsOKFound = 0;
            break;
        }
    }
    
    return ui8fIsOKFound;
}

unsigned int8 EnableSecureHttpMode(void)
{
	unsigned int8 ui8fIsOKFound = 1;
	g_fIsTimedOut = 0;
	
	// Reset Rx Buffer.
	ResetSIM900RxBuffer();
	// Enable HTTPS Mode.
	//fputs("AT+HTTPSSL=1\r",SIM900);
	// Set timeout.
    SetTimeout(5);
    // Wait for response.
    while(g_fIsTimedOut == 0 && ui8fIsOKFound == 1)
    {   
		  // Restart WDT.
	       restart_wdt();
	       // Loop through the entire SIM900 Rx buffer.
	       for(unsigned int16 ui16ByteCount = 0; ui16ByteCount < g_ui16SIM900RxByteCount; ui16ByteCount++)
	       {  
		  		// Check for "OK".
		  		if(g_ui8SIM900RxBuffer[(ui16ByteCount - 0)] == 0x0A &&
		     	   g_ui8SIM900RxBuffer[(ui16ByteCount - 1)] == 0x0D &&
		     	   g_ui8SIM900RxBuffer[(ui16ByteCount - 2)] == 'K' &&
		     	   g_ui8SIM900RxBuffer[(ui16ByteCount - 3)] == 'O')
		  		{
		      		// Mark flag denoting "OK" was found.
		      		ui8fIsOKFound = 0;
		      		break;
		  		}
	   	  }
	}	  
	
	return ui8fIsOKFound;
}

unsigned int8 SetHTTPBearerProfileID(void)
{
	unsigned int8 ui8fIsOKFound = 1;
	g_fIsTimedOut = 0;

	// Reset Rx Buffer.
	ResetSIM900RxBuffer();
    // Set HTTP Bearer Profile ID.
    ///fputs("AT+HTTPPARA=\"CID\",1\r",SIM900);
    // Set timeout.
    SetTimeout(5);
    // Wait for response.
    while(g_fIsTimedOut == 0 && ui8fIsOKFound == 1)
    {
        // Restart WDT.
        restart_wdt();
        // Loop through the entire SIM900 Rx buffer.
        for(unsigned int16 ui16ByteCount = 0; ui16ByteCount < g_ui16SIM900RxByteCount; ui16ByteCount++)
        {
            // Check for "OK".
            if(g_ui8SIM900RxBuffer[(ui16ByteCount - 0)] == 0x0A &&
               g_ui8SIM900RxBuffer[(ui16ByteCount - 1)] == 0x0D &&
               g_ui8SIM900RxBuffer[(ui16ByteCount - 2)] == 'K' &&
               g_ui8SIM900RxBuffer[(ui16ByteCount - 3)] == 'O')
            {
                // Mark flag denoting "OK" was found.
                ui8fIsOKFound = 0;
                break;
            }
        }
    }
    
    return ui8fIsOKFound;
}

void SendSetHTTPBearerProfileIDRequest(void)
{
    g_fIsTimedOut = 0;
    
    // Reset Rx Buffer.
	ResetSIM900RxBuffer();
    // Set HTTP Bearer Profile ID.
    //fputs("AT+HTTPPARA=\"CID\",1\r",SIM900);
    // Set timeout.
    SetTimeout(2);
}

unsigned int8 SetHTTPBearerProfileIDReqResponse(void)
{
    unsigned int8 ui8fIsOKFound = 1;
    
    // Loop through entire SIM900 Rx buffer
    for(unsigned int16 ui16ByteCount = 0; ui16ByteCount < g_ui16SIM900RxByteCount; ui16ByteCount++)
    {
        // Check for "OK".
        if(g_ui8SIM900RxBuffer[(ui16ByteCount - 0)] == 0x0A &&
           g_ui8SIM900RxBuffer[(ui16ByteCount - 1)] == 0x0D &&
           g_ui8SIM900RxBuffer[(ui16ByteCount - 2)] == 'K' &&
           g_ui8SIM900RxBuffer[(ui16ByteCount - 3)] == 'O')
        {
            // Mark flag denoting "OK" was found.
            ui8fIsOKFound = 0;
            break;
        }
    }
    
    return ui8fIsOKFound;
    ;
}

//unsigned int8 SetWebServiceURL(void)
//{
//    unsigned int8 ui8fIsOKFound   = 1;
//    unsigned int8 ui8DataQueueLen = 0;
//    g_fIsTimedOut = 0;
//    
//    
//    //Increase GPRS send msg queue front.
//    if(g_stGPRSMsgSendQueue.m_i8Front != 0)
//    {
//        g_stGPRSMsgSendQueue.m_i8Front++;
//    }    
//    
//    // Check remaining data length of queue
//    ui8DataQueueLen = (g_stGPRSMsgSendQueue.m_i8Rear - g_stGPRSMsgSendQueue.m_i8Front + 1);
//    
//    if(ui8DataQueueLen >= 3)
//    {   
//        // Set number of message to send in message header
//        g_stGPRSMsgHeader.m_ui8NoOfMessages = 3;
//        // Set Web Service URL.
//        //unsigned int16 ui16StopID = 4;
//        fprintf (SIM900, "AT+HTTPPARA=\"URL\",\"http://www.trackweights.com/api/GetCtlrData.php?a=%u&b=%Lu&c=%u&m=%Lu$%Lu$%Lu$%u|%Lu$%Lu$%Lu$%u|%Lu$%Lu$%Lu$%u\"\r", g_stGPRSMsgHeader.m_ui8NoOfMessages, g_stGPRSMsgHeader.m_ui16MachineID, g_stGPRSMsgHeader.m_ui8Variety, 
//                                                                                                                                                                     g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front].m_ui32TimeStamp, g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front].m_ui16SequenceNo, g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front].m_ui16ItemWeight, g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front++].m_ui8RejectionStatus,
//                                                                                                                                                                     g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front].m_ui32TimeStamp, g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front].m_ui16SequenceNo, g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front].m_ui16ItemWeight, g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front++].m_ui8RejectionStatus
//                                                                                                                                                                     g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front].m_ui32TimeStamp, g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front].m_ui16SequenceNo, g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front].m_ui16ItemWeight, g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front].m_ui8RejectionStatus);
//        //fprintf (SIM900, "AT+HTTPPARA=\"URL\",\"http://www.vyomainnovus.com/ServiceSet.php?%Lu\"\r", g_ui16HTTPData++);
//        //fputs("AT+HTTPPARA=\"URL\",\"http://www.vyomainnovus.com/BusStandService.php?id=81\"\r",SIM900);
//    } 
//    else 
//    {
//        // Set number of message to send in message header.
//        g_stGPRSMsgHeader.m_ui8NoOfMessages = 1;
//        // Set Web Service URL.
//        fprintf (SIM900, "AT+HTTPPARA=\"URL\",\"http://www.trackweights.com/api/GetCtlrData.php?a=%u&b=%Lu&c=%u&m=%Lu$%Lu$%Lu$%u\"\r", g_stGPRSMsgHeader.m_ui8NoOfMessages, g_stGPRSMsgHeader.m_ui16MachineID, g_stGPRSMsgHeader.m_ui8Variety, 
//                                                                                                                                       g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front].m_ui32TimeStamp, g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front].m_ui16SequenceNo, g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front].m_ui16ItemWeight, g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front].m_ui8RejectionStatus);
//    }    
//    // Set timeout.
//    SetTimeout(5);
//    // Wait for response.
//    while(g_fIsTimedOut == 0 && ui8fIsOKFound == 1)
//    {
//        // Restart WDT.
//        restart_wdt();
//        // Loop through the entire SIM900 Rx buffer.
//        for(unsigned int16 ui16ByteCount = 0; ui16ByteCount < g_ui16SIM900RxByteCount; ui16ByteCount++)
//        {
//            // Check for "OK".
//            if(g_ui8SIM900RxBuffer[(ui16ByteCount - 0)] == 0x0A &&
//               g_ui8SIM900RxBuffer[(ui16ByteCount - 1)] == 0x0D &&
//               g_ui8SIM900RxBuffer[(ui16ByteCount - 2)] == 'K' &&
//               g_ui8SIM900RxBuffer[(ui16ByteCount - 3)] == 'O')
//            {
//                // Mark flag denoting "OK" was found.
//                ui8fIsOKFound = 0;
//            }               
//        }
//    }
//   
//    return ui8fIsOKFound;
//}
//
//void SendSetWebServiceURLrequest(void)
//{
//    //unsigned int8 ui8DataQueueLen = 0;
//    g_fIsTimedOut = 0;
//    
//    //Increase GPRS send msg queue front.
//    g_stGPRSMsgSendQueue.m_i8Front++;
//    
//     // Check remaining data length of queue
//    g_ui8DataQueueLen = (g_stGPRSMsgSendQueue.m_i8Rear - g_stGPRSMsgSendQueue.m_i8Front + 1);
//    
//    
//     if(g_ui8DataQueueLen >= 5)
//    {   
//        // Set number of message to send in message header
//\
//         g_stGPRSMsgHeader.m_ui8NoOfMessages = 5;
//        // Set Web Service URL.
//        //unsigned int16 ui16StopID = 4;
//        fprintf (SIM900, "AT+HTTPPARA=\"URL\",\"http://www.trackweights.com/api/GetCtlrData.php?a=%u&b=%Lu&c=%u&m=%Lu$%Lu$%Lu$%u|%Lu$%Lu$%Lu$%u|%Lu$%Lu$%Lu$%u|%Lu$%Lu$%Lu$%u|%Lu$%Lu$%Lu$%u\"\r", g_stGPRSMsgHeader.m_ui8NoOfMessages, g_stGPRSMsgHeader.m_ui16MachineID, g_stGPRSMsgHeader.m_ui8Variety, 
//                                                                                                                                                                                                   g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front].m_ui32TimeStamp, g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front].m_ui16SequenceNo, g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front].m_ui16ItemWeight, g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front++].m_ui8RejectionStatus,
//                                                                                                                                                                                                   g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front].m_ui32TimeStamp, g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front].m_ui16SequenceNo, g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front].m_ui16ItemWeight, g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front++].m_ui8RejectionStatus,
//                                                                                                                                                                                                   g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front].m_ui32TimeStamp, g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front].m_ui16SequenceNo, g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front].m_ui16ItemWeight, g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front++].m_ui8RejectionStatus,
//                                                                                                                                                                                                   g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front].m_ui32TimeStamp, g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front].m_ui16SequenceNo, g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front].m_ui16ItemWeight, g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front++].m_ui8RejectionStatus,
//                                                                                                                                                                                                   g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front].m_ui32TimeStamp, g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front].m_ui16SequenceNo, g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front].m_ui16ItemWeight, g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front].m_ui8RejectionStatus);
//        //fprintf (SIM900, "AT+HTTPPARA=\"URL\",\"http://www.vyomainnovus.com/ServiceSet.php?%Lu\"\r", g_ui16HTTPData++);
//        //fputs("AT+HTTPPARA=\"URL\",\"http://www.vyomainnovus.com/BusStandService.php?id=81\"\r",SIM900);
//    } 
//    else 
//    {
//        // Set number of message to send in message header.
//        g_stGPRSMsgHeader.m_ui8NoOfMessages = 1;
//        // Set Web Service URL.
//        fprintf (SIM900, "AT+HTTPPARA=\"URL\",\"http://www.trackweights.com/api/GetCtlrData.php?a=%u&b=%Lu&c=%u&m=%Lu$%Lu$%Lu$%u\"\r", g_stGPRSMsgHeader.m_ui8NoOfMessages, g_stGPRSMsgHeader.m_ui16MachineID, g_stGPRSMsgHeader.m_ui8Variety, 
//                                                                                                                                       g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front].m_ui32TimeStamp, g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front].m_ui16SequenceNo, g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front].m_ui16ItemWeight, g_stGPRSMsgSendQueue.m_stArrGPRSDataNode[g_stGPRSMsgSendQueue.m_i8Front].m_ui8RejectionStatus);
//        
//        
//    }    
//    // Set timeout.
//    SetTimeout(2);
//}

unsigned int8 SetWebServiceURLReqresponse(void)
{
    unsigned int8 ui8fIsOKFound   = 1;
    
    // Loop through entire SIM900 Rx buffer
    for(unsigned int16 ui16ByteCount = 0; ui16ByteCount < g_ui16SIM900RxByteCount; ui16ByteCount++)
    {
        // Check for "OK".
        if(g_ui8SIM900RxBuffer[(ui16ByteCount - 0)] == 0x0A &&
           g_ui8SIM900RxBuffer[(ui16ByteCount - 1)] == 0x0D &&
           g_ui8SIM900RxBuffer[(ui16ByteCount - 2)] == 'K' &&
           g_ui8SIM900RxBuffer[(ui16ByteCount - 3)] == 'O')
        {
            // Mark flag denoting "OK" was found.
            ui8fIsOKFound = 0;
        }   
    }
    
    return ui8fIsOKFound;
}

unsigned int8 GetWebServiceURL(void)
{
	unsigned int8 ui8fIsOKFound = 1;
	g_fIsTimedOut = 0;
	
	// Reset Rx Buffer.
	ResetSIM900RxBuffer();
	// Set the URL to Read data.
	//fputs("AT+HTTPPARA=\"URL\",\"http://www.vyomainnovus.com/Service.php\"\r", SIM900);
	// Set Timeout.
	SetTimeout(15);
	// Wait for Response.
	while(g_fIsTimedOut == 0 && ui8fIsOKFound == 1)
	{
		// Restart WDT.
        restart_wdt();
        // Loop through the entire SIM900 Rx buffer.
        for(unsigned int16 ui16ByteCount = 0; ui16ByteCount < g_ui16SIM900RxByteCount; ui16ByteCount++)
        {
        	// Check for "OK".
            if(g_ui8SIM900RxBuffer[(ui16ByteCount - 0)] == 0x0A &&
               g_ui8SIM900RxBuffer[(ui16ByteCount - 1)] == 0x0D &&
               g_ui8SIM900RxBuffer[(ui16ByteCount - 2)] == 'K' &&
               g_ui8SIM900RxBuffer[(ui16ByteCount - 3)] == 'O')
            {
                // Mark flag denoting "OK" was found.
                ui8fIsOKFound = 0;
                break;
            }
        }
	}
	
	return ui8fIsOKFound;
} 


unsigned int8 StartHTTPGETSession(void)
{
	unsigned int8 ui8fIsOKFound 	 = 1;
    unsigned int8 ui8IsColonFound 	 = 0;
    unsigned int8 ui8CommaCount 	 = 0;
    unsigned int8 ui8IsCommaFound 	 = 0;
    unsigned int8 ui8FirstCommaIndex = 0; 
    
    
    // Reset Rx Buffer.
	ResetSIM900RxBuffer();
	// Set Web Service Action.
    //fputs("AT+HTTPACTION=0\r",SIM900);
    // Wait for 10 seconds to properly hit the URL.
    Delay_milisecond_(5000);  /* This will take sometimes more time */
    // Set timeout.
    SetTimeout(10);
    // Reset variable.
    g_fIsTimedOut = 0;
    // Wait for response.
    while(g_fIsTimedOut == 0 && ui8fIsOKFound == 1)
    {
    	// Restart WDT.
        restart_wdt();
        // Reset variables.
        ui8IsColonFound = 0;
        ui8CommaCount = 0;
        ui8IsCommaFound = 0;
        ui8FirstCommaIndex = 0;
        
        // Loop through the entire SIM900 Rx buffer.
        for(unsigned int16 ui16ByteCount = 0; ui16ByteCount < g_ui16SIM900RxByteCount; ui16ByteCount++)
        {
        	// Looking for First character sequence..i.e ':'
        	if(ui8IsColonFound == 0 && ui8IsCommaFound == 0)
        	{
        		if(g_ui8SIM900RxBuffer[(ui16ByteCount - 0)] == ':' &&
                   g_ui8SIM900RxBuffer[(ui16ByteCount - 1)] == 'N' &&
                   g_ui8SIM900RxBuffer[(ui16ByteCount - 2)] == 'O' &&
                   g_ui8SIM900RxBuffer[(ui16ByteCount - 3)] == 'I' &&
                   g_ui8SIM900RxBuffer[(ui16ByteCount - 4)] == 'T' &&
                   g_ui8SIM900RxBuffer[(ui16ByteCount - 5)] == 'C' &&
                   g_ui8SIM900RxBuffer[(ui16ByteCount - 6)] == 'A' &&
                   g_ui8SIM900RxBuffer[(ui16ByteCount - 7)] == 'P' &&
                   g_ui8SIM900RxBuffer[(ui16ByteCount - 8)] == 'T' &&
                   g_ui8SIM900RxBuffer[(ui16ByteCount - 9)] == 'T' &&
                   g_ui8SIM900RxBuffer[(ui16ByteCount - 10)] == 'H' &&
                   g_ui8SIM900RxBuffer[(ui16ByteCount - 11)] == '+' &&
                   g_ui8SIM900RxBuffer[(ui16ByteCount - 12)] == 0x0A &&
                   g_ui8SIM900RxBuffer[(ui16ByteCount - 13)] == 0x0D &&
                   g_ui8SIM900RxBuffer[(ui16ByteCount - 14)] == 0x0A &&
                   g_ui8SIM900RxBuffer[(ui16ByteCount - 15)] == 0x0D &&
                   g_ui8SIM900RxBuffer[(ui16ByteCount - 16)] == 'K' &&
                   g_ui8SIM900RxBuffer[(ui16ByteCount - 17)] == 'O')
                {
                    // Mark flag denoting colon is found.
                    ui8IsColonFound = 1;
                }
        	}
        	// Looking for second character sequence..i.e ','
        	else if (ui8IsColonFound == 1 && ui8IsCommaFound == 0) // First character sequence found.
        	{
	        	// Look for comma..','
        		if(g_ui8SIM900RxBuffer[(ui16ByteCount - 0)] == ',')
        		{
	        		if(!ui8FirstCommaIndex)
	        		{
	        			// Copy the index of the first comma.
	        			ui8FirstCommaIndex = ui16ByteCount;
	        		}
        			// Comma found...increment counter.
                    ui8CommaCount++;
                    // Check if 2 comma were received.
                    if(ui8CommaCount == 2)
                    {
                    	// Mark flag denoting second comma found.
                    	ui8IsCommaFound = 1;
                    }
        		}
        	}
        	// Both First and second character sequence found..check the received data.
        	else if(ui8IsColonFound == 1 && ui8IsCommaFound == 1)
        	{
        		if(g_ui8SIM900RxBuffer[(ui8FirstCommaIndex - 1)] == '0')
        		{
        			if(g_ui8SIM900RxBuffer[(ui8FirstCommaIndex + 1)] == '2' &&
        			   g_ui8SIM900RxBuffer[(ui8FirstCommaIndex + 2)] == '0' &&
        			   g_ui8SIM900RxBuffer[(ui8FirstCommaIndex + 3)] == '0')
        			{
        				// Mark flag denoting all received data is ok...send data successfully.
        				ui8fIsOKFound = 0;
        				break;
        			}
        		}
        	}
        }
    
    }
    
    return ui8fIsOKFound;
}

void SendStartHTTPGetSessionRequest(void)
{
    g_fIsTimedOut = 0;
    
    // Reset Rx Buffer.
	ResetSIM900RxBuffer();
	// Set Web Service Action.
    //fputs("AT+HTTPACTION=0\r",SIM900);
    // Set timeout.
    SetTimeout(20);
    
}

unsigned int8 StartHTTPGetSessionReqResponse(void)
{
    unsigned int8 ui8fIsOKFound 	 = 1;
    unsigned int8 ui8IsColonFound 	 = 0;
    unsigned int8 ui8CommaCount 	 = 0;
    unsigned int8 ui8IsCommaFound 	 = 0;
    unsigned int8 ui8FirstCommaIndex = 0; 
    
    
    // Loop through the entire SIM900 Rx buffer.
    for(unsigned int16 ui16ByteCount = 0; ui16ByteCount < g_ui16SIM900RxByteCount; ui16ByteCount++)
    {
    // Looking for First character sequence..i.e ':'
        if(ui8IsColonFound == 0 && ui8IsCommaFound == 0)
        {
            if(g_ui8SIM900RxBuffer[(ui16ByteCount - 0)] == ':' &&
               g_ui8SIM900RxBuffer[(ui16ByteCount - 1)] == 'N' &&
               g_ui8SIM900RxBuffer[(ui16ByteCount - 2)] == 'O' &&
               g_ui8SIM900RxBuffer[(ui16ByteCount - 3)] == 'I' &&
               g_ui8SIM900RxBuffer[(ui16ByteCount - 4)] == 'T' &&
               g_ui8SIM900RxBuffer[(ui16ByteCount - 5)] == 'C' &&
               g_ui8SIM900RxBuffer[(ui16ByteCount - 6)] == 'A' &&
               g_ui8SIM900RxBuffer[(ui16ByteCount - 7)] == 'P' &&
               g_ui8SIM900RxBuffer[(ui16ByteCount - 8)] == 'T' &&
               g_ui8SIM900RxBuffer[(ui16ByteCount - 9)] == 'T' &&
               g_ui8SIM900RxBuffer[(ui16ByteCount - 10)] == 'H' &&
               g_ui8SIM900RxBuffer[(ui16ByteCount - 11)] == '+' &&
               g_ui8SIM900RxBuffer[(ui16ByteCount - 12)] == 0x0A &&
               g_ui8SIM900RxBuffer[(ui16ByteCount - 13)] == 0x0D &&
               g_ui8SIM900RxBuffer[(ui16ByteCount - 14)] == 0x0A &&
               g_ui8SIM900RxBuffer[(ui16ByteCount - 15)] == 0x0D &&
               g_ui8SIM900RxBuffer[(ui16ByteCount - 16)] == 'K' &&
               g_ui8SIM900RxBuffer[(ui16ByteCount - 17)] == 'O')
            {
                // Mark flag denoting colon is found.
                ui8IsColonFound = 1;
            }
        }
        // Looking for second character sequence..i.e ','
        else if (ui8IsColonFound == 1 && ui8IsCommaFound == 0) // First character sequence found.
        {
            // Look for comma..','
            if(g_ui8SIM900RxBuffer[(ui16ByteCount - 0)] == ',')
            {
                if(!ui8FirstCommaIndex)
                {
                    // Copy the index of the first comma.
                    ui8FirstCommaIndex = ui16ByteCount;
                }
                // Comma found...increment counter.
                ui8CommaCount++;
                // Check if 2 comma were received.
                if(ui8CommaCount == 2)
                {
                    // Mark flag denoting second comma found.
                    ui8IsCommaFound = 1;
                }
            }
        }
        // Both First and second character sequence found..check the received data.
        else if(ui8IsColonFound == 1 && ui8IsCommaFound == 1)
        {
            if(g_ui8SIM900RxBuffer[(ui8FirstCommaIndex - 1)] == '0')
            {
                if(g_ui8SIM900RxBuffer[(ui8FirstCommaIndex + 1)] == '2' &&
                   g_ui8SIM900RxBuffer[(ui8FirstCommaIndex + 2)] == '0' &&
                   g_ui8SIM900RxBuffer[(ui8FirstCommaIndex + 3)] == '0')
                {
                    // Mark flag denoting all received data is ok...send data successfully.
                    ui8fIsOKFound = 0;
                    break;
                }
            }
        }
    }
    
    return ui8fIsOKFound;
}

void TerminateHttpSession(void)
{
    // Set command to terminate HTTP Session.
    //fputs("AT+HTTPTERM\r",SIM900);
    delay_ms(3000);
}

void TermHTTPSession(void)
{
    // Set command to terminate HTTP Session.
    //fputs("AT+HTTPTERM\r",SIM900);
    
    // Set timeout
    //SetTimeout(3);
}

// ########################################################## End of HTTP Application Driver #############################################################

#endif	/* VM_SIM900_GPRS_CONFIG_H */

