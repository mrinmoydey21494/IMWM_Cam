#ifndef _VM_STATE_MACHINE_FOR_IMW_M0_H_
#define _VM_STATE_MACHINE_FOR_IMW_M0_H_

#include "vmSystemConfig.h"
#include "vmStateMachine.h"

#define STPR_1_EMPTY_ON              0
#define STPR_1_EMPTY_OFF             1
#define STPR_0_SET_TRANSFERED_ON     0
#define STPR_0_SET_TRANSFERED_OFF    1
#define STPR_1_SET_TRANSFERED_ON     0
#define STPR_1_SET_TRANSFERED_OFF    1
#define STPR_1_CAN_SET_COUNT_DONE    0
#define STPR_1_CAN_SET_COUNT_NOT_DONE 1
#define PUSHER_EMPTY_ON              0
#define PUSHER_EMPTY_OFF             1
#define PUSHER_0_TIME_OUT_ON         0
#define PUSHER_0_TIME_OUT_OFF        1
#define PUSHER_0_ALL_DONE            0
#define PUSHER_0_ALL_NOT_DONE        1
#define PUSHER_1_TIME_OUT_ON         0
#define PUSHER_1_TIME_OUT_OFF        1
#define PUSHER_1_ALL_DONE            0
#define PUSHER_1_ALL_NOT_DONE        1
#define STACKER_READY_ON             0
#define STACKER_READY_OFF            1
#define PUSHER_0_FIRST_PUSH_ON       0
#define PUSHER_0_FIRST_PUSH_OFF      1
#define PUSHER_1_FIRST_PUSH_ON       0
#define PUSHER_1_FIRST_PUSH_OFF      1


#define SPCL_FLAG_SM_1_START_TRANSFER_OFF 0
#define SPCL_FLAG_SM_1_START_TRANSFER_ON  1


#define NO_OF_ACTIVE_MACHINE        2

#define INITAIL_DI_STATE_ON_PWR_ON   0xFFFF3F5F
#define INITAIL_DO_STATE_ON_PWR_ON   0x3FFF
#define SAFE_MODE_DO_STATE           0x3FFF
#define PRE_PRODUCTION_DO_STATE      0xBFFF

//////////////////// State Machine 0 DI DO States Start ////////////////////////
#define 	STATE_SM0_HWDI_FILTER	0xD0000010
#define 	STATE_SM0_HWDI_01	0xD0000010
#define 	STATE_SM0_HWDI_02	0x50000010
#define 	STATE_SM0_HWDI_03	0x10000010
#define 	STATE_SM0_HWDI_04	0x90000010
#define 	STATE_SM0_HWDI_05	0xD0000010
#define 	STATE_SM0_HWDI_06	0x80000010
#define 	STATE_SM0_HWDI_07	0xC0000010
#define 	STATE_SM0_HWDI_08	0xD0000010
#define 	STATE_SM0_HWDI_09	0xD0000000
#define 	STATE_SM0_HWDI_10	0xD0000010

#define 	STATE_SM0_HWDO_FILTER	0x8000
#define 	STATE_SM0_HWDO_01	0x8000
#define 	STATE_SM0_HWDO_02	0x8000
#define 	STATE_SM0_HWDO_03	0x0000
#define 	STATE_SM0_HWDO_04	0x0000
#define 	STATE_SM0_HWDO_05	0x0000
#define 	STATE_SM0_HWDO_06	0x0000
#define 	STATE_SM0_HWDO_07	0x0000
#define 	STATE_SM0_HWDO_08	0x0000
#define 	STATE_SM0_HWDO_09	0x0000
#define 	STATE_SM0_HWDO_10	0x8000


//////////////////// State Machine 0 DI DO States End //////////////////////////

//////////////////// State Machine 1 DI DO States Start ////////////////////////
#define 	STATE_SM1_HWDI_FILTER	0x08001800
#define 	STATE_SM1_HWDI_01	0x08000800
#define 	STATE_SM1_HWDI_02	0x00000800
#define 	STATE_SM1_HWDI_03	0x00001800
#define 	STATE_SM1_HWDI_04	0x08000000

#define 	STATE_SM1_HWDO_FILTER	0x4000
#define 	STATE_SM1_HWDO_01	0x4000
#define 	STATE_SM1_HWDO_02	0x0000
#define 	STATE_SM1_HWDO_03	0x4000
#define 	STATE_SM1_HWDO_04	0x4000

