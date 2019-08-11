#ifndef _VI_STATE_MACHINE_H_
#define _VI_STATE_MACHINE_H_

#include "vmDigitalIOConfig.h"
/*************************** Inclusion files **********************************/

#define FLAG_STATE_CHANGED           1
#define FLAG_STATE_NOT_CHANGED       0
#define MAX_NUMBER_OF_STATE_MACHINES 5
#define MIN_NUMBER_OF_STATE_MACHINES 1
#define MAX_NUMBER_OF_STATE          10
#define MAX_NO_OF_EXP_NXT_STATES     4

#define STATE_MACHINE_0              0
#define STATE_MACHINE_1              1
#define STATE_MACHINE_2              2
#define STATE_MACHINE_3              3
#define STATE_MACHINE_4              4

//#define TIMER_FLAG_SUCTION_WAIT_ON   1
//#define TIMER_FLAG_SUCTION_WAIT_OFF  0
//#define COUNTER_SUCTION_WAIT         300

// Return codes
#define VI_RETN_SUCCESS               0

#define STATE_NOTHING 0
#define STATE_01 1
#define STATE_02 2
#define STATE_03 3
#define STATE_04 4
#define STATE_05 5
#define STATE_06 6
#define STATE_07 7
#define STATE_08 8
#define STATE_09 9
#define STATE_10 10
#define STATE_11 11
#define STATE_12 12
#define STATE_13 13
#define STATE_14 14
#define STATE_15 15
#define STATE_16 16
#define STATE_17 17
#define STATE_18 18
#define STATE_19 19

#define STATE_CONTINUE_OFF 0
#define STATE_CONTINUE_ON  1

