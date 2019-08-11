#ifndef _VM_DIGITAL_IO_PORT_PIN_CONFIG_H_
#define _VM_DIGITAL_IO_PORT_PIN_CONFIG_H_

//#include "18F46K80.h"

// For setting of TRIS resister.
// Set bit = 1 - makes port as Digital Input.
#define SET_TO_1_TRIS_BIT0     0x01 // 0000 0001 binary.
#define SET_TO_1_TRIS_BIT1     0x02 // 0000 0010 binary.    
#define SET_TO_1_TRIS_BIT2     0x04 // 0000 0100 binary.      
#define SET_TO_1_TRIS_BIT3     0x08 // 0000 1000 binary.      
#define SET_TO_1_TRIS_BIT4     0x10 // 0001 0000 binary.     
#define SET_TO_1_TRIS_BIT5     0x20 // 0010 0000 binary.      
#define SET_TO_1_TRIS_BIT6     0x40 // 0100 0000 binary.      
#define SET_TO_1_TRIS_BIT7     0x80 // 1000 0000 binary.      
 
// For setting of TRIS resister.
// Reset bit = 0 - makes port as Digital Output.
#define RESET_TO_0_TRIS_BIT0   0xFE // 1111 1110 binary.
#define RESET_TO_0_TRIS_BIT1   0xFD // 1111 1101 binary.
#define RESET_TO_0_TRIS_BIT2   0xFB // 1111 1011 binary.
#define RESET_TO_0_TRIS_BIT3   0xF7 // 1111 0111 binary.
#define RESET_TO_0_TRIS_BIT4   0xEF // 1110 1111 binary.
#define RESET_TO_0_TRIS_BIT5   0xDF // 1101 1111 binary.
#define RESET_TO_0_TRIS_BIT6   0xBF // 1011 1111 binary.
#define RESET_TO_0_TRIS_BIT7   0x7F // 0111 1111 binary.

#define SET_PORT_PIN_AS_DIGITAL_INPUT(portName,bitNumber)  \
   (set_tris_##portName##(get_tris_##portName##() |  SET_TO_1_TRIS_BIT##bitNumber))

#define SET_PORT_PIN_AS_DIGITAL_OUTPUT(portName,bitNumber) \
   (set_tris_##portName##(get_tris_##portName##() &  RESET_TO_0_TRIS_BIT##bitNumber))


void SetDigitalInputPortPinsOfMicro(void)
{
    SET_PORT_PIN_AS_DIGITAL_INPUT(A,0);
    SET_PORT_PIN_AS_DIGITAL_INPUT(A,1);
    SET_PORT_PIN_AS_DIGITAL_INPUT(A,2);
    SET_PORT_PIN_AS_DIGITAL_INPUT(A,3);
   // SET_PORT_PIN_AS_DIGITAL_INPUT(B,1);
    SET_PORT_PIN_AS_DIGITAL_INPUT(A,5);
    SET_PORT_PIN_AS_DIGITAL_INPUT(E,0);
    SET_PORT_PIN_AS_DIGITAL_INPUT(E,1);
    SET_PORT_PIN_AS_DIGITAL_INPUT(E,2);
   // SET_PORT_PIN_AS_DIGITAL_INPUT(A,6);
    SET_PORT_PIN_AS_DIGITAL_INPUT(C,0);
    SET_PORT_PIN_AS_DIGITAL_INPUT(C,1);
    SET_PORT_PIN_AS_DIGITAL_INPUT(C,2);
    SET_PORT_PIN_AS_DIGITAL_INPUT(C,3);
    SET_PORT_PIN_AS_DIGITAL_INPUT(D,0);
    SET_PORT_PIN_AS_DIGITAL_INPUT(D,1);
}

void SetDigitalOutputPortPinsOfMicro(void)
{
    SET_PORT_PIN_AS_DIGITAL_OUTPUT(D,2);
    SET_PORT_PIN_AS_DIGITAL_OUTPUT(D,3);
    SET_PORT_PIN_AS_DIGITAL_OUTPUT(C,4);
    SET_PORT_PIN_AS_DIGITAL_OUTPUT(C,5);
   // SET_PORT_PIN_AS_DIGITAL_OUTPUT(D,4);
   // SET_PORT_PIN_AS_DIGITAL_OUTPUT(D,5);
}

void SetShiftRegisterPortPinsOfMicro(void)
{
    SET_PORT_PIN_AS_DIGITAL_OUTPUT(B,4);
    SET_PORT_PIN_AS_DIGITAL_OUTPUT(B,5);
}

void SetRS485PortPinOfMicro(void)
{
    SET_PORT_PIN_AS_DIGITAL_OUTPUT(C,6);
    SET_PORT_PIN_AS_DIGITAL_INPUT(C,7);
    //SET_PORT_PIN_AS_DIGITAL_OUTPUT(B,6);
    SET_PORT_PIN_AS_DIGITAL_OUTPUT(B,4);
}

#endif /* _VM_DIGITAL_IO_PORT_PIN_CONFIG_H_ */
 