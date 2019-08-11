#ifndef _VI_SHIFT_REG_H_
#define _VI_SHIFT_REG_H_

#include "vmSystemConfig.h"

#define LOGICAL_ON  0
#define LOGICAL_OFF 1

#define DO_DBG_SHIFT_REG_CLOCK       PIN_B4 //PIN_B5
//#define DO_DBG_SHIFT_REG_CLOCK       PIN_D0
#define DO_DBG_SHIFT_REG_DATA_A      PIN_B5 //PIN_B3

typedef struct {
  unsigned int8 bit_0: 1;
  unsigned int8 bit_1: 1;
  unsigned int8 bit_2: 1;
  unsigned int8 bit_3: 1;
  unsigned int8 bit_4: 1;
  unsigned int8 bit_5: 1;
  unsigned int8 bit_6: 1;
  unsigned int8 bit_7: 1;
} _DbgLedWordType;

typedef union {
   unsigned int     gDbgLedWord;
  _DbgLedWordType   DbgLedWord;
} DbgLedDataType;

DbgLedDataType gDbgLedDataWord;
DbgLedDataType gPreviousDbgLedDataWord;




/* Set Reset Debug Shift Register Outputs */
static void SetSystemLedState(unsigned int8 ledId,
                             unsigned int8 ledState)
{
  /* Set LED Data */
  unsigned int tempWord = 0x01;
    
  switch(ledState)
  {
    case 0: /* Reset bit - LED On */
      tempWord <<= (ledId-1);
      tempWord = ~(tempWord);
      gDbgLedDataWord.gDbgLedWord &= tempWord;
      break;
    case 1: /* Set bit - LED Off */
      tempWord <<= (ledId-1);
      gDbgLedDataWord.gDbgLedWord |= tempWord;
      break;
    default: /* Nothing to do */
      break;
  } 

  /* Check for change in Data */
  if(gDbgLedDataWord.gDbgLedWord != gPreviousDbgLedDataWord.gDbgLedWord)
  {
    /* Send Byte to Shift register 74LS164 */
    output_bit(DO_DBG_SHIFT_REG_CLOCK, 1);

    /* Shift in 1st QH */
    output_bit(DO_DBG_SHIFT_REG_DATA_A, gDbgLedDataWord.DbgLedWord.bit_7);
    output_bit(DO_DBG_SHIFT_REG_CLOCK, 0);
    //delay_us(10);
    output_bit(DO_DBG_SHIFT_REG_CLOCK, 1);
    /* Shift in 1st QG */
    output_bit(DO_DBG_SHIFT_REG_DATA_A, gDbgLedDataWord.DbgLedWord.bit_6);
    output_bit(DO_DBG_SHIFT_REG_CLOCK, 0);
    //delay_us(10);
    output_bit(DO_DBG_SHIFT_REG_CLOCK, 1);
    /* Shift in 1st QF */
    output_bit(DO_DBG_SHIFT_REG_DATA_A, gDbgLedDataWord.DbgLedWord.bit_5);
    output_bit(DO_DBG_SHIFT_REG_CLOCK, 0);
   //delay_us(10);
    output_bit(DO_DBG_SHIFT_REG_CLOCK, 1);
    /* Shift in 1st QE */
    output_bit(DO_DBG_SHIFT_REG_DATA_A, gDbgLedDataWord.DbgLedWord.bit_4);
    output_bit(DO_DBG_SHIFT_REG_CLOCK, 0);
    //delay_us(10);
    output_bit(DO_DBG_SHIFT_REG_CLOCK, 1);
    /* Shift in 1st QD */
    output_bit(DO_DBG_SHIFT_REG_DATA_A, gDbgLedDataWord.DbgLedWord.bit_3);
    output_bit(DO_DBG_SHIFT_REG_CLOCK, 0);
    //delay_us(10);
    output_bit(DO_DBG_SHIFT_REG_CLOCK, 1);
    /* Shift in 1st QC */
    output_bit(DO_DBG_SHIFT_REG_DATA_A, gDbgLedDataWord.DbgLedWord.bit_2);
    output_bit(DO_DBG_SHIFT_REG_CLOCK, 0);
    //delay_us(10);
    output_bit(DO_DBG_SHIFT_REG_CLOCK, 1);
    /* Shift in 1st QB */
    output_bit(DO_DBG_SHIFT_REG_DATA_A, gDbgLedDataWord.DbgLedWord.bit_1);
    output_bit(DO_DBG_SHIFT_REG_CLOCK, 0);
    //delay_us(10);
    output_bit(DO_DBG_SHIFT_REG_CLOCK, 1);
    /* Shift in 1st QA */
    output_bit(DO_DBG_SHIFT_REG_DATA_A, gDbgLedDataWord.DbgLedWord.bit_0);
    output_bit(DO_DBG_SHIFT_REG_CLOCK, 0);
    //delay_us(10);
    output_bit(DO_DBG_SHIFT_REG_CLOCK, 1);

    /* Update previous LED data */ 
    gPreviousDbgLedDataWord.gDbgLedWord = gDbgLedDataWord.gDbgLedWord;
  }
  else
  {
    /* Do nothing */
  }
}

