#include "vmSystemConfig.h"
#include "../Common/vmRS485Config.h"
#include "vmTimerConfig.h"

#include "vmDigitalIOPortPinConfig.h"
#include "vmGSMM66Config.h"
#include "vmGSMM66_GPRSConfig.h"
#include "../Common/vmIBCConfig.h"
#include "vmIBCProcessM1.h"
#include "../Common/vmTime.h"
//#include "viShiftRegister.h"


void Initialize(void)
{
    
    // Set RS485 Transmit Inactive.
    output_bit(PIN_RS485_RTS,0);

    // Initialize external interrupt for InterBoard Communication.
    ext_int_edge(1, H_TO_L); 
    clear_interrupt(INT_EXT1);
    enable_interrupts(INT_EXT1);
    
    // Enable 1MS main timer.
    clear_interrupt(INT_TIMER0);
    enable_interrupts(INT_TIMER0);
    
    // Enable timer interrupts for InterBoard Communication(IBC).
    // TIMER1 => used for IBC receive; TIMER3=> used for IBC transmit.
    clear_interrupt(INT_TIMER1);
    enable_interrupts(INT_TIMER1);
    clear_interrupt(INT_TIMER3);
    enable_interrupts(INT_TIMER3);
    
    // Enable Global Interrupts.
    enable_interrupts(GLOBAL);
    

//    Initialize for GPRS message queue.
    InitializeGPRSMsgQueue();
    InitializeGPRSMsgHeader();
    StuffDummyTimeStamp();
    
   
    // Initialize the sending queue to M0
    //InitializeDataSendQueue();
    InitializeDataQueue(&g_stIBCRcvQueue);
    InitializeDataQueue(&g_stIBCSendQueue);
    
    // Initialize global variables for RS485 communication.    
    g_ui16CBBCount =  0;
    g_ui16CBBCountPrev = 0;
    g_ui16CurrentWt = 0;
    g_ui16CurrentPassStatus = 0;
    g_ui16RunningAvgWt = 0;
    g_ui16RunningTareValue = 0;
    g_ui16ServerStatus = 0;
    g_ui16CurrentMode = 0;
    
    // initialize variables for set point
    g_ui16gAvgWtForCalibration = 0;
    //g_ui16CounterForAvgCalibrationWt = 0;

    // use for production type
    g_fAutoCalibrationMode = 0;

    // use for sample count for auto calibration
    g_ui16AutoCalibrationSampleCount = 10;

    // TESTING End
    g_unIBCRcvByteNode.m_ui8Arr[0] = 0;
    g_unIBCRcvByteNode.m_ui8Arr[1] = 0;
    
    g_stIBCRcvData.g_ui8ArrIBCData[0] = 0;
    g_stIBCRcvData.g_ui8ArrIBCData[1] = 0;
    g_stIBCRcvData.g_ui8ArrIBCData[2] = 0;
    g_stIBCRcvData.g_ui8ArrIBCData[3] = 0;
    g_stIBCRcvData.g_ui8ArrIBCData[4] = 0;
    
    
}	