//#define 	STATE_SM1_HWDI_FILTER	0x0C001800
//#define 	STATE_SM1_HWDI_01	0x0C000800
//#define 	STATE_SM1_HWDI_02	0x08000800
//#define 	STATE_SM1_HWDI_03	0x04000800
//#define 	STATE_SM1_HWDI_04	0x04001800
//#define 	STATE_SM1_HWDI_05	0x00000800
//#define 	STATE_SM1_HWDI_06	0x0C000000
//
//#define 	STATE_SM1_HWDO_FILTER	0x4000
//#define 	STATE_SM1_HWDO_01	0x4000
//#define 	STATE_SM1_HWDO_02	0x4000
//#define 	STATE_SM1_HWDO_03	0x0000
//#define 	STATE_SM1_HWDO_04	0x4000
//#define 	STATE_SM1_HWDO_05	0x4000
//#define 	STATE_SM1_HWDO_06	0x4000
//////////////////// State Machine 1 DI DO States End //////////////////////////

ST_STATE_MACHINE g_stArrStateMachines[NO_OF_ACTIVE_MACHINE];

unsigned int8 g_fStateMac1StartCanTransfer = SPCL_FLAG_SM_1_START_TRANSFER_ON;



//void InitializeStateMachines()
//{
    //g_unDIState.m_ui32AllDIs = 0xFFFFFFFF;
    //g_unDOState.m_ui16AllDOs = 0xFFFF;
//}
/*
// 01 : Home State
unsigned int8 StateFunc_GenericState(int8 i8StateMachine, int8 i8State)
{
    // Take action
    // Main intention is to not change values of other DO fields, except the fields under the state DO Filter
    // Also only the changes will added to the original ALL DOs 
    unsigned int16 ui16Temp0 = 0;
    unsigned int16 ui16Temp1 = 0;
    //g_unAllDOs.m_ui16AllDOs = g_stArrStateMachines[i8StateMachine].m_stArrStateLookupTable[i8State].m_ui16DOState;
    
    // Hold the Current DO Values to temp
    ui16Temp0 = g_unDOState.m_ui16AllDOs;

    // Bitwise AND with the State DO Filter with the Value to change
    ui16Temp1 = g_stArrStateMachines[i8StateMachine].m_stArrStateLookupTable[i8State].m_ui16DOState &
                g_stArrStateMachines[i8StateMachine].m_ui16StateDOFilter;
    
    // Finally Bitwise OR operation between original value to the temp
    g_unDOState.m_ui16AllDOs = ui16Temp0 | ui16Temp1;

    return 0;
} 
*/
unsigned int8 StateFunc_GenericState_(int8 i8StateMachine, int8 i8State)
{

        unsigned int16 ui8DO       = g_unDOState.m_ui16AllDOs;
        unsigned int16 ui8Filter   = g_stArrStateMachines[i8StateMachine].m_ui16StateDOFilter;
        unsigned int16 ui8Value    = g_stArrStateMachines[i8StateMachine].m_stArrStateLookupTable[i8State].m_ui16DOState;
        unsigned int16 ui8BitValue = 0;
        
        for(int i = 0; i < 16; i++)
        {
            if(1 == ((ui8Filter >> i)&0x0001))
            {
                ui8BitValue = ((ui8Value >> i)&0x0001);
                ui8DO = (ui8DO & (~(1 << i))) | (ui8BitValue << i);
            }
        }

    g_unDOState.m_ui16AllDOs = ui8DO;
    return 0;
} 

