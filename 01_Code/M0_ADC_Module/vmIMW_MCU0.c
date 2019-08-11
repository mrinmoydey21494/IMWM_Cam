
#include "vmSystemConfig.h"
#include "../Common/vmIBCConfig.h"
#include "../Common/vmIBCGlobal.h"
#include "../Common/vmADS1131.h"
#include "../Common/vmEEPROMConfig.h"
#include "../Common/vmStateMachine.h"

#include "vmTimerConfig.h"
#include "vmStateMachine_0.h"
#include "vmStateMachine_1.h"
#include "vmIBCProcessM0.h"
#include "vmCalibration.h"
#include "vmAverageWeightCalculation.h"
#include "vmEEPROMConfig_Britania.h"


void InitGlobalVariables(void)
{
    
    g_manualModeActionOnHw = MANUAL_MODE_ACTION_ON_NOTHING;
    
    g_ui16DIStatus = 0x1A;
    g_ui16DOStatus = 0x1A;
    g_ui16ErrorStatus = 0x1A;
    g_ui8IncrementIndex = 0;
    
    // Set all pins digital.
    setup_adc_ports(NO_ANALOGS);
    
    // Initialize IBC Send Message Queue
    //InitializeDataSendQueue();
    InitializeDataQueue(&g_stIBCRcvQueue);
    InitializeDataQueue(&g_stIBCSendQueue);
    // Initialize Average Weight Queue
    InitializeAvgWtQueue();
    
    // Wakeup ADS1131
    output_high(PIN_ADC_PDWN);
    output_high(PIN_ADC_SCLK);
    
    int8 i8Count = 0;
    for(i8Count = 0; i8Count < 100; i8Count++)
    {
        delay_us(1);
        output_high(PIN_ADC_SCLK);
        delay_us(1);
        output_low(PIN_ADC_SCLK);
    }
    
    // TESTING Start
    g_ui8ADCScanningStatus = ADC_SCAN_ON;
    
    // Reset Data Queue
    ResetADCDataQ(COLLECT_DYNAMIC_TARE_DATA);   // TODO it should be COLLECT_NOTHING for TESTING we are using
                                      // COLLECT_DYNAMIC_TARE_DATA
    // Initialize Batch Info 
    g_stCurBatchInfo.m_i16AvgWeight = 1000;
    g_stCurBatchInfo.m_i16NegativeTol = 50;
    g_stCurBatchInfo.m_i16PositiveTol = 50;
    
    // Initialize last average of CBBs in average weight calculations.
    g_stAvgWtQueue.m_i16LstAvg = 0;

    // Initialize Calib Reset Value
    g_unIBCResetCalibAck.m_stIBCResetCalibPoint.m_i16CalibValue = 0;
    g_unIBCResetCalibAck.m_stIBCResetCalibPoint.m_i16PassCount  = 0;
    
    // Get the stored load cell factor from EEPROM.
    g_i16LoadCellFactor = ReadLCFFromEEPROM();
}

void InitSystemInterrupts(void)
{
    // Initialize external interrupt for InterBoard Communication.
    ext_int_edge(3, H_TO_L); 
    clear_interrupt(INT_EXT3);
    enable_interrupts(INT_EXT3);
    
    
     // Enable 1MS main timer.
    clear_interrupt(INT_TIMER0);
    enable_interrupts(INT_TIMER0);
    

    // Enable timer interrupts for InterBoard Communication.
    clear_interrupt(INT_TIMER1);
    enable_interrupts(INT_TIMER1);
    clear_interrupt(INT_TIMER3);
    enable_interrupts(INT_TIMER3);

    // Enable RS232 interrupt.
    clear_interrupt(INT_RDA);
	enable_interrupts(INT_RDA);
    clear_interrupt(INT_RDA2);
	enable_interrupts(INT_RDA2);
    
    // Init Modbus Interrupt
    //InitRS485RxInterrupt();

    
}

