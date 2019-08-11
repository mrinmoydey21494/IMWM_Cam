
#ifndef _VM_STATE_MACHINE_1_H_
#define _VM_STATE_MACHINE_1_H_

/*************************** Inclusion files **********************************/
#include "vmSystemConfig.h"
#include "vmStateMachineForIMW_M0.h"


// State 1: Home State
unsigned int8 StateFunc_SM1_State1(int8 i8StateMachine, int8 i8State)
{
    // First set the DOs
    StateFunc_GenericState(i8StateMachine, i8State);
    
    return 0;
}

// State 2: Rejected Box Touches Reject Sensor	
unsigned int8 StateFunc_SM1_State2(int8 i8StateMachine, int8 i8State)
{
    // Start Pusher Delay
    g_fPusherDelayStatus = PUSHER_DELAY_START_ON;
    
    if (SYS_MODE_CALIB == g_ui8SystemMode)
    {
        // We do not want Reject Station at the time of calibration
        return 0;
    }
    // First set the DOs
    StateFunc_GenericState(i8StateMachine, i8State);
    

    
    return 0;
}

// State 3: Accepted Box Touches Reject Sensor
unsigned int8 StateFunc_SM1_State3(int8 i8StateMachine, int8 i8State)
{
    // Change Box Rejection Status
    g_unDIState.m_stAllDIBits.m_ui1GDICn01 = BOX_REJECTED;
    
    if (SYS_MODE_CALIB == g_ui8SystemMode)
    {
        // We do not want Reject Station at the time of calibration
        return 0;
    }

    // First set the DOs
    StateFunc_GenericState(i8StateMachine, i8State);
    
    return 0;
}

// State 4: Reject Pusher Delay Over
unsigned int8 StateFunc_SM1_State4(int8 i8StateMachine, int8 i8State)
{
    // First set the DOs
    StateFunc_GenericState(i8StateMachine, i8State);
    
    // Change Box Rejection Status
    g_unDIState.m_stAllDIBits.m_ui1GDICn01 = BOX_REJECTED;
    
    // Reset Pusher Delay
    g_unDIState.m_stAllDIBits.m_ui1GDICn02 = 1;
    
    return 0;
}


// Define State Lookup Table
void Initialize_StateMachine_1()
{
    // Initialize State DI Filter
    g_stArrStateMachines[STATE_MACHINE_1].m_ui32StateDIFilter    = STATE_SM1_HWDI_FILTER;
    g_stArrStateMachines[STATE_MACHINE_1].m_ui16StateDOFilter    = STATE_SM1_HWDO_FILTER;
    g_stArrStateMachines[STATE_MACHINE_1].m_uiStateCounterFilter = 0x0000;
    g_stArrStateMachines[STATE_MACHINE_1].m_ui32LastDIState      = STATE_SM1_HWDI_01;
    g_stArrStateMachines[STATE_MACHINE_1].m_ui16LastDOState      = STATE_SM1_HWDO_01;
    g_stArrStateMachines[STATE_MACHINE_1].m_i8PrevState          = STATE_01;
    g_stArrStateMachines[STATE_MACHINE_1].m_fContinue            = STATE_CONTINUE_OFF;
    
    // We are leaving first state index for No State
    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[0].m_ui32DIState     = 0x0;
    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[0].m_ui16DOState     = 0x0; 
    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[0].pFunStateFunction = StateFunc_GenericState;

    // SM1: State 01: Home State
    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_01].m_ui32DIState = STATE_SM1_HWDI_01;
    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_01].m_ui16DOState = STATE_SM1_HWDO_01;
    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_01].m_i8ArrExpNextState[0] = STATE_02;
    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_01].m_i8ArrExpNextState[1] = STATE_03;
    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_01].m_i8ArrExpNextState[2] = STATE_NOTHING;
    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_01].m_i8ArrExpNextState[3] = STATE_NOTHING;
    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_01].pFunStateFunction = StateFunc_SM1_State1;


    // SM1: State 02: Rejected Box Touches Reject Sensor
    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_02].m_ui32DIState = STATE_SM1_HWDI_02;
    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_02].m_ui16DOState = STATE_SM1_HWDO_02;
    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_02].m_i8ArrExpNextState[0] = STATE_04;
    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_02].m_i8ArrExpNextState[1] = STATE_NOTHING;
    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_02].m_i8ArrExpNextState[2] = STATE_NOTHING;
    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_02].m_i8ArrExpNextState[3] = STATE_NOTHING;
    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_02].pFunStateFunction = StateFunc_SM1_State2;
    
    
    // SM1: State 03: Accepted Box Touches Reject Sensor
    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_03].m_ui32DIState = STATE_SM1_HWDI_03;
    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_03].m_ui16DOState = STATE_SM1_HWDO_03;
    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_03].m_i8ArrExpNextState[0] = STATE_01;
    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_03].m_i8ArrExpNextState[1] = STATE_NOTHING;
    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_03].m_i8ArrExpNextState[2] = STATE_NOTHING;
    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_03].m_i8ArrExpNextState[3] = STATE_NOTHING;
    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_03].pFunStateFunction = StateFunc_SM1_State3;


    // SM1: State 04: Reject Pusher Delay Over
    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_04].m_ui32DIState = STATE_SM1_HWDI_04;
    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_04].m_ui16DOState = STATE_SM1_HWDO_04;
    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_04].m_i8ArrExpNextState[0] = STATE_01;
    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_04].m_i8ArrExpNextState[1] = STATE_NOTHING;
    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_04].m_i8ArrExpNextState[2] = STATE_NOTHING;
    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_04].m_i8ArrExpNextState[3] = STATE_NOTHING;
    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_04].pFunStateFunction = StateFunc_SM1_State4;
    