typedef struct _ST_BYTE_TYPE
{
    unsigned int8 ui8Bit_00:1;
    unsigned int8 ui8Bit_01:1;
    unsigned int8 ui8Bit_02:1;
    unsigned int8 ui8Bit_03:1;
    unsigned int8 ui8Bit_04:1;
    unsigned int8 ui8Bit_05:1;
    unsigned int8 ui8Bit_06:1;
    unsigned int8 ui8Bit_07:1;
} ST_BYTE_TYPE;

typedef union _UN_BYTE_TYPE
{
    unsigned int8 m_uiByte;
    ST_BYTE_TYPE  m_stByte;
} UN_BYTE_TYPE;

UN_BYTE_TYPE unByte;


void Reset()
{
    unsigned int8 ui8LoopCount = 0;
    
    gDbgLedDataWord.gDbgLedWord = gPreviousDbgLedDataWord.gDbgLedWord = 0xFF;
    
    for(ui8LoopCount=0; ui8LoopCount<8; ui8LoopCount++)
    {
        output_bit(DO_DBG_SHIFT_REG_DATA_A, 1);
        output_bit(DO_DBG_SHIFT_REG_CLOCK, 0);
        output_bit(DO_DBG_SHIFT_REG_CLOCK, 1);
    }
}

//###################################### Above Driver ###################################################################

//###################################### Below Non Driver ###############################################################
#define LED_BOX_PASSED               	1
#define LED_BOX_REJECTED             	2
#define LED_PC_COMMUNICATION_RX         3
#define LED_PC_COMMUNICATION_TX         4
#define LED_LDCD_COMMUNICATION_RX     	5
#define LED_LDCD_COMMUNICATION_TX     	6
#define LED_PROD_MODE   				7
#define LED_CALIB_MODE   				8



void SetLEDsOnPowerOn()
{
    SetSystemLedState(LED_PROD_MODE,LOGICAL_OFF);
    SetSystemLedState(LED_CALIB_MODE,LOGICAL_OFF);
    SetSystemLedState(LED_BOX_PASSED,LOGICAL_OFF);
    SetSystemLedState(LED_BOX_REJECTED,LOGICAL_OFF);
    SetSystemLedState(LED_PC_COMMUNICATION_RX,LOGICAL_OFF);
    SetSystemLedState(LED_PC_COMMUNICATION_TX,LOGICAL_OFF);
    SetSystemLedState(LED_LDCD_COMMUNICATION_RX,LOGICAL_OFF);
    SetSystemLedState(LED_LDCD_COMMUNICATION_TX,LOGICAL_OFF);
    
}    