unsigned int8 StateFunc_GenericState(int8 i8StateMachine, int8 i8State)
{
    UN_ALL_DOS unFilter;
    UN_ALL_DOS unValue;
    
    unFilter.m_ui16AllDOs = g_stArrStateMachines[i8StateMachine].m_ui16StateDOFilter;
    unValue.m_ui16AllDOs  = g_stArrStateMachines[i8StateMachine].m_stArrStateLookupTable[i8State].m_ui16DOState;
    
    if(unFilter.m_stAllDOBits.m_ui1HWDOCn00) g_unDOState.m_stAllDOBits.m_ui1HWDOCn00 = unValue.m_stAllDOBits.m_ui1HWDOCn00;
    if(unFilter.m_stAllDOBits.m_ui1HWDOCn01) g_unDOState.m_stAllDOBits.m_ui1HWDOCn01 = unValue.m_stAllDOBits.m_ui1HWDOCn01;
    if(unFilter.m_stAllDOBits.m_ui1HWDOCn02) g_unDOState.m_stAllDOBits.m_ui1HWDOCn02 = unValue.m_stAllDOBits.m_ui1HWDOCn02;
    if(unFilter.m_stAllDOBits.m_ui1HWDOCn03) g_unDOState.m_stAllDOBits.m_ui1HWDOCn03 = unValue.m_stAllDOBits.m_ui1HWDOCn03;
    if(unFilter.m_stAllDOBits.m_ui1HWDOCn04) g_unDOState.m_stAllDOBits.m_ui1HWDOCn04 = unValue.m_stAllDOBits.m_ui1HWDOCn04;
    if(unFilter.m_stAllDOBits.m_ui1HWDOCn05) g_unDOState.m_stAllDOBits.m_ui1HWDOCn05 = unValue.m_stAllDOBits.m_ui1HWDOCn05;
    if(unFilter.m_stAllDOBits.m_ui1HWDOCn06) g_unDOState.m_stAllDOBits.m_ui1HWDOCn06 = unValue.m_stAllDOBits.m_ui1HWDOCn06;
    if(unFilter.m_stAllDOBits.m_ui1HWDOCn07) g_unDOState.m_stAllDOBits.m_ui1HWDOCn07 = unValue.m_stAllDOBits.m_ui1HWDOCn07;
    if(unFilter.m_stAllDOBits.m_ui1SRDOCn00) g_unDOState.m_stAllDOBits.m_ui1SRDOCn00 = unValue.m_stAllDOBits.m_ui1SRDOCn00;
    if(unFilter.m_stAllDOBits.m_ui1SRDOCn01) g_unDOState.m_stAllDOBits.m_ui1SRDOCn01 = unValue.m_stAllDOBits.m_ui1SRDOCn01;
    if(unFilter.m_stAllDOBits.m_ui1SRDOCn02) g_unDOState.m_stAllDOBits.m_ui1SRDOCn02 = unValue.m_stAllDOBits.m_ui1SRDOCn02;
    if(unFilter.m_stAllDOBits.m_ui1SRDOCn03) g_unDOState.m_stAllDOBits.m_ui1SRDOCn03 = unValue.m_stAllDOBits.m_ui1SRDOCn03;
    if(unFilter.m_stAllDOBits.m_ui1SRDOCn04) g_unDOState.m_stAllDOBits.m_ui1SRDOCn04 = unValue.m_stAllDOBits.m_ui1SRDOCn04;
    if(unFilter.m_stAllDOBits.m_ui1SRDOCn05) g_unDOState.m_stAllDOBits.m_ui1SRDOCn05 = unValue.m_stAllDOBits.m_ui1SRDOCn05;
    if(unFilter.m_stAllDOBits.m_ui1SRDOCn06) g_unDOState.m_stAllDOBits.m_ui1SRDOCn06 = unValue.m_stAllDOBits.m_ui1SRDOCn06;
    if(unFilter.m_stAllDOBits.m_ui1SRDOCn07) g_unDOState.m_stAllDOBits.m_ui1SRDOCn07 = unValue.m_stAllDOBits.m_ui1SRDOCn07;
    
}    

        

        
// 02: Start to Pick
unsigned int8 StateFunc_State02()
{
    // We got message from outer board that Stacker Ready and Hopper Ready
    // Now PD will start moving downword
    //g_unAllDOs.m_bArr[2] = 1;
    //g_unAllDOs.m_bArr[3] = 0;
    // Expected Next State Declaration
    g_stArrStateMachines[0].m_stArrStateLookupTable[STATE_02].m_i8ArrExpNextState[0] = STATE_03;
    g_stArrStateMachines[0].m_stArrStateLookupTable[STATE_02].m_i8ArrExpNextState[1] = STATE_03;
    g_stArrStateMachines[0].m_stArrStateLookupTable[STATE_02].m_i8ArrExpNextState[2] = STATE_03;
    g_stArrStateMachines[0].m_stArrStateLookupTable[STATE_02].m_i8ArrExpNextState[3] = STATE_03;
    return 0;
}