/*typedef struct _ST_ALL_DI_STATE_BIT
{
    unsigned int8 m_ui1SWDICn07: 1;
    unsigned int8 m_ui1SWDICn06: 1;
    unsigned int8 m_ui1SWDICn05: 1;
    unsigned int8 m_ui1SWDICn04: 1;
    unsigned int8 m_ui1SWDICn03: 1;
    unsigned int8 m_ui1SWDICn02: 1;
    unsigned int8 m_ui1SWDICn01: 1;
    unsigned int8 m_ui1SWDICn00: 1;

    unsigned int8 m_ui1GDICn05: 1;
    unsigned int8 m_ui1GDICn04: 1;
    unsigned int8 m_ui1GDICn03: 1;
    unsigned int8 m_ui1GDICn02: 1;
    unsigned int8 m_ui1GDICn01: 1;
    unsigned int8 m_ui1GDICn00: 1;

    unsigned int8 m_ui1HWDICn17: 1;
    unsigned int8 m_ui1HWDICn16: 1;
    unsigned int8 m_ui1HWDICn15: 1;
    unsigned int8 m_ui1HWDICn14: 1;
    unsigned int8 m_ui1HWDICn13: 1;
    unsigned int8 m_ui1HWDICn12: 1;
    unsigned int8 m_ui1HWDICn11: 1;
    unsigned int8 m_ui1HWDICn10: 1;
    unsigned int8 m_ui1HWDICn09: 1;
    unsigned int8 m_ui1HWDICn08: 1;
    unsigned int8 m_ui1HWDICn07: 1;
    unsigned int8 m_ui1HWDICn06: 1;
    unsigned int8 m_ui1HWDICn05: 1;
    unsigned int8 m_ui1HWDICn04: 1;
    unsigned int8 m_ui1HWDICn03: 1;
    unsigned int8 m_ui1HWDICn02: 1;
    unsigned int8 m_ui1HWDICn01: 1;
    unsigned int8 m_ui1HWDICn00: 1;

} ST_ALL_DI_STATE_BIT;

typedef ST_ALL_DI_STATE_BIT ST_ALL_DIS;
typedef union _UN_ALL_DIS
{
	unsigned int8      m_bArr[4];
    unsigned int32     m_ui32AllDIs;
    ST_ALL_DIS         m_stAllDIBits;
} UN_ALL_DIS;

typedef struct _ST_ALL_DO_STATE_BIT
{
    unsigned int8 m_ui1SRDOCn07: 1;
    unsigned int8 m_ui1SRDOCn06: 1;
    unsigned int8 m_ui1SRDOCn05: 1;
    unsigned int8 m_ui1SRDOCn04: 1;
    unsigned int8 m_ui1SRDOCn03: 1;
    unsigned int8 m_ui1SRDOCn02: 1;
    unsigned int8 m_ui1SRDOCn01: 1;
    unsigned int8 m_ui1SRDOCn00: 1;

    unsigned int8 m_ui1HWDOCn07: 1;
    unsigned int8 m_ui1HWDOCn06: 1;
    unsigned int8 m_ui1HWDOCn05: 1;
    unsigned int8 m_ui1HWDOCn04: 1;
    unsigned int8 m_ui1HWDOCn03: 1;
    unsigned int8 m_ui1HWDOCn02: 1;
    unsigned int8 m_ui1HWDOCn01: 1;
    unsigned int8 m_ui1HWDOCn00: 1;
} ST_ALL_DO_STATE_BIT;

typedef ST_ALL_DO_STATE_BIT ST_ALL_DOS;
typedef union _UN_ALL_DOS
{
    unsigned int8     m_bArr[2];
    unsigned int16    m_ui16AllDOs;
    ST_ALL_DOS        m_stAllDOBits;
} UN_ALL_DOS;

typedef struct _ST_ALL_COUNTER_STATE_BIT
{
    unsigned int8 m_ui3Counter0: 3;
    unsigned int8 m_ui3Counter1: 3;
    unsigned int8 m_ui2Counter2: 2;

    unsigned int8 m_ui2Counter3: 2;
    unsigned int8 m_ui2Counter4: 2;
    unsigned int8 m_ui2Counter5: 2;
    unsigned int8 m_ui2Counter6: 2;
} ST_ALL_COUNTER_STATE_BIT;

typedef ST_ALL_COUNTER_STATE_BIT ST_ALL_COUNTERS;
typedef union _UN_ALL_COUNTERS
{
    unsigned int8     m_bArr[2];
    unsigned int16    m_ui16AllCounters;
    ST_ALL_COUNTERS   m_stAllCounterBits;
} UN_ALL_COUNTERS;
*/

int8 g_ui8NumberOfActiveMachines; //= MIN_NUMBER_OF_STATE_MACHINES;



typedef struct _ST_STATE_INDEX_TABLE_NODE
{
    int32 m_ui32DIState;      // Populate the DI State after applying the State DI Filter
    int16 m_ui16DOState;      // Populate the DO State after applying the State DO Filter
} ST_STATE_INDEX_TABLE_NODE;


typedef unsigned int8(*PTR_STATE_FUNCTION)(int8, int8);
//typedef unsigned int8(*PTR_STATE_FUNCTION)(void);

typedef struct _ST_STATE_LOOKUP_TABLE_NODE
{
    int32              m_ui32DIState;      // Populate the DI State after applying the State DI Filter
    int16              m_ui16DOState;      // Populate the DO State after applying the State DO Filter
    int8               m_i8ArrExpNextState[MAX_NO_OF_EXP_NXT_STATES];
    PTR_STATE_FUNCTION pFunStateFunction;
    
} ST_STATE_LOOKUP_TABLE_NODE;


typedef struct _ST_STATE_MACHINE
{
    unsigned int32               m_ui32StateDIFilter;
    unsigned int16               m_ui16StateDOFilter;
    unsigned int16               m_uiStateCounterFilter;
    ST_STATE_INDEX_TABLE_NODE    m_stArrStateIndexTable[MAX_NUMBER_OF_STATE];
    ST_STATE_LOOKUP_TABLE_NODE   m_stArrStateLookupTable[MAX_NUMBER_OF_STATE];
    // Temporary Values
    unsigned int32               m_ui32LastDIState;
    unsigned int16               m_ui16LastDOState;

    //int8                       m_iExpNxtStates;
    int8                         m_i8ArrCurExpNextState[MAX_NO_OF_EXP_NXT_STATES];    // Expected Next States
    int8                         m_i8PrevState;
    int8                         m_fContinue;
} ST_STATE_MACHINE;


