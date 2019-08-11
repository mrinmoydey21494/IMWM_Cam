#ifndef _VM_STATE_MACHINE_0_H_
#define _VM_STATE_MACHINE_0_H_

/*************************** Inclusion files **********************************/
#include "vmSystemConfig.h"
#include "vmStateMachineForIMW_M0.h"
#include "vmADCManager.h"
#include "vmAverageWeightCalculation.h"


// State 1: Home State
unsigned int8 StateFunc_SM0_State1(int8 i8StateMachine, int8 i8State)
{
    // First set the DOs
    StateFunc_GenericState(i8StateMachine, i8State);

    // Step 1: Start Reading the ADC Data for Weighing
    ResetADCDataQ(COLLECT_DYNAMIC_TARE_DATA);

    // Step 2: Start ADC Scanning
    g_ui8ADCScanningStatus = ADC_SCAN_ON;
    
    // Reset Weighing Calculation Status
    g_fWtCalculationState = FLAG_WT_CALCULATION_NOT_DONE;

    return 0;
}

// State 2: Box Touches Box Entry Sensor
unsigned int8 StateFunc_SM0_State2(int8 i8StateMachine, int8 i8State)
{
    // First set the DOs
    StateFunc_GenericState(i8StateMachine, i8State);
    
    // Step 1: Stop Tare Data Collection
    g_ui8ADCScanningStatus = ADC_SCAN_OFF;
    
    // Step 2: Stop Intermediate Data Sending
    g_fIntermediateDataSend = IBC_INTERMEDIATE_DATA_SEND_OFF; // Dose not matter but Still for safety ...
    
    // Step 3: Stop Main Split Average Calculations and IBC Intermediate Data Sending
    //         and Start fresh Calculations from Start for Final Tare Values
    // Reset Flags
    g_ui8ADCMeanCalState  = ADC_MEAN_CALCULATION_START;
    g_ui8ADCMeanDataState = ADC_MEAN_DATA_FINAL_TARE;

    return 0;
}

// State 3: Box Touches the Stopper Sensor
unsigned int8 StateFunc_SM0_State3(int8 i8StateMachine, int8 i8State)
{
    // Moves up the Stopper

    // First set the DOs
    StateFunc_GenericState(i8StateMachine, i8State);
    
    return 0;
}

// State 4: Box Completely Entered into the weighing Conveyor
unsigned int8 StateFunc_SM0_State4(int8 i8StateMachine, int8 i8State)
{
    // First set the DOs
    StateFunc_GenericState(i8StateMachine, i8State);
    
    // Step 1: Start Reading the ADC Data for Weighing
    ResetADCDataQ(COLLECT_WEIGHING_DATA);
    
    // Step 2: Start ADC Scanning
    g_ui8ADCScanningStatus = ADC_SCAN_ON;
    return 0;
}

// State 5: Small Box Exits from Stopper Sensor		
unsigned int8 StateFunc_SM0_State5(int8 i8StateMachine, int8 i8State)
{
    // First set the DOs
    StateFunc_GenericState(i8StateMachine, i8State);
    
    return 0;
}

// State 6 and State 7:  Small or Big Box Touches The Exit Sensor
unsigned int8 StateFunc_SM0_State6_7(int8 i8StateMachine, int8 i8State)
{
    // First set the DOs
    StateFunc_GenericState(i8StateMachine, i8State);
    
    // Raise a flag to get a delay between box left the exit sensor and achieving home state.
    g_unDIState.m_stAllDIBits.m_ui1GDICn03 = BOX_LEFT_DELAY_START;
    
    if (FLAG_WT_CALCULATION_DONE == g_fWtCalculationState)
    {
        return 0;
    }
    // Step 1: Stop Weighing Data Collection
    g_ui8ADCScanningStatus = ADC_SCAN_OFF;
    
    if (IBC_RESET_CALIB_ON == g_fResetCalibStatus)
    {
        g_ui8ADCMeanCalState  = ADC_MEAN_CALCULATION_START;
        g_ui8ADCMeanDataState = ADC_MEAN_DATA_CALIB_WEIGHT;
    }

    else
    {
        // Step 1: Increment Box Count
       g_ui16CurrentBoxCount++;
       
        // Step 2: Get Weighing Data
        g_ui8ADCMeanCalState  = ADC_MEAN_CALCULATION_START;
        g_ui8ADCMeanDataState = ADC_MEAN_DATA_FINAL_WEIGHT;
    }
    
    return 0;
}