void InitExpNextStateDefault()
{
    int8 i8StMcCount = 0;
    int8 i8StCount   = 0;
    int8 i8ExpNxtCount = 0;
    for (i8StMcCount = 0; i8StMcCount < NO_OF_ACTIVE_MACHINE; i8StMcCount++)
    {
        // For all Active Look up tables
        for (i8StCount = 0; i8StCount < MAX_NUMBER_OF_STATE; i8StCount++)
        {
            for (i8ExpNxtCount = 0; i8ExpNxtCount < MAX_NO_OF_EXP_NXT_STATES; i8ExpNxtCount++)
            {
                g_stArrStateMachines[i8StMcCount].m_stArrStateLookupTable[i8StCount].m_i8ArrExpNextState[i8ExpNxtCount] = STATE_NOTHING;
            }
        }
    }
}

void PostProductionSafeMode()
{
    g_unDOState.m_ui16AllDOs = 0xFFFF;
    
    // 1. Stopper 0 is in Push State
    // 2. Can Stopper 1 is in Push state
    // 3. Pusher 0, Pusher 1 and Can Guide all is in drag state
    g_unDOState.m_ui16AllDOs = SAFE_MODE_DO_STATE;
    ExecuteALLDOs();
}

void PowerOnDOStateInit()
{
    g_unDOState.m_ui16AllDOs = 0xFFFF;
    
    // 1. Stopper 0 is in drag State
    // 2. Can Stopper 1 is in Push state
    // 3. Pusher 0, Pusher 1 and Can Guide all is in drag state
    g_unDOState.m_ui16AllDOs = INITAIL_DO_STATE_ON_PWR_ON;
    ExecuteALLDOs();
}

#endif /* _VI_STATE_MACHINE_FOR_CP_H_ */


