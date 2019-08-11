#ifndef _VM_TIMER_CONFIG_H_
#define _VM_TIMER_CONFIG_H_


//#include "vmCommon.h"   //TESTING
#include "../Common/viShiftRegister.h"
#include "../Common/vmModbusGlobal.h"
//#include "../Common/vmRS485Config.h"
#include "vmM1Common.h"
#include "../Common/vmIBCConfig.h"


#define T0_DIV_PRESCALE                     T0_DIV_8 
#define TIMER_0_VALUE_LOAD_FOR_1MS          0xFD8F
#define ALL_TIMER_TEST_VAL_FOR_100_US       (0xFFFF - 0x003F + 1)
#define ALL_TIMER_TEST_VAL_FOR_70_US        (0xFFFF - 0x002C + 1)


#define FLAG_DI_STATE_CHANGED               11
#define FLAG_DI_STATE_NOT_CHANGED           12

#define ISR_TOTAL_BYTE_RECEIVE_TIME             9 //5
#define ISR_RECEIVED_ALL_BYTE                   21
#define ISR_PROCESSED_ALL_BYTE                  22
#define ISR_IS_READY                            40
#define ISR_IS_NOT_READY                        41

#define CHECK_FOR_AT_RESPONSE                   15
#define DO_NOT_CHECK_FOR_AT_RESPONSE            16
#define AT_RESPONSE_CHECK_INTRVL_TIMEOUT        60

#define MILLISECOND_TIMEOUT                     1000
#define SECOND_TIMEOUT                          60

#define SHOULD_SEND_DATA                        25
#define SHOULD_NOT_SEND_DATA                    26
#define DATA_SEND_INTERVAL_TIMEOUT              30

#define GPRS_CONNECTED                          1
#define GPRS_NOT_CONNECTED                      0

#define  GPRS_QUEUE_EMPTY_TIME_COUNT_START      80
#define  GPRS_QUEUE_EMPTY_TIME_COUNT_STOP       81
#define  GPRS_QUEUE_EMPTY_TIMEOUT               15

#define  SHOULD_CHECK_GPRS_CONNECTIVITY         55
#define  SHOULD_NOT_CHECK_GPRS_CONNECTIVITY     56

#define  REDAY_TO_RESTART_MCU                   33
#define  DO_NOT_RESTART_MCU                     34
#define  RESTART_MCU_INTERVAL_TIME              15  // In Minutes. Two Consecutive MCU restart will takes place at an interval of 15 minutes.

#define  GPRS_CONNECTION_QUERY_TIMEOUT          3  // In seconds.
#define  HTTP_URL_PARAMETER_SET_TIMEOUT         2
#define  HTTP_GET_SESSION_TIMEOUT               8
#define  DATA_RESEND_INTRVL_TIMEOUT             10

// *****************************************************************************
// ***************** Below -> Global Variable Declarations *********************
// *****************************************************************************

unsigned int16 g_ui16StackerEmptyDelayCount           = 0;
unsigned int8  g_fIsReadyToReceiveByte                = ISR_IS_NOT_READY;
unsigned int8  g_ui8ISRReceiveByteTimeCounter         = 0;
unsigned int8  g_fISRAllByteReceived                  = ISR_PROCESSED_ALL_BYTE;
unsigned int8  g_fDIChangeState                       = FLAG_DI_STATE_NOT_CHANGED;
unsigned int8  g_fShouldCheckForAtResponse            = CHECK_FOR_AT_RESPONSE;
unsigned int16 g_ui16MillisecondToSecondCounter       = 0;
unsigned int8  g_ui8SecondToMinuteCounter             = 0;
unsigned int8  g_ui8ATResponseCheckTimeInterval       = 0;
unsigned int16 g_ui16TimeOutMilliseconds              = 0;
unsigned int8  g_fShouldLookForTimeOut                = 0;
unsigned int16 g_ui16ShouldLookTimerCounter           = 0;
unsigned int8  g_fIsTimedOut                          = 0;
unsigned int8  g_fShouldSendData                      = SHOULD_NOT_SEND_DATA;
unsigned int16 g_ui16DataSendIntervalTime             = 0;
//unsigned int8  g_fIsGPRSConnected                     = 0;
unsigned int8  g_ui8GPRSQueueEmptyTimeCounter         = 0;
unsigned int8  g_fCheckGPRSConnectivity               = SHOULD_NOT_CHECK_GPRS_CONNECTIVITY;
unsigned int8  g_fGPRSQueueEmptyTimeStatus            = 0;
unsigned int8  g_fIsReadyToRestartMCU                 = DO_NOT_RESTART_MCU;
unsigned int8  g_ui8RestartMCUIntrvlTimeCounter       = 0;
unsigned int8  g_ui8RS485ByteCnt                      = 0;
unsigned int8  g_ui8RTSOnCountBefore                  = 0;
unsigned int8  g_ui8RTSOnCountAfter                   = 0;


