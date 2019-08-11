/* 
 * File:   vmCalibrationConfig.h
 * Author: Mrinmoy
 *
 * Created on December 10, 2018, 7:32 PM
 */

#ifndef VM_CALIBRATION_CONFIG_H
#define	VM_CALIBRATION_CONFIG_H

#include "../Common/vmIBCConfig.h"
#include "vmIBCProcessM1.h"

#define SIZE_CALIB_TABLE              11   // Only for the values 0, 1000, 2000, 3000, 4000,
                                            // 5000, 6000, 7000, 8000, 9000 and 10000

typedef struct _ST_CALIB_NODE
{
    int16          m_i16CalibPoint;
    unsigned int32 m_ui32CalibValue;
} ST_CALIB_NODE;

// Parse Union for Calib Point
typedef union _UN_CALIB_POINT
{
    unsigned int16 m_ui16CalibPoint;
    unsigned int8  m_ui8Arr[2];
} UN_CALIB_POINT;

// Parse Union for Calib Value
typedef union _UN_RCV_CALIB_VALUE
{
    unsigned int32 m_ui32Value;
    unsigned int8  m_ui8Arr[4];
} UN_RCV_CALIB_VALUE;



// *****************************************************************************
// ***************** Below -> Global Variable Declarations *********************
// *****************************************************************************

ST_CALIB_NODE g_stCalibTable[SIZE_CALIB_TABLE];
UN_CALIB_POINT g_unCalibPoint;
UN_RCV_CALIB_VALUE g_unCalibValue[SIZE_CALIB_TABLE];
int16 g_i16CalibPoint = 0;
int32 g_i32CalibValue = 0;


// *****************************************************************************
// ***************** Above -> Global Variable Declarations *********************
// *****************************************************************************

// Segregate merged calib table and prepare proper calib table.
void PrepareCalibNode(void)
{                                            //################# Data Receive Format ############################
    unsigned int16 ui16TempCalibPoint = 0;  // Calib Point while in int16 is stored in byte array as  |b1|a1|
                                            // Calib Value while in int32 is stored in byte array as  |d2|c2|b2|a2|
                                            // In EEPROM it should be like |a1|b1+c2|a2|b2|
    unsigned int32 ui32TempCalibValue = 0;
    unsigned int8 ui8TempData0        = 0;
    unsigned int8 ui8TempData1        = 0;
    unsigned int8 ui8TempData2        = 0;
    unsigned int8 ui8TempData3        = 0;
    unsigned int8 ui8Temp             = 0;
    
    
    // Assign received 4 bytes in temp data.
    ui8TempData0 = g_stIBCRcvData.g_ui8ArrIBCData[1];
    ui8TempData1 = g_stIBCRcvData.g_ui8ArrIBCData[2];
    ui8TempData2 = g_stIBCRcvData.g_ui8ArrIBCData[3];
    ui8TempData3 = g_stIBCRcvData.g_ui8ArrIBCData[4];

    // Set the Calibration Point
    ui16TempCalibPoint = ui8TempData1;
    ui16TempCalibPoint = ui16TempCalibPoint >> 2;
    g_i16CalibPoint = ui16TempCalibPoint;
    g_i16CalibPoint = g_i16CalibPoint << 8;
    g_i16CalibPoint = g_i16CalibPoint | ui8TempData0;

    // Set the Calibration Value
    ui8Temp = ui8TempData1;
    ui8Temp = ui8Temp << 6;
    ui8Temp = ui8Temp >> 6;
    g_i32CalibValue = ui8Temp;
    g_i32CalibValue = g_i32CalibValue << 16;

    ui32TempCalibValue = ui8TempData3;
    ui32TempCalibValue = ui32TempCalibValue << 8;
    g_i32CalibValue = g_i32CalibValue | ui32TempCalibValue;

    ui32TempCalibValue = ui8TempData2;
    g_i32CalibValue = g_i32CalibValue | ui32TempCalibValue;

        
    
}


#endif	/* VM_CALIBRATION_CONFIG_H */