//================= All Variable Declarations Start ==========================//
//UN_ALL_DIS      g_unAllDIs;
//UN_ALL_DOS      g_unDOState;
//UN_ALL_COUNTERS g_unAllCounters;

unsigned int32 g_ui32AllDIsPreviousState      = 0;      //TODO This should be overwrite by Board Initialization
unsigned int16 g_ui32AllDOsCurrentState       = 0;      //TODO This should be overwrite by Board Initialization
unsigned int16 g_ui32AllDOsPreviousState      = 0;      //TODO This should be overwrite by Board Initialization
unsigned int16 g_ui32AllCountersCurrentState  = 0;      //TODO This should be overwrite by Board Initialization
unsigned int16 g_ui32AllCountersPreviousState = 0;      //TODO This should be overwrite by Board Initialization

unsigned int8  g_uiStateChange = FLAG_STATE_NOT_CHANGED;
unsigned int8  g_fSuctionWait = 0; 
unsigned int16 g_ui16TimerCount = 0; 
//================= All Variable Declarations End ============================//

// Timer Function for DI Polling
/*void PollingInterrupt()
{
    // Step 1 : Poll all DIs
    g_unAllDIs.m_stAllDIBits.m_ui1HWDICn00 = input(PIN_IN_DI_00);
    g_unAllDIs.m_stAllDIBits.m_ui1HWDICn01 = input(PIN_IN_DI_01);
    g_unAllDIs.m_stAllDIBits.m_ui1HWDICn02 = input(PIN_IN_DI_02);
    g_unAllDIs.m_stAllDIBits.m_ui1HWDICn03 = input(PIN_IN_DI_03);
    g_unAllDIs.m_stAllDIBits.m_ui1HWDICn04 = input(PIN_IN_DI_04);
    g_unAllDIs.m_stAllDIBits.m_ui1HWDICn05 = input(PIN_IN_DI_05);
    g_unAllDIs.m_stAllDIBits.m_ui1HWDICn06 = input(PIN_IN_DI_06);
    g_unAllDIs.m_stAllDIBits.m_ui1HWDICn07 = input(PIN_IN_DI_07);

    g_unAllDIs.m_stAllDIBits.m_ui1HWDICn08 = input(PIN_IN_DI_08);
    g_unAllDIs.m_stAllDIBits.m_ui1HWDICn09 = input(PIN_IN_DI_09);
    g_unAllDIs.m_stAllDIBits.m_ui1HWDICn10 = input(PIN_IN_DI_10);
    g_unAllDIs.m_stAllDIBits.m_ui1HWDICn11 = input(PIN_IN_DI_11);
    g_unAllDIs.m_stAllDIBits.m_ui1HWDICn12 = input(PIN_IN_DI_12);
    g_unAllDIs.m_stAllDIBits.m_ui1HWDICn13 = input(PIN_IN_DI_13);
    g_unAllDIs.m_stAllDIBits.m_ui1HWDICn14 = input(PIN_IN_DI_14);
    unsigned int8 ui8Test = 
    g_unAllDIs.m_stAllDIBits.m_ui1HWDICn15 = input(PIN_IN_DI_15);

    g_unAllDIs.m_stAllDIBits.m_ui1HWDICn16 = input(PIN_IN_DI_16);
    g_unAllDIs.m_stAllDIBits.m_ui1HWDICn17 = input(PIN_IN_DI_17);

    // Step 2: Check for any state change
    // Check ALL DI States Changes and all DO State Changes
    if (g_unAllDIs.m_ui32AllDIs == g_ui32AllDIsPreviousState &&
        g_unDOState.m_ui16AllDOs == g_ui32AllDOsPreviousState)
    {
        g_uiStateChange = FLAG_STATE_NOT_CHANGED;
    }
    else
    {
        g_uiStateChange = FLAG_STATE_CHANGED;
        g_ui32AllDIsPreviousState = g_unAllDIs.m_ui32AllDIs;
        g_ui32AllDOsPreviousState = g_unDOState.m_ui16AllDOs;
    }


    if (g_fSuctionWait == TIMER_FLAG_SUCTION_WAIT_ON)
    {
        g_ui16TimerCount++;
    }

}*/

void ExecuteALLDOs(void)
{
    delay_us(10);
    output_bit(PIN_OUT_DO_00, g_unDOState.m_stAllDOBits.m_ui1HWDOCn00);
    output_bit(PIN_OUT_DO_01, g_unDOState.m_stAllDOBits.m_ui1HWDOCn01);
    output_bit(PIN_OUT_DO_02, g_unDOState.m_stAllDOBits.m_ui1HWDOCn02);
    output_bit(PIN_OUT_DO_03, g_unDOState.m_stAllDOBits.m_ui1HWDOCn03);
    output_bit(PIN_OUT_DO_04, g_unDOState.m_stAllDOBits.m_ui1HWDOCn04);
    output_bit(PIN_OUT_DO_05, g_unDOState.m_stAllDOBits.m_ui1HWDOCn05);
    output_bit(PIN_OUT_DO_06, g_unDOState.m_stAllDOBits.m_ui1HWDOCn06);
    output_bit(PIN_OUT_DO_07, g_unDOState.m_stAllDOBits.m_ui1HWDOCn07);   
}
    
int8 HandleStateError(ST_STATE_MACHINE *pstStateMachine)
{
    int8 iStateIdx = 1;
    // Check for all the states and find suitable state
    for (iStateIdx = 1; iStateIdx < MAX_NUMBER_OF_STATE; iStateIdx++)
    {
        
    }    
    return 0;
}


int8 CheckState(ST_STATE_MACHINE *pstStateMachine, int8 i8StMacIndex)
{
    unsigned int32 ui32DIState   = 0;
    unsigned int16 ui16DOState   = 0;
    int8           i8ExpNxtState = 0;
    int8           i8TempState   = 0;
    ST_STATE_LOOKUP_TABLE_NODE* pStLookUptableNode = 0;
    unsigned int8 ui8ExpNextState = 0;
    
    // Step 1: Apply DI and DO Filter
    ui32DIState = g_unDIState.m_ui32AllDIs & pstStateMachine->m_ui32StateDIFilter;
    //ui16DOState = g_unDOState.m_ui16AllDOs & pstStateMachine->m_ui16StateDOFilter;
   
    // Step 2: Check with DO State Filter
    // If Any change not found then Return
    if (!(ui32DIState ^ pstStateMachine->m_ui32LastDIState) && pstStateMachine->m_fContinue == STATE_CONTINUE_OFF)
    {
        // No Change
        // So Return 0
        return 0;
    }
    // Step 3: IF Change found then do the following:
    //     a. Update last state value
    //     b. Check if the State is matching with the expected next state
    //     c. If YES then call State function
    //     d. If the State is not matching then perform Error Handling
    
    // Stape 3a: Update last state value
    //pstStateMachine->m_ui32LastDIState = ui32DIState;

    // Step 3b: Check if the State is matching with the expected next states
    for (i8ExpNxtState = 0; i8ExpNxtState < MAX_NO_OF_EXP_NXT_STATES; i8ExpNxtState++)
    {       
        //pstStateMachine->m_stArrStateLookupTable[pstStateMachine->m_i8PrevState].;
        //if (pstStateMachine->m_stArrStateLookupTable[pstStateMachine->m_i8ArrCurExpNextState[i8ExpNxtState]].m_ui32DIState ^ ui32DIState)
        //if (0 == (pstStateMachine->m_stArrStateLookupTable[pstStateMachine->m_i8ArrCurExpNextState[i8ExpNxtState]].m_ui32DIState ^ ui32DIState))
        ui8ExpNextState = pstStateMachine->m_stArrStateLookupTable[pstStateMachine->m_i8PrevState].m_i8ArrExpNextState[i8ExpNxtState];
        if (ui8ExpNextState == STATE_NOTHING)
        {
            // No State Matched, this is an Error
            // Handle Error
            return HandleStateError(pstStateMachine);
        }    
        
        if (0 == (pstStateMachine->m_stArrStateLookupTable[ui8ExpNextState].m_ui32DIState ^ ui32DIState))
        {   
            // State Is matching
            // Call State Function
            pstStateMachine->m_stArrStateLookupTable[ui8ExpNextState].pFunStateFunction(i8StMacIndex, ui8ExpNextState);
            
            pstStateMachine->m_i8PrevState = ui8ExpNextState;
            
            //if (pstStateMachine->m_ui32StateDIFilter == 0x7000007)
            //{
            //    output_bit(PIN_OUT_DO_07, 0);
            //    g_unDOState.m_stAllDOBits.m_ui1HWDOCn07 = 0;
            //}    
            // Stape 3a: Update last state value
            pstStateMachine->m_ui32LastDIState = ui32DIState;
            
            break;
            
        }
        else
        {
            // Next State is not matching
            // ERROR
        }
    }


    // If Expected State does not match
    if (i8ExpNxtState == MAX_NO_OF_EXP_NXT_STATES)
    {
        // Handle Error
        return HandleStateError(pstStateMachine);
    }

    return VI_RETN_SUCCESS;
}

void InitilizeStates()
{
    g_unDIState.m_ui32AllDIs = 0xFFFFFFFF;
    g_unDOState.m_ui16AllDOs = 0xFFFF;
}
    

#endif /* _VI_STATE_MACHINE_H_ */




//#ifndef _VM_STATE_MACHINE_H_
//#define _VM_STATE_MACHINE_H_
//
//#include "vmDigitalIOConfig.h"
///*************************** Inclusion files **********************************/
//
//#define FLAG_STATE_CHANGED           1
//#define FLAG_STATE_NOT_CHANGED       0
//#define MAX_NUMBER_OF_STATE_MACHINES 5
//#define MIN_NUMBER_OF_STATE_MACHINES 1
//#define MAX_NUMBER_OF_STATE          8
//#define MAX_NO_OF_EXP_NXT_STATES     4
//
//#define STATE_MACHINE_0   0
//#define STATE_MACHINE_1   1
//#define STATE_MACHINE_2   2
//#define STATE_MACHINE_3   3
//#define STATE_MACHINE_4   4
//
////#define TIMER_FLAG_SUCTION_WAIT_ON   1
////#define TIMER_FLAG_SUCTION_WAIT_OFF  0
////#define COUNTER_SUCTION_WAIT         300
//
//// Return codes
//#define VI_RETN_SUCCESS               0
//
//#define STATE_NOTHING 0
//#define STATE_01 1
//#define STATE_02 2
//#define STATE_03 3
//#define STATE_04 4
//#define STATE_05 5
//#define STATE_06 6
//#define STATE_07 7
//#define STATE_08 8
//#define STATE_09 9
//#define STATE_10 10
//#define STATE_11 11
//#define STATE_12 12
//#define STATE_13 13
//#define STATE_14 14
//#define STATE_15 15
//#define STATE_16 16
//#define STATE_17 17
//#define STATE_18 18
//#define STATE_19 19
//
//#define STATE_CONTINUE_OFF 0
//#define STATE_CONTINUE_ON  1
//
//
//int8 g_ui8NumberOfActiveMachines; //= MIN_NUMBER_OF_STATE_MACHINES;
//
//
//
//typedef struct _ST_STATE_INDEX_TABLE_NODE
//{
//    int32 m_ui32DIState;      // Populate the DI State after applying the State DI Filter
//    int16 m_ui16DOState;      // Populate the DO State after applying the State DO Filter
//} ST_STATE_INDEX_TABLE_NODE;
//
//
//typedef unsigned int8(*PTR_STATE_FUNCTION)(int8, int8);
////typedef unsigned int8(*PTR_STATE_FUNCTION)(void);
//
//typedef struct _ST_STATE_LOOKUP_TABLE_NODE
//{
//    int32              m_ui32DIState;      // Populate the DI State after applying the State DI Filter
//    int16              m_ui16DOState;      // Populate the DO State after applying the State DO Filter
//    int8               m_i8ArrExpNextState[MAX_NO_OF_EXP_NXT_STATES];
//    PTR_STATE_FUNCTION pFunStateFunction;
//    
//} ST_STATE_LOOKUP_TABLE_NODE;
//
//
//typedef struct _ST_STATE_MACHINE
//{
//    unsigned int32               m_ui32StateDIFilter;
//    unsigned int16               m_ui16StateDOFilter;
//    unsigned int16               m_uiStateCounterFilter;
//    ST_STATE_INDEX_TABLE_NODE    m_stArrStateIndexTable[MAX_NUMBER_OF_STATE];
//    ST_STATE_LOOKUP_TABLE_NODE   m_stArrStateLookupTable[MAX_NUMBER_OF_STATE];
//    // Temporary Values
//    unsigned int32               m_ui32LastDIState;
//    unsigned int16               m_ui16LastDOState;
//
//    //int8                       m_iExpNxtStates;
//    int8                         m_i8ArrCurExpNextState[MAX_NO_OF_EXP_NXT_STATES];    // Expected Next States
//    int8                         m_i8PrevState;
//    int8                         m_fContinue;
//} ST_STATE_MACHINE;
//
//
////================= All Variable Declarations Start ==========================//
////UN_ALL_DIS      g_unAllDIs;
////UN_ALL_DOS      g_unDOState;
////UN_ALL_COUNTERS g_unAllCounters;
//
//unsigned int32 g_ui32AllDIsPreviousState      = 0;      //TODO This should be overwrite by Board Initialization
//unsigned int16 g_ui32AllDOsCurrentState       = 0;      //TODO This should be overwrite by Board Initialization
//unsigned int16 g_ui32AllDOsPreviousState      = 0;      //TODO This should be overwrite by Board Initialization
//unsigned int16 g_ui32AllCountersCurrentState  = 0;      //TODO This should be overwrite by Board Initialization
//unsigned int16 g_ui32AllCountersPreviousState = 0;      //TODO This should be overwrite by Board Initialization
//
//unsigned int8  g_uiStateChange = FLAG_STATE_NOT_CHANGED;
//unsigned int8  g_fSuctionWait = 0; 
//unsigned int16 g_ui16TimerCount = 0; 
////================= All Variable Declarations End ============================//
//
//// Timer Function for DI Polling
///*void PollingInterrupt()
//{
//    // Step 1 : Poll all DIs
//    g_unAllDIs.m_stAllDIBits.m_ui1HWDICn00 = input(PIN_IN_DI_00);
//    g_unAllDIs.m_stAllDIBits.m_ui1HWDICn01 = input(PIN_IN_DI_01);
//    g_unAllDIs.m_stAllDIBits.m_ui1HWDICn02 = input(PIN_IN_DI_02);
//    g_unAllDIs.m_stAllDIBits.m_ui1HWDICn03 = input(PIN_IN_DI_03);
//    g_unAllDIs.m_stAllDIBits.m_ui1HWDICn04 = input(PIN_IN_DI_04);
//    g_unAllDIs.m_stAllDIBits.m_ui1HWDICn05 = input(PIN_IN_DI_05);
//    g_unAllDIs.m_stAllDIBits.m_ui1HWDICn06 = input(PIN_IN_DI_06);
//    g_unAllDIs.m_stAllDIBits.m_ui1HWDICn07 = input(PIN_IN_DI_07);
//
//    g_unAllDIs.m_stAllDIBits.m_ui1HWDICn08 = input(PIN_IN_DI_08);
//    g_unAllDIs.m_stAllDIBits.m_ui1HWDICn09 = input(PIN_IN_DI_09);
//    g_unAllDIs.m_stAllDIBits.m_ui1HWDICn10 = input(PIN_IN_DI_10);
//    g_unAllDIs.m_stAllDIBits.m_ui1HWDICn11 = input(PIN_IN_DI_11);
//    g_unAllDIs.m_stAllDIBits.m_ui1HWDICn12 = input(PIN_IN_DI_12);
//    g_unAllDIs.m_stAllDIBits.m_ui1HWDICn13 = input(PIN_IN_DI_13);
//    g_unAllDIs.m_stAllDIBits.m_ui1HWDICn14 = input(PIN_IN_DI_14);
//    unsigned int8 ui8Test = 
//    g_unAllDIs.m_stAllDIBits.m_ui1HWDICn15 = input(PIN_IN_DI_15);
//
//    g_unAllDIs.m_stAllDIBits.m_ui1HWDICn16 = input(PIN_IN_DI_16);
//    g_unAllDIs.m_stAllDIBits.m_ui1HWDICn17 = input(PIN_IN_DI_17);
//
//    // Step 2: Check for any state change
//    // Check ALL DI States Changes and all DO State Changes
//    if (g_unAllDIs.m_ui32AllDIs == g_ui32AllDIsPreviousState &&
//        g_unDOState.m_ui16AllDOs == g_ui32AllDOsPreviousState)
//    {
//        g_uiStateChange = FLAG_STATE_NOT_CHANGED;
//    }
//    else
//    {
//        g_uiStateChange = FLAG_STATE_CHANGED;
//        g_ui32AllDIsPreviousState = g_unAllDIs.m_ui32AllDIs;
//        g_ui32AllDOsPreviousState = g_unDOState.m_ui16AllDOs;
//    }
//
//
//    if (g_fSuctionWait == TIMER_FLAG_SUCTION_WAIT_ON)
//    {
//        g_ui16TimerCount++;
//    }
//
//}*/
//
//void ExecuteALLDOs(void)
//{
//    delay_us(10);
//    output_bit(PIN_OUT_DO_00, g_unDOState.m_stAllDOBits.m_ui1HWDOCn00);
//    output_bit(PIN_OUT_DO_01, g_unDOState.m_stAllDOBits.m_ui1HWDOCn01);
//    output_bit(PIN_OUT_DO_02, g_unDOState.m_stAllDOBits.m_ui1HWDOCn02);
//    output_bit(PIN_OUT_DO_03, g_unDOState.m_stAllDOBits.m_ui1HWDOCn03);
//    output_bit(PIN_OUT_DO_04, g_unDOState.m_stAllDOBits.m_ui1HWDOCn04);
//    output_bit(PIN_OUT_DO_05, g_unDOState.m_stAllDOBits.m_ui1HWDOCn05);
//    output_bit(PIN_OUT_DO_06, g_unDOState.m_stAllDOBits.m_ui1HWDOCn06);
//    output_bit(PIN_OUT_DO_07, g_unDOState.m_stAllDOBits.m_ui1HWDOCn07);   
//}
//    
//int8 HandleStateError(ST_STATE_MACHINE *pstStateMachine)
//{
//    int8 iStateIdx = 1;
//    // Check for all the states and find suitable state
//    for (iStateIdx = 1; iStateIdx < MAX_NUMBER_OF_STATE; iStateIdx++)
//    {
//        
//    }    
//    return 0;
//}
//
//
//int8 CheckState(ST_STATE_MACHINE *pstStateMachine, int8 i8StMacIndex)
//{
//    unsigned int32 ui32DIState   = 0;
//    unsigned int16 ui16DOState   = 0;
//    int8           i8ExpNxtState = 0;
//    int8           i8TempState   = 0;
//    ST_STATE_LOOKUP_TABLE_NODE* pStLookUptableNode = 0;
//    unsigned int8 ui8ExpNextState = 0;
//    
//    // Step 1: Apply DI and DO Filter
//    //ui32DIState = g_unDIState.m_ui32AllDIs & pstStateMachine->m_ui32StateDIFilter;
//    //ui16DOState = g_unDOState.m_ui16AllDOs & pstStateMachine->m_ui16StateDOFilter;
//   
//    // Step 2: Check with DO State Filter
//    // If Any change not found then Return
//    if (!(ui32DIState ^ pstStateMachine->m_ui32LastDIState) && pstStateMachine->m_fContinue == STATE_CONTINUE_OFF)
//    {
//        // No Change
//        // So Return 0
//        return 0;
//    }
//    // Step 3: IF Change found then do the following:
//    //     a. Update last state value
//    //     b. Check if the State is matching with the expected next state
//    //     c. If YES then call State function
//    //     d. If the State is not matching then perform Error Handling
//    
//    // Stape 3a: Update last state value
//    //pstStateMachine->m_ui32LastDIState = ui32DIState;
//
//    // Step 3b: Check if the State is matching with the expected next states
//    for (i8ExpNxtState = 0; i8ExpNxtState < MAX_NO_OF_EXP_NXT_STATES; i8ExpNxtState++)
//    {       
//        //pstStateMachine->m_stArrStateLookupTable[pstStateMachine->m_i8PrevState].;
//        //if (pstStateMachine->m_stArrStateLookupTable[pstStateMachine->m_i8ArrCurExpNextState[i8ExpNxtState]].m_ui32DIState ^ ui32DIState)
//        //if (0 == (pstStateMachine->m_stArrStateLookupTable[pstStateMachine->m_i8ArrCurExpNextState[i8ExpNxtState]].m_ui32DIState ^ ui32DIState))
//        ui8ExpNextState = pstStateMachine->m_stArrStateLookupTable[pstStateMachine->m_i8PrevState].m_i8ArrExpNextState[i8ExpNxtState];
//        if (ui8ExpNextState == STATE_NOTHING)
//        {
//            // No State Matched, this is an Error
//            // Handle Error
//            return HandleStateError(pstStateMachine);
//        }    
//        
//        if (0 == (pstStateMachine->m_stArrStateLookupTable[ui8ExpNextState].m_ui32DIState ^ ui32DIState))
//        {   
//            // State Is matching
//            // Call State Function
//            pstStateMachine->m_stArrStateLookupTable[ui8ExpNextState].pFunStateFunction(i8StMacIndex, ui8ExpNextState);
//            
//            pstStateMachine->m_i8PrevState = ui8ExpNextState;
//            
//            //if (pstStateMachine->m_ui32StateDIFilter == 0x7000007)
//            //{
//            //    output_bit(PIN_OUT_DO_07, 0);
//            //    g_unDOState.m_stAllDOBits.m_ui1HWDOCn07 = 0;
//            //}    
//            // Stape 3a: Update last state value
//            pstStateMachine->m_ui32LastDIState = ui32DIState;
//            
//            break;
//            
//        }
//        else
//        {
//            // Next State is not matching
//            // ERROR
//        }
//    }
//
//
//    // If Expected State does not match
//    if (i8ExpNxtState == MAX_NO_OF_EXP_NXT_STATES)
//    {
//        // Handle Error
//        return HandleStateError(pstStateMachine);
//    }
//
//    return VI_RETN_SUCCESS;
//}
//
//void InitilizeStates()
//{
//   // g_unDIState.m_ui32AllDIs = 0xFFFFFFFF;
//   // g_unDOState.m_ui16AllDOs = 0xFFFF;
//}
//    
//
//#endif /* _VM_STATE_MACHINE_H_ */
//
//