// *****************************************************************************
// ***************** Above -> Global Variable Declarations *********************
// *****************************************************************************


//******************************************************************************
//**************** Below -> Timer Initialization Function **********************
//******************************************************************************

//Initialize 1MS Timer.
void Init1MSTimer(void)
{
	//setup timer0 for 1MS time.
	setup_timer_0( T0_INTERNAL | T0_DIV_PRESCALE );
	set_timer0(TIMER_0_VALUE_LOAD_FOR_1MS);

	//Enable timer0
	clear_interrupt( INT_TIMER0 );
	enable_interrupts( INT_TIMER0 );
}

#INT_TIMER0
void timer0_isr(void)
{   
    
	// Increase millisecond counter at each millisecond.
    g_ui16MillisecondToSecondCounter++;  //=========================================  1
    
////    if (IBC_TIMER_0_CRITICAL_ON == g_ui8fTimer0State)
////    {
////        // Reset Timer0.
////        set_timer0( TIMER_0_VALUE_LOAD_FOR_1MS );
////        clear_interrupt( INT_TIMER0 );
////        
////        return;
////    }
    
     // Check if millisecond counter overflows
    if(g_ui16MillisecondToSecondCounter >= MILLISECOND_TIMEOUT)  //==================== 2
    {
        // Reset counter.
        g_ui16MillisecondToSecondCounter = 0;
        
        // 1000MS i.e. 1second completed.. put stuff here that repeats after every second.
        
        // Increase each second counter
        g_ui8SecondToMinuteCounter++;
        
        // If data should not send through GPRS then count data send interval time
        if(SHOULD_NOT_SEND_DATA == g_fShouldSendData)
        {
            // Increase data send interval time.
            g_ui16DataSendIntervalTime++;
        }
        
        // Increment counter to Sync with HMI Time to get proper timestamp.
        g_ui16RegisterTime_Diff++;
    }   
    
    // Check If There is data ready to send HMI through RS485
    if (FLAG_RS485_DATA_READY == g_fRS485DataReady) // =================================== 4
    {
        // Raise RTS Pin On to hold the bus
        output_bit(PIN_RS485_RTS, 1);
        if (g_ui8RTSOnCountBefore++ >= DELAY_TIME_RTS_BEFORE)
        {
            g_ui8RTSOnCountBefore = 0;
            g_ui8RS485ByteCnt = 0;
            g_ui8fRS485DataSend = FLAG_RS485_DATA_SEND_ON;
            g_fRS485DataReady = FLAG_RS485_DATA_NOT_READY;
            
        }
        
    }
    
    // Check for RTS Flag
    if (FLAG_RS485_DATA_SEND_DONE == g_ui8fRS485DataSend) // ================================ 5
    {

        if (g_ui8RTSOnCountAfter++ >= DELAY_TIME_RTS_AFTER)
        {
            // Release RTS Pin
            output_bit(PIN_RS485_RTS,0);
            g_ui8RTSOnCountAfter = 0;
            g_ui8fRS485DataSend = FLAG_RS485_DATA_SEND_OFF;
        }
    }
    
    // AR start
    // Checking ISR received byte flag.
    if(ISR_IS_READY == g_fIsReadyToReceiveByte) // ===========================================
    {
        // Increase the counter.
        g_ui8ISRReceiveByteTimeCounter++;
    }
    
    if(ISR_TOTAL_BYTE_RECEIVE_TIME == g_ui8ISRReceiveByteTimeCounter) // ============================
    {
        // Update all received byte flag.
        g_fISRAllByteReceived = ISR_RECEIVED_ALL_BYTE;
        
        // Update Is received byte flag. 
        g_fIsReadyToReceiveByte =  ISR_IS_NOT_READY;
        
        // Reset Counter
        g_ui8ISRReceiveByteTimeCounter = 0;
    }
    // AR end
    
    if (IBC_TIMER_0_CRITICAL_ON == g_ui8fTimer0State)
    {
        // Reset Timer0.
        set_timer0( TIMER_0_VALUE_LOAD_FOR_1MS );
        clear_interrupt( INT_TIMER0 );
        
        return;
    }
    
    // ############################### Non Critical ###############################

    
    // Check if second counter overflows
    if(g_ui8SecondToMinuteCounter >= SECOND_TIMEOUT)
    {
        // Reset counter
        g_ui8SecondToMinuteCounter = 0;
        
        // 60Seconds i.e. 1 Minute completed.. put stuff here that repeats after every minute.
        
        // Increase AT response check time interval counter..allows upto 60 minutes i.e. for 1 hour
        g_ui8ATResponseCheckTimeInterval++;
        
        // Check GPRS queue empty time status
        if(GPRS_QUEUE_EMPTY_TIME_COUNT_START == g_fGPRSQueueEmptyTimeStatus)
        {
            // Increase time count
            g_ui8GPRSQueueEmptyTimeCounter++;
        }
        else if(GPRS_QUEUE_EMPTY_TIME_COUNT_STOP == g_fGPRSQueueEmptyTimeStatus)
        {
            // when data available..reset time count
            g_ui8GPRSQueueEmptyTimeCounter = 0;
        }
        
        // Check ready to restart MCU flag.
        if(g_fIsReadyToRestartMCU >= DO_NOT_RESTART_MCU)
        {
            // Increment counter.
            g_ui8RestartMCUIntrvlTimeCounter++;
        }
    }
    
    // Check if AT response check interval time overflow.
    if(AT_RESPONSE_CHECK_INTRVL_TIMEOUT == g_ui8ATResponseCheckTimeInterval)
    {
        // Reset counter
        g_ui8ATResponseCheckTimeInterval = 0;
        
        // Update AT response check flag.
        g_fShouldCheckForAtResponse = CHECK_FOR_AT_RESPONSE;
    }
    

    
    // Check if timer should look for timeout
    if(g_fShouldLookForTimeOut)
    {
        // Increase timer counter
        g_ui16ShouldLookTimerCounter++;
        
        // Check if timer counter reached timeout value
        if(g_ui16ShouldLookTimerCounter >= g_ui16TimeOutMilliseconds)
        {
            // Set flag denoting that timer declares set time is timed out
            g_fIsTimedOut = 1;
            
            // Reset variables
            g_fShouldLookForTimeOut      = 0;
            g_ui16ShouldLookTimerCounter = 0;
            g_ui16TimeOutMilliseconds    = 0;
        }
    }
    
    // Check GPRS queue empty time count
    if(GPRS_QUEUE_EMPTY_TIMEOUT == g_ui8GPRSQueueEmptyTimeCounter)
    {
        // Set flag that data is not available in GPRS queue for 15 minutes..check GPRS connection
        g_fCheckGPRSConnectivity = SHOULD_CHECK_GPRS_CONNECTIVITY;
        
        // Reset counter
        g_ui8GPRSQueueEmptyTimeCounter = 0;
    }
    
    // Check if data send interval time reached timeout
    if(DATA_SEND_INTERVAL_TIMEOUT == g_ui16DataSendIntervalTime)
    {
        // Update should send data flag
        g_fShouldSendData = SHOULD_SEND_DATA;
        
        // Reset variable
        g_ui16DataSendIntervalTime = 0;
    }
    
    // Check restart MCU interval time counter.
    if(g_ui8RestartMCUIntrvlTimeCounter >= RESTART_MCU_INTERVAL_TIME)
    {
        // Update flag status.
        g_fIsReadyToRestartMCU = REDAY_TO_RESTART_MCU;
        
        // Reset counter.
        g_ui8RestartMCUIntrvlTimeCounter = 0;
    }
    
    // TESTING
	//output_toggle(PIN_IN_EXT_DI_00);
    
	// Reset Timer0.
    set_timer0( TIMER_0_VALUE_LOAD_FOR_1MS );
    clear_interrupt( INT_TIMER0 );	
}