void SetLEDsOnPowerOff()
{
    SetSystemLedState(LED_PROD_MODE,LOGICAL_ON);
    SetSystemLedState(LED_CALIB_MODE,LOGICAL_ON);
    SetSystemLedState(LED_BOX_PASSED,LOGICAL_ON);
    SetSystemLedState(LED_BOX_REJECTED,LOGICAL_ON);
    SetSystemLedState(LED_PC_COMMUNICATION_RX,LOGICAL_ON);
    SetSystemLedState(LED_PC_COMMUNICATION_TX,LOGICAL_ON);
    SetSystemLedState(LED_LDCD_COMMUNICATION_RX,LOGICAL_ON);
    SetSystemLedState(LED_LDCD_COMMUNICATION_TX,LOGICAL_ON);
}  

void SetLEDsAlter()
{
	SetSystemLedState(LED_PROD_MODE,LOGICAL_ON);
	SetSystemLedState(LED_CALIB_MODE,LOGICAL_OFF);
	SetSystemLedState(LED_BOX_PASSED,LOGICAL_ON);
	SetSystemLedState(LED_BOX_REJECTED,LOGICAL_OFF);
	SetSystemLedState(LED_PC_COMMUNICATION_RX,LOGICAL_ON);
	SetSystemLedState(LED_PC_COMMUNICATION_TX,LOGICAL_OFF);
	SetSystemLedState(LED_LDCD_COMMUNICATION_RX,LOGICAL_ON);
	SetSystemLedState(LED_LDCD_COMMUNICATION_TX,LOGICAL_OFF);

}
void SetLEDs1stON()
{
	SetSystemLedState(LED_PROD_MODE,LOGICAL_ON);
	SetSystemLedState(LED_CALIB_MODE,LOGICAL_OFF);
    SetSystemLedState(LED_BOX_PASSED,LOGICAL_OFF);
    SetSystemLedState(LED_BOX_REJECTED,LOGICAL_OFF);
    SetSystemLedState(LED_PC_COMMUNICATION_RX,LOGICAL_OFF);
    SetSystemLedState(LED_PC_COMMUNICATION_TX,LOGICAL_OFF);
    SetSystemLedState(LED_LDCD_COMMUNICATION_RX,LOGICAL_OFF);
    SetSystemLedState(LED_LDCD_COMMUNICATION_TX,LOGICAL_OFF);
	
}

void SetLEDs2ndON()
{
	SetSystemLedState(LED_PROD_MODE,LOGICAL_OFF);
	SetSystemLedState(LED_CALIB_MODE,LOGICAL_ON);
	SetSystemLedState(LED_BOX_PASSED,LOGICAL_OFF);
    SetSystemLedState(LED_BOX_REJECTED,LOGICAL_OFF);
    SetSystemLedState(LED_PC_COMMUNICATION_RX,LOGICAL_OFF);
    SetSystemLedState(LED_PC_COMMUNICATION_TX,LOGICAL_OFF);
    SetSystemLedState(LED_LDCD_COMMUNICATION_RX,LOGICAL_OFF);
    SetSystemLedState(LED_LDCD_COMMUNICATION_TX,LOGICAL_OFF);

}

void SetLEDs3rdON()
{
	SetSystemLedState(LED_PROD_MODE,LOGICAL_OFF);
	SetSystemLedState(LED_CALIB_MODE,LOGICAL_OFF);
	SetSystemLedState(LED_BOX_PASSED,LOGICAL_ON);
	SetSystemLedState(LED_BOX_REJECTED,LOGICAL_OFF);
    SetSystemLedState(LED_PC_COMMUNICATION_RX,LOGICAL_OFF);
    SetSystemLedState(LED_PC_COMMUNICATION_TX,LOGICAL_OFF);
    SetSystemLedState(LED_LDCD_COMMUNICATION_RX,LOGICAL_OFF);
    SetSystemLedState(LED_LDCD_COMMUNICATION_TX,LOGICAL_OFF);
}