//#include "vmSystemConfig.h"
////#include "vmStateMachine.h"
//
//#define STPR_1_EMPTY_ON              0
//#define STPR_1_EMPTY_OFF             1
//#define STPR_0_SET_TRANSFERED_ON     0
//#define STPR_0_SET_TRANSFERED_OFF    1
//#define STPR_1_SET_TRANSFERED_ON     0
//#define STPR_1_SET_TRANSFERED_OFF    1
//#define STPR_1_CAN_SET_COUNT_DONE    0
//#define STPR_1_CAN_SET_COUNT_NOT_DONE 1
//#define PUSHER_EMPTY_ON              0
//#define PUSHER_EMPTY_OFF             1
//#define PUSHER_0_TIME_OUT_ON         0
//#define PUSHER_0_TIME_OUT_OFF        1
//#define PUSHER_0_ALL_DONE            0
//#define PUSHER_0_ALL_NOT_DONE        1
//#define PUSHER_1_TIME_OUT_ON         0
//#define PUSHER_1_TIME_OUT_OFF        1
//#define PUSHER_1_ALL_DONE            0
//#define PUSHER_1_ALL_NOT_DONE        1
//#define STACKER_READY_ON             0
//#define STACKER_READY_OFF            1
//#define PUSHER_0_FIRST_PUSH_ON       0
//#define PUSHER_0_FIRST_PUSH_OFF      1
//#define PUSHER_1_FIRST_PUSH_ON       0
//#define PUSHER_1_FIRST_PUSH_OFF      1
//
//
//#define SPCL_FLAG_SM_1_START_TRANSFER_OFF 0
//#define SPCL_FLAG_SM_1_START_TRANSFER_ON  1
//
//
//#define NO_OF_ACTIVE_MACHINE        5
//
//#define INITAIL_DI_STATE_ON_PWR_ON   0xFFFF3F5F
//#define INITAIL_DO_STATE_ON_PWR_ON   0x3FFF
//#define SAFE_MODE_DO_STATE           0x3FFF
//#define PRE_PRODUCTION_DO_STATE      0xBFFF
//
//ST_STATE_MACHINE g_stArrStateMachines[NO_OF_ACTIVE_MACHINE];
//
//unsigned int8 g_fStateMac1StartCanTransfer = SPCL_FLAG_SM_1_START_TRANSFER_ON;
//
//
//
////void InitializeStateMachines()
////{
//    //g_unDIState.m_ui32AllDIs = 0xFFFFFFFF;
//    //g_unDOState.m_ui16AllDOs = 0xFFFF;
////}
///*
//// 01 : Home State
//unsigned int8 StateFunc_GenericState(int8 i8StateMachine, int8 i8State)
//{
//    // Take action
//    // Main intention is to not change values of other DO fields, except the fields under the state DO Filter
//    // Also only the changes will added to the original ALL DOs 
//    unsigned int16 ui16Temp0 = 0;
//    unsigned int16 ui16Temp1 = 0;
//    //g_unAllDOs.m_ui16AllDOs = g_stArrStateMachines[i8StateMachine].m_stArrStateLookupTable[i8State].m_ui16DOState;
//    
//    // Hold the Current DO Values to temp
//    ui16Temp0 = g_unDOState.m_ui16AllDOs;
//
//    // Bitwise AND with the State DO Filter with the Value to change
//    ui16Temp1 = g_stArrStateMachines[i8StateMachine].m_stArrStateLookupTable[i8State].m_ui16DOState &
//                g_stArrStateMachines[i8StateMachine].m_ui16StateDOFilter;
//    
//    // Finally Bitwise OR operation between original value to the temp
//    g_unDOState.m_ui16AllDOs = ui16Temp0 | ui16Temp1;
//
//    return 0;
//} 
//*/
//unsigned int8 StateFunc_GenericState_(int8 i8StateMachine, int8 i8State)
//{
//
////        unsigned int16 ui8DO       = g_unDOState.m_ui16AllDOs;
////        unsigned int16 ui8Filter   = g_stArrStateMachines[i8StateMachine].m_ui16StateDOFilter;
////        unsigned int16 ui8Value    = g_stArrStateMachines[i8StateMachine].m_stArrStateLookupTable[i8State].m_ui16DOState;
////        unsigned int16 ui8BitValue = 0;
////        
////        for(int i = 0; i < 16; i++)
////        {
////            if(1 == ((ui8Filter >> i)&0x0001))
////            {
////                ui8BitValue = ((ui8Value >> i)&0x0001);
////                ui8DO = (ui8DO & (~(1 << i))) | (ui8BitValue << i);
////            }
////        }
////
////    g_unDOState.m_ui16AllDOs = ui8DO;
////    return 0;
//} 
//
//unsigned int8 StateFunc_GenericState(int8 i8StateMachine, int8 i8State)
//{
////    UN_ALL_DOS unFilter;
////    UN_ALL_DOS unValue;
////    
////    unFilter.m_ui16AllDOs = g_stArrStateMachines[i8StateMachine].m_ui16StateDOFilter;
////    unValue.m_ui16AllDOs  = g_stArrStateMachines[i8StateMachine].m_stArrStateLookupTable[i8State].m_ui16DOState;
////    
////    if(unFilter.m_stAllDOBits.m_ui1HWDOCn00) g_unDOState.m_stAllDOBits.m_ui1HWDOCn00 = unValue.m_stAllDOBits.m_ui1HWDOCn00;
////    if(unFilter.m_stAllDOBits.m_ui1HWDOCn01) g_unDOState.m_stAllDOBits.m_ui1HWDOCn01 = unValue.m_stAllDOBits.m_ui1HWDOCn01;
////    if(unFilter.m_stAllDOBits.m_ui1HWDOCn02) g_unDOState.m_stAllDOBits.m_ui1HWDOCn02 = unValue.m_stAllDOBits.m_ui1HWDOCn02;
////    if(unFilter.m_stAllDOBits.m_ui1HWDOCn03) g_unDOState.m_stAllDOBits.m_ui1HWDOCn03 = unValue.m_stAllDOBits.m_ui1HWDOCn03;
////    if(unFilter.m_stAllDOBits.m_ui1HWDOCn04) g_unDOState.m_stAllDOBits.m_ui1HWDOCn04 = unValue.m_stAllDOBits.m_ui1HWDOCn04;
////    if(unFilter.m_stAllDOBits.m_ui1HWDOCn05) g_unDOState.m_stAllDOBits.m_ui1HWDOCn05 = unValue.m_stAllDOBits.m_ui1HWDOCn05;
////    if(unFilter.m_stAllDOBits.m_ui1HWDOCn06) g_unDOState.m_stAllDOBits.m_ui1HWDOCn06 = unValue.m_stAllDOBits.m_ui1HWDOCn06;
////    if(unFilter.m_stAllDOBits.m_ui1HWDOCn07) g_unDOState.m_stAllDOBits.m_ui1HWDOCn07 = unValue.m_stAllDOBits.m_ui1HWDOCn07;
////    if(unFilter.m_stAllDOBits.m_ui1SRDOCn00) g_unDOState.m_stAllDOBits.m_ui1SRDOCn00 = unValue.m_stAllDOBits.m_ui1SRDOCn00;
////    if(unFilter.m_stAllDOBits.m_ui1SRDOCn01) g_unDOState.m_stAllDOBits.m_ui1SRDOCn01 = unValue.m_stAllDOBits.m_ui1SRDOCn01;
////    if(unFilter.m_stAllDOBits.m_ui1SRDOCn02) g_unDOState.m_stAllDOBits.m_ui1SRDOCn02 = unValue.m_stAllDOBits.m_ui1SRDOCn02;
////    if(unFilter.m_stAllDOBits.m_ui1SRDOCn03) g_unDOState.m_stAllDOBits.m_ui1SRDOCn03 = unValue.m_stAllDOBits.m_ui1SRDOCn03;
////    if(unFilter.m_stAllDOBits.m_ui1SRDOCn04) g_unDOState.m_stAllDOBits.m_ui1SRDOCn04 = unValue.m_stAllDOBits.m_ui1SRDOCn04;
////    if(unFilter.m_stAllDOBits.m_ui1SRDOCn05) g_unDOState.m_stAllDOBits.m_ui1SRDOCn05 = unValue.m_stAllDOBits.m_ui1SRDOCn05;
////    if(unFilter.m_stAllDOBits.m_ui1SRDOCn06) g_unDOState.m_stAllDOBits.m_ui1SRDOCn06 = unValue.m_stAllDOBits.m_ui1SRDOCn06;
////    if(unFilter.m_stAllDOBits.m_ui1SRDOCn07) g_unDOState.m_stAllDOBits.m_ui1SRDOCn07 = unValue.m_stAllDOBits.m_ui1SRDOCn07;
////    
//}    
//
//        
//
//        
//// 02: Start to Pick
//unsigned int8 StateFunc_State02()
//{
//    // We got message from outer board that Stacker Ready and Hopper Ready
//    // Now PD will start moving downword
//    //g_unAllDOs.m_bArr[2] = 1;
//    //g_unAllDOs.m_bArr[3] = 0;
//    // Expected Next State Declaration
//    g_stArrStateMachines[0].m_stArrStateLookupTable[STATE_02].m_i8ArrExpNextState[0] = STATE_03;
//    g_stArrStateMachines[0].m_stArrStateLookupTable[STATE_02].m_i8ArrExpNextState[1] = STATE_03;
//    g_stArrStateMachines[0].m_stArrStateLookupTable[STATE_02].m_i8ArrExpNextState[2] = STATE_03;
//    g_stArrStateMachines[0].m_stArrStateLookupTable[STATE_02].m_i8ArrExpNextState[3] = STATE_03;
//    return 0;
//}
//
//
//void InitExpNextStateDefault()
//{
//    int8 i8StMcCount = 0;
//    int8 i8StCount   = 0;
//    int8 i8ExpNxtCount = 0;
//    for (i8StMcCount = 0; i8StMcCount < NO_OF_ACTIVE_MACHINE; i8StMcCount++)
//    {
//        // For all Active Look up tables
//        for (i8StCount = 0; i8StCount < MAX_NUMBER_OF_STATE; i8StCount++)
//        {
//            for (i8ExpNxtCount = 0; i8ExpNxtCount < MAX_NO_OF_EXP_NXT_STATES; i8ExpNxtCount++)
//            {
//                g_stArrStateMachines[i8StMcCount].m_stArrStateLookupTable[i8StCount].m_i8ArrExpNextState[i8ExpNxtCount] = STATE_NOTHING;
//            }
//        }
//    }
//}
//
//void PostProductionSafeMode()
//{
//    //g_unDOState.m_ui16AllDOs = 0xFFFF;
//    
//    // 1. Stopper 0 is in Push State
//    // 2. Can Stopper 1 is in Push state
//    // 3. Pusher 0, Pusher 1 and Can Guide all is in drag state
//    //g_unDOState.m_ui16AllDOs = SAFE_MODE_DO_STATE;
//    //ExecuteALLDOs();
//}
//
//void PowerOnDOStateInit()
//{
//   // g_unDOState.m_ui16AllDOs = 0xFFFF;
//    
//    // 1. Stopper 0 is in drag State
//    // 2. Can Stopper 1 is in Push state
//    // 3. Pusher 0, Pusher 1 and Can Guide all is in drag state
//   // g_unDOState.m_ui16AllDOs = INITAIL_DO_STATE_ON_PWR_ON;
//    //ExecuteALLDOs();
//}
//
//#endif /* _VM_STATE_MACHINE_FOR_IMW_M0_H_ */