// State 8: Small Box Touches Exit Sensor
unsigned int8 StateFunc_SM0_State8(int8 i8StateMachine, int8 i8State)
{
    // First set the DOs
    StateFunc_GenericState(i8StateMachine, i8State);
    

    
    return 0;
}

////// State 8: Box Left Exit Sensor(HOME)
////unsigned int8 StateFunc_SM0_State8(int8 i8StateMachine, int8 i8State)
////{
////    // First set the DOs
////    StateFunc_GenericState(i8StateMachine, i8State);
////    
////    return 0;
////}

// Define State Lookup Table
void Initialize_StateMachine_0()
{
    // Initialize State DI Filter
    g_stArrStateMachines[STATE_MACHINE_0].m_ui32StateDIFilter      = STATE_SM0_HWDI_FILTER;
    g_stArrStateMachines[STATE_MACHINE_0].m_ui16StateDOFilter      = STATE_SM0_HWDO_FILTER;
    g_stArrStateMachines[STATE_MACHINE_0].m_uiStateCounterFilter   = 0x0000;
    g_stArrStateMachines[STATE_MACHINE_0].m_ui32LastDIState        = STATE_SM0_HWDI_01;
    g_stArrStateMachines[STATE_MACHINE_0].m_ui16LastDOState        = STATE_SM0_HWDO_01;
    g_stArrStateMachines[STATE_MACHINE_0].m_i8PrevState            = STATE_01;
    //g_stArrStateMachines[STATE_MACHINE_0].m_fContinue = STATE_CONTINUE_OFF;
    
    // Initialize Expected Next State for all States
    // We are leaving first state index for No State
    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[0].m_ui32DIState     = 0x0;
    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[0].m_ui16DOState     = 0x0;    
    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[0].pFunStateFunction = StateFunc_GenericState;//(STATE_MACHINE_0, STATE_NOTHING);

    // SM0: State 01: SM0: Home State, Expected no CBBs are in Weighing Conveyor
    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_01].m_ui32DIState = STATE_SM0_HWDI_01;
    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_01].m_ui16DOState = STATE_SM0_HWDO_01;
    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_01].m_i8ArrExpNextState[0] = STATE_02;
    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_01].m_i8ArrExpNextState[1] = STATE_NOTHING;
    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_01].m_i8ArrExpNextState[2] = STATE_NOTHING;
    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_01].m_i8ArrExpNextState[3] = STATE_NOTHING;
    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_01].pFunStateFunction = StateFunc_SM0_State1;//(STATE_MACHINE_0, STATE_01);

    // SM0: State 02: Box Touches Box Entry Sensor
    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_02].m_ui32DIState = STATE_SM0_HWDI_02;
    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_02].m_ui16DOState = STATE_SM0_HWDO_02;
    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_02].m_i8ArrExpNextState[0] = STATE_03;
    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_02].m_i8ArrExpNextState[1] = STATE_NOTHING;
    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_02].m_i8ArrExpNextState[2] = STATE_NOTHING;
    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_02].m_i8ArrExpNextState[3] = STATE_NOTHING;
    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_02].pFunStateFunction = StateFunc_SM0_State2;//(STATE_MACHINE_0, STATE_02);
    
    // SM0: State 03: Box Touches Box Stopper Sensor
    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_03].m_ui32DIState = STATE_SM0_HWDI_03;
    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_03].m_ui16DOState = STATE_SM0_HWDO_03;
    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_03].m_i8ArrExpNextState[0] = STATE_04;
    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_03].m_i8ArrExpNextState[1] = STATE_NOTHING;
    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_03].m_i8ArrExpNextState[2] = STATE_NOTHING;
    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_03].m_i8ArrExpNextState[3] = STATE_NOTHING;
    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_03].pFunStateFunction = StateFunc_SM0_State3;//(STATE_MACHINE_0, STATE_02);
    
    // SM0: State 04: Box Completely Entered into Weighing Conveyor
    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_04].m_ui32DIState = STATE_SM0_HWDI_04;
    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_04].m_ui16DOState = STATE_SM0_HWDO_04;
    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_04].m_i8ArrExpNextState[0] = STATE_05;
    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_04].m_i8ArrExpNextState[1] = STATE_06;
    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_04].m_i8ArrExpNextState[2] = STATE_NOTHING;
    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_04].m_i8ArrExpNextState[3] = STATE_NOTHING;
    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_04].pFunStateFunction = StateFunc_SM0_State4;//(STATE_MACHINE_0, STATE_02);
 
    // SM0: State 05: Small Box Exits from Stopper Sensor
    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_05].m_ui32DIState = STATE_SM0_HWDI_05;
    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_05].m_ui16DOState = STATE_SM0_HWDO_05;
    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_05].m_i8ArrExpNextState[0] = STATE_07;
    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_05].m_i8ArrExpNextState[1] = STATE_NOTHING;
    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_05].m_i8ArrExpNextState[2] = STATE_NOTHING;
    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_05].m_i8ArrExpNextState[3] = STATE_NOTHING;
    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_05].pFunStateFunction = StateFunc_SM0_State5;//(STATE_MACHINE_0, STATE_02);
 
    // SM0: State 06: Big Box Touches Exit Sensor
    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_06].m_ui32DIState = STATE_SM0_HWDI_06;
    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_06].m_ui16DOState = STATE_SM0_HWDO_06;
    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_06].m_i8ArrExpNextState[0] = STATE_08;
    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_06].m_i8ArrExpNextState[1] = STATE_NOTHING;
    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_06].m_i8ArrExpNextState[2] = STATE_NOTHING;
    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_06].m_i8ArrExpNextState[3] = STATE_NOTHING;
    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_06].pFunStateFunction = StateFunc_SM0_State6_7;//(STATE_MACHINE_0, STATE_02);
 
    // SM0: State 07: Small Box Touches Exit Sensor 
    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_07].m_ui32DIState = STATE_SM0_HWDI_07;
    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_07].m_ui16DOState = STATE_SM0_HWDO_07;
    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_07].m_i8ArrExpNextState[0] = STATE_08;
    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_07].m_i8ArrExpNextState[1] = STATE_NOTHING;
    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_07].m_i8ArrExpNextState[2] = STATE_NOTHING;
    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_07].m_i8ArrExpNextState[3] = STATE_NOTHING;
    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_07].pFunStateFunction = StateFunc_SM0_State6_7;//(STATE_MACHINE_0, STATE_02);
    
    // SM0: State 08: Box Left Exit Sensor 
    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_08].m_ui32DIState = STATE_SM0_HWDI_08;
    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_08].m_ui16DOState = STATE_SM0_HWDO_08;
    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_08].m_i8ArrExpNextState[0] = STATE_01;
    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_08].m_i8ArrExpNextState[1] = STATE_NOTHING;
    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_08].m_i8ArrExpNextState[2] = STATE_NOTHING;
    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_08].m_i8ArrExpNextState[3] = STATE_NOTHING;
    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_08].pFunStateFunction = StateFunc_SM0_State8;//(STATE_MACHINE_0, STATE_02);

