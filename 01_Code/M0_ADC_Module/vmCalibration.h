/* 
 * File:   vmCalibration.h
 * Author: Amit
 *
 * Created on December 9, 2018, 7:55 PM
 */

#ifndef VM_CALIBRATION_H
#define	VM_CALIBRATION_H

#define SIZE_CALIB_TABLE              11 // Only for the values 0, 1000, 2000, 3000, 4000,
                                            // 5000, 6000, 7000, 8000, 9000 and 10000
#define EEPROM_START_ADDRESS_CALIB    0
#define MAX_CALIB_RESET_PASS          3

#define 	CALIB_ADC_VAL_0000	43302
#define 	CALIB_ADC_VAL_1000	44550
#define 	CALIB_ADC_VAL_2000	45780
#define 	CALIB_ADC_VAL_3000	47005
#define 	CALIB_ADC_VAL_4000	48238
#define 	CALIB_ADC_VAL_5000	49471
#define 	CALIB_ADC_VAL_6000	50716
#define 	CALIB_ADC_VAL_7000	51963
#define 	CALIB_ADC_VAL_8000	53203
#define 	CALIB_ADC_VAL_9000	54435
#define 	CALIB_ADC_VAL_10000	55676



















typedef struct _ST_CALIB_NODE
{
    int16          m_i16CalibPoint;
    unsigned int32 m_ui32CalibValue;
} ST_CALIB_NODE;

// Node for dynamic error calculations
typedef struct _ST_DYNAMIC_ERR_NODE
{
    int16           m_i16WeightPoint;
    unsigned int8   m_ui8ErrValue;
} ST_DYNAMIC_ERR_NODE;

// Parse Union for Calib Point
typedef union _UN_CALIB_POINT
{
    unsigned int16 m_un16CalibPoint;
    unsigned int8  m_un8Arr[2];
} UN_CALIB_POINT;

// Parse Union for Calib Value
typedef union _UN_CALIB_VALUE
{
    unsigned int32 m_un32Value;
    unsigned int8  m_un8Arr[4];
} UN_CALIB_VALUE;


ST_CALIB_NODE g_stCalibTable[SIZE_CALIB_TABLE];
ST_CALIB_NODE g_stTempCalibTable[SIZE_CALIB_TABLE];
ST_DYNAMIC_ERR_NODE g_stDynamicErrTable[SIZE_CALIB_TABLE];

// Update Single Calibration Point in EEPROM
void UpdateCalibPointInRAM(unsigned int16 ui16CalibPoint, unsigned int32 ui32CalibValue)
{
    // Check if the Calibration Point value is 0 or in Multiple of 1000 or not
    if (!(ui16CalibPoint == 0 || ui16CalibPoint % 1000 == 0))
    {
        return;
    }
    
    // We are only considering 0, or values which are multiple of 1000
    // Now need to check that the value should not be greater than 10000,
    // as we are only considering 10KG of max calibration
    if (ui16CalibPoint > 10000)
    {
        return;
    }
    
    // Here we are considering only the following values as calibration point
    // 0, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000 and 10000
    // That is only 11 values
    if (!ui16CalibPoint)
    {
        g_stCalibTable[0].m_ui32CalibValue = ui32CalibValue;
    }
    else
    {
        g_stCalibTable[ui16CalibPoint/1000].m_ui32CalibValue = ui32CalibValue;
    }
}

// Update entire table in RAM into EEPROM
void UpdateCalibTableInEEPROM()
{
    int iLoopCount = 0;
    unsigned int16 ui16StartAddress = EEPROM_START_ADDRESS_CALIB;
    UN_CALIB_POINT unTempCalibPoint;
    UN_CALIB_VALUE unTempCalibValue;
    unsigned int8  ui8Byte0 = 0;
    unsigned int8  ui8Byte1 = 0;
    unsigned int8  ui8Byte2 = 0;
    unsigned int8  ui8Byte3 = 0;
    unsigned int8  ui8Temp  = 0;
    
    for (iLoopCount = 0; iLoopCount < SIZE_CALIB_TABLE; iLoopCount++)
    {
        // Calib Point while in int16 is stored in byte array as  |b1|a1|
        // Calib Value while in int32 is stored in byte array as  |d2|c2|b2|a2|
        // In EEPROM it should be like |a1|b1+c2|a2|b2|
        
        // Step 1 : Parse Calib Point and Calib Value
        unTempCalibPoint.m_un16CalibPoint = g_stCalibTable[iLoopCount].m_i16CalibPoint;
        unTempCalibValue.m_un32Value      = g_stCalibTable[iLoopCount].m_ui32CalibValue;
        
        // Step2 : Prepare EEPROM Data 0
        ui8Byte0 = unTempCalibPoint.m_un8Arr[0];
        
        ui8Byte1 = unTempCalibPoint.m_un8Arr[1];
        
        // Shift the byte to accommodate high part two bit of value
        ui8Byte1 = ui8Byte1 << 2;
        // Get the High Part 2 bit of of Calib Value
        ui8Temp = unTempCalibValue.m_un8Arr[2];
        // Make sure that only two bit will present and rest is 0
        ui8Temp = ui8Temp << 6;
        ui8Temp = ui8Temp >> 6;
        
        // Now Marge two byte to get the second byte
        ui8Byte1 = ui8Byte1 | ui8Temp;
        
        // Step 3: Get the third and 4th Bytes
        ui8Byte2 = unTempCalibValue.m_un8Arr[0];
        ui8Byte3 = unTempCalibValue.m_un8Arr[1];
        
        // Step 4 : Write to EEPROM
        WriteEEPROMInt8((ui16StartAddress + (4*iLoopCount) + 0), ui8Byte0);
        WriteEEPROMInt8((ui16StartAddress + (4*iLoopCount) + 1), ui8Byte1);
        WriteEEPROMInt8((ui16StartAddress + (4*iLoopCount) + 2), ui8Byte2);
        WriteEEPROMInt8((ui16StartAddress + (4*iLoopCount) + 3), ui8Byte3);
    }
}

// Update entire table in RAM into EEPROM
void UpdateCalibPointInEEPROM()
{
    
}

void IBCSendCalibTable()
{
    int8           i8LoopCount1      = 0;
    int8           i8LoopCount2      = 1;
    unsigned int16 ui16StartAddress  = EEPROM_START_ADDRESS_CALIB;
    unsigned int16 ui16EEPROMAddress = EEPROM_START_ADDRESS_CALIB;
    ST_IBC_DATA_NODE stIBCDataNode;
    
    for (i8LoopCount1 = 0; i8LoopCount1 < SIZE_CALIB_TABLE; i8LoopCount1++)
    {
        // Step 1: First Stuff the IBC Command in the First Byte
        //stIBCDataNode.g_ui8ArrIBCData[0] = IBC_CMD_SET_CALIB_POINT;
        
        // Step 2: Stuff 4 Data Bytes from EEPROM
        for (i8LoopCount2 = 1; i8LoopCount2 < IBC_MSG_BYTE_COUNT; i8LoopCount2++)
        {
            stIBCDataNode.g_ui8ArrIBCData[i8LoopCount2] =
                   ReadEEPROMInt8(ui16StartAddress + 
                                 (i8LoopCount1 * (IBC_MSG_BYTE_COUNT - 1)) +
                                 (i8LoopCount2 - 1));
        }
        
        // Step 3: Put it into the IBC Send QUEUE
        //InsertSendQueue(&stIBCDataNode);
        InsertIBCMsgQueue(&g_stIBCSendQueue, &stIBCDataNode);
        //delay_ms(100);  //TODO Just for Testing
    }
}

// Get Calibration Table From EEPROM
void PrepareCalibTable()
{
    int8 i8LoopCount = 0;
    unsigned int16 ui16TempCalibPoint = 0;
    unsigned int32 ui32TempCalibValue = 0;
    unsigned int8 ui8Data0 = 0;
    unsigned int8 ui8Data1 = 0;
    unsigned int8 ui8Data2 = 0;
    unsigned int8 ui8Data3 = 0;
    unsigned int8 ui8Temp  = 0;
    
    unsigned int16 ui16CalibTableStartAddress = 0;   //TODO Set the Calibration Table Start Address
    
    // Read from EEPROM and set Calibration Table
    for (i8LoopCount = 0; i8LoopCount < SIZE_CALIB_TABLE; i8LoopCount++)
    {
        // Get data from EEPROM
        // First 14 bit stores the Calibration Point Second 18 bit stores the Value
        ui8Data0 = ReadEEPROMInt8(ui16CalibTableStartAddress + ((i8LoopCount *4) + 0));
        ui8Data1 = ReadEEPROMInt8(ui16CalibTableStartAddress + ((i8LoopCount *4) + 1));
        ui8Data2 = ReadEEPROMInt8(ui16CalibTableStartAddress + ((i8LoopCount *4) + 2));
        ui8Data3 = ReadEEPROMInt8(ui16CalibTableStartAddress + ((i8LoopCount *4) + 3));
        
        // Set the Calibration Point
        ui16TempCalibPoint = ui8Data1;
        ui16TempCalibPoint = ui16TempCalibPoint >> 2;
        g_stCalibTable[i8LoopCount].m_i16CalibPoint = ui16TempCalibPoint;
        g_stCalibTable[i8LoopCount].m_i16CalibPoint = g_stCalibTable[i8LoopCount].m_i16CalibPoint << 8;
        g_stCalibTable[i8LoopCount].m_i16CalibPoint = g_stCalibTable[i8LoopCount].m_i16CalibPoint | ui8Data0;
        
        // Set the Calibration Value
        ui8Temp = ui8Data1;
        ui8Temp = ui8Temp << 6;
        ui8Temp = ui8Temp >> 6;
        g_stCalibTable[i8LoopCount].m_ui32CalibValue = ui8Temp;
        g_stCalibTable[i8LoopCount].m_ui32CalibValue = g_stCalibTable[i8LoopCount].m_ui32CalibValue << 16;
        
        ui32TempCalibValue = ui8Data3;
        ui32TempCalibValue = ui32TempCalibValue << 8;
        g_stCalibTable[i8LoopCount].m_ui32CalibValue = g_stCalibTable[i8LoopCount].m_ui32CalibValue | ui32TempCalibValue;
        
        ui32TempCalibValue = ui8Data2;
        g_stCalibTable[i8LoopCount].m_ui32CalibValue = g_stCalibTable[i8LoopCount].m_ui32CalibValue | ui32TempCalibValue;
        
    }
}


// TESTING Start
void PrepareTestCalibTable()
{
    g_stCalibTable[0].m_i16CalibPoint  = 0;
    g_stCalibTable[0].m_ui32CalibValue = CALIB_ADC_VAL_0000;  //41925
    
    g_stCalibTable[1].m_i16CalibPoint  = 1000;
    g_stCalibTable[1].m_ui32CalibValue = CALIB_ADC_VAL_1000;  //43081
    
    g_stCalibTable[2].m_i16CalibPoint  = 2000;
    g_stCalibTable[2].m_ui32CalibValue = CALIB_ADC_VAL_2000;  //44233
    
    g_stCalibTable[3].m_i16CalibPoint  = 3000;
    g_stCalibTable[3].m_ui32CalibValue = CALIB_ADC_VAL_3000;  //45379
    
    g_stCalibTable[4].m_i16CalibPoint  = 4000;
    g_stCalibTable[4].m_ui32CalibValue = CALIB_ADC_VAL_4000;  //46538
    
    g_stCalibTable[5].m_i16CalibPoint  = 5000;
    g_stCalibTable[5].m_ui32CalibValue = CALIB_ADC_VAL_5000;  //47686
    
    g_stCalibTable[6].m_i16CalibPoint  = 6000;
    g_stCalibTable[6].m_ui32CalibValue = CALIB_ADC_VAL_6000;  //48842
    
    g_stCalibTable[7].m_i16CalibPoint  = 7000;
    g_stCalibTable[7].m_ui32CalibValue = CALIB_ADC_VAL_7000;  //49992
    
    g_stCalibTable[8].m_i16CalibPoint  = 8000;
    g_stCalibTable[8].m_ui32CalibValue = CALIB_ADC_VAL_8000;  //51151
    
    g_stCalibTable[9].m_i16CalibPoint  = 9000;
    g_stCalibTable[9].m_ui32CalibValue = CALIB_ADC_VAL_9000;  //52303
    
    g_stCalibTable[10].m_i16CalibPoint  = 10000;
    g_stCalibTable[10].m_ui32CalibValue = CALIB_ADC_VAL_10000; //53456
}

// In RAM Only
void PrepareTempCalibTable(void)
{
    int8 i8LoopCount = 0;
    
    // Prepare table
    for (i8LoopCount = 0; i8LoopCount < SIZE_CALIB_TABLE; i8LoopCount++)
    {
        g_stTempCalibTable[i8LoopCount].m_i16CalibPoint = g_stCalibTable[i8LoopCount].m_i16CalibPoint;
        g_stTempCalibTable[i8LoopCount].m_ui32CalibValue = g_stCalibTable[i8LoopCount].m_ui32CalibValue;
    }
}

void ResetCalibTable(int16 i16TareValue)
{
    int8 i8LoopCount = 0;
    
    for (i8LoopCount = 0; i8LoopCount < SIZE_CALIB_TABLE; i8LoopCount++)
    {
        g_stCalibTable[i8LoopCount].m_ui32CalibValue += i16TareValue;
    }
}

void PrepareTestCalibTableX(unsigned int16 ui16X)
{
    g_stCalibTable[0].m_i16CalibPoint  = 0;
    g_stCalibTable[0].m_ui32CalibValue = 165 + ui16X;
    
    g_stCalibTable[1].m_i16CalibPoint  = 1000;
    g_stCalibTable[1].m_ui32CalibValue = 1005 + ui16X;
    
    g_stCalibTable[2].m_i16CalibPoint  = 2000;
    g_stCalibTable[2].m_ui32CalibValue = 2005 + ui16X;
    
    g_stCalibTable[3].m_i16CalibPoint  = 3000;
    g_stCalibTable[3].m_ui32CalibValue = 3005 + ui16X;
    
    g_stCalibTable[4].m_i16CalibPoint  = 4000;
    g_stCalibTable[4].m_ui32CalibValue = 4005 + ui16X;
    
    g_stCalibTable[5].m_i16CalibPoint  = 5000;
    g_stCalibTable[5].m_ui32CalibValue = 5005 + ui16X;
    
    g_stCalibTable[6].m_i16CalibPoint  = 6000;
    g_stCalibTable[6].m_ui32CalibValue = 6005 + ui16X;
    
    g_stCalibTable[7].m_i16CalibPoint  = 7000;
    g_stCalibTable[7].m_ui32CalibValue = 7005 + ui16X;
    
    g_stCalibTable[8].m_i16CalibPoint  = 8000;
    g_stCalibTable[8].m_ui32CalibValue = 8005 + ui16X;
    
    g_stCalibTable[9].m_i16CalibPoint  = 9000 + ui16X;
    g_stCalibTable[9].m_ui32CalibValue = 9005;
    
    g_stCalibTable[10].m_i16CalibPoint  = 10000;
    g_stCalibTable[10].m_ui32CalibValue = 10005 + ui16X;
}
// TESTING End
#endif	/* VM_CALIBRATION_H */