//+++++++++++++++++++++++++++++++++++++++++++++++++

}


#endif /* _VM_STATE_MACHINE_1_H_ */

//#ifndef _VM_STATE_MACHINE_1_H_
//#define _VM_STATE_MACHINE_1_H_
//
///*************************** Inclusion files **********************************/
//#include "vmSystemConfig.h"
//#include "vmStateMachineForIMW_M0.h"
//
//
//// State 1: Home State
//unsigned int8 StateFunc_SM1_State1(int8 i8StateMachine, int8 i8State)
//{
//    // First set the DOs
//    StateFunc_GenericState(i8StateMachine, i8State);
//    
//    return 0;
//}
//
//// State 2: Box Touches Reject Sensor		
//unsigned int8 StateFunc_SM1_State2(int8 i8StateMachine, int8 i8State)
//{
//    // First set the DOs
//    StateFunc_GenericState(i8StateMachine, i8State);
//    
//    return 0;
//}
//
//// State 3: Box Touches Reject Sensor But Reject Station Busy
//unsigned int8 StateFunc_SM1_State3(int8 i8StateMachine, int8 i8State)
//{
//    if (IBC_RESET_CALIB_ON == g_fResetCalibStatus)
//    {
//        // We do not want Reject Station at the time of calibration
//        return 0;
//    }
//    // First set the DOs
//    StateFunc_GenericState(i8StateMachine, i8State);
//    
//    // Start Pusher Delay
//    g_fPusherDelayStatus = PUSHER_DELAY_START_ON;
//
//    return 0;
//}
//
//// State 4: Reject Pusher Delay Over
//unsigned int8 StateFunc_SM1_State4(int8 i8StateMachine, int8 i8State)
//{
//    if (IBC_RESET_CALIB_ON == g_fResetCalibStatus)
//    {
//        // We do not want Reject Station at the time of calibration
//        return 0;
//    }
//    // First set the DOs
//    StateFunc_GenericState(i8StateMachine, i8State);
//    
//    // Change Box Rejection Status
//    g_unDIState.m_stAllDIBits.m_ui1GDICn01 = BOX_REJECTED;
//    
//    return 0;
//}
//
//// State 5: Reject Pusher Delay Over
//unsigned int8 StateFunc_SM1_State5(int8 i8StateMachine, int8 i8State)
//{
//    // First set the DOs
//    StateFunc_GenericState(i8StateMachine, i8State);
//    
//    // Change Box Rejection Status
//    g_unDIState.m_stAllDIBits.m_ui1GDICn01 = BOX_REJECTED;
//    
//    return 0;
//}
//
//// State 6: Reject Pusher Delay Over
//unsigned int8 StateFunc_SM1_State6(int8 i8StateMachine, int8 i8State)
//{
//    // First set the DOs
//    StateFunc_GenericState(i8StateMachine, i8State);
//    
//    // Change Box Rejection Status
//    g_unDIState.m_stAllDIBits.m_ui1GDICn02 = 1;
//    
//    return 0;
//}
//
//// Define State Lookup Table
//void Initialize_StateMachine_1()
//{
//    // Initialize State DI Filter
//    g_stArrStateMachines[STATE_MACHINE_1].m_ui32StateDIFilter    = STATE_SM1_HWDI_FILTER;
//    g_stArrStateMachines[STATE_MACHINE_1].m_ui16StateDOFilter    = STATE_SM1_HWDO_FILTER;
//    g_stArrStateMachines[STATE_MACHINE_1].m_uiStateCounterFilter = 0x0000;
//    g_stArrStateMachines[STATE_MACHINE_1].m_ui32LastDIState      = STATE_SM1_HWDI_01;
//    g_stArrStateMachines[STATE_MACHINE_1].m_ui16LastDOState      = STATE_SM1_HWDO_01;
//    g_stArrStateMachines[STATE_MACHINE_1].m_i8PrevState          = STATE_01;
//    g_stArrStateMachines[STATE_MACHINE_1].m_fContinue            = STATE_CONTINUE_OFF;
//    
//    // We are leaving first state index for No State
//    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[0].m_ui32DIState     = 0x0;
//    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[0].m_ui16DOState     = 0x0; 
//    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[0].pFunStateFunction = StateFunc_GenericState;
//
//    // SM1: State 01: Home State
//    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_01].m_ui32DIState = STATE_SM1_HWDI_01;
//    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_01].m_ui16DOState = STATE_SM1_HWDO_01;
//    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_01].m_i8ArrExpNextState[0] = STATE_02;
//    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_01].m_i8ArrExpNextState[1] = STATE_03;
//    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_01].m_i8ArrExpNextState[2] = STATE_04;
//    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_01].m_i8ArrExpNextState[3] = STATE_NOTHING;
//    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_01].pFunStateFunction = StateFunc_SM1_State1;
//
//
//    // SM1: State 02: Box Touches Reject Sensor		
//    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_02].m_ui32DIState = STATE_SM1_HWDI_02;
//    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_02].m_ui16DOState = STATE_SM1_HWDO_02;
//    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_02].m_i8ArrExpNextState[0] = STATE_05;
//    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_02].m_i8ArrExpNextState[1] = STATE_01;
//    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_02].m_i8ArrExpNextState[2] = STATE_NOTHING;
//    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_02].m_i8ArrExpNextState[3] = STATE_NOTHING;
//    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_02].pFunStateFunction = StateFunc_SM1_State2;
//    
//    
//    // SM1: State 03: SM1: Box Touches Reject Sensor But Reject Station Busy
//    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_03].m_ui32DIState = STATE_SM1_HWDI_03;
//    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_03].m_ui16DOState = STATE_SM1_HWDO_03;
//    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_03].m_i8ArrExpNextState[0] = STATE_06;
//    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_03].m_i8ArrExpNextState[1] = STATE_NOTHING;
//    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_03].m_i8ArrExpNextState[2] = STATE_NOTHING;
//    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_03].m_i8ArrExpNextState[3] = STATE_NOTHING;
//    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_03].pFunStateFunction = StateFunc_SM1_State3;
//
//
//    // SM1: State 04: SM1: Reject Pusher Delay Over
//    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_04].m_ui32DIState = STATE_SM1_HWDI_04;
//    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_04].m_ui16DOState = STATE_SM1_HWDO_04;
//    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_04].m_i8ArrExpNextState[0] = STATE_01;
//    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_04].m_i8ArrExpNextState[1] = STATE_NOTHING;
//    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_04].m_i8ArrExpNextState[2] = STATE_NOTHING;
//    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_04].m_i8ArrExpNextState[3] = STATE_NOTHING;
//    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_04].pFunStateFunction = StateFunc_SM1_State4;
//    
//    // SM1: State 04: SM1: Reject Pusher Delay Over
//    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_05].m_ui32DIState = STATE_SM1_HWDI_05;
//    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_05].m_ui16DOState = STATE_SM1_HWDO_05;
//    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_05].m_i8ArrExpNextState[0] = STATE_01;
//    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_05].m_i8ArrExpNextState[1] = STATE_02;
//    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_05].m_i8ArrExpNextState[2] = STATE_NOTHING;
//    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_05].m_i8ArrExpNextState[3] = STATE_NOTHING;
//    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_05].pFunStateFunction = StateFunc_SM1_State5;
//    
//    // SM1: State 04: SM1: Reject Pusher Delay Over
//    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_06].m_ui32DIState = STATE_SM1_HWDI_06;
//    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_06].m_ui16DOState = STATE_SM1_HWDO_06;
//    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_06].m_i8ArrExpNextState[0] = STATE_01;
//    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_06].m_i8ArrExpNextState[1] = STATE_02;
//    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_06].m_i8ArrExpNextState[2] = STATE_NOTHING;
//    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_06].m_i8ArrExpNextState[3] = STATE_NOTHING;
//    g_stArrStateMachines[STATE_MACHINE_1].m_stArrStateLookupTable[STATE_06].pFunStateFunction = StateFunc_SM1_State6;
//    
////+++++++++++++++++++++++++++++++++++++++++++++++++
//
//}
//
//
//#endif /* _VM_STATE_MACHINE_1_H_ */