void SetLEDs4thON()
{
	SetSystemLedState(LED_PROD_MODE,LOGICAL_OFF);
	SetSystemLedState(LED_CALIB_MODE,LOGICAL_OFF);
	SetSystemLedState(LED_BOX_PASSED,LOGICAL_OFF);
	SetSystemLedState(LED_BOX_REJECTED,LOGICAL_ON);
    SetSystemLedState(LED_PC_COMMUNICATION_RX,LOGICAL_OFF);
    SetSystemLedState(LED_PC_COMMUNICATION_TX,LOGICAL_OFF);
    SetSystemLedState(LED_LDCD_COMMUNICATION_RX,LOGICAL_OFF);
    SetSystemLedState(LED_LDCD_COMMUNICATION_TX,LOGICAL_OFF);
}

void SetLEDs5thON()
{
	SetSystemLedState(LED_PROD_MODE,LOGICAL_OFF);
	SetSystemLedState(LED_CALIB_MODE,LOGICAL_OFF);
	SetSystemLedState(LED_BOX_PASSED,LOGICAL_OFF);
	SetSystemLedState(LED_BOX_REJECTED,LOGICAL_OFF);
	SetSystemLedState(LED_PC_COMMUNICATION_RX,LOGICAL_ON);
    SetSystemLedState(LED_PC_COMMUNICATION_TX,LOGICAL_OFF);
    SetSystemLedState(LED_LDCD_COMMUNICATION_RX,LOGICAL_OFF);
    SetSystemLedState(LED_LDCD_COMMUNICATION_TX,LOGICAL_OFF);

}

void SetLEDs6thON()
{
	SetSystemLedState(LED_PROD_MODE,LOGICAL_OFF);
	SetSystemLedState(LED_CALIB_MODE,LOGICAL_OFF);
	SetSystemLedState(LED_BOX_PASSED,LOGICAL_OFF);
	SetSystemLedState(LED_BOX_REJECTED,LOGICAL_OFF);
	SetSystemLedState(LED_PC_COMMUNICATION_RX,LOGICAL_OFF);
    SetSystemLedState(LED_PC_COMMUNICATION_TX,LOGICAL_ON);
    SetSystemLedState(LED_LDCD_COMMUNICATION_RX,LOGICAL_OFF);
    SetSystemLedState(LED_LDCD_COMMUNICATION_TX,LOGICAL_OFF);	
}

void SetLEDs7thON()
{
	SetSystemLedState(LED_PROD_MODE,LOGICAL_OFF);
	SetSystemLedState(LED_CALIB_MODE,LOGICAL_OFF);
	SetSystemLedState(LED_BOX_PASSED,LOGICAL_OFF);
	SetSystemLedState(LED_BOX_REJECTED,LOGICAL_OFF);
	SetSystemLedState(LED_PC_COMMUNICATION_RX,LOGICAL_OFF);
    SetSystemLedState(LED_PC_COMMUNICATION_TX,LOGICAL_OFF);
    SetSystemLedState(LED_LDCD_COMMUNICATION_RX,LOGICAL_ON);
    SetSystemLedState(LED_LDCD_COMMUNICATION_TX,LOGICAL_OFF);
}

void SetLEDs8thON()
{
	SetSystemLedState(LED_PROD_MODE,LOGICAL_OFF);
	SetSystemLedState(LED_CALIB_MODE,LOGICAL_OFF);
	SetSystemLedState(LED_BOX_PASSED,LOGICAL_OFF);
	SetSystemLedState(LED_BOX_REJECTED,LOGICAL_OFF);
	SetSystemLedState(LED_PC_COMMUNICATION_RX,LOGICAL_OFF);
    SetSystemLedState(LED_PC_COMMUNICATION_TX,LOGICAL_OFF);
    SetSystemLedState(LED_LDCD_COMMUNICATION_RX,LOGICAL_OFF);
    SetSystemLedState(LED_LDCD_COMMUNICATION_TX,LOGICAL_ON);
}

#endif /* _VI_SHIFT_REG_H_ */