void InitializeStateMachines()
{
    g_ui8NumberOfActiveMachines = NO_OF_ACTIVE_MACHINE;
//    g_unHWDIState.m_ui32AllDIs          = INITAIL_DI_STATE_ON_PWR_ON;
//    g_ui32AllHWDIsPreviousState         = INITAIL_DI_STATE_ON_PWR_ON;
//    g_ui32AllSWDIsPreviousState         = INITAIL_DI_STATE_ON_PWR_ON;
//    g_unHWDOState.m_ui16AllDOs          = INITAIL_DO_STATE_ON_PWR_ON;
    g_ui32AllDOsCurrentState            = INITAIL_DO_STATE_ON_PWR_ON;
    g_ui32AllDOsPreviousState           = INITAIL_DO_STATE_ON_PWR_ON;
    g_ui32AllCountersCurrentState       = 0;
    g_ui32AllCountersPreviousState      = 0;
    
//    g_unHWDIState.m_ui32AllDIs          = INITAIL_DI_STATE_ON_PWR_ON;
    //g_unHWDOState.m_ui16AllDOs        = INITAIL_DO_STATE_ON_PWR_ON;
    
    // This is only for TESTING BIG Coils 
 //   g_fStateMac1StartCoilTransfer        = SPCL_FLAG_SM_1_START_TRANSFER_OFF;
    
    // Initialize Counter
    //g_unAllCounters.m_ui16AllCounters   = 0;
    
    Initialize_StateMachine_0();
    Initialize_StateMachine_1();
    
    // Initialize DI Filter
//    g_unHWDIStateFilter.m_ui32AllDIs    = 0; // We are initializing with 0 as it is a OR flag
}    


void main()
{
    // Set Watchdog timer ON
    setup_wdt(WDT_ON);
    
    // Initialization Section Start
    int8 i8StateMachineIndex = 0;
    
    InitGlobalVariables();
    InitSystemInterrupts();
    InitializeStateMachines(); //TODO Must go to Pre Production Mode
    InitilizeStates();
    delay_ms(10);
    
    // Initialize 1MS main timer i.e.=>TIMER0.
    InitDigitalIOPollingTimer();
    //delay_ms(1000);
    
//////    // TESTING Start.
//////        // Initialize TIMER1.
//////        InitTIMER1();
//////        delay_ms(1000);
//////        // Initialize TIMER3.
//////        InitTIMER3();
//////    // TESTING End.
      
    // Enable Global Interrupts.
    enable_interrupts(GLOBAL);   
    
    
    // TESTING Start
    ST_IBC_DATA_NODE stIBCData;
    stIBCData.g_ui8ArrIBCData[0] = 32;
    stIBCData.g_ui8ArrIBCData[1] = 64;
    stIBCData.g_ui8ArrIBCData[2] = 128;
    stIBCData.g_ui8ArrIBCData[3] = 0;
    stIBCData.g_ui8ArrIBCData[4] = 0;
    //InsertSendQueue(&stIBCData);

    g_ui8SystemMode = SYS_MODE_NOTHING;
  
    unsigned int32 arr[200];
    //unsigned int16 ui16X = 0;
    
    // Reset WDT.
	restart_wdt();

    // Set Rejection State ON
    g_unDIState.m_stAllDIBits.m_ui1GDICn01  = BOX_REJECTED;
    g_unDIState.m_stAllDIBits.m_ui1GDICn02  = 1;
    g_unDOState.m_stAllDOBits.m_ui1HWDOCn01 = 1;
    
    for(;;)
    {
        // Check if Mean Calculation is started
        if (ADC_MEAN_CALCULATION_START == g_ui8ADCMeanCalState)
        {
            StartADCSplitAvg();
        }
        if (ADC_MEAN_CALCULATION_RUNNING == g_ui8ADCMeanCalState)
        {
            GetADCSplitSum();
        }
        if (ADC_MEAN_CALCULATION_DONE == g_ui8ADCMeanCalState)
        {
            // Mean Value Calculated
            // Will get mean value at g_ui32ADCCurMeanValue
            AfterADCMeanCalculationDone();
        }
        
        
        
        if (1 == g_fIBCDataReceived)
        {
            // The complete 5 byte data has been received
            // Now we can process the data
            g_fIBCDataReceived = 0;

            // Insert Message into IBC Receive Queue
            InsertIBCMsgQueue(&g_stIBCRcvQueue, &g_stIBCRcvData);
        }
        
        // Check if there is any message in IBC Send Queue
        if (IsMSGInIBCQueue(&g_stIBCSendQueue) && (0 == g_fIBCSendBusy))
        {
            // Make IBC Send Busy
            g_fIBCSendBusy = 1;
            // Call Send IBC Message
            SendIBCMSGFromQ();
        }
        // Check if there is any message in IBC Receive Queue
        if (IsMSGInIBCQueue(&g_stIBCRcvQueue))
        {
            // Process IBC 
            ProcessIBCMsg();
        }

        // Check if ADC is ready
        if (0 == input(PIN_ADC_DOUT_DRDY) && IBC_SEND_UNLOCK == g_fIBCSendLockStatus)
        {
            g_ui32ADCValue = read_ext_adc();

            if (g_ui32ADCValue)
            {
                g_fADCDataReadyStatus = ADC_DATA_READY;
            }
        }

        // Check If We need to send Intermediate Weighing or Tare Data to HMI
        if (IBC_INTERMEDIATE_DATA_SEND_ON == g_fIntermediateDataSend)
        {
            // Reset Flag
            g_fIntermediateDataSend = IBC_INTERMEDIATE_DATA_SEND_OFF;
            
            // Check Weighing is running or not
            if (COLLECT_WEIGHING_DATA == g_stADCDataQ.m_i8CollectionState)
            {
                g_ui8ADCMeanCalState  = ADC_MEAN_CALCULATION_START;
                g_ui8ADCMeanDataState = ADC_MEAN_DATA_INTERMEDIATE_WEIGHT;
            }
            else if (COLLECT_DYNAMIC_TARE_DATA == g_stADCDataQ.m_i8CollectionState)
            {
                g_ui8ADCMeanCalState  = ADC_MEAN_CALCULATION_START;
                g_ui8ADCMeanDataState = ADC_MEAN_DATA_INTERMEDIATE_TARE;
            }
        }
        
                
        // Mode Scanner
        switch(g_ui8SystemMode) //g_ui8AllBoardCurrentMode
        {
            // if it is in Wake up mode
            //case SYS_MODE_WAKE_UP:
            case SYS_MODE_NOTHING:
            {
            }
            break;
            case SYS_MODE_HOME:
            {
                
            }
            break;
            case SYS_MODE_PRE_PROD:
            {

            }
            break;
            case SYS_MODE_CALIB:  // Intentional Fall through
            case SYS_MODE_PROD:
            {
                // State Machine Running
                // Check for any change in All DIs and All DOs
                if (g_uiStateChange)
                {
                    g_uiStateChange = FLAG_STATE_NOT_CHANGED;
                    // Check all the active State Machines
                    for (i8StateMachineIndex = 0; i8StateMachineIndex < g_ui8NumberOfActiveMachines; i8StateMachineIndex++)
                    {
                        // Check for State Filter
                        CheckState(&g_stArrStateMachines[i8StateMachineIndex], i8StateMachineIndex);
                    }
                }
            }
            break;
            case SYS_MODE_POST_PROD:
            {
            }    
            break;
        }
        
        // TESTING
        output_toggle(PIN_OUT_DO_02);
        
        // Reset WDT.
        restart_wdt();
    }
}