//    // SM0: State 09: Box Left Exit Sensor with Delay time 
//    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_09].m_ui32DIState = STATE_SM0_HWDI_09;
//    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_09].m_ui16DOState = STATE_SM0_HWDO_09;
//    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_09].m_i8ArrExpNextState[0] = STATE_01;
//    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_09].m_i8ArrExpNextState[1] = STATE_NOTHING;
//    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_09].m_i8ArrExpNextState[2] = STATE_NOTHING;
//    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_09].m_i8ArrExpNextState[3] = STATE_NOTHING;
//    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_09].pFunStateFunction = StateFunc_SM0_State8;
//    
//    // SM0: State 10: Box Left Exit Sensor(HOME)
//    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_10].m_ui32DIState = STATE_SM0_HWDI_08;
//    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_10].m_ui16DOState = STATE_SM0_HWDO_08;
//    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_10].m_i8ArrExpNextState[0] = STATE_01;
//    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_10].m_i8ArrExpNextState[1] = STATE_NOTHING;
//    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_10].m_i8ArrExpNextState[2] = STATE_NOTHING;
//    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_10].m_i8ArrExpNextState[3] = STATE_NOTHING;
//    g_stArrStateMachines[STATE_MACHINE_0].m_stArrStateLookupTable[STATE_10].pFunStateFunction = StateFunc_SM0_State8;
}    

#endif /* _VM_STATE_MACHINE_0_H_ */
