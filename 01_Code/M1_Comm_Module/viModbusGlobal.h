#ifndef _VI_MODBUS_GOBAL_H_
#define _VI_MODBUS_GOBAL_H_

#define MODBUS_SERIAL_RX_BUFFER_SIZE 64  
#define MODBUS_REG_BUFFER_SIZE 10   
 
#define MODBUS_REG_OUTPUT_0  	0 
#define MODBUS_REG_OUTPUT_1		1
#define MODBUS_REG_OUTPUT_2		2
#define MODBUS_REG_OUTPUT_3		4
#define MODBUS_REG_OUTPUT_4		8
#define MODBUS_REG_OUTPUT_5		16
#define MODBUS_REG_OUTPUT_6		32
#define MODBUS_REG_OUTPUT_7		64
#define MODBUS_REG_OUTPUT_8		128
#define MODBUS_REG_OUTPUT_9		256
#define MODBUS_REG_OUTPUT_10	512
#define MODBUS_REG_OUTPUT_11	1024
#define MODBUS_REG_OUTPUT_12	2048
#define MODBUS_REG_OUTPUT_13	4096
#define MODBUS_REG_OUTPUT_14	8192
#define MODBUS_REG_OUTPUT_15	16384
#define MODBUS_REG_OUTPUT_16	32768


enum function {
	RS485_FUNCTION_READ_COILS                 = 1,
	RS485_FUNCTION_READ_DISCRETE_INPUT        = 2,
	RS485_FUNCTION_READ_HOLDING_REGISTERS     = 3,
	RS485_FUNCTION_READ_INPUT_REGISTERS       = 4,
	RS485_FUNCTION_WRITE_SINGLE_COIL          = 5,
	RS485_FUNCTION_WRITE_SINGLE_REGISTER      = 6,
	RS485_FUNCTION_READ_MULTIPLE_REGISTERS    = 15,
	RS485_FUNCTION_WRITE_MULTIPLE_REGISTERS   = 16		
};

enum exception {
	RS485_ILLEGAL_FUNCTION        = 1,	
	RS485_ILLEGAL_DATA_ADDRESS    = 2,
	RS485_ILLEGAL_DATA_VALUE      = 3,
	RS485_SLAVE_DEVICE_FAILED     = 4,
	RS485_ACKNOWLAGE              = 5,
	RS485_SLAVE_DEVICE_BUSY       = 6,
	RS485_NEG_ACKNOWLAGE          = 7,
	RS485_MEMORY_PARITY_ERROR     = 8,
	RS485_GETWAY_PATH             = 10,
	RS485_GETWAY_DEVICE           = 11	
};

typedef struct
{ 
   unsigned int8    ui8Slaveid; 
   unsigned int16   u16Address;
   unsigned int16   u16LenData;                       //number of bytes in the message received 
   function         func;                           //the function of the message received 
   exception        error;                         //error recieved, if any 
   unsigned int16   ui16RegisterData;
//   unsigned int8    ui8Data[MODBUS_SERIAL_RX_BUFFER_SIZE]; //data of the message received 
   BOOLEAN          bDataProcessed;
   unsigned int8    ui8NoRegDataBytes;
   unsigned int16   ui16RegisterDataArr[MODBUS_REG_BUFFER_SIZE];
   unsigned int32   u32RegData;
} Modbus_Rx; 

Modbus_Rx g_ModbusRx;

unsigned int16 g_ui16RegisterForCount;
unsigned int16 g_ui16RegisterForCounter1_1;
unsigned int16 g_ui16RegisterForCounter1_2;
unsigned int16 g_ui16RegisterForCounter2_1;
unsigned int16 g_ui16RegisterForCounter2_2;
unsigned int16 g_ui16RegisterForCounter3_1;
unsigned int16 g_ui16RegisterForCounter3_2;

unsigned int16 g_ui16Register0_1;
unsigned int16 g_ui16Register0_2;
unsigned int16 g_ui16Register1000_1;
unsigned int16 g_ui16Register1000_2;
unsigned int16 g_ui16Register2000_1;
unsigned int16 g_ui16Register2000_2;
unsigned int16 g_ui16Register3000_1;
unsigned int16 g_ui16Register3000_2;
unsigned int16 g_ui16Register4000_1;
unsigned int16 g_ui16Register4000_2;
unsigned int16 g_ui16Register5000_1;
unsigned int16 g_ui16Register5000_2;
unsigned int16 g_ui16Register6000_1;
unsigned int16 g_ui16Register6000_2;
unsigned int16 g_ui16Register7000_1;
unsigned int16 g_ui16Register7000_2;
unsigned int16 g_ui16Register8000_1;
unsigned int16 g_ui16Register8000_2;
unsigned int16 g_ui16Register9000_1;
unsigned int16 g_ui16Register9000_2;
unsigned int16 g_ui16Register10000_1;
unsigned int16 g_ui16Register10000_2;

unsigned int16 g_ui16RegisterAvgWeight;
unsigned int16 g_ui16RegisterTolPv;
unsigned int16 g_ui16RegisterTolNv;
unsigned int16 g_ui16RegisterCurrProductIndex;

#endif /* _VI_MODBUS_GOBAL_H_ */

