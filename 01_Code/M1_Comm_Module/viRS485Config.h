#ifndef _VI_RS485_CONFIG_H_
#define _VI_RS485_CONFIG_H_

#include "vmCRCConfig.h"
#include "viModbusGlobal.h"
#include "vmIBCGlobal.h"
#include "vmTimerConfig.h"
//#include "vmHMIBoardModbusSetup.h"

#define RS485_TX_BUFF_SIZE 		20
#define RS485_RX_BUFF_SIZE 		50

//#define PIN_HIGH     			0
//#define PIN_LOW      			1
#define MODBUS_ADDRESS_FOR_SET_AVG_CALIBRATION_POINT        0x296
#define SLAVE_ID                                            0x1

#define MODE_ADDRESS                            0x05
#define ADDRESS_COUNTER_SEND_DATA               0x04

#define ADDRESS_FOR_SET_CALIB_0                 0x272
#define ADDRESS_FOR_SET_CALIB_1000              0x274
#define ADDRESS_FOR_SET_CALIB_2000              0x276
#define ADDRESS_FOR_SET_CALIB_3000              0x278
#define ADDRESS_FOR_SET_CALIB_4000              0x280
#define ADDRESS_FOR_SET_CALIB_5000              0x282
#define ADDRESS_FOR_SET_CALIB_6000              0x284
#define ADDRESS_FOR_SET_CALIB_7000              0x286
#define ADDRESS_FOR_SET_CALIB_8000              0x288
#define ADDRESS_FOR_SET_CALIB_9000              0x290
#define ADDRESS_FOR_SET_CALIB_10000             0x292
#define ADDRESS_FOR_SET_CALIB_DATA              0x296

#define ADDRESS_FOR_SET_CONFIG_AVG_WT           0x250
#define ADDRESS_FOR_SET_CONFIG_TOL_PV           0x252
#define ADDRESS_FOR_SET_CONFIG_TOL_NV           0x254
#define ADDRESS_FOR_SET_CONFIG_INDEX            0x256

#define DELAY_TIME_RTS_AFTER                        2
#define DELAY_TIME_RTS_BEFORE                       2


////////////////////////////////////////////////////////////////////////////////
#define SIZE_TEMP_ARR_CRC_VALID                        15
#define MSG_QUEUE_EMPTY                                 0
#define MSG_QUEUE_NOT_EMPTY                             1
#define MODBUS_MASTER_QUERY_LENGTH_MIN                  8
#define MODBUS_MASTER_QUERY_LENGTH_MIX                 12
#define MODBUS_MASTER_QUERY_READ_REG_LENGTH             8
#define MODBUS_MASTER_QUERY_SINGLE_WRITE_REG_LENGTH     8

#define MODBUS_OFFSET_ONE       1
#define MODBUS_OFFSET_TWO       2
#define MODBUS_OFFSET_THREE     3
#define MODBUS_OFFSET_FOUR      4
#define MODBUS_OFFSET_FIVE      5
#define MODBUS_OFFSET_SIX       6
#define MODBUS_OFFSET_SEVEN     7
#define MODBUS_OFFSET_8         8
#define MODBUS_OFFSET_9         9
#define MODBUS_OFFSET_10        10
#define MODBUS_OFFSET_11        11
#define MODBUS_OFFSET_12        12
#define MODBUS_OFFSET_13        13
#define MODBUS_OFFSET_14        14
#define MODBUS_OFFSET_15        15
#define MODBUS_OFFSET_16        16

typedef struct _ST_ISR_QUEUE_MEMBERS
{
    unsigned int8 m_ui8Arr[RS485_RX_BUFF_SIZE];
    int8 m_i8Front;
    int8 m_i8Rear;
    
}ST_ISR_QUEUE_MEMBERS;

ST_ISR_QUEUE_MEMBERS g_stUART2RcvBuffModbus;
unsigned int8 g_ui8TempByte = 0;
unsigned int8 g_ui8TempArrForCRCValidation[SIZE_TEMP_ARR_CRC_VALID] = {0};

////////////////////////////////////////////////////////////////////////////////

#define REG_DATA_LO_TO_HI                       1 // 1 = lo byte first, 2 = lo byte last

unsigned int8 g_RS485TxBuffer[RS485_TX_BUFF_SIZE] = {0};
unsigned int8 g_RS485RxBuffer[RS485_RX_BUFF_SIZE] = {0};
unsigned int8 g_ui8RS485RxByteCount               = 0;
unsigned int8 g_ui8ReceivedModbusResponse         = 0;
unsigned int8 g_ui8RS485MaxByteCount              = 8;
unsigned int8 g_ui8RS485Done              		  = 0;
unsigned int8 g_uiInputOutputMatch				  = 0;

unsigned int8 g_ui8Front                          = 0;
unsigned int8 g_ui8End                            = 0;





// init interept
void InitRS485RxInterrupt(void)
{
    // Enable RS485 interrupt.
    //clear_interrupt(INT_RDA);
	//enable_interrupts(INT_RDA);
    
    // Initialize Queue
    g_stUART2RcvBuffModbus.m_i8Front = g_stUART2RcvBuffModbus.m_i8Rear = -1;
    memset(g_stUART2RcvBuffModbus.m_ui8Arr, 0, RS485_RX_BUFF_SIZE);

    clear_interrupt(INT_RDA2);
	enable_interrupts(INT_RDA2);
}

void Delay_milisecond(unsigned int16 ui16DelayTime)
{
	unsigned int16 ui16Counter = 0;
	while(ui16Counter < ui16DelayTime)
	{
		delay_ms(1);
		ui16Counter++;
	}
}

// reset buffer
void ResetRS485RxBuffer(void)
{
    // Reset Rx Byte count.
  //  g_ui8RS485RxByteCount       = 0;
    // Set 0 to Rx Buffer.
 //   memset(g_RS485RxBuffer, 0, RS485_RX_BUFF_SIZE);
    g_ui8ReceivedModbusResponse--;
    if(g_ui8ReceivedModbusResponse < 0)
    {
        g_ui8ReceivedModbusResponse = 0;
    }
    g_ui8RS485MaxByteCount = 8;
    
    if((g_ui8Front == g_ui8End) && (g_ui8Front!=0))
    {
        g_ui8Front = 0;
        g_ui8End = 0;
        g_ui8RS485RxByteCount = 0;
    }
    
    // Check if Rx Byte count is equal to the Rx Buffer capacity.
    if(g_ui8RS485RxByteCount >= RS485_RX_BUFF_SIZE - 15) //g_ui8RS485MaxByteCount)
    {
        // Reset Rx Byte count to prevent buffer overflow.
    //    g_ui8Front = 0;
        g_ui8End = 0;
        g_ui8RS485RxByteCount = g_ui8End; 
    }

    // Reset holding buffer
    g_ui8RS485Done = 0;
   	clear_interrupt(INT_RDA2);
}

void SetHILowRS485()
{	
  	//set transmit active.
	output_bit(PIN_RS485_RTS,1);
	delay_ms(4);
	
	//set transmit inactive.
	output_bit(PIN_RS485_RTS,0);
	delay_ms(4);
}

void TestPollRS485(
                   unsigned int8 ui8SlaveID, 
                   unsigned int8 ui8FunctionType, 
                   unsigned int16 ui16StartAddress,
                   unsigned int16 ui16RegisterData)
{
	// function for send data to server/master
	unsigned int16 ui16CRCvalue = 0;
	
	//value assign in transmit buffer of RS485
  	g_RS485TxBuffer[0]  = ui8SlaveID;
  	g_RS485TxBuffer[1]  = ui8FunctionType; //3;//Function code;read holding register. 3 for read, 6 for write
  	g_RS485TxBuffer[2]  = (unsigned int8)((ui16StartAddress >> 8) & 0xFF);
  	g_RS485TxBuffer[3]  = (unsigned int8)((ui16StartAddress >> 0) & 0xFF); // Address of the register.

  	g_RS485TxBuffer[4]  = (unsigned int8)((ui16RegisterData >> 8) & 0xFF);
  	g_RS485TxBuffer[5]  = (unsigned int8)((ui16RegisterData >> 0) & 0xFF);
   	g_RS485TxBuffer[6]  = 0;
  	g_RS485TxBuffer[7]  = 0;
  	
	//Calculate CRC values using function call. 
  	ui16CRCvalue = calculateCrc(g_RS485TxBuffer,6); 
  	
	//assign CRC values in transmit buffer.
  	g_RS485TxBuffer[6]  = (unsigned int8)((ui16CRCvalue >> 0) & 0xFF);
  	g_RS485TxBuffer[7]  = (unsigned int8)((ui16CRCvalue >> 8) & 0xFF);
  	
  	//set transmit active.
  	delay_ms(DELAY_TIME_RTS_BEFORE);
	output_bit(PIN_RS485_RTS,1);
	delay_ms(DELAY_TIME_RTS_AFTER);
	//send bytes.
	for(unsigned int8 ui8SendBytes = 0; ui8SendBytes<8; ui8SendBytes++)
	{
		fputc(g_RS485TxBuffer[ui8SendBytes],RS485);
	}
	//hold to bytes properly transmit.
	delay_ms(DELAY_TIME_RTS_AFTER);
	//set transmit inactive.
	output_bit(PIN_RS485_RTS,0);
//	delay_ms(DELAY_TIME_RTS_AFTER);
}

// wirte to register
void WriteToOneRegRS485(
                        unsigned int8 ui8SlaveID, 
                        unsigned int8 ui8FunctionType, 
                        unsigned int8 ui8Len,
                        unsigned int16 ui16RegisterData)
{
	unsigned int16 ui16CRCvalue = 0;
	
	//value assign in transmit buffer of RS485
  	g_RS485TxBuffer[0]  = ui8SlaveID;  //37;
  	g_RS485TxBuffer[1]  = ui8FunctionType;  //3;
  	g_RS485TxBuffer[2]  = ui8Len;  //2;

  	g_RS485TxBuffer[3]  = (unsigned int8)((ui16RegisterData >> 8) & 0xFF);
  	g_RS485TxBuffer[4]  = (unsigned int8)((ui16RegisterData >> 0) & 0xFF);
  	g_RS485TxBuffer[5]  = 0;
   	g_RS485TxBuffer[6]  = 0;
  	g_RS485TxBuffer[7]  = 0;
  	
	//Calculate CRC values using function call. 
  	ui16CRCvalue = calculateCrc(g_RS485TxBuffer,5); 
  	
	//assign CRC values in transmit buffer.
  	g_RS485TxBuffer[5]  = (unsigned int8)((ui16CRCvalue >> 0) & 0xFF);
  	g_RS485TxBuffer[6]  = (unsigned int8)((ui16CRCvalue >> 8) & 0xFF);
  	
  	//set transmit active.
  	delay_ms(DELAY_TIME_RTS_BEFORE);
	output_bit(PIN_RS485_RTS,1);
	delay_ms(DELAY_TIME_RTS_AFTER);
	//send bytes.
	for(unsigned int8 ui8SendBytes = 0; ui8SendBytes<7; ui8SendBytes++)
	{
		fputc(g_RS485TxBuffer[ui8SendBytes],RS485);
	}
	//hold to bytes properly transmit.
	delay_ms(DELAY_TIME_RTS_AFTER);
	//set transmit inactive.
	output_bit(PIN_RS485_RTS,0);
//	delay_ms(DELAY_TIME_RTS_AFTER);
}

// write to a register
void WriteToTwoRegRS485(
                        unsigned int8 ui8SlaveID, 
                        unsigned int8 ui8FunctionType, 
                        unsigned int8 ui8Len,
                        unsigned int16 ui16RegisterData1, 
                        unsigned int16 ui16RegisterData2)
{
	unsigned int16 ui16CRCvalue = 0;
	
	//value assign in transmit buffer of RS485
  	g_RS485TxBuffer[0]  = ui8SlaveID;
  	g_RS485TxBuffer[1]  = ui8FunctionType;
  	g_RS485TxBuffer[2]  = ui8Len;

  	g_RS485TxBuffer[3]  = (unsigned int8)((ui16RegisterData1 >> 8) & 0xFF);
  	g_RS485TxBuffer[4]  = (unsigned int8)((ui16RegisterData1 >> 0) & 0xFF);
  	
  	g_RS485TxBuffer[5]  = (unsigned int8)((ui16RegisterData2 >> 8) & 0xFF);
  	g_RS485TxBuffer[6]  = (unsigned int8)((ui16RegisterData2 >> 0) & 0xFF);

   	g_RS485TxBuffer[7]  = 0;
  	g_RS485TxBuffer[8]  = 0;
  	
	//Calculate CRC values using function call. 
  	ui16CRCvalue        = calculateCrc(g_RS485TxBuffer,7); 
  	
	//assign CRC values in transmit buffer.
  	g_RS485TxBuffer[7]  = (unsigned int8)((ui16CRCvalue >> 0) & 0xFF);
  	g_RS485TxBuffer[8]  = (unsigned int8)((ui16CRCvalue >> 8) & 0xFF);
  	
  	//set transmit active.
  	delay_ms(DELAY_TIME_RTS_BEFORE);
	output_bit(PIN_RS485_RTS,1);
	delay_ms(DELAY_TIME_RTS_AFTER);
	//send bytes.
	for(unsigned int8 ui8SendBytes = 0; ui8SendBytes<9; ui8SendBytes++)
	{
		fputc(g_RS485TxBuffer[ui8SendBytes],RS485);
	}
	//hold to bytes properly transmit.
	delay_ms(DELAY_TIME_RTS_AFTER);
	//set transmit inactive.
	output_bit(PIN_RS485_RTS,0);
//	delay_ms(DELAY_TIME_RTS_AFTER);
}

// write to a register
void WriteToThreeRegRS485(
    unsigned int8  ui8SlaveID,
    unsigned int8  ui8FunctionType,
    unsigned int8  ui8Len,
    unsigned int16 ui16RegisterData1,
    unsigned int16 ui16RegisterData2,
    unsigned int16 ui16RegisterData3)
{
	unsigned int16 ui16CRCvalue = 0;
	
	//value assign in transmit buffer of RS485
  	g_RS485TxBuffer[0]  = ui8SlaveID;
  	g_RS485TxBuffer[1]  = ui8FunctionType;
  	g_RS485TxBuffer[2]  = ui8Len;

  	g_RS485TxBuffer[3]  = (unsigned int8)((ui16RegisterData1 >> 8) & 0x00FF);
  	g_RS485TxBuffer[4]  = (unsigned int8)((ui16RegisterData1 >> 0) & 0x00FF);
  	
  	g_RS485TxBuffer[5]  = (unsigned int8)((ui16RegisterData2 >> 8) & 0x00FF);
  	g_RS485TxBuffer[6]  = (unsigned int8)((ui16RegisterData2 >> 0) & 0x00FF);

   	g_RS485TxBuffer[7]  = (unsigned int8)((ui16RegisterData3 >> 8) & 0x00FF);
  	g_RS485TxBuffer[8]  = (unsigned int8)((ui16RegisterData3 >> 0) & 0x00FF);
  	
	//Calculate CRC values using function call. 
  	ui16CRCvalue        = calculateCrc(g_RS485TxBuffer,9); 
  	
	//assign CRC values in transmit buffer.
  	g_RS485TxBuffer[9]   = (unsigned int8)((ui16CRCvalue >> 0) & 0x00FF);
  	g_RS485TxBuffer[10]  = (unsigned int8)((ui16CRCvalue >> 8) & 0x00FF);
  	
  	//set transmit active.
  	delay_ms(DELAY_TIME_RTS_BEFORE);
	output_bit(PIN_RS485_RTS,1);
	delay_ms(DELAY_TIME_RTS_AFTER);
    delay_ms(3);
	//send bytes.
	for(unsigned int8 ui8SendBytes = 0; ui8SendBytes<11; ui8SendBytes++)
	{
		fputc(g_RS485TxBuffer[ui8SendBytes],RS485);
	}
	//hold to bytes properly transmit.
	delay_ms(DELAY_TIME_RTS_AFTER);
	//set transmit inactive.
	output_bit(PIN_RS485_RTS,0);
//	delay_ms(DELAY_TIME_RTS_AFTER);
}

// write to a register
void ReplyForTwoRegister(
                        unsigned int8 ui8SlaveID, 
                        unsigned int8 ui8FunctionType, 
                        unsigned int8 ui8Len,
                        unsigned int16 ui16RegisterAddress, 
                        unsigned int16 ui16RegisterData1, 
                        unsigned int16 ui16RegisterData2)
{
	unsigned int16 ui16CRCvalue = 0;
	
	//value assign in transmit buffer of RS485
  	g_RS485TxBuffer[0]  = ui8SlaveID;
  	g_RS485TxBuffer[1]  = ui8FunctionType;
  	g_RS485TxBuffer[2]  = (unsigned int8)((ui16RegisterAddress >> 8) & 0xFF);
  	g_RS485TxBuffer[3]  = (unsigned int8)((ui16RegisterAddress >> 0) & 0xFF);
  	g_RS485TxBuffer[4]  = (unsigned int8)((ui8Len >> 8) & 0xFF);
  	g_RS485TxBuffer[5]  = (unsigned int8)((ui8Len >> 0) & 0xFF);
    
  	g_RS485TxBuffer[6]  = 2 * ui8Len;

  	g_RS485TxBuffer[7]  = (unsigned int8)((ui16RegisterData1 >> 8) & 0xFF);
  	g_RS485TxBuffer[8]  = (unsigned int8)((ui16RegisterData1 >> 0) & 0xFF);
  	
  	g_RS485TxBuffer[9]  = (unsigned int8)((ui16RegisterData2 >> 8) & 0xFF);
  	g_RS485TxBuffer[10]  = (unsigned int8)((ui16RegisterData2 >> 0) & 0xFF);

   	g_RS485TxBuffer[11]  = 0;
  	g_RS485TxBuffer[12]  = 0;
  	
	//Calculate CRC values using function call. 
  	ui16CRCvalue        = calculateCrc(g_RS485TxBuffer,11); 
  	
	//assign CRC values in transmit buffer.
  	g_RS485TxBuffer[11]  = (unsigned int8)((ui16CRCvalue >> 0) & 0xFF);
  	g_RS485TxBuffer[12]  = (unsigned int8)((ui16CRCvalue >> 8) & 0xFF);
  	
  	//set transmit active.
  	delay_ms(DELAY_TIME_RTS_BEFORE);
	output_bit(PIN_RS485_RTS,1);
	delay_ms(DELAY_TIME_RTS_AFTER);
	//send bytes.
	for(unsigned int8 ui8SendBytes = 0; ui8SendBytes<13; ui8SendBytes++)
	{
		fputc(g_RS485TxBuffer[ui8SendBytes],RS485);
	}
	//hold to bytes properly transmit.
	delay_ms(DELAY_TIME_RTS_AFTER);
	//set transmit inactive.
	output_bit(PIN_RS485_RTS,0);
//	delay_ms(DELAY_TIME_RTS_AFTER);
}

// is bit is on a data
int8 IsBitOn(int8 uiBitIndex, int16 u16RegData)
{
	int8 uiRetVal   = 1;
	int8 uiMaxBits  = 15;
	
	int16 u16Data   = u16RegData << (uiMaxBits-uiBitIndex);
	
	u16Data         = u16Data >> uiMaxBits;
	
	if(u16Data == 1) 
		uiRetVal = 0;
	else 
		uiRetVal = 1;	
	return uiRetVal;
}

// set bit on in a data
int32 SetBitOn(int8 uiBitIndex, int32 u32RegData)
{
	int32 uiRetVal = u32RegData;
	int8 uiMaxBits = 31;
	
	if(uiBitIndex > uiMaxBits) 
    {
		return uiRetVal;
	}
	uiRetVal = uiRetVal | (1 << uiBitIndex);
	
	return uiRetVal;
}

// dlear a bit from data
int32 ClearBitOn(int8 uiBitIndex, int32 u32RegData)
{
	int32 uiRetVal = u32RegData;
	int8 uiMaxBits = 31;
	
	if(uiBitIndex > uiMaxBits) {
		return uiRetVal;
	}
	uiRetVal = uiRetVal & ~(1 << uiBitIndex);
	
	return uiRetVal;
}

// toggle bit on
int32 ToggleBitOn(int8 uiBitIndex, int32 u32RegData)
{
	int32 uiRetVal = u32RegData;
	int8 uiMaxBits = 31;
	
	if(uiBitIndex > uiMaxBits) {
		return uiRetVal;
	}
	uiRetVal = uiRetVal ^ (1 << uiBitIndex);
	
	return uiRetVal;
}

//// wait for movement completion
//int8 WaitForMovementCompletion()
//{
//	int8 uiRetVal = 0;
//	if((SLAVE_ID != SLAVE_ID_FOR_LA_1) && (SLAVE_ID != SLAVE_ID_FOR_LA_2))
//	{
//		return;
//	}
//		
//	delay_ms(1000);
//	int8 uiDIVal7   = 1;
//	
//	int8 uiIndex    = 0;
//	
//	while(1)
//	{
//		uiIndex++;
//		uiDIVal7 = input(PIN_IN_DI_06); 
//		if(uiDIVal7 == 0)
//		{
//			uiRetVal = 1;
//			break;
//		}
//		if(uiIndex == 100) 
//		{
//			break;
//		}
//	}
//	
//	// if ok then return 1
//	return uiRetVal;
//}


void PollRS485(
                unsigned int8 uiFunctionCode, 
                unsigned int16 ui16StartAddress, 
                unsigned int16 ui16FirstRegisterData, 
                unsigned int16 ui16SecondregisterData)
{
	// function for polling data
	unsigned int16 ui16CRCvalue = 0;
	
	//Reset the receiver buffer.
	ResetRS485RxBuffer();
	
	g_RS485TxBuffer[0]  = SLAVE_ID;
	g_RS485TxBuffer[1]  = uiFunctionCode;// Function code.
	g_RS485TxBuffer[2]  = ((ui16StartAddress >> 8)& 0xFF);
	g_RS485TxBuffer[3]  = ((ui16StartAddress >> 0)& 0xFF);
	g_RS485TxBuffer[4]  = 0x00;
	g_RS485TxBuffer[5]  = 0x02;// Number of registers to write.
	g_RS485TxBuffer[6]  = 0x04;// Number of bytes to write. 
	g_RS485TxBuffer[7]  = (unsigned int8)((ui16FirstRegisterData >> 8)& 0xFF);
	g_RS485TxBuffer[8]  = (unsigned int8)((ui16FirstRegisterData >> 0)& 0xFF);// Data of the first register.
	g_RS485TxBuffer[9]  = (unsigned int8)((ui16SecondregisterData >> 8)& 0xFF);
	g_RS485TxBuffer[10] = (unsigned int8)((ui16SecondregisterData >> 0)& 0xFF);// Data of the second register. 
	g_RS485TxBuffer[11] = 0;
	g_RS485TxBuffer[12] = 0;
	
	//Function call CRC calculation.
	ui16CRCvalue = ModbusCRC16(g_RS485TxBuffer,11);
	
	g_RS485TxBuffer[11] = (unsigned int8)((ui16CRCvalue >> 0)& 0xFF);
	g_RS485TxBuffer[12] = (unsigned int8)((ui16CRCvalue >> 8)& 0xFF);

	//set transmit active.
	delay_ms(2);
	output_bit(PIN_RS485_RTS,1);
	delay_ms(3);
	//send bytes.
	for(unsigned int8 ui8SendBytes = 0; ui8SendBytes<13; ui8SendBytes++)
	{
		fputc(g_RS485TxBuffer[ui8SendBytes],RS485);
	}
	//hold to bytes properly transmit.
	delay_ms(3);
	//set transmit inactive.
	output_bit(PIN_RS485_RTS,0);
	delay_ms(3);
}

////=============================== START NEW CODE ======================================================
#INT_RDA2
void RS485_RxByteReady_ISR2(void)
{
    
    // Checking for Is ISR ready flag.
    if(ISR_IS_NOT_READY == g_fIsReadyToReceiveByte)
    {
        g_fIsReadyToReceiveByte = ISR_IS_READY;
    }
    
    g_ui8TempByte = fgetc(RS485);
    
    // Checking queue boundary conditions.
    if(g_stUART2RcvBuffModbus.m_i8Front == -1 && g_stUART2RcvBuffModbus.m_i8Rear == -1)
    {
        // Initialize queue front and rear.
        g_stUART2RcvBuffModbus.m_i8Front = g_stUART2RcvBuffModbus.m_i8Rear = 0;
        
        // Inserting first element.
        g_stUART2RcvBuffModbus.m_ui8Arr[g_stUART2RcvBuffModbus.m_i8Rear] = g_ui8TempByte;
    } 
    else if((RS485_RX_BUFF_SIZE - 1) == g_stUART2RcvBuffModbus.m_i8Rear && g_stUART2RcvBuffModbus.m_i8Front > 0)
    {
        // Rear reached last position..initialize it.
        g_stUART2RcvBuffModbus.m_i8Rear = 0;
        // Then insert data.
        g_stUART2RcvBuffModbus.m_ui8Arr[g_stUART2RcvBuffModbus.m_i8Rear] = g_ui8TempByte;
    }
    else if(((RS485_RX_BUFF_SIZE - 1) == g_stUART2RcvBuffModbus.m_i8Rear && g_stUART2RcvBuffModbus.m_i8Front == 0) ||
            (g_stUART2RcvBuffModbus.m_i8Rear == g_stUART2RcvBuffModbus.m_i8Front - 1))
    {
        // Queue is full
        //TODO Handle This
    }
    else 
    {
        g_stUART2RcvBuffModbus.m_i8Rear++;
        g_stUART2RcvBuffModbus.m_ui8Arr[g_stUART2RcvBuffModbus.m_i8Rear] = g_ui8TempByte;
    }
    
	// Clear interrupt.
	clear_interrupt(INT_RDA2);
}

BYTE GetUART2Queue(BYTE* pbIsQueueEmpty )
{
    BYTE bData = 0;
    
    *pbIsQueueEmpty = MSG_QUEUE_NOT_EMPTY;

    if (-1 == g_stUART2RcvBuffModbus.m_i8Front)
    {
        // Queue is empty
        *pbIsQueueEmpty = MSG_QUEUE_EMPTY;
        return 0;
    }
    
    bData = g_stUART2RcvBuffModbus.m_ui8Arr[g_stUART2RcvBuffModbus.m_i8Front];
    g_stUART2RcvBuffModbus.m_ui8Arr[g_stUART2RcvBuffModbus.m_i8Front] = 0;  // Fill the Queue by 0
    
    if (g_stUART2RcvBuffModbus.m_i8Front == g_stUART2RcvBuffModbus.m_i8Rear)
    {
        g_stUART2RcvBuffModbus.m_i8Front = g_stUART2RcvBuffModbus.m_i8Rear = -1;
    }
    else if ((RS485_RX_BUFF_SIZE - 1) == g_stUART2RcvBuffModbus.m_i8Front)
    {
        g_stUART2RcvBuffModbus.m_i8Front = 0;
    }
    else
    {
        g_stUART2RcvBuffModbus.m_i8Front++;
    }
    
    // Return Data
    return bData;
}

int8 GetQueueLength()
{
    // Check if Front is less then rear, Normal case
    if (g_stUART2RcvBuffModbus.m_i8Front < g_stUART2RcvBuffModbus.m_i8Rear)
    {
        return (g_stUART2RcvBuffModbus.m_i8Rear - g_stUART2RcvBuffModbus.m_i8Front + 1);
    }
    // Else if the Rear precedes front
    else if (g_stUART2RcvBuffModbus.m_i8Front > g_stUART2RcvBuffModbus.m_i8Rear)
    {
        return ((RS485_RX_BUFF_SIZE - g_stUART2RcvBuffModbus.m_i8Front) +
                g_stUART2RcvBuffModbus.m_i8Rear + 1);
        
    }
    // If the queue is empty
    return 0;
}

void EmptyQueue()
{
    g_stUART2RcvBuffModbus.m_i8Front = g_stUART2RcvBuffModbus.m_i8Rear = -1;
}

unsigned int8 CheckValidityForReadReg(void)
{
    unsigned int16 ui16CRCValue       = 0;
    BYTE           bIsQueueEmpty      = MSG_QUEUE_NOT_EMPTY;
    BYTE           bTemp              = 0;
    unsigned int8  ui8IsValidResponse = 0;
    int8           i8LoopCount        = 0;
    int8           i8TempFront        = g_stUART2RcvBuffModbus.m_i8Front;
    
    // Step 1: Populate the Message in a temporary array
    g_ui8TempArrForCRCValidation[0] = SLAVE_ID;
    for(i8LoopCount = 1; i8LoopCount != MODBUS_MASTER_QUERY_READ_REG_LENGTH - 2; i8LoopCount++)
    {
        g_ui8TempArrForCRCValidation[i8LoopCount] = g_stUART2RcvBuffModbus.m_ui8Arr[i8TempFront++];
    }
    
    // Calculate CRC value with received bytes for validation.
    ui16CRCValue = calculateCrc(g_ui8TempArrForCRCValidation, MODBUS_MASTER_QUERY_READ_REG_LENGTH - 2);
    
    // Match with received first CRC byte
    bTemp = g_stUART2RcvBuffModbus.m_ui8Arr[i8TempFront++];
    if(bTemp == (unsigned int8)((ui16CRCValue >> 0) & 0xFF))
    {
        // Match with received second CRC byte.
        bTemp = g_stUART2RcvBuffModbus.m_ui8Arr[i8TempFront];
        if(bTemp == (unsigned int8)((ui16CRCValue >> 8) & 0xFF))
        {
            // Update flag denote CRC bytes matched..valid response.
            ui8IsValidResponse = 1;
        }
    }
    
    return ui8IsValidResponse;
}

// send response
void SendRS485Response()
{
	// function for sending reply
	
	//value assign in transmit buffer of RS485
    int8 i8TempFront             = 0;
    BYTE ui8Temp                 = 0;
    int8 i8TempLen               = 0;  
    int8 i8IndexTx               = 0;  
            
    i8TempFront = g_stUART2RcvBuffModbus.m_i8Front;    
    ui8Temp     = g_stUART2RcvBuffModbus.m_ui8Arr[i8TempFront];  // g_ui8TempArrForCRCValidation
    i8TempLen   = GetQueueLength();
    
    g_RS485TxBuffer[i8IndexTx] = SLAVE_ID;
    i8IndexTx++;
	for(int8 i8Index = i8TempFront; i8Index <= i8TempLen; i8Index++)
	{
		g_RS485TxBuffer[i8IndexTx]= g_stUART2RcvBuffModbus.m_ui8Arr[i8Index]; 
        i8IndexTx++;
	}
	
  	//set transmit active.
  	delay_ms(DELAY_TIME_RTS_BEFORE);
	output_bit(PIN_RS485_RTS,1);
	delay_ms(DELAY_TIME_RTS_AFTER);
	
	//send bytes.
	for(unsigned int8 ui8SendBytes = 0; ui8SendBytes <= i8TempLen; ui8SendBytes++) // g_ui8RS485MaxByteCount
	{
		fputc(g_RS485TxBuffer[ui8SendBytes],RS485);
	}
	
	//hold to bytes properly transmit.
	
	//set transmit inactive.
	delay_ms(DELAY_TIME_RTS_AFTER);
	output_bit(PIN_RS485_RTS,0);
//	delay_ms(DELAY_TIME_RTS_AFTER);
}

// send response
void SendRS485ResponseForMultipleRegWrite()
{
	// function for sending reply
	
	//value assign in transmit buffer of RS485
    int8 i8TempFront             = 0;
    BYTE ui8Temp                 = 0;
    int8 i8TempLen               = 0;  
    int8 i8IndexTx               = 0;  
            	// function for polling data
	unsigned int16 ui16CRCvalue = 0;
    
    i8TempFront = g_stUART2RcvBuffModbus.m_i8Front;    
    ui8Temp     = g_stUART2RcvBuffModbus.m_ui8Arr[i8TempFront];  // g_ui8TempArrForCRCValidation
    i8TempLen   = 6;
    
    g_RS485TxBuffer[i8IndexTx] = SLAVE_ID;
    i8IndexTx++;
	for(int8 i8Index = i8TempFront; i8IndexTx < i8TempLen; i8Index++)
	{
		g_RS485TxBuffer[i8IndexTx]= g_stUART2RcvBuffModbus.m_ui8Arr[i8Index]; 
        i8IndexTx++;
	}
	
    	//Function call CRC calculation.
	ui16CRCvalue = ModbusCRC16(g_RS485TxBuffer,6);
	
	g_RS485TxBuffer[6] = (unsigned int8)((ui16CRCvalue >> 0)& 0xFF);
	g_RS485TxBuffer[7] = (unsigned int8)((ui16CRCvalue >> 8)& 0xFF);

  	//set transmit active.
  	delay_ms(DELAY_TIME_RTS_BEFORE);
	output_bit(PIN_RS485_RTS,1);
	delay_ms(DELAY_TIME_RTS_AFTER);
	
	//send bytes.
	for(unsigned int8 ui8SendBytes = 0; ui8SendBytes <= 7; ui8SendBytes++) // g_ui8RS485MaxByteCount
	{
		fputc(g_RS485TxBuffer[ui8SendBytes],RS485);
	}
	
	//hold to bytes properly transmit.
	
	//set transmit inactive.
	delay_ms(DELAY_TIME_RTS_AFTER);
	output_bit(PIN_RS485_RTS,0);
//	delay_ms(DELAY_TIME_RTS_AFTER);
}

void MakeData(void)
{
	//value assign in transmit buffer of RS485
    int8 i8TempFront             = 0;
    BYTE ui8Temp                 = 0;
    int8 i8TempLen               = 0;  
    unsigned int16 ui16Address    = 0;
    unsigned int16 ui16RegData    = 0;
    
    g_ModbusRx.ui8Slaveid = SLAVE_ID;
    g_ModbusRx.func = g_stUART2RcvBuffModbus.m_ui8Arr[i8TempFront+MODBUS_OFFSET_ONE];
            
    i8TempFront = g_stUART2RcvBuffModbus.m_i8Front;    
    ui8Temp     = g_stUART2RcvBuffModbus.m_ui8Arr[i8TempFront];  // g_ui8TempArrForCRCValidation
    i8TempLen   = GetQueueLength();
    
	int8 i8AddH = g_stUART2RcvBuffModbus.m_ui8Arr[i8TempFront+MODBUS_OFFSET_TWO];
	int8 i8AddL = g_stUART2RcvBuffModbus.m_ui8Arr[i8TempFront+MODBUS_OFFSET_THREE];
	
    g_ModbusRx.u16Address = i8AddH;
	g_ModbusRx.u16Address <<= 8;
	g_ModbusRx.u16Address += i8AddL;
	
	int8 i8DataH = g_stUART2RcvBuffModbus.m_ui8Arr[i8TempFront+MODBUS_OFFSET_FOUR];
	int8 i8DataL = g_stUART2RcvBuffModbus.m_ui8Arr[i8TempFront+MODBUS_OFFSET_FIVE];
	
	unsigned int16 ui16WriteAddress = ui16Address + 1;
	
	g_ModbusRx.u16LenData = i8DataH;
	g_ModbusRx.u16LenData <<= 8;
	g_ModbusRx.u16LenData += i8DataL;
    
    
    	// prepare data for multiple register or coils
	if((g_ModbusRx.func == RS485_FUNCTION_WRITE_MULTIPLE_REGISTERS) || (g_ModbusRx.func == RS485_FUNCTION_READ_MULTIPLE_REGISTERS)) 
	{
		g_ModbusRx.ui8NoRegDataBytes = g_stUART2RcvBuffModbus.m_ui8Arr[i8TempFront+MODBUS_OFFSET_SIX];
		int8 iStartIndx = i8TempFront+7;
		int8 iRegIndx = 0;
	
		for(int i = 0; i < g_ModbusRx.ui8NoRegDataBytes; ) 
		{
			int8 i8DataH = g_stUART2RcvBuffModbus.m_ui8Arr[iStartIndx];
			int8 i8DataL = g_stUART2RcvBuffModbus.m_ui8Arr[iStartIndx+MODBUS_OFFSET_ONE];
	
			unsigned int16 u16LenData = i8DataH;
			u16LenData <<= 8;
			u16LenData += i8DataL;
			
			g_ModbusRx.ui16RegisterDataArr[iRegIndx] = u16LenData;
			
			iStartIndx +=2;
			iRegIndx++;
			i +=2;
		}
	
		unsigned int32 u32RegData = 0;
		
		if(REG_DATA_LO_TO_HI == 1) 
		{
			int8 j = iRegIndx-1;
			
			for(int8 count = 0; count < iRegIndx; count++) 
			{
				unsigned int16 u16LenData = g_ModbusRx.ui16RegisterDataArr[j];
					
				if(j == (iRegIndx-1)) 
				{
					u32RegData = u16LenData;
				}
				else 
				{
					u32RegData <<= 16;
					u32RegData += u16LenData;
				}
				j--;
			}
		}
		else 
		{
			int8 j = 0;
			
			for(int8 count = 0; count < iRegIndx; count++) 
			{
				unsigned int16 u16LenData = g_ModbusRx.ui16RegisterDataArr[j];
					
				if(j == 0) 
				{
					u32RegData = u16LenData;
				}
				else 
				{
					u32RegData <<= 16;
					u32RegData += u16LenData;
				}
				j++;
			}
		}
		
		g_ModbusRx.u32RegData = u32RegData;
	}
	else 
	{
		// prepare data for single register or coil
		g_ModbusRx.ui8NoRegDataBytes = 0;
		if(g_ModbusRx.func == RS485_FUNCTION_WRITE_SINGLE_REGISTER) 
		{
			g_ModbusRx.ui16RegisterDataArr[0] = g_ModbusRx.u16LenData;
			g_ModbusRx.u32RegData = g_ModbusRx.u16LenData;
		}
	}
		        
}
void ProcessSingleRegister(void)
{
	// function for process single register
	
	//value assign in transmit buffer of RS485
    int8 i8TempFront              = 0;
    BYTE ui8Temp                  = 0;
    int8 i8TempLen                = 0;  
    unsigned int16 ui16Address    = 0;
    unsigned int16 ui16RegData    = 0;
    
            
    i8TempFront = g_stUART2RcvBuffModbus.m_i8Front;    
    ui8Temp     = g_stUART2RcvBuffModbus.m_ui8Arr[i8TempFront];  // g_ui8TempArrForCRCValidation
    i8TempLen   = GetQueueLength();
    
	int8 i8AddH = g_stUART2RcvBuffModbus.m_ui8Arr[i8TempFront+MODBUS_OFFSET_TWO - 1];
	int8 i8AddL = g_stUART2RcvBuffModbus.m_ui8Arr[i8TempFront+MODBUS_OFFSET_THREE - 1];
	
    ui16Address = i8AddH;
	ui16Address <<= 8;
	ui16Address += i8AddL;
    
	
	int8 i8DataH = g_stUART2RcvBuffModbus.m_ui8Arr[i8TempFront+MODBUS_OFFSET_FOUR -1];
	int8 i8DataL = g_stUART2RcvBuffModbus.m_ui8Arr[i8TempFront+MODBUS_OFFSET_FIVE -1];
	
	unsigned int16 ui16WriteAddress = ui16Address + 1;
	
	ui16RegData = i8DataH;
	ui16RegData <<= 8;
	ui16RegData += i8DataL;
		    
    // set led on or off depending on received bytes
    unsigned int8 ui8RetVal1  = IsBitOn(0,ui16RegData);
    unsigned int8 ui8RetVal2  = IsBitOn(1,ui16RegData);
    unsigned int8 ui8RetVal3  = IsBitOn(2,ui16RegData);
    unsigned int8 ui8RetVal4  = IsBitOn(3,ui16RegData);
    unsigned int8 ui8RetVal5  = IsBitOn(4,ui16RegData);
    unsigned int8 ui8RetVal6  = IsBitOn(5,ui16RegData);
    unsigned int8 ui8RetVal7  = IsBitOn(6,ui16RegData);
    unsigned int8 ui8RetVal8  = IsBitOn(7,ui16RegData);
    unsigned int8 ui8RetVal9  = IsBitOn(8,ui16RegData);
    unsigned int8 ui8RetVal10 = IsBitOn(9,ui16RegData);
    unsigned int8 ui8RetVal11 = IsBitOn(10,ui16RegData);
    unsigned int8 ui8RetVal12 = IsBitOn(11,ui16RegData);
    unsigned int8 ui8RetVal13 = IsBitOn(12,ui16RegData);
    unsigned int8 ui8RetVal14 = IsBitOn(13,ui16RegData);
    unsigned int8 ui8RetVal15 = IsBitOn(14,ui16RegData);
    unsigned int8 ui8RetVal16 = IsBitOn(15,ui16RegData);
    
    // Set Mode 
    switch(ui16Address)
    {
        case MODE_ADDRESS:
        {
            switch(ui16RegData)
            {
                case MODE_TYPE_NOTHING:
                {
                    SetModeNothing();
                   // output_bit(PIN_OUT_DO_00,0);
                }
                break;
                case MODE_TYPE_HOME:
                {
                    SetModeHome();
                 //   output_bit(PIN_OUT_DO_01,0);
                }
                break;
                case MODE_TYPE_PRE_PRODUCTION:
                {
                    SetModePreProduction();
                //    output_bit(PIN_OUT_DO_02,0);
                }
                break;
                case MODE_TYPE_PRODUCTION:
                {
                    SetModeProduction();
                //    output_bit(PIN_OUT_DO_03,0);
                }
                break;
                case MODE_TYPE_CONFIGURATION: 
                {
                    SetModeConfiguration();
                //    output_bit(PIN_OUT_DO_04,0);
                }
                break;
                case MODE_TYPE_CALIBRATION:
                {
                    SetModeCalibration();
                //    output_bit(PIN_OUT_DO_05,0);
                }
                break;
            };
        }
        break;
        case ADDRESS_FOR_SET_CONFIG_AVG_WT:
        {
            g_ui16RegisterAvgWeight = ui16RegData;
        }
        break;
        case ADDRESS_FOR_SET_CONFIG_TOL_PV:
        {
            g_ui16RegisterTolPv = ui16RegData;
        }
        break;
        case ADDRESS_FOR_SET_CONFIG_TOL_NV:
        {
            g_ui16RegisterTolNv = ui16RegData;
        }
        break;
        case ADDRESS_FOR_SET_CONFIG_INDEX:
        {
            g_ui16RegisterCurrProductIndex = ui16RegData;
        }
        break;

    };
}

void ProcessUART2ReceivedData(void)
{
    BYTE ui8Temp                 = 0;
    BYTE bIsQueueEmpty           = MSG_QUEUE_NOT_EMPTY;
    BYTE bIsValidMSG             = FALSE;
    int8 i8TempFront             = 0;
    int8 i8MainProcessLoopStatue = 0;
    int8 i8TempLen               = 0;  
    
    
    do
    {
        // Step 1: Seek for Slave ID
        do
        {
            // Get Queue
            ui8Temp = GetUART2Queue(&bIsQueueEmpty);
            if (MSG_QUEUE_EMPTY == bIsQueueEmpty)
            {
                // Return
                break;    // Handle this break
            }

            // Check for Slave ID

        } while (SLAVE_ID != ui8Temp); // If Slave ID not matched then continue

        // Handle Break
        if (MSG_QUEUE_EMPTY == bIsQueueEmpty)
        {
            // Nothing to return
            // This may be the Slave ID is not matched, So might be this is for other board
            // So nothing to process
            break;
        }
        
        // Step 2: Now Check for minimum length (Which is 8 for now, but it could go upto 12)
        i8TempLen = GetQueueLength();
        if (MODBUS_MASTER_QUERY_LENGTH_MIN > (i8TempLen + 1))  // +1 is just due to we have deleted one from queue
        {
            // Empty the message buffer queue
            EmptyQueue();
            
            // Break from ProcessUART2ReceivedData function
            break;
        }
        
        // We have available message in queue
        
        // Should insert slave ID in temp buffer for CRC check.
        
        // Slave ID Matched
        // Step 3: Look for the Function Code
        i8TempFront = g_stUART2RcvBuffModbus.m_i8Front;
        ui8Temp = g_stUART2RcvBuffModbus.m_ui8Arr[i8TempFront];  // g_ui8TempArrForCRCValidation
            
        
	//value assign in transmit buffer of RS485
        int8 i8TempLen                = 0;  
        unsigned int16 ui16Address    = 0;
        unsigned int16 ui16RegData    = 0;


        i8TempFront = g_stUART2RcvBuffModbus.m_i8Front;    
        ui8Temp     = g_stUART2RcvBuffModbus.m_ui8Arr[i8TempFront];  // g_ui8TempArrForCRCValidation
        i8TempLen   = GetQueueLength();

        int8 i8AddH = g_stUART2RcvBuffModbus.m_ui8Arr[i8TempFront+MODBUS_OFFSET_TWO - 1];
        int8 i8AddL = g_stUART2RcvBuffModbus.m_ui8Arr[i8TempFront+MODBUS_OFFSET_THREE - 1];

        ui16Address = i8AddH;
        ui16Address <<= 8;
        ui16Address += i8AddL;


        int8 i8DataH = g_stUART2RcvBuffModbus.m_ui8Arr[i8TempFront+MODBUS_OFFSET_FOUR -1];
        int8 i8DataL = g_stUART2RcvBuffModbus.m_ui8Arr[i8TempFront+MODBUS_OFFSET_FIVE -1];

        unsigned int16 ui16WriteAddress = ui16Address + 1;

        ui16RegData = i8DataH;
        ui16RegData <<= 8;
        ui16RegData += i8DataL;        
        
        // Now check for the available function code
        switch(ui8Temp)
        {
            case RS485_FUNCTION_READ_HOLDING_REGISTERS:
                // Check queue length here if full message is received or not.
                
                // Insert function code in the temp buffer.
                // For this function code fetch next four consecutive bytes in temp buffer.
                bIsValidMSG = CheckValidityForReadReg();
                if(bIsValidMSG)
                {
                    
                    // Empty the message buffer queue
                    EmptyQueue();
                   
                    if(ui16RegData == 1)
                    {                 
                        WriteToOneRegRS485(
                            SLAVE_ID,
                            RS485_FUNCTION_READ_HOLDING_REGISTERS,
                            2,
                            4464 //TODO**********************************************
                            );
                            //65535);                        
                    }
                    else if(ui16RegData == 2)
                    {
                        if(ui16Address == ADDRESS_FOR_SET_CALIB_DATA) 
                        {
                            if(g_ui16RegisterForCount == 1) 
                            {
                                WriteToTwoRegRS485(
                                    SLAVE_ID,
                                    RS485_FUNCTION_READ_HOLDING_REGISTERS,
                                    4,
                                    g_ui16RegisterForCounter1_1, //TODO**********************************************
                                    g_ui16RegisterForCounter1_2);
                            }
                            else if(g_ui16RegisterForCount == 2) 
                            {
                                WriteToTwoRegRS485(
                                    SLAVE_ID,
                                    RS485_FUNCTION_READ_HOLDING_REGISTERS,
                                    4,
                                    g_ui16RegisterForCounter2_1, //TODO**********************************************
                                    g_ui16RegisterForCounter2_2);
                            }
                            else if(g_ui16RegisterForCount == 3) 
                            {
                                WriteToTwoRegRS485(
                                    SLAVE_ID,
                                    RS485_FUNCTION_READ_HOLDING_REGISTERS,
                                    4,
                                    g_ui16RegisterForCounter3_1, //TODO**********************************************
                                    g_ui16RegisterForCounter3_2);
                            }
                        }
                        else if(ui16Address == ADDRESS_FOR_SET_CALIB_0)
                        {
                                WriteToTwoRegRS485(
                                    SLAVE_ID,
                                    RS485_FUNCTION_READ_HOLDING_REGISTERS,
                                    4,
                                    g_ui16Register0_1, //TODO**********************************************
                                    g_ui16Register0_2);
                        }
                        else if(ui16Address == ADDRESS_FOR_SET_CALIB_1000)
                        {
                                WriteToTwoRegRS485(
                                    SLAVE_ID,
                                    RS485_FUNCTION_READ_HOLDING_REGISTERS,
                                    4,
                                    g_ui16Register1000_1, //TODO**********************************************
                                    g_ui16Register1000_2);
                        }
                        else if(ui16Address == ADDRESS_FOR_SET_CALIB_2000)
                        {
                                WriteToTwoRegRS485(
                                    SLAVE_ID,
                                    RS485_FUNCTION_READ_HOLDING_REGISTERS,
                                    4,
                                    g_ui16Register2000_1, //TODO**********************************************
                                    g_ui16Register2000_2);
                        }
                        else if(ui16Address == ADDRESS_FOR_SET_CALIB_3000)
                        {
                                WriteToTwoRegRS485(
                                    SLAVE_ID,
                                    RS485_FUNCTION_READ_HOLDING_REGISTERS,
                                    4,
                                    g_ui16Register3000_1, //TODO**********************************************
                                    g_ui16Register3000_2);
                        }
                        else if(ui16Address == ADDRESS_FOR_SET_CALIB_4000)
                        {
                                WriteToTwoRegRS485(
                                    SLAVE_ID,
                                    RS485_FUNCTION_READ_HOLDING_REGISTERS,
                                    4,
                                    g_ui16Register4000_1, //TODO**********************************************
                                    g_ui16Register4000_2);
                        }
                        else if(ui16Address == ADDRESS_FOR_SET_CALIB_5000)
                        {
                                WriteToTwoRegRS485(
                                    SLAVE_ID,
                                    RS485_FUNCTION_READ_HOLDING_REGISTERS,
                                    4,
                                    g_ui16Register5000_1, //TODO**********************************************
                                    g_ui16Register5000_2);
                        }
                        else if(ui16Address == ADDRESS_FOR_SET_CALIB_6000)
                        {
                                WriteToTwoRegRS485(
                                    SLAVE_ID,
                                    RS485_FUNCTION_READ_HOLDING_REGISTERS,
                                    4,
                                    g_ui16Register6000_1, //TODO**********************************************
                                    g_ui16Register6000_2);
                        }
                        else if(ui16Address == ADDRESS_FOR_SET_CALIB_7000)
                        {
                                WriteToTwoRegRS485(
                                    SLAVE_ID,
                                    RS485_FUNCTION_READ_HOLDING_REGISTERS,
                                    4,
                                    g_ui16Register7000_1, //TODO**********************************************
                                    g_ui16Register7000_2);
                        }
                        else if(ui16Address == ADDRESS_FOR_SET_CALIB_8000)
                        {
                                WriteToTwoRegRS485(
                                    SLAVE_ID,
                                    RS485_FUNCTION_READ_HOLDING_REGISTERS,
                                    4,
                                    g_ui16Register8000_1, //TODO**********************************************
                                    g_ui16Register8000_2);
                        }
                        else if(ui16Address == ADDRESS_FOR_SET_CALIB_9000)
                        {
                                WriteToTwoRegRS485(
                                    SLAVE_ID,
                                    RS485_FUNCTION_READ_HOLDING_REGISTERS,
                                    4,
                                    g_ui16Register9000_1, //TODO**********************************************
                                    g_ui16Register9000_2);
                        }
                        else if(ui16Address == ADDRESS_FOR_SET_CALIB_10000)
                        {
                                WriteToTwoRegRS485(
                                    SLAVE_ID,
                                    RS485_FUNCTION_READ_HOLDING_REGISTERS,
                                    4,
                                    g_ui16Register10000_1, //TODO**********************************************
                                    g_ui16Register10000_2);
                        }
                        else 
                        {    
                            // Valid MSG, so return
                            WriteToTwoRegRS485(
                                SLAVE_ID,
                                RS485_FUNCTION_READ_HOLDING_REGISTERS,
                                4,
                                4464, //TODO**********************************************
                                1);
                                //65535);
                        }
                    }
                    else 
                    {
                        // Valid MSG, so return
                        WriteToThreeRegRS485(
                            SLAVE_ID,
                            RS485_FUNCTION_READ_HOLDING_REGISTERS,
                            6,                                      //TODO**********************************************
                            g_ui16DOStatus,
                            g_ui16DIStatus,
                            g_ui16ErrorStatus);
                    }
                    // Update main loop process status.
                    i8MainProcessLoopStatue = 0;
                }
                break;
            case RS485_FUNCTION_WRITE_SINGLE_REGISTER:
            {
                // Insert function code in the temp buffer.
                // For this function code fetch next four consecutive bytes in temp buffer.
                //bIsValidMSG = CheckValidityForWriteSingleReg();
       		 	SendRS485Response();
                ProcessSingleRegister();
            }
            break;
            case RS485_FUNCTION_READ_MULTIPLE_REGISTERS:
            {
                if(ui16RegData == 2)
                {
                    if(ui16Address == ADDRESS_FOR_SET_CALIB_0)
                    {
                            ReplyForTwoRegister(
                            SLAVE_ID,
                            ui8Temp,
                            ui16RegData,
                            ui16Address,    
                            g_ui16Register0_1, //TODO**********************************************
                            g_ui16Register0_2);
                    }
                    else if(ui16Address == ADDRESS_FOR_SET_CALIB_1000)
                    {
                            ReplyForTwoRegister(
                            SLAVE_ID,
                            ui8Temp,
                            ui16RegData,
                            ui16Address,    
                            g_ui16Register1000_1, //TODO**********************************************
                            g_ui16Register1000_2);
                    }
                    else if(ui16Address == ADDRESS_FOR_SET_CALIB_2000)
                    {
                            ReplyForTwoRegister(
                            SLAVE_ID,
                            ui8Temp,
                            ui16RegData,
                            ui16Address,    
                            g_ui16Register2000_1, //TODO**********************************************
                            g_ui16Register2000_2);
                    }
                    else if(ui16Address == ADDRESS_FOR_SET_CALIB_3000)
                    {
                            ReplyForTwoRegister(
                            SLAVE_ID,
                            ui8Temp,
                            ui16RegData,
                            ui16Address,    
                            g_ui16Register3000_1, //TODO**********************************************
                            g_ui16Register3000_2);
                    }
                    else if(ui16Address == ADDRESS_FOR_SET_CALIB_4000)
                    {
                            ReplyForTwoRegister(
                            SLAVE_ID,
                            ui8Temp,
                            ui16RegData,
                            ui16Address,    
                            g_ui16Register4000_1, //TODO**********************************************
                            g_ui16Register4000_2);
                    }
                    else if(ui16Address == ADDRESS_FOR_SET_CALIB_5000)
                    {
                            ReplyForTwoRegister(
                            SLAVE_ID,
                            ui8Temp,
                            ui16RegData,
                            ui16Address,    
                            g_ui16Register5000_1, //TODO**********************************************
                            g_ui16Register5000_2);
                    }
                    else if(ui16Address == ADDRESS_FOR_SET_CALIB_6000)
                    {
                            ReplyForTwoRegister(
                            SLAVE_ID,
                            ui8Temp,
                            ui16RegData,
                            ui16Address,    
                            g_ui16Register6000_1, //TODO**********************************************
                            g_ui16Register6000_2);
                    }
                    else if(ui16Address == ADDRESS_FOR_SET_CALIB_7000)
                    {
                            ReplyForTwoRegister(
                            SLAVE_ID,
                            ui8Temp,
                            ui16RegData,
                            ui16Address,    
                            g_ui16Register7000_1, //TODO**********************************************
                            g_ui16Register7000_2);
                    }
                    else if(ui16Address == ADDRESS_FOR_SET_CALIB_8000)
                    {
                            ReplyForTwoRegister(
                            SLAVE_ID,
                            ui8Temp,
                            ui16RegData,
                            ui16Address,    
                            g_ui16Register8000_1, //TODO**********************************************
                            g_ui16Register8000_2);
                    }
                    else if(ui16Address == ADDRESS_FOR_SET_CALIB_9000)
                    {
                            ReplyForTwoRegister(
                            SLAVE_ID,
                            ui8Temp,
                            ui16RegData,
                            ui16Address,    
                            g_ui16Register9000_1, //TODO**********************************************
                            g_ui16Register9000_2);
                    }
                    else if(ui16Address == ADDRESS_FOR_SET_CALIB_10000)
                    {
                            ReplyForTwoRegister(
                            SLAVE_ID,
                            ui8Temp,
                            ui16RegData,
                            ui16Address,    
                            g_ui16Register10000_1, //TODO**********************************************
                            g_ui16Register10000_2);
                    }
                    else {
                        ReplyForTwoRegister(
                            SLAVE_ID,
                            ui8Temp,
                            ui16RegData,
                            ui16Address,    
                            g_ui16Register0_1, //TODO**********************************************
                            g_ui16Register0_2);
                    }
                }
                else 
                {
                        ReplyForTwoRegister(
                            SLAVE_ID,
                            ui8Temp,
                            ui16RegData,
                            ui16Address,    
                            g_ui16Register0_1, //TODO**********************************************
                            g_ui16Register0_2);
                }
            }
            break;
            case RS485_FUNCTION_WRITE_MULTIPLE_REGISTERS:
            {
                SendRS485ResponseForMultipleRegWrite();
                
                unsigned int16 ui16RegDataLen    = ui16RegData;
                int8 i8DataLen = g_stUART2RcvBuffModbus.m_ui8Arr[i8TempFront+MODBUS_OFFSET_SIX - 1];

                int8 i8DataH = g_stUART2RcvBuffModbus.m_ui8Arr[i8TempFront+MODBUS_OFFSET_SEVEN -1];
                int8 i8DataL = g_stUART2RcvBuffModbus.m_ui8Arr[i8TempFront+MODBUS_OFFSET_8 -1];

                unsigned int16 ui16RegDataValue1 = 0;

                ui16RegDataValue1 = i8DataH;
                ui16RegDataValue1 <<= 8;
                ui16RegDataValue1 += i8DataL;  
                
                i8DataH = g_stUART2RcvBuffModbus.m_ui8Arr[i8TempFront+MODBUS_OFFSET_9 -1];
                i8DataL = g_stUART2RcvBuffModbus.m_ui8Arr[i8TempFront+MODBUS_OFFSET_10 -1];

                unsigned int16 ui16RegDataValue2 = 0;

                ui16RegDataValue2 = i8DataH;
                ui16RegDataValue2 <<= 8;
                ui16RegDataValue2 += i8DataL;   
                
                if(ui16RegDataLen == 2)
                {
                    if(ui16Address == ADDRESS_FOR_SET_CALIB_0)
                    {
                        g_ui16Register0_1 = ui16RegDataValue1;
                        g_ui16Register0_2 = ui16RegDataValue2;                        
                    }
                    else if(ui16Address == ADDRESS_FOR_SET_CALIB_1000)
                    {
                        g_ui16Register1000_1 = ui16RegDataValue1;
                        g_ui16Register1000_2 = ui16RegDataValue2;                        
                    }
                    else if(ui16Address == ADDRESS_FOR_SET_CALIB_2000)
                    {
                        g_ui16Register2000_1 = ui16RegDataValue1;
                        g_ui16Register2000_2 = ui16RegDataValue2;                        
                    }
                    else if(ui16Address == ADDRESS_FOR_SET_CALIB_3000)
                    {
                        g_ui16Register3000_1 = ui16RegDataValue1;
                        g_ui16Register3000_2 = ui16RegDataValue2;                        
                    }
                    else if(ui16Address == ADDRESS_FOR_SET_CALIB_4000)
                    {
                        g_ui16Register4000_1 = ui16RegDataValue1;
                        g_ui16Register4000_2 = ui16RegDataValue2;                        
                    }
                    else if(ui16Address == ADDRESS_FOR_SET_CALIB_5000)
                    {
                        g_ui16Register5000_1 = ui16RegDataValue1;
                        g_ui16Register5000_2 = ui16RegDataValue2;                        
                    }
                    else if(ui16Address == ADDRESS_FOR_SET_CALIB_6000)
                    {
                        g_ui16Register6000_1 = ui16RegDataValue1;
                        g_ui16Register6000_2 = ui16RegDataValue2;                        
                    }
                    else if(ui16Address == ADDRESS_FOR_SET_CALIB_7000)
                    {
                        g_ui16Register7000_1 = ui16RegDataValue1;
                        g_ui16Register7000_2 = ui16RegDataValue2;                        
                    }
                    else if(ui16Address == ADDRESS_FOR_SET_CALIB_8000)
                    {
                        g_ui16Register8000_1 = ui16RegDataValue1;
                        g_ui16Register8000_2 = ui16RegDataValue2;                        
                    }
                    else if(ui16Address == ADDRESS_FOR_SET_CALIB_9000)
                    {
                        g_ui16Register9000_1 = ui16RegDataValue1;
                        g_ui16Register9000_2 = ui16RegDataValue2;                        
                    }
                    else if(ui16Address == ADDRESS_FOR_SET_CALIB_10000)
                    {
                        g_ui16Register10000_1 = ui16RegDataValue1;
                        g_ui16Register10000_2 = ui16RegDataValue2;                        
                    }
                    else {
                        ReplyForTwoRegister(
                            SLAVE_ID,
                            ui8Temp,
                            ui16RegData,
                            ui16Address,    
                            g_ui16Register0_1, //TODO**********************************************
                            g_ui16Register0_2);
                    }
                }
            }
            break;
            default:   // Function Code is not Matching
                // Continue the ProcessUART2ReceivedData from beginning
                SendRS485Response();
                i8MainProcessLoopStatue = 1;
                break;
        }
        
        // If Function Code not Matching
        if (i8MainProcessLoopStatue)
        {
            // Then continue from beginning
            continue;
        }
        
        
        // If the message is not valid
        // Start 
    } while (i8MainProcessLoopStatue);
    
}
////=============================== END NEW CODE ======================================================

#endif /* _VI_RS485_CONFIG_H_ */