void main()
{
    unsigned int8 ui8Response                       = 0;
    unsigned int8 ui8RetVal                         = 0;
//    unsigned int8 GprsFailedConnectionAttemptCount  = 0;
//	unsigned int8 GprsFailedToDisconnectCount 		= 0;
//	unsigned int8 ui8SendAttemptCount               = 0;	
    g_fIsGPRSConnected                              = GPRS_NOT_CONNECTED;
    g_fGPRSSequence                                 = 1;
    
	// Set Watch Dog Timer ON.
    setup_wdt(WDT_ON);
    
	//Initialize 1 millisecond timer.
	Init1MSTimer();
    
//////    // TESTING Start
//////    // Initialize TIMER1.
//////    InitTIMER1();
//////    // Initialize TIMER3.
//////    InitTIMER3();
//////    // TESTING End
	
	//Initialize RS485 Receive interrupt.
	InitRS485RxInterrupt();
	
	//Initialize UART2 Receive interrupt
    InitM66RxInterrupt();

	//Configure RS485 pins of micro.
	SetRS485PortPinOfMicro();
	
	
	//for TESTING. 
	//ManipulateToleranceTable();
	
	// Initialize Main Global variables 
	Initialize();
    
    // Set GSM modem ready
    delay_ms(3000);
    
    // Configure SIM900 Module
    //SetAutomaticSMSRecvOn();  // ##################### TODO must setup echo disable ####################
    //SetM66BasicSMSConfiguration();

	// Reset WDT.
	restart_wdt();

	for(;;)
	{ 
        // TESTING Start
        if (1 == g_fIBCDataReceived)
        {
            // The complete 5 byte data has been received
            // Now we can process the data
            g_fIBCDataReceived = 0;

            // Insert Message into IBC Receive Queue
            InsertIBCMsgQueue(&g_stIBCRcvQueue, &g_stIBCRcvData);
        }
        // 
        //GetRTC();
        // Step 1: Check if a Node received in IBC..then process data.
//        if (g_fIBCDataReceived)
//        {
//            // Reset the flag
//            g_fIBCDataReceived = 0;
//            
//            // Process data
//           ProcessIBCMsg();
//        }
        
        // Step 2: Check if there is any message in IBC Send Queue
//        if (IsMSGAvailableInIBCSendQueue() && (0 == g_fIBCSendBusy))
//        {
//            // Make IBC Send Busy
//            g_fIBCSendBusy = 1;
//            // Call Send IBC Message
//            SendIBCMSG(); SendIBCMSGFromQ
//        }
//        if (IsMSGAvailableInIBCSendQueue() && (0 == g_fIBCSendBusy))
//        {
//            // Make IBC Send Busy
//            g_fIBCSendBusy = 1;
//            // Call Send IBC Message
//            SendIBCMSGFromQ();
//        }
        
    // Check If There is data ready to send HMI through RS485
    if (FLAG_RS485_DATA_SEND_ON == g_ui8fRS485DataSend)// && g_ui8ByteToSend > 0 )
    {
        fputc(g_RS485TxBuffer[g_ui8RS485ByteCnt++], RS485);
        if (g_ui8RS485ByteCnt >= g_ui8ByteToSend)
        {
            g_ui8RS485ByteCnt = 0;
            g_ui8ByteToSend = 0;
            g_ui8fRS485DataSend = FLAG_RS485_DATA_SEND_DONE;
        }
    }
    
    // Data Sending through IBC.    
    // Check if there is any message in IBC Send Queue
    if (IsMSGInIBCQueue(&g_stIBCSendQueue) && (0 == g_fIBCSendBusy))
    {
        // Make IBC Send Busy
        g_fIBCSendBusy = 1;
        // Call Send IBC Message
        SendIBCMSGFromQ();
    }

    // Data receive through IBC.
    // Check if there is any message in IBC Receive Queue
    if (IsMSGInIBCQueue(&g_stIBCRcvQueue))
    {
        // Process IBC 
        ProcessIBCMsg();
    }
 

    // Step 3: Check for "AT-OK" after every 60 minutes.
    if(CHECK_FOR_AT_RESPONSE == g_fShouldCheckForAtResponse)
    {
        ui8Response = 0;
        // Send AT Command.
        ui8Response = SendATCommand();

        // Check "AT" command response.
        // If "OK" is not received then restart SIM900 module and MCU.
        if(!ui8Response)
        {
            // Check if MCU's consecutive restart prevents to reset.
            if(REDAY_TO_RESTART_MCU == g_fIsReadyToRestartMCU)
            {
                // Update SIM900 Module health check flag
                g_fIsSIM900ModuleOK = GPRS_MODULE_HEALTH_MODULE_IS_NOT_OK;

                // Reset SIM900
                output_bit(PIN_IN_EXT_DI_00, 0);
                delay_ms(5000);
                output_bit(PIN_IN_EXT_DI_00, 1);

                // Reset MCU  // ##### Warning -> Restart MCU will cause to loose GPRS queue data permanently ###########
                for(;;)
                {

                }
            }
        }

        else
        {
            // Update SIM900 Module health check flag
            g_fIsSIM900ModuleOK = GPRS_MODULE_HEALTH_OK;
            // If "OK" received then reset flag
            g_fShouldCheckForAtResponse = DO_NOT_CHECK_FOR_AT_RESPONSE;
        }    
    }


    // Step 4: Check if a byte received in Modbus..then process data.
    if(ISR_RECEIVED_ALL_BYTE == g_fISRAllByteReceived)
    { 
        // Process received data.
        ProcessUART2ReceivedData();

        // Reset flag.
        g_fISRAllByteReceived = ISR_PROCESSED_ALL_BYTE;
    }
    
        
        // Whole data sending process is segregated into small modules to freed-up 
        // the time for GPRS data sending so that main() get more time to run.
        // Preliminary data is sent after every 1 minute
        
 // ################ Below steps are for GPRS modular data sending ######################
        
        // Step 1: Check for should data send through GPRS
        if(SHOULD_SEND_DATA == g_fShouldSendData && g_fGPRSSequence == GPRS_DATA_SENDING_SEQUENCE_1 && g_fIsSIM900ModuleOK == GPRS_MODULE_HEALTH_OK)
        {
            // Check GPRS connection
            // Send GPRS connection check query
            //SendCheckGPRSConnectionQuery(); // SIM900
            SendGPRSConnectionQuery(); // 4G_EC20
            
            // Set timeout of 1 second.
            SetTimeout(GPRS_CONNECTION_QUERY_TIMEOUT);
            
            // Update sequence flag
            g_fGPRSSequence = GPRS_DATA_SENDING_SEQUENCE_2;
            
        }
        
        // Step 2: Check for GPRS check connection query response
        if(1 == g_fIsTimedOut && g_fGPRSSequence == GPRS_DATA_SENDING_SEQUENCE_2)
        {
            // Check GPRS connection check query response
            ui8RetVal = CheckGPRSConnectionQueryResponse();
            
            if(1 == ui8RetVal) // GPRS already connected
            {
                // Update Flag.
                g_fIsGPRSConnected = GPRS_CONNECTED;
                
                // Update sequence flag
                g_fGPRSSequence = GPRS_DATA_SENDING_SEQUENCE_4;
            }
            else if(0 == ui8RetVal) // GPRS is not connected
            {
                // Update Flag
                g_fIsGPRSConnected = GPRS_NOT_CONNECTED;
                
                // Update sequence flag
                g_fGPRSSequence = GPRS_DATA_SENDING_SEQUENCE_3;
           
            }
        }
        
        // Step 3: If GPRS is not connected then connect GPRS
        if(GPRS_NOT_CONNECTED == g_fIsGPRSConnected && g_fGPRSSequence == GPRS_DATA_SENDING_SEQUENCE_3)
        {
            // Connect GPRS
            ui8RetVal = ConnectGPRS();
            
            // Check return value
            if(0 == ui8RetVal) // On success
            {
                // Update Flag.
                g_fIsGPRSConnected = GPRS_CONNECTED;
                
                // Update sequence flag
                g_fGPRSSequence = GPRS_DATA_SENDING_SEQUENCE_4;
            }
            else // On failure
            {
                // Set failure flag from where GPRS process fails so that error
                // handling stuff can handle that easily
                g_fGPRSFailureBit = GPRS_CONNECTION_FAILS;
                
                // Set timeout of 3 seconds
                g_fIsTimedOut = 0;
                SetTimeout(DATA_RESEND_INTRVL_TIMEOUT);
                
                // Update sequence flag
                g_fGPRSSequence = GPRS_DATA_SENDING_SEQUENCE_8;
            }
        }
        
        // Step 4: If GPRS connected then process data sending
        if(GPRS_CONNECTED == g_fIsGPRSConnected && g_fGPRSSequence == GPRS_DATA_SENDING_SEQUENCE_4)
        {
            // Check if there is msg available in  queue
            ui8RetVal = IsGPRSMsgQueueEmpty();
            
            // check return value
            if(0 == ui8RetVal) // Queue is not empty
            {
                // Data available in queue..stop GPRS queue empty time count
                g_fGPRSQueueEmptyTimeStatus = GPRS_QUEUE_EMPTY_TIME_COUNT_STOP;
                        
                // Send request to enable HTTP mode
                //SendEnableHTTPModeRequest();
                SetHTTPURLParameter();
                
                // Set timeout for 2 seconds
                SetTimeout(HTTP_URL_PARAMETER_SET_TIMEOUT);
                
                // Update flag
                g_fGPRSSequence = GPRS_DATA_SENDING_SEQUENCE_5;
            }
            else // Queue is empty
            {
                // Data is not available..wait until data is available
                // Start GPRS queue empty time count
                g_fGPRSQueueEmptyTimeStatus = GPRS_QUEUE_EMPTY_TIME_COUNT_START;
                
                // Update sequence flag
                g_fGPRSSequence = GPRS_DATA_SENDING_SEQUENCE_4;
                
                // Check queue empty timeout
                if(SHOULD_CHECK_GPRS_CONNECTIVITY == g_fCheckGPRSConnectivity)
                {
                    // Update sequence flag
                    g_fGPRSSequence = GPRS_DATA_SENDING_SEQUENCE_1;
                    
                    // Reset flag
                    g_fCheckGPRSConnectivity = SHOULD_NOT_CHECK_GPRS_CONNECTIVITY;
                }
            }
        }
        // Step 5: Check enable HTTP mode request response
        if(g_fIsTimedOut == 1 && GPRS_DATA_SENDING_SEQUENCE_5 == g_fGPRSSequence)
        {
            // Check response function
            ui8RetVal = SetHTTPURLL();
            
            // Check return value
            if(0 == ui8RetVal) // On success            
            {
                // Update sequence flag
                g_fGPRSSequence = GPRS_DATA_SENDING_SEQUENCE_6;
            }
            else // On failure
            {
                // GPRS module fails to set web service URL properly.
                // Set the failure bit.
                g_fGPRSFailureBit = GPRS_SET_WEB_SERVICE_URL_FAILS;
                
                // Reset queue front to resend the data which gets failure on sending.
                if(g_ui8DataQueueLen >= 5)
                {
                    g_stGPRSMsgSendQueue.m_i8Front -= 4;
                }
                else
                {
                    //g_stGPRSMsgSendQueue.m_i8Front -= 1;
                }
                
                
                // Set the sequence bit.
                g_fGPRSSequence = GPRS_DATA_SENDING_SEQUENCE_8;
                
                // wait for 10 seconds before resend.
                g_fIsTimedOut = 0;
                SetTimeout(DATA_RESEND_INTRVL_TIMEOUT);

            }
        }
        
        // Step 6: Send request to set HTTP Bearer profile ID
        if(GPRS_DATA_SENDING_SEQUENCE_6 == g_fGPRSSequence)
        {
            // Send request to start HTTP get session to hit the URL.
            SendHTTPGetSessionRequest();
            
            // Set timeout of 10 seconds
            SetTimeout(HTTP_GET_SESSION_TIMEOUT);
            
            // Update sequence flag
            g_fGPRSSequence = GPRS_DATA_SENDING_SEQUENCE_7;
        }
        
        // Step 8: Check HTTP bearer profile id set request response
        if(g_fIsTimedOut == 1 && g_fGPRSSequence == GPRS_DATA_SENDING_SEQUENCE_7)
        {
            // Check response
            ui8RetVal = CheckHTTPGETRequestResponse();

            // Check return value
            if(0 == ui8RetVal) // On success
            {
                // Update data sending flag
                g_fShouldSendData = SHOULD_NOT_SEND_DATA;

                //Increase GPRS send msg queue front.
                g_stGPRSMsgSendQueue.m_i8Front++; 
                
                // Check if the sent data is the last data in GPRS queue.
                // If so then initialize GPRS message queue.
                if(1 == g_fIsLastElementInGPRSQueue && (g_stGPRSMsgSendQueue.m_i8Front > g_stGPRSMsgSendQueue.m_i8Rear))
                {
                    // Reset flag.
                    g_fIsLastElementInGPRSQueue = 0;
                    
                    // Reset queue.
                    InitializeGPRSMsgQueue();
                    StuffDummyTimeStamp();
                }

                // Update sequence flag
                g_fGPRSSequence = 1;

                // Check if queue front reached at end
                if(g_stGPRSMsgSendQueue.m_i8Front != -1)
                {
                    if(g_stGPRSMsgSendQueue.m_i8Front >= (SIZE_GPRS_DATA_NODE_QUEUE - 1))
                    {
                        // Reset front
                        g_stGPRSMsgSendQueue.m_i8Front = 0;

                        // Update sequence flag
                        g_fGPRSSequence = 1;
                    }
                }    
            }
            else // On failure
            {       
                // GPRS module fails to hit the URL properly.
                // Set the failure bit.
                g_fGPRSFailureBit = GPRS_START_HTTP_GET_SESSION_FAILS;
                
                // Reset queue front to resend the data which gets failure on sending.
                if(g_ui8DataQueueLen >= 5)
                {
                    g_stGPRSMsgSendQueue.m_i8Front -= 4;
                }
                else
                {
                    //g_stGPRSMsgSendQueue.m_i8Front -= 1;
                }
                
                // Set the sequence bit.
                g_fGPRSSequence = GPRS_DATA_SENDING_SEQUENCE_8;
                
                // wait for 10 seconds before resend.
                g_fIsTimedOut = 0;
                SetTimeout(DATA_RESEND_INTRVL_TIMEOUT);
            }
        }
  
        // Step 15: Handle error;if occur any on GRPS data sending process
        if(g_fIsTimedOut == 1 && g_fGPRSSequence == GPRS_DATA_SENDING_SEQUENCE_8)
        {
            // Check GPRS failure bit flag
            if(GPRS_CONNECTION_FAILS == g_fGPRSFailureBit)
            {
                // Update sequence flag
                g_fGPRSSequence = GPRS_DATA_SENDING_SEQUENCE_1;
        
                 //Update data sending flag
                 g_fShouldSendData = SHOULD_NOT_SEND_DATA;
            }
            else if(GPRS_SET_WEB_SERVICE_URL_FAILS == g_fGPRSFailureBit||
                    GPRS_START_HTTP_GET_SESSION_FAILS == g_fGPRSFailureBit)
            {
                // Update sequence flag
                g_fGPRSSequence = GPRS_DATA_SENDING_SEQUENCE_4;
            }
            
        }
    
		// Reset WDT.
        restart_wdt();                
	}	
}
    	





	