//void main()
//{
//    // Set Watchdog timer ON
//    setup_wdt(WDT_ON);
//    
//    // Initialization Section Start
//    int8 i8StateMachineIndex = 0;
//    
//    InitGlobalVariables();
//    InitSystemInterrupts();
//    InitializeStateMachines(); //TODO Must go to Pre Production Mode
//    InitilizeStates();
//    delay_ms(10);
//    
//    InitDigitalIOPollingTimer();
//    
//    // TESTING Start
//    ST_IBC_DATA_NODE stIBCData;
//    stIBCData.g_ui8ArrIBCData[0] = 32;
//    stIBCData.g_ui8ArrIBCData[1] = 64;
//    stIBCData.g_ui8ArrIBCData[2] = 128;
//    stIBCData.g_ui8ArrIBCData[3] = 0;
//    stIBCData.g_ui8ArrIBCData[4] = 0;
//    //InsertSendQueue(&stIBCData);
//    
//    
////    UN_IBC_NODE unIBCSendTest;
////    unIBCSendTest.m_ui8ArrNode[0] = 0xAA;
////    unIBCSendTest.m_ui8ArrNode[1] = 0xAA;
////    unIBCSendTest.m_ui8ArrNode[2] = 0xAA;
////    unIBCSendTest.m_ui8ArrNode[3] = 0xAA;
////    unIBCSendTest.m_ui8ArrNode[4] = 0xAA;
//    
//    //delay_ms(10000);  //TODO Must Take care, used just for Testing
//    //UpdateCalibTableInEEPROM();
//    //PrepareCalibTable();
//    
//    // TESTING Start
//    //ResetCalibTable();
//    
//    //TestEEPROMData();
//    // TESTING End
//    
//    // Prepare Calibration table
//    ReadCalibTableFromEEPROM();
//    PrepareTestCalibTable();
//    PrepareTempCalibTable();
//    
//    //IBCSendCalibTable();
//    g_ui8SystemMode = SYS_MODE_PROD;
//    // TESTING Start
//
//    unsigned int32 arr[200];
//    unsigned int16 ui16X = 0;
//    
//    // Reset WDT.
//	restart_wdt();
//    // Set Rejection State ON
//    g_unDIState.m_stAllDIBits.m_ui1GDICn01  = 0;
//    g_unDIState.m_stAllDIBits.m_ui1GDICn02  = 1;
//    g_unDOState.m_stAllDOBits.m_ui1HWDOCn01 = 1;
//    
//    for(;;)
//    {
//        // Check if there is any message in IBC Send Queue
//        if (IsMSGInIBCQueue(&g_stIBCSendQueue) && (0 == g_fIBCSendBusy))
//        {
//            // Make IBC Send Busy
//            g_fIBCSendBusy = 1;
//            // Call Send IBC Message
//            SendIBCMSGFromQ();
//        }
//        // Check if there is any message in IBC Receive Queue
//        if (IsMSGInIBCQueue(&g_stIBCRcvQueue))
//        {
//            // Process IBC 
//            ProcessIBCMsg();
//        }
//
//        // Check if ADC is ready
//        if (0 == input(PIN_ADC_DOUT_DRDY) && IBC_SEND_UNLOCK == g_fIBCSendLockStatus)
//        {
//            g_ui32ADCValue = read_ext_adc();
//
//            if (g_ui32ADCValue)
//            {
//                g_fADCDataReadyStatus = ADC_DATA_READY;
//            }
//        }
//
//        // Check If We need to send Intermediate Weighing or Tare Data to HMI
//        if (IBC_INTERMEDIATE_DATA_SEND_ON == g_fIntermediateDataSend)
//        {
//            // Reset Flag
//            g_fIntermediateDataSend = IBC_INTERMEDIATE_DATA_SEND_OFF;
//            
//            // Check Weighing is running or not
//            if (COLLECT_WEIGHING_DATA == g_stADCDataQ.m_i8CollectionState)
//            {
//                // Step 1: Get Mean Value
//                // Start Flag CALC_MEAN_ON
//                //g_ui8ADCMeanCalState = ADC_MEAN_CALCULATION_START;
//                
//                // Step 1: Get Weighing Data
//                //g_i16CurBoxWeight = GetWeighingData();
//                g_i16CurBoxWeight = g_ui32ADCAvg;
//
//                // Step 2: Send Weighing data to IBC
//                //IBCSendWeighingData(g_i16CurBoxWeight, 0);
//                IBCSendWtInterMidData(g_i16CurBoxWeight);
//            }
//            else if (COLLECT_DYNAMIC_TARE_DATA== g_stADCDataQ.m_i8CollectionState)
//            {
//                // Send Dynamic Tare Data to IBC
//                IBCSendTareData(g_ui32ADCAvg);
//            }
//        }
//        
//        // Mode Scanner
//        switch(g_ui8SystemMode) //g_ui8AllBoardCurrentMode
//        {
//            // if it is in Wake up mode
//            //case SYS_MODE_WAKE_UP:
//            case SYS_MODE_NOTHING:
//            {
//            }
//            break;
//            case SYS_MODE_HOME:
//            {
//                
//            }
//            break;
//            case SYS_MODE_PRE_PROD:
//            {
//
//            }
//            break;
//            case SYS_MODE_CALIB:  // Intentional Fall through
//            case SYS_MODE_PROD:
//            {
//                // State Machine Running
//                // Check for any change in All DIs and All DOs
//                if (g_uiStateChange)
//                {
//                    g_uiStateChange = FLAG_STATE_NOT_CHANGED;
//                    // Check all the active State Machines
//                    for (i8StateMachineIndex = 0; i8StateMachineIndex < g_ui8NumberOfActiveMachines; i8StateMachineIndex++)
//                    {
//                        // Check for State Filter
//                        CheckState(&g_stArrStateMachines[i8StateMachineIndex], i8StateMachineIndex);
//                    }
//                }
//            }
//            break;
//            case SYS_MODE_POST_PROD:
//            {
//            }    
//            break;
//        }
//        
//        output_toggle(PIN_OUT_DO_02);
//        // Reset WDT.
//        restart_wdt();
//    }
//}