//////// TESTING Start
//////void InitTIMER1(void)
//////{
//////     // Setup Timer0 at 1 ms Time.
//////    setup_timer_1( T1_INTERNAL | T1_DIV_BY_8 );
//////    set_timer0( ALL_TIMER_TEST_VAL_FOR_70_US );
//////    // Enable Timer0.
//////    clear_interrupt( INT_TIMER1 );
//////    enable_interrupts( INT_TIMER1 );
//////}
//////
//////void InitTIMER3(void)
//////{
//////     // Setup Timer0 at 1 ms Time.
//////    setup_timer_3( T3_INTERNAL | T3_DIV_BY_8 );
//////    set_timer0( ALL_TIMER_TEST_VAL_FOR_70_US );
//////    // Enable Timer0.
//////    clear_interrupt( INT_TIMER3 );
//////    enable_interrupts( INT_TIMER3 );
//////}
//////
//////// TESTING End.

//******************************************************************************
//**************** Above -> Timer Initialization Function **********************
//******************************************************************************




void ExecuteALLDOs(void)
{
//    delay_us(10);
//    output_bit(PIN_OUT_DO_00, g_unDOState.m_stAllDOBits.m_ui1HWDOCn00); // Solenoid opeartion.
//    //output_bit(PIN_OUT_DO_01, g_unDOState.m_stAllDOBits.m_ui1HWDOCn01); // Shipper rejection RED LED.
//    //output_bit(PIN_OUT_DO_02, g_unDOState.m_stAllDOBits.m_ui1HWDOCn02);
//    output_bit(PIN_OUT_DO_03, g_unDOState.m_stAllDOBits.m_ui1HWDOCn03); // Ready To Feed.
//    //output_bit(PIN_OUT_DO_04, g_unDOState.m_stAllDOBits.m_ui1HWDOCn04);
//    //output_bit(PIN_OUT_DO_05, g_unDOState.m_stAllDOBits.m_ui1HWDOCn05);
//    //output_bit(PIN_OUT_DO_06, g_unDOState.m_stAllDOBits.m_ui1HWDOCn06);
//    //output_bit(PIN_OUT_DO_07, g_unDOState.m_stAllDOBits.m_ui1HWDOCn07);   
}

//******************************************************************************
//******************* Below -> 1MS Timer Implementation ************************
//******************************************************************************

////////#INT_TIMER0
////////void timer0_isr(void)
////////{   
////////    
////////	// Increase millisecond counter at each millisecond.
////////    g_ui16MillisecondToSecondCounter++;  //=========================================  1
////////    
////////    // Check if millisecond counter overflows
////////    if(g_ui16MillisecondToSecondCounter >= MILLISECOND_TIMEOUT)  //==================== 2
////////    {
////////        // Reset counter.
////////        g_ui16MillisecondToSecondCounter = 0;
////////        
////////        // 1000MS i.e. 1second completed.. put stuff here that repeats after every second.
////////        
////////        // Increase each second counter
////////        g_ui8SecondToMinuteCounter++;
////////        
////////        // If data should not send through GPRS then count data send interval time
////////        if(SHOULD_NOT_SEND_DATA == g_fShouldSendData)
////////        {
////////            // Increase data send interval time.
////////            g_ui16DataSendIntervalTime++;
////////        }
////////        
////////        // Increment counter to Sync with HMI Time to get proper timestamp.
////////        g_ui16RegisterTime_Diff++;
////////    }
////////    
////////    // Check if second counter overflows
////////    if(SECOND_TIMEOUT == g_ui8SecondToMinuteCounter) // =============================== 3
////////    {
////////        // Reset counter
////////        g_ui8SecondToMinuteCounter = 0;
////////        
////////        // 60Seconds i.e. 1 Minute completed.. put stuff here that repeats after every minute.
////////        
////////        // Increase AT response check time interval counter..allows upto 60 minutes i.e. for 1 hour
////////        g_ui8ATResponseCheckTimeInterval++;
////////        
////////        // Check GPRS queue empty time status
////////        if(GPRS_QUEUE_EMPTY_TIME_COUNT_START == g_fGPRSQueueEmptyTimeStatus)
////////        {
////////            // Increase time count
////////            g_ui8GPRSQueueEmptyTimeCounter++;
////////        }
////////        else if(GPRS_QUEUE_EMPTY_TIME_COUNT_STOP == g_fGPRSQueueEmptyTimeStatus)
////////        {
////////            // when data available..reset time count
////////            g_ui8GPRSQueueEmptyTimeCounter = 0;
////////        }
////////        
////////        // Check ready to restart MCU flag.
////////        if(DO_NOT_RESTART_MCU == g_fIsReadyToRestartMCU)
////////        {
////////            // Increment counter.
////////            g_ui8RestartMCUIntrvlTimeCounter++;
////////        }
////////    }
////////    
////////    // Check If There is data ready to send HMI through RS485
////////    if (FLAG_RS485_DATA_READY == g_fRS485DataReady) // =================================== 4
////////    {
////////        // Raise RTS Pin On to hold the bus
////////        output_bit(PIN_RS485_RTS, 1);
////////        if (g_ui8RTSOnCountBefore++ >= DELAY_TIME_RTS_BEFORE)
////////        {
////////            g_ui8RTSOnCountBefore = 0;
////////            g_ui8RS485ByteCnt = 0;
////////            g_ui8fRS485DataSend = FLAG_RS485_DATA_SEND_ON;
////////            g_fRS485DataReady = FLAG_RS485_DATA_NOT_READY;
////////            
////////        }
////////        
////////    }
////////        
////////
////////
////////    // Check for RTS Flag
////////    if (FLAG_RS485_DATA_SEND_DONE == g_ui8fRS485DataSend) // ================================ 5
////////    {
////////
////////        if (g_ui8RTSOnCountAfter++ >= DELAY_TIME_RTS_AFTER)
////////        {
////////            // Release RTS Pin
////////            output_bit(PIN_RS485_RTS,0);
////////            g_ui8RTSOnCountAfter = 0;
////////            g_ui8fRS485DataSend = FLAG_RS485_DATA_SEND_OFF;
////////        }
////////    }
////////    // Check if AT response check interval time overflow.
////////    if(AT_RESPONSE_CHECK_INTRVL_TIMEOUT == g_ui8ATResponseCheckTimeInterval)
////////    {
////////        // Reset counter
////////        g_ui8ATResponseCheckTimeInterval = 0;
////////        
////////        // Update AT response check flag.
////////        g_fShouldCheckForAtResponse = CHECK_FOR_AT_RESPONSE;
////////    }
////////    
////////    // AR start
////////    // Checking ISR received byte flag.
////////    if(ISR_IS_READY == g_fIsReadyToReceiveByte) // ===========================================
////////    {
////////        // Increase the counter.
////////        g_ui8ISRReceiveByteTimeCounter++;
////////    }
////////    
////////    if(ISR_TOTAL_BYTE_RECEIVE_TIME == g_ui8ISRReceiveByteTimeCounter) // ============================
////////    {
////////        // Update all received byte flag.
////////        g_fISRAllByteReceived = ISR_RECEIVED_ALL_BYTE;
////////        
////////        // Update Is received byte flag. 
////////        g_fIsReadyToReceiveByte =  ISR_IS_NOT_READY;
////////        
////////        // Reset Counter
////////        g_ui8ISRReceiveByteTimeCounter = 0;
////////    }
////////    // AR end
////////    
////////    // Check if timer should look for timeout
////////    if(g_fShouldLookForTimeOut)
////////    {
////////        // Increase timer counter
////////        g_ui16ShouldLookTimerCounter++;
////////        
////////        // Check if timer counter reached timeout value
////////        if(g_ui16ShouldLookTimerCounter >= g_ui16TimeOutMilliseconds)
////////        {
////////            // Set flag denoting that timer declares set time is timed out
////////            g_fIsTimedOut = 1;
////////            
////////            // Reset variables
////////            g_fShouldLookForTimeOut      = 0;
////////            g_ui16ShouldLookTimerCounter = 0;
////////            g_ui16TimeOutMilliseconds    = 0;
////////        }
////////    }
////////    
////////    // Check GPRS queue empty time count
////////    if(GPRS_QUEUE_EMPTY_TIMEOUT == g_ui8GPRSQueueEmptyTimeCounter)
////////    {
////////        // Set flag that data is not available in GPRS queue for 15 minutes..check GPRS connection
////////        g_fCheckGPRSConnectivity = SHOULD_CHECK_GPRS_CONNECTIVITY;
////////        
////////        // Reset counter
////////        g_ui8GPRSQueueEmptyTimeCounter = 0;
////////    }
////////    
////////    // Check if data send interval time reached timeout
////////    if(DATA_SEND_INTERVAL_TIMEOUT == g_ui16DataSendIntervalTime)
////////    {
////////        // Update should send data flag
////////        g_fShouldSendData = SHOULD_SEND_DATA;
////////        
////////        // Reset variable
////////        g_ui16DataSendIntervalTime = 0;
////////    }
////////    
////////    // Check restart MCU interval time counter.
////////    if(g_ui8RestartMCUIntrvlTimeCounter >= RESTART_MCU_INTERVAL_TIME)
////////    {
////////        // Update flag status.
////////        g_fIsReadyToRestartMCU = REDAY_TO_RESTART_MCU;
////////        
////////        // Reset counter.
////////        g_ui8RestartMCUIntrvlTimeCounter = 0;
////////    }
////////    
////////    // TESTING
////////	//output_toggle(PIN_IN_EXT_DI_00);
////////    
////////	// Reset Timer0.
////////    set_timer0( TIMER_0_VALUE_LOAD_FOR_1MS );
////////    clear_interrupt( INT_TIMER0 );	
////////}

//////// TESTING Start
//////#INT_TIMER1
//////void timer_1_isr(void)
//////{
//////    output_toggle(PIN_IN_EXT_DI_01);
//////    
//////     // Reset Timer1.
//////    set_timer1( ALL_TIMER_TEST_VAL_FOR_70_US );
//////    clear_interrupt( INT_TIMER1 );
//////}
//////
//////
//////#INT_TIMER3
//////void timer_3_isr(void)
//////{
//////    output_toggle(PIN_IN_EXT_DI_02);
//////    
//////     // Reset Timer1.
//////    set_timer3( ALL_TIMER_TEST_VAL_FOR_70_US );
//////    clear_interrupt( INT_TIMER3 );
//////}
//////// TESTING End

//******************************************************************************
//******************* Above -> 1MS Timer Implementation ************************
//******************************************************************************

void SetTimeout(unsigned int8 ui8TimedOutSeconds)
{
    // Convert timeout seconds into milliseconds
    g_ui16TimeOutMilliseconds = ui8TimedOutSeconds * 1000;
    
    // Set flag denoting now timer should look for timeout value
    g_fShouldLookForTimeOut = 1;
}


#endif /* _VM_TIMER_CONFIG_H_ */