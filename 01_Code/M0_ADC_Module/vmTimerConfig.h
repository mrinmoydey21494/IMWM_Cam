
#ifndef _VM_TIMER_CONFIG_H_
#define _VM_TIMER_CONFIG_H_

#include "vmSystemConfig.h"
#include "vmStateMachineForIMW_M0.h"
#include "vmDigitalIOConfig.h"
#include "vmADCManager.h"
#include "../Common/vmIBCConfig.h"



#define T0_DIV_PRESCALE                 RTCC_DIV_8
#define TIMER_0_VALUE_LOAD_FOR_1MS      (0xFFFF - 0x0271 + 1)
#define ALL_TIMER_TEST_VAL_FOR_100_US   (0xFFFF - 0x003F + 1)
#define ALL_TIMER_TEST_VAL_FOR_70_US    (0xFFFF - 0x002C + 1)


#define FLAG_DI_STATE_NOT_CHANGED    1
#define FLAG_DI_STATE_CHANGED        0
#define ADC_READ_ON                  10
#define ADC_READ_OFF                 11

//#define DELAY_PUSHER               400
//#define DELAY_CAN_STPR_1           2000
//#define DELAY_CAN_PUSHER_1_TIMER_2 500
//#define DELAY_START_CAN_PLACER     20
#define DELAY_INTRMDAT_DATA_SEND     300
#define DELAY_REJECT_PUSHER_BACK     1000
#define PUSHER_DELAY_START_ON          0
#define PUSHER_DELAY_START_OFF         1

#define BOX_LEFT_DELAY_START           0
#define BOX_LEFT_DELAY_END             1
#define AFTER_BOX_LEFT_DELAY_TIME      500

// TESTING Start
#define DELAY_GET_ADC_DATA           500
unsigned int16 g_uiCountSendADCData = 0;
// TESTING End


unsigned int8  g_fDIChangeState =  FLAG_DI_STATE_NOT_CHANGED;
unsigned int8  g_fADCCurrState  =  ADC_READ_OFF;
unsigned int16 g_ui16IntmidDataSendTikCount = 0;
unsigned int16 g_uiPusherDelayTikCount      = 0;
int8 g_fPusherDelayStatus = PUSHER_DELAY_START_OFF;
int8 g_fRejectPusherPrevState = 1;
//unsigned int8  g_fBeforeHomeStateDelayStatus = BOX_LEFT_DELAY_END;
unsigned int16  g_ui16AfterBoxLeftDelayTimeCounter = 0;

//*****************************************************************************
// Below -> 1MS Timer Initialization Functions ********************************
//*****************************************************************************
// Init Polling Timer.
void InitDigitalIOPollingTimer(void)
{
    // Setup Timer0 at 1 ms Time.
    setup_timer_0( RTCC_INTERNAL | T0_DIV_PRESCALE );
    set_timer0( TIMER_0_VALUE_LOAD_FOR_1MS );  // TODO => Need to change the value load in timer
    // Enable Timer0.
    clear_interrupt( INT_TIMER0 );
    enable_interrupts( INT_TIMER0 );
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
//*****************************************************************************
// Above -> 1MS Timer Initialization Functions ********************************
//*****************************************************************************

//void ExecuteALLDOs(void)
//{
//    delay_us(10);
//    output_bit(PIN_OUT_DO_00, g_unDOCurrState.m_stAllDOBits.m_ui1HWDOCn00);
//    output_bit(PIN_OUT_DO_01, g_unDOCurrState.m_stAllDOBits.m_ui1HWDOCn01);
//    output_bit(PIN_OUT_DO_02, g_unDOCurrState.m_stAllDOBits.m_ui1HWDOCn02);
//    output_bit(PIN_OUT_DO_03, g_unDOCurrState.m_stAllDOBits.m_ui1HWDOCn03);
//    output_bit(PIN_OUT_DO_04, g_unDOCurrState.m_stAllDOBits.m_ui1HWDOCn04);
//    output_bit(PIN_OUT_DO_05, g_unDOCurrState.m_stAllDOBits.m_ui1HWDOCn05);
//    output_bit(PIN_OUT_DO_06, g_unDOCurrState.m_stAllDOBits.m_ui1HWDOCn06);
//    output_bit(PIN_OUT_DO_07, g_unDOCurrState.m_stAllDOBits.m_ui1HWDOCn07);   
//}

     


//*****************************************************************************
// Below -> 1MS Timer Implementation ******************************************
//*****************************************************************************

#INT_TIMER0
void DigitalIOPolling_isr(void)
{
    //output_high(PIN_OUT_DO_02);
    if (g_fIgnore_TIMER == TIMER_IGNORE_ON)
    {
        return;
    }
////////    if (IBC_TIMER_0_CRITICAL_ON == g_ui8fTimer0State)
////////    {
////////        // Reset Timer0.
////////        set_timer0( TIMER_0_VALUE_LOAD_FOR_1MS );
////////        clear_interrupt( INT_TIMER0 );
////////        
////////        return;
////////    }    
    // Step 3: Check if the ADC is ready with data or not
    // Check if ADC is ready
    if (ADC_SCAN_ON == g_ui8ADCScanningStatus && ADC_DATA_READY == g_fADCDataReadyStatus)
    {
        // Increment the Counter
        g_stADCDataQ.m_i16Rear++;
        if (g_stADCDataQ.m_i16Rear >= SIZE_DATA_QUEUE)
        {
            g_stADCDataQ.m_i16Rear       = 0;
            g_stADCDataQ.m_ui8Multiplier = 1;
        }

        g_stADCDataQ.m_ui32Arr[g_stADCDataQ.m_i16Rear] = g_ui32ADCValue;

        // Reset Temp ADC Variable
        g_ui32ADCValue = 0;
        // Reset Data Ready Status
        g_fADCDataReadyStatus = ADC_DATA_NOT_READY;
    }
    
    // Check for Box after box left delay status.
    if(BOX_LEFT_DELAY_START == g_unDIState.m_stAllDIBits.m_ui1GDICn03)
    {
        // Delay time count start.
        if(++g_ui16AfterBoxLeftDelayTimeCounter >= AFTER_BOX_LEFT_DELAY_TIME)
        {
            // Reset counter.
            g_ui16AfterBoxLeftDelayTimeCounter = 0;
            
            // Update flag status.
            g_unDIState.m_stAllDIBits.m_ui1GDICn03 = BOX_LEFT_DELAY_END;
            
        }
    }
    
    if (IBC_TIMER_0_CRITICAL_ON == g_ui8fTimer0State)
    {
        // Reset Timer0.
        set_timer0( TIMER_0_VALUE_LOAD_FOR_1MS );
        clear_interrupt( INT_TIMER0 );
        
        return;
    }
    // ############################### Non Critical ###############################
    
    // Digital Input (DI) Polling Start
    g_unDIState.m_stAllDIBits.m_ui1HWDICn00 = input(PIN_IN_DI_00);
    g_unDIState.m_stAllDIBits.m_ui1HWDICn01 = input(PIN_IN_DI_01);
    g_unDIState.m_stAllDIBits.m_ui1HWDICn02 = 1;//input(PIN_IN_DI_02);
    g_unDIState.m_stAllDIBits.m_ui1HWDICn03 = input(PIN_IN_DI_03);
    g_unDIState.m_stAllDIBits.m_ui1HWDICn04 = input(PIN_IN_DI_04);
    g_unDIState.m_stAllDIBits.m_ui1HWDICn05 = 1; //input(PIN_IN_DI_05);
    g_unDIState.m_stAllDIBits.m_ui1HWDICn06 = 1; //input(PIN_IN_DI_06);
    g_unDIState.m_stAllDIBits.m_ui1HWDICn07 = 1; //input(PIN_IN_DI_07);

    // TESTING Start
    //g_unDIState.m_stAllDIBits.m_ui1HWDICn05 = 1;
    // TESTING End

    // Step 2: Check for any state change
    // Check ALL DI States Changes and all DO State Changes
    if (g_unDIState.m_ui32AllDIs == g_ui32AllDIsPreviousState &&
        g_unDOState.m_ui16AllDOs == g_ui32AllDOsPreviousState)
    {
        g_uiStateChange = FLAG_STATE_NOT_CHANGED;
    }
    else
    {
        // Change in DO state noticed
        if(g_unDOState.m_ui16AllDOs != g_ui32AllDOsPreviousState)
        {
            // Execute all DOs
            ExecuteALLDOs();
        }
        
        g_uiStateChange = FLAG_STATE_CHANGED;
        g_ui32AllDIsPreviousState = g_unDIState.m_ui32AllDIs;
        g_ui32AllDOsPreviousState = g_unDOState.m_ui16AllDOs;
    }




    //----------------------------------------------------
    // Extra Timers Start
    //----------------------------------------------------
    if (COLLECT_DYNAMIC_TARE_DATA == g_stADCDataQ.m_i8CollectionState ||
        COLLECT_WEIGHING_DATA     == g_stADCDataQ.m_i8CollectionState)
    {
        // Increment tick count
        if (g_ui16IntmidDataSendTikCount++ >= DELAY_INTRMDAT_DATA_SEND)
        {
            // Reset Counter
            g_ui16IntmidDataSendTikCount = 0;

            g_fIntermediateDataSend = IBC_INTERMEDIATE_DATA_SEND_ON; 
        }
    }
    //----------------------------------------------------
    // Extra Timers End
    //----------------------------------------------------    

    if (PUSHER_DELAY_START_ON == g_fPusherDelayStatus)
    {
        g_uiPusherDelayTikCount++;
        if (g_uiPusherDelayTikCount >= DELAY_REJECT_PUSHER_BACK)
        {
            g_uiPusherDelayTikCount = 0;
            g_unDIState.m_stAllDIBits.m_ui1GDICn02 = 0;
            //g_unDOState.m_stAllDOBits.m_ui1HWDOCn01 = 1;
            
            g_fPusherDelayStatus = PUSHER_DELAY_START_OFF;
        }
    }
    
    //TESTING
    //output_toggle(PIN_OUT_DO_03);
    
    // Reset Timer0.
    set_timer0( TIMER_0_VALUE_LOAD_FOR_1MS );  // TODO => 
    clear_interrupt( INT_TIMER0 );
}


//////// TESTING Start
//////#INT_TIMER1
//////void timer_1_isr(void)
//////{
//////    output_toggle(PIN_OUT_DO_04);
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
//////    output_toggle(PIN_OUT_DO_05);
//////    
//////     // Reset Timer1.
//////    set_timer3( ALL_TIMER_TEST_VAL_FOR_70_US );
//////    clear_interrupt( INT_TIMER3 );
//////}
////// //TESTING End


//*****************************************************************************
// Above -> 1MS Timer Implementation ******************************************
//*****************************************************************************

#endif /* _VM_TIMER_CONFIG_H_ */