//#ifndef _VI_SHIFT_REG_H_
//#define _VI_SHIFT_REG_H_
//
//#include "viSystemConfig.h"
//
//#define LOGICAL_ON  0
//#define LOGICAL_OFF 1
//
//#define DO_DBG_SHIFT_REG_CLOCK       PIN_B4
////#define DO_DBG_SHIFT_REG_CLOCK       PIN_D0
//#define DO_DBG_SHIFT_REG_DATA_A      PIN_B3
//
//typedef struct {
//  unsigned int8 bit_0: 1;
//  unsigned int8 bit_1: 1;
//  unsigned int8 bit_2: 1;
//  unsigned int8 bit_3: 1;
//  unsigned int8 bit_4: 1;
//  unsigned int8 bit_5: 1;
//  unsigned int8 bit_6: 1;
//  unsigned int8 bit_7: 1;
//  unsigned int8 bit_8: 1;
//  unsigned int8 bit_9: 1;
//  unsigned int8 bit_10: 1;
//  unsigned int8 bit_11: 1;
//  unsigned int8 bit_12: 1;
//  unsigned int8 bit_13: 1;
//  unsigned int8 bit_14: 1;
//  unsigned int8 bit_15: 1;
//  unsigned int8 bit_16: 1;
//  unsigned int8 bit_17: 1;
//  unsigned int8 bit_18: 1;
//  unsigned int8 bit_19: 1;
//  unsigned int8 bit_20: 1;
//  unsigned int8 bit_21: 1;
//  unsigned int8 bit_22: 1;
//  unsigned int8 bit_23: 1;
//  unsigned int8 bit_24: 1;
//  unsigned int8 bit_25: 1;
//  unsigned int8 bit_26: 1;
//  unsigned int8 bit_27: 1;
//  unsigned int8 bit_28: 1;
//  unsigned int8 bit_29: 1;
//  unsigned int8 bit_30: 1;
//  unsigned int8 bit_31: 1;
//} _DbgLedWordType;
//
//typedef union {
//  unsigned int32    gDbgLedWord;
//  _DbgLedWordType   DbgLedWord;
//} DbgLedDataType;
//
//DbgLedDataType gDbgLedDataWord;
//DbgLedDataType gPreviousDbgLedDataWord;
//
//
//
//
///* Set Reset Debug Shift Register Outputs */
//static void SetSystemLedState(unsigned int8 ledId,
//                             unsigned int8 ledState)
//{
//  /* Set LED Data */
//  unsigned int32 tempWord = 0x00000001;
//    
//  switch(ledState)
//  {
//    case 0: /* Reset bit - LED On */
//      tempWord <<= (ledId-1);
//      tempWord = ~(tempWord);
//      gDbgLedDataWord.gDbgLedWord &= tempWord;
//      break;
//    case 1: /* Set bit - LED Off */
//      tempWord <<= (ledId-1);
//      gDbgLedDataWord.gDbgLedWord |= tempWord;
//      break;
//    default: /* Nothing to do */
//      break;
//  } 
//
//  /* Check for change in Data */
//  if(gDbgLedDataWord.gDbgLedWord != gPreviousDbgLedDataWord.gDbgLedWord)
//  {
//    /* Send Byte to Shift register 74LS164 */
//    output_bit(DO_DBG_SHIFT_REG_CLOCK, 1);
//
//
//    /* Shift in 3rd QH */
//    output_bit(DO_DBG_SHIFT_REG_DATA_A, gDbgLedDataWord.DbgLedWord.bit_23);
//    output_bit(DO_DBG_SHIFT_REG_CLOCK, 0);
//    output_bit(DO_DBG_SHIFT_REG_CLOCK, 1);
//    /* Shift in 3rd QG */
//    output_bit(DO_DBG_SHIFT_REG_DATA_A, gDbgLedDataWord.DbgLedWord.bit_22);
//    output_bit(DO_DBG_SHIFT_REG_CLOCK, 0);
//    output_bit(DO_DBG_SHIFT_REG_CLOCK, 1);
//    /* Shift in 3rd QF */
//    output_bit(DO_DBG_SHIFT_REG_DATA_A, gDbgLedDataWord.DbgLedWord.bit_21);
//    output_bit(DO_DBG_SHIFT_REG_CLOCK, 0);
//    output_bit(DO_DBG_SHIFT_REG_CLOCK, 1);
//    /* Shift in 3rd QE */
//    output_bit(DO_DBG_SHIFT_REG_DATA_A, gDbgLedDataWord.DbgLedWord.bit_20);
//    output_bit(DO_DBG_SHIFT_REG_CLOCK, 0);
//    output_bit(DO_DBG_SHIFT_REG_CLOCK, 1);
//    /* Shift in 3rd QD */
//    output_bit(DO_DBG_SHIFT_REG_DATA_A, gDbgLedDataWord.DbgLedWord.bit_19);
//    output_bit(DO_DBG_SHIFT_REG_CLOCK, 0);
//    output_bit(DO_DBG_SHIFT_REG_CLOCK, 1);
//    /* Shift in 3rd QC */
//    output_bit(DO_DBG_SHIFT_REG_DATA_A, gDbgLedDataWord.DbgLedWord.bit_18);
//    output_bit(DO_DBG_SHIFT_REG_CLOCK, 0);
//    output_bit(DO_DBG_SHIFT_REG_CLOCK, 1);
//    /* Shift in 3rd QB */
//    output_bit(DO_DBG_SHIFT_REG_DATA_A, gDbgLedDataWord.DbgLedWord.bit_17);
//    output_bit(DO_DBG_SHIFT_REG_CLOCK, 0);
//    output_bit(DO_DBG_SHIFT_REG_CLOCK, 1);
//    /* Shift in 3rd QA */
//    output_bit(DO_DBG_SHIFT_REG_DATA_A, gDbgLedDataWord.DbgLedWord.bit_16);
//    output_bit(DO_DBG_SHIFT_REG_CLOCK, 0);
//    output_bit(DO_DBG_SHIFT_REG_CLOCK, 1);
//
//    /* Shift in 2nd QH */
//    output_bit(DO_DBG_SHIFT_REG_DATA_A, gDbgLedDataWord.DbgLedWord.bit_15);
//    output_bit(DO_DBG_SHIFT_REG_CLOCK, 0);
//    output_bit(DO_DBG_SHIFT_REG_CLOCK, 1);
//    /* Shift in 2nd QG */
//    output_bit(DO_DBG_SHIFT_REG_DATA_A, gDbgLedDataWord.DbgLedWord.bit_14);
//    output_bit(DO_DBG_SHIFT_REG_CLOCK, 0);
//    output_bit(DO_DBG_SHIFT_REG_CLOCK, 1);
//    /* Shift in 2nd QF */
//    output_bit(DO_DBG_SHIFT_REG_DATA_A, gDbgLedDataWord.DbgLedWord.bit_13);
//    output_bit(DO_DBG_SHIFT_REG_CLOCK, 0);
//    output_bit(DO_DBG_SHIFT_REG_CLOCK, 1);
//    /* Shift in 2nd QE */
//    output_bit(DO_DBG_SHIFT_REG_DATA_A, gDbgLedDataWord.DbgLedWord.bit_12);
//    output_bit(DO_DBG_SHIFT_REG_CLOCK, 0);
//    output_bit(DO_DBG_SHIFT_REG_CLOCK, 1);
//    /* Shift in 2nd QD */
//    output_bit(DO_DBG_SHIFT_REG_DATA_A, gDbgLedDataWord.DbgLedWord.bit_11);
//    output_bit(DO_DBG_SHIFT_REG_CLOCK, 0);
//    output_bit(DO_DBG_SHIFT_REG_CLOCK, 1);
//    /* Shift in 2nd QC */
//    output_bit(DO_DBG_SHIFT_REG_DATA_A, gDbgLedDataWord.DbgLedWord.bit_10);
//    output_bit(DO_DBG_SHIFT_REG_CLOCK, 0);
//    output_bit(DO_DBG_SHIFT_REG_CLOCK, 1);
//    /* Shift in 2nd QB */
//    output_bit(DO_DBG_SHIFT_REG_DATA_A, gDbgLedDataWord.DbgLedWord.bit_9);
//    output_bit(DO_DBG_SHIFT_REG_CLOCK, 0);
//    output_bit(DO_DBG_SHIFT_REG_CLOCK, 1);
//    /* Shift in 2nd QA */
//    output_bit(DO_DBG_SHIFT_REG_DATA_A, gDbgLedDataWord.DbgLedWord.bit_8);
//    output_bit(DO_DBG_SHIFT_REG_CLOCK, 0);
//    output_bit(DO_DBG_SHIFT_REG_CLOCK, 1);
//
//    /* Shift in 1st QH */
//    output_bit(DO_DBG_SHIFT_REG_DATA_A, gDbgLedDataWord.DbgLedWord.bit_7);
//    output_bit(DO_DBG_SHIFT_REG_CLOCK, 0);
//    output_bit(DO_DBG_SHIFT_REG_CLOCK, 1);
//    /* Shift in 1st QG */
//    output_bit(DO_DBG_SHIFT_REG_DATA_A, gDbgLedDataWord.DbgLedWord.bit_6);
//    output_bit(DO_DBG_SHIFT_REG_CLOCK, 0);
//    output_bit(DO_DBG_SHIFT_REG_CLOCK, 1);
//    /* Shift in 1st QF */
//    output_bit(DO_DBG_SHIFT_REG_DATA_A, gDbgLedDataWord.DbgLedWord.bit_5);
//    output_bit(DO_DBG_SHIFT_REG_CLOCK, 0);
//    output_bit(DO_DBG_SHIFT_REG_CLOCK, 1);
//    /* Shift in 1st QE */
//    output_bit(DO_DBG_SHIFT_REG_DATA_A, gDbgLedDataWord.DbgLedWord.bit_4);
//    output_bit(DO_DBG_SHIFT_REG_CLOCK, 0);
//    output_bit(DO_DBG_SHIFT_REG_CLOCK, 1);
//    /* Shift in 1st QD */
//    output_bit(DO_DBG_SHIFT_REG_DATA_A, gDbgLedDataWord.DbgLedWord.bit_3);
//    output_bit(DO_DBG_SHIFT_REG_CLOCK, 0);
//    output_bit(DO_DBG_SHIFT_REG_CLOCK, 1);
//    /* Shift in 1st QC */
//    output_bit(DO_DBG_SHIFT_REG_DATA_A, gDbgLedDataWord.DbgLedWord.bit_2);
//    output_bit(DO_DBG_SHIFT_REG_CLOCK, 0);
//    output_bit(DO_DBG_SHIFT_REG_CLOCK, 1);
//    /* Shift in 1st QB */
//    output_bit(DO_DBG_SHIFT_REG_DATA_A, gDbgLedDataWord.DbgLedWord.bit_1);
//    output_bit(DO_DBG_SHIFT_REG_CLOCK, 0);
//    output_bit(DO_DBG_SHIFT_REG_CLOCK, 1);
//    /* Shift in 1st QA */
//    output_bit(DO_DBG_SHIFT_REG_DATA_A, gDbgLedDataWord.DbgLedWord.bit_0);
//    output_bit(DO_DBG_SHIFT_REG_CLOCK, 0);
//    output_bit(DO_DBG_SHIFT_REG_CLOCK, 1);
//
//    /* Update previous LED data */ 
//    gPreviousDbgLedDataWord.gDbgLedWord = gDbgLedDataWord.gDbgLedWord;
//  }
//  else
//  {
//    /* Do nothing */
//  }
//}
//
//typedef struct _ST_BYTE_TYPE
//{
//    unsigned int8 ui8Bit_00:1;
//    unsigned int8 ui8Bit_01:1;
//    unsigned int8 ui8Bit_02:1;
//    unsigned int8 ui8Bit_03:1;
//    unsigned int8 ui8Bit_04:1;
//    unsigned int8 ui8Bit_05:1;
//    unsigned int8 ui8Bit_06:1;
//    unsigned int8 ui8Bit_07:1;
//} ST_BYTE_TYPE;
//
//typedef union _UN_BYTE_TYPE
//{
//    unsigned int8 m_uiByte;
//    ST_BYTE_TYPE  m_stByte;
//} UN_BYTE_TYPE;
//
//UN_BYTE_TYPE unByte;
//
//void Reset()
//{
//    unsigned int8 ui8LoopCount = 0;
//    
//    for(ui8LoopCount=0; ui8LoopCount<24; ui8LoopCount++)
//    {
//        output_bit(DO_DBG_SHIFT_REG_DATA_A, 1);
//        output_bit(DO_DBG_SHIFT_REG_CLOCK, 0);
//        output_bit(DO_DBG_SHIFT_REG_CLOCK, 1);
//    }
//}
//
////###################################### Above Driver ###################################################################
//
////###################################### Below Non Driver ###############################################################
//#define LED_PROD_MODE               24
//#define LED_CALIB_MODE              25
//#define LED_BOX_PASSED              26
//#define LED_BOX_REJECTED            27
//#define LED_PC_COMMUNICATION_RX     28
//#define LED_PC_COMMUNICATION_TX     29
//#define LED_LDCD_COMMUNICATION_RX   30
//#define LED_LDCD_COMMUNICATION_TX   31
//
//
//#define LED_B2_NON_PROD_MODE   6
//#define LED_B3_PROD_MODE       7
//#define LED_B3_NON_PROD_MODE   8
//#define LED_S0_PROD_MODE       9
//#define LED_S0_NON_PROD_MODE  10
//#define LED_S1_PROD_MODE      11
//#define LED_S1_NON_PROD_MODE  12
//#define LED_S2_PROD_MODE      13
//#define LED_S2_NON_PROD_MODE  14
//#define LED_BIG_CAN_SMALL_LID 15
//#define LED_BIG_CAN_BIG_LID   16
//#define LED_SML_CAN_SMALL_LID 17
////#define LED_CTRL_ON           18
//#define LED_E_STOP_ON          21
//
//void SetLEDsOnPowerOn()
//{
//    SetSystemLedState(LED_PROD_MODE,LOGICAL_OFF);
//    SetSystemLedState(LED_CALIB_MODE,LOGICAL_OFF);
//    SetSystemLedState(LED_BOX_PASSED,LOGICAL_OFF);
//    SetSystemLedState(LED_BOX_REJECTED,LOGICAL_OFF);
//    SetSystemLedState(LED_PC_COMMUNICATION_RX,LOGICAL_OFF);
//    SetSystemLedState(LED_PC_COMMUNICATION_TX,LOGICAL_OFF);
//    SetSystemLedState(LED_LDCD_COMMUNICATION_RX,LOGICAL_OFF);
//    SetSystemLedState(LED_LDCD_COMMUNICATION_TX,LOGICAL_OFF);
//    
////    SetSystemLedState(LED_PROD_MODE,LOGICAL_ON);
////    SetSystemLedState(LED_CALIB_MODE,LOGICAL_ON);
////    SetSystemLedState(LED_BOX_PASSED,LOGICAL_ON);
////    SetSystemLedState(LED_BOX_REJECTED,LOGICAL_ON);
////    SetSystemLedState(LED_PC_COMMUNICATION_RX,LOGICAL_ON);
////    SetSystemLedState(LED_PC_COMMUNICATION_TX,LOGICAL_ON);
////    SetSystemLedState(LED_LDCD_COMMUNICATION_RX,LOGICAL_ON);
////    SetSystemLedState(LED_LDCD_COMMUNICATION_TX,LOGICAL_ON);
//}    
//
//
//#endif /* _VI_SHIFT_REG_H_ */