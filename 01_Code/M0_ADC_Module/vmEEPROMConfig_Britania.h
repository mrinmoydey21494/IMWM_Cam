#ifndef _VM_EEPROM_CONFIG_BRITANIA_H_
#define _VM_EEPROM_CONFIG_BRITANIA_H_

#include "vmEEPROMConfig.h"
#include "vmCalibration.h"

//#define EEPROM_ADDRESS_FOR_0_GM       200
#define EEPROM_ADDRESS_FOR_LCF          100
//#define CALIB_PONT_DIV_FACTOR         1000
//#define CALIB_POINT_MULT_FACTOR       4

////void WriteDataToEEPROM(unsigned int16 ui16CalibPointEEPROM, unsigned int16 ui16ADCCalibValueEEPROM)
////{
////    unsigned int16 ui16EEPROMStartAddress = 0;
////    
////    // Calculate EEPROM Write Address for each calibration point 
////    if(!ui16CalibPointEEPROM)
////    {
////        ui16EEPROMStartAddress = EEPROM_ADDRESS_FOR_0_GM;
////    }
////    else
////    {
////        ui16EEPROMStartAddress = EEPROM_ADDRESS_FOR_0_GM + ((ui16CalibPointEEPROM / CALIB_PONT_DIV_FACTOR) * CALIB_POINT_MULT_FACTOR);
////    }
////    
////    // Write Calib point to EEPROM
////    WriteEEPROMInt16(ui16EEPROMStartAddress, ui16CalibPointEEPROM);
////    // Increment address
////    ui16EEPROMStartAddress += 2;
////    // Write calib value to EEPROM
////    WriteEEPROMInt16(ui16EEPROMStartAddress, ui16ADCCalibValueEEPROM);
////   
////}

////void ReadCalibTableFromEEPROM(void)
////{
////    unsigned int16 ui16EEPROMStartAddress = 0;
////    unsigned int16 ui16EEPROMPoint        = 0;
////    unsigned int16 ui16EEPROMValue        = 0;
////    unsigned int8 ui8LoopCount = 0;
////    
////    // Write calib value for 0gm predefined
////    g_stCalibTable[0].m_i16CalibPoint = 0;
////    g_stCalibTable[0].m_ui32CalibValue = 41925;
////    
////    for (ui8LoopCount = 1; ui8LoopCount < SIZE_CALIB_TABLE; ui8LoopCount++)
////    {
////        // Calculate EEPROM Write Address for each calibration point
////        ui16EEPROMStartAddress = EEPROM_ADDRESS_FOR_0_GM + (ui8LoopCount * CALIB_POINT_MULT_FACTOR);
////
////        // Read calib point from EEPROM 
////        ui16EEPROMPoint = ReadEEPROMInt16(ui16EEPROMStartAddress);
////        // Assign calibration point in calib table
////        g_stCalibTable[ui8LoopCount].m_i16CalibPoint = ui16EEPROMPoint;
////        // Increment address
////        ui16EEPROMStartAddress += 2;
////        // Read calib value from EEPROM
////        ui16EEPROMValue = ReadEEPROMInt16(ui16EEPROMStartAddress);
////        // Assign calib value in calib table
////        g_stCalibTable[ui8LoopCount].m_ui32CalibValue = ui16EEPROMValue;
////    }
////}

////void WriteDynamicErrToEEPROM(int16 i16DynamicErrValue)
////{
////    unsigned int16 ui16DynamicErrValue = 0;
////    
////    ui16DynamicErrValue = (unsigned int16)i16DynamicErrValue;
////
////    //WriteEEPROMInt16(EEPROM_ADRESS_FOR_DYNAMIC_ERR, ui16DynamicErrValue);
////    
////    return;
////}

void WriteLCFToEEPROM(int16 i16CurrLoadCellFactor)
{
    unsigned int16 ui16CurrLoadCellFactor = 0;
    
    // cast the receive parameter in unsugned.
    ui16CurrLoadCellFactor = (unsigned int16)i16CurrLoadCellFactor;
    
    // Store data in EEPROM.
    WriteEEPROMInt16(EEPROM_ADDRESS_FOR_LCF,ui16CurrLoadCellFactor);
}

int16 ReadLCFFromEEPROM(void)
{
    unsigned int16 ui16RetrieveLCF = 0;
    
    // Retrieve stored LCF from EEPROM.
    ui16RetrieveLCF = ReadEEPROMInt16(EEPROM_ADDRESS_FOR_LCF);
    
    return((int16)ui16RetrieveLCF);
}

//int16 ReadDynamicERRValueFromEEPROM()
//{
//    int16          i16DynamicErrValue  = 0;
//    unsigned int16 ui16DynamicERRValue = 0;
//
//    ui16DynamicERRValue = ReadEEPROMInt16(EEPROM_ADRESS_FOR_DYNAMIC_ERR);
//    
////    if (ui16DynamicERRValue > 0x8000)
////    {
////        ui16DynamicERRValue = ui16DynamicERRValue << 1;
////        ui16DynamicERRValue = ui16DynamicERRValue >> 1;
////        
////        i16DynamicErrValue = (int16)ui16DynamicERRValue;
////        i16DynamicErrValue = 0 - i16DynamicErrValue;
////    }
////    else
////    {
////        i16DynamicErrValue =  (int16)ui16DynamicERRValue;
////    }
//    i16DynamicErrValue =  (int16)ui16DynamicERRValue;
//    return i16DynamicErrValue;
//}

//// TESTING
//void TestEEPROMData(void)
//{
//    unsigned int16 ui16TempCalibPoint = 0;
//    unsigned int16 ui16TempCalibValue = 41926;
//    int8           i8LoopCount        = 0;
//    
//    for (i8LoopCount = 1; i8LoopCount < SIZE_CALIB_TABLE; i8LoopCount++)
//    {
//       ui16TempCalibPoint= ui16TempCalibPoint + 1000;
//       ui16TempCalibValue = ui16TempCalibValue + 1156;
//       WriteDataToEEPROM(ui16TempCalibPoint, ui16TempCalibValue);
//    }    
//}

//void ResetCalibTable(void)
//{
//    unsigned int8 ui8LoopCount = 0;
//    
//    for (ui8LoopCount = 0; ui8LoopCount < SIZE_CALIB_TABLE; ui8LoopCount++)
//    {
//        g_stCalibTable[ui8LoopCount].m_i16CalibPoint = 0;
//        g_stCalibTable[ui8LoopCount].m_ui32CalibValue = 0;
//    }
//}

#endif /* _VM_EEPROM_CONFIG_BRITANIA_H_ */