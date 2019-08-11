#ifndef _VM_DIGITAL_IO_CONFIG_H_
#define _VM_DIGITAL_IO_CONFIG_H_

//*****************************************************************************
// Below -> Data Structure for Digital Input Output ***************************
//*****************************************************************************
typedef struct _ST_ALL_DI_STATE_BIT
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

typedef union _UN_ALL_DIS
{
	unsigned int8       m_bArr[4];
    unsigned int32      m_ui32AllDIs;
    ST_ALL_DI_STATE_BIT m_stAllDIBits;
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

typedef union _UN_ALL_DOS
{
    unsigned int8       m_bArr[2];
    unsigned int16      m_ui16AllDOs;
    ST_ALL_DO_STATE_BIT m_stAllDOBits;
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

typedef union _UN_ALL_COUNTERS
{
    unsigned int8              m_bArr[2];
    unsigned int16             m_ui16AllCounters;
    ST_ALL_COUNTER_STATE_BIT   m_stAllCounterBits;
} UN_ALL_COUNTERS;

//*****************************************************************************
// Above -> Data Structure for Digital Input Output ***************************
//*****************************************************************************


//*****************************************************************************
// Below -> Global Variable Declaration ***************************************
//*****************************************************************************
UN_ALL_DIS      g_unDIState;
UN_ALL_DOS      g_unDOState;
UN_ALL_COUNTERS g_unAllCounters;
UN_ALL_DIS      g_unDIStateFilter;
//*****************************************************************************
// Above -> Global Variable Declaration ***************************************
//***************************************************************************** 

#endif /* _VI_DIGITAL_IO_CONFIG_H_ */

//typedef struct _ST_ALL_DI_STATE_BIT
//{
//    unsigned int8 m_ui1HWDICn15: 1;
//    unsigned int8 m_ui1HWDICn14: 1;
//    unsigned int8 m_ui1HWDICn13: 1;
//    unsigned int8 m_ui1HWDICn12: 1;
//    unsigned int8 m_ui1HWDICn11: 1;
//    unsigned int8 m_ui1HWDICn10: 1;
//    unsigned int8 m_ui1HWDICn09: 1;
//    unsigned int8 m_ui1HWDICn08: 1;
//
//    unsigned int8 m_ui1HWDICn07: 1;
//    unsigned int8 m_ui1HWDICn06: 1;
//    unsigned int8 m_ui1HWDICn05: 1;
//    unsigned int8 m_ui1HWDICn04: 1;
//    unsigned int8 m_ui1HWDICn03: 1;
//    unsigned int8 m_ui1HWDICn02: 1;
//    unsigned int8 m_ui1HWDICn01: 1;
//    unsigned int8 m_ui1HWDICn00: 1;
//}ST_ALL_DI_STATE_BIT;
//
//typedef union _UN_ALL_DIS
//{
//	unsigned int8       m_bArr[2];
//    unsigned int16      m_ui16AllDIs;
//    ST_ALL_DI_STATE_BIT m_stAllDIBits;
//} UN_ALL_DIS;
//
//typedef struct _ST_ALL_DO_STATE_BIT
//{
//    unsigned int8 m_ui1HWDOCn07: 1;
//    unsigned int8 m_ui1HWDOCn06: 1;
//    unsigned int8 m_ui1HWDOCn05: 1;
//    unsigned int8 m_ui1HWDOCn04: 1;
//    unsigned int8 m_ui1HWDOCn03: 1;
//    unsigned int8 m_ui1HWDOCn02: 1;
//    unsigned int8 m_ui1HWDOCn01: 1;
//    unsigned int8 m_ui1HWDOCn00: 1;
//
//}ST_ALL_DO_STATE_BIT;
//
//typedef union _UN_ALL_DOS
//{
//    unsigned int8     m_ui8AllDOs;
//    ST_ALL_DO_STATE_BIT m_stAllDOBits;
//} UN_ALL_DOS;
//
////*****************************************************************************
//// Above -> Data Structure for Digital Input Output ***************************
////*****************************************************************************
//
//
////*****************************************************************************
//// Below -> Global Variable Declaration ***************************************
////*****************************************************************************
//UN_ALL_DIS      g_unDICurrState;
//UN_ALL_DOS      g_unDOCurrState;
//
//UN_ALL_DIS      g_unDIPrevState;
//UN_ALL_DOS      g_unDOPrevState;
//
//UN_ALL_DIS      g_unDIStateFilter;
////*****************************************************************************
//// Above -> Global Variable Declaration ***************************************
////***************************************************************************** 

//#endif /* _VM_DIGITAL_IO_CONFIG_H_ */