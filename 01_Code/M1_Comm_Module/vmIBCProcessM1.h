/* 
 * File:   vmIBCProcessM1.h
 * Author: Mrinmoy
 *
 * Created on December 11, 2018, 3:11 PM
 */

#ifndef VM_IBC_PROCESS_M1_H
#define	VM_IBC_PROCESS_M1_H

#include "../Common/vmIBCConfig.h"
#include "vmCalibrationConfig.h"
#include "vmSIM900_GPRSConfig.h"
#include "viModbusGlobal.h"
#include "vmGSMM66_GPRSConfig.h"

//#define IBC_CMD_SET_CALIB_POINT             1
//#define IBC_CMD_GET_CALIB_POINT             2
//#define IBC_CMD_RESET_CALIB_POINT           3
//#define IBC_CMD_TARE_REQUEST                4
//#define IBC_CMD_PRE_PROD_AVG_WEIGHT         5
//#define IBC_CMD_PRE_PROD_TOLERANCE          6
//#define IBC_CMD_START_PROD                  7
//#define IBC_CMD_PROD_WEIGH_DATA             

unsigned int16 g_ui16PrevItemCount = 0;

void StuffCalibTable(void)
{
    unsigned int8 ui8Index = (g_i16CalibPoint / 1000);
    
    if((g_i16CalibPoint % 1000) == 0 && g_i16CalibPoint <= 10000)
    {
        g_stCalibTable[ui8Index].m_i16CalibPoint = g_i16CalibPoint;
        g_stCalibTable[ui8Index].m_ui32CalibValue = g_i32CalibValue;
       
        switch(ui8Index)
        {
            case 0:
                g_ui16Register0_2 = (unsigned int16)((g_i32CalibValue >> 16) & 0x0000FFFF);
                g_ui16Register0_1 = (unsigned int16)(g_i32CalibValue & 0x0000FFFF);
                break;
            case 1:
                g_ui16Register1000_2 = (unsigned int16)((g_i32CalibValue >> 16) & 0x0000FFFF);
                g_ui16Register1000_1 = (unsigned int16)(g_i32CalibValue & 0x0000FFFF);
                break;
            case 2:
                g_ui16Register2000_2 = (unsigned int16)((g_i32CalibValue >> 16) & 0x0000FFFF);
                g_ui16Register2000_1 = (unsigned int16)(g_i32CalibValue & 0x0000FFFF);
                break;
            case 3:
                g_ui16Register3000_2 = (unsigned int16)((g_i32CalibValue >> 16) & 0x0000FFFF);
                g_ui16Register3000_1 = (unsigned int16)(g_i32CalibValue & 0x0000FFFF);
                break;
            case 4:
                g_ui16Register4000_2 = (unsigned int16)((g_i32CalibValue >> 16) & 0x0000FFFF);
                g_ui16Register4000_1 = (unsigned int16)(g_i32CalibValue & 0x0000FFFF);
                break;
            case 5:
                g_ui16Register5000_2 = (unsigned int16)((g_i32CalibValue >> 16) & 0x0000FFFF);
                g_ui16Register5000_1 = (unsigned int16)(g_i32CalibValue & 0x0000FFFF);
                break;
            case 6:
                g_ui16Register6000_2 = (unsigned int16)((g_i32CalibValue >> 16) & 0x0000FFFF);
                g_ui16Register6000_1 = (unsigned int16)(g_i32CalibValue & 0x0000FFFF);
                break;
            case 7:
                g_ui16Register7000_2 = (unsigned int16)((g_i32CalibValue >> 16) & 0x0000FFFF);
                g_ui16Register7000_1 = (unsigned int16)(g_i32CalibValue & 0x0000FFFF);
                break;
            case 8:
                g_ui16Register8000_2 = (unsigned int16)((g_i32CalibValue >> 16) & 0x0000FFFF);
                g_ui16Register8000_1 = (unsigned int16)(g_i32CalibValue & 0x0000FFFF);
                break;
            case 9:
                g_ui16Register9000_2 = (unsigned int16)((g_i32CalibValue >> 16) & 0x0000FFFF);
                g_ui16Register9000_1 = (unsigned int16)(g_i32CalibValue & 0x0000FFFF);
                break;
            case 10:
                g_ui16Register10000_2 = (unsigned int16)((g_i32CalibValue >> 16) & 0x0000FFFF);
                g_ui16Register10000_1 = (unsigned int16)(g_i32CalibValue & 0x0000FFFF);
                break;   
        }
        
    }   
       
}

 // TESTING 
    unsigned int8 ui8Counter = 0;

// IBC Received message process proc
void ProcessIBCMsg()
{   
    ST_IBC_DATA_NODE stIBCRcvNode;
    unsigned int8  ui8LoopCount = 0;
    unsigned int16 ui16ItemCount = 0;
    unsigned int16 ui16TempCurrPassStatus = 50;;

    // Get Node from IBC Receive Message Queue
    DeleteIBCMsgQueue(&g_stIBCRcvQueue, &stIBCRcvNode);
    
    // Valid message present in IBC receive buffer
    // So go for process the data
    switch(stIBCRcvNode.g_ui8ArrIBCData[0])
    {
        case IBC_CMD_RESET_CALIB_POINT_TARE:
            // Store receive data.
            for(ui8LoopCount = 1; ui8LoopCount < IBC_MSG_BYTE_COUNT; ui8LoopCount++)
            {
                g_unIBCResetCalibTare.m_ui8Arr[ui8LoopCount - 1] = stIBCRcvNode.g_ui8ArrIBCData[ui8LoopCount];
            }
            
            // Retrieve and store the dynamic ADC tare value.
            g_ui32TareADCValueForCalibration = g_unIBCResetCalibTare.m_stIBCResetCalibTare.m_ui32CalibADCForTare;
    
            break;
        case IBC_CMD_GET_CALIB_POINT:
            break;
        case IBC_CMD_RESET_CALIB_POINT:            
            break;
        case IBC_CMD_PROD_TARE_DATA:
            {
                for(ui8LoopCount = 1; ui8LoopCount < IBC_MSG_BYTE_COUNT; ui8LoopCount++)
                {
                    g_unIBCProdModeTrData.m_ui8Arr[ui8LoopCount-1] = stIBCRcvNode.g_ui8ArrIBCData[ui8LoopCount];
                }
                g_ui16RunningTareValue = (unsigned int16)(g_unIBCProdModeTrData.m_stTareData.m_i16TareData);
            }
            
            break;
        case IBC_CMD_PRE_PROD_AVG_WEIGHT:
            break;
        case IBC_CMD_PRE_PROD_TOLERANCE:
            break;
        case IBC_CMD_START_PROD:
            break;
        case IBC_CMD_PROD_AVG_WT_DATA:
            {
                for(ui8LoopCount = 1; ui8LoopCount < IBC_MSG_BYTE_COUNT; ui8LoopCount++)
                {
                    g_unIBCProdModeAvg.m_ui8Arr[ui8LoopCount-1] = stIBCRcvNode.g_ui8ArrIBCData[ui8LoopCount];
                }
                
                g_ui16RunningAvgWt = g_unIBCProdModeAvg.m_stAvgData.m_i16CurAvg;
            }
            break;
            
        case IBC_CMD_PROD_WEIGH_DATA:
            {
                // Copy individual Box data.
                for(ui8LoopCount = 1; ui8LoopCount < IBC_MSG_BYTE_COUNT; ui8LoopCount++)
                {
                    g_unIBCProdModeWtData.m_ui8Arr[ui8LoopCount - 1] = stIBCRcvNode.g_ui8ArrIBCData[ui8LoopCount];
                    //g_unIBCProdModeWtData.m_ui8Arr[ui8LoopCount] = g_stIBCRcvData.g_ui8ArrIBCData[ui8LoopCount + 1];
                }

                // Segregate Box count, Rejection status and weight of single box and store in RAM.
                // HMI data display variables.
                g_ui16CBBCount = g_unIBCProdModeWtData.m_stWeighingData.m_ui16BoxCount >> 1;
                //g_ui16CBBCount = g_unIBCProdModeWtData.m_stWeighingData.m_ui16BoxCount >> 2;
                // Store the CBB count temporarily.
                ui16ItemCount = g_ui16CBBCount;
                
                g_stProdModeCurrBoxData.m_i16BoxWeight = g_unIBCProdModeWtData.m_stWeighingData.m_i16BoxWeight;
                // Insert current box data to variables for Display in HMI
                g_ui16CurrentWt = g_stProdModeCurrBoxData.m_i16BoxWeight;
                
                // Update current Box pass status.
                g_ui16CurrentPassStatus = g_unIBCProdModeWtData.m_stWeighingData.m_ui16BoxCount << 15;
                g_ui16CurrentPassStatus = g_ui16CurrentPassStatus >> 15;
        
                //TODO => IBC Error adjustment Start.
                // Just to Cleaning first two erroneous bits due to IBC Sampling Problem.
                g_ui16CBBCount = g_ui16CBBCount << 2;
                g_ui16CBBCount = g_ui16CBBCount >> 2;
                //TODO IBC Error adjustment End
                
                if(ui16ItemCount > 0 && g_ui16PrevItemCount != ui16ItemCount)
                {
                    // Update production mode current box data.
                    g_stProdModeCurrBoxData.m_ui16SequenceNo = g_ui16CBBCount;
                    g_stProdModeCurrBoxData.m_i16BoxWeight = g_ui16CurrentWt;
                    g_stProdModeCurrBoxData.m_ui16RejectionStatus = g_ui16CurrentPassStatus;  // TODO
                    //g_stProdModeCurrBoxData.m_ui32TimeStamp = GetRTC();
                    // Insert current box data in GPRS send Message queue.
                    InsertInGPRSMsgQueue();

                    // update previous item count for next box reference.
                    g_ui16PrevItemCount = ui16ItemCount;
                }
                // Arijita
                // Current box weight to avg array
                if(g_ui16CBBCount > 0) 
                {
                     //g_ui16RunningAvgWt = ProcessWeightData(g_ui16CurrentWt);
                }
                else
                {
                    // TESTING
                    int8 i8NewData = 0;
                    i8NewData++;
                    i8NewData = 100;
                }
            }
            break;
        case IBC_CMD_PROD_INTMD_DATA:
            {
                // Copy individual Box data.
                for(ui8LoopCount = 1; ui8LoopCount < IBC_MSG_BYTE_COUNT; ui8LoopCount++)
                {
                    g_unIBCProdModeWtData.m_ui8Arr[ui8LoopCount - 1] = stIBCRcvNode.g_ui8ArrIBCData[ui8LoopCount];
                    //g_unIBCProdModeWtData.m_ui8Arr[ui8LoopCount] = g_stIBCRcvData.g_ui8ArrIBCData[ui8LoopCount + 1];
                }

                g_stProdModeCurrBoxData.m_i16BoxWeight = g_unIBCProdModeWtData.m_stWeighingData.m_i16BoxWeight;

                // Insert current box data to variables for Display in HMI
                g_ui16CurrentWt = g_stProdModeCurrBoxData.m_i16BoxWeight;

                
            }
            break;            
        case IBC_CMD_RESET_CALIB_POINT_WEIGHT:
            // Store receive data.
            for(ui8LoopCount = 1; ui8LoopCount < IBC_MSG_BYTE_COUNT; ui8LoopCount++)
            {
                g_unIBCResetCalibWeight.m_ui8Arr[ui8LoopCount - 1] = stIBCRcvNode.g_ui8ArrIBCData[ui8LoopCount];
            }
            
            // Retrieve and store the dynamic ADC tare value.
            g_ui32gWeightADCValueForCalibration = g_unIBCResetCalibWeight.m_stIBCResetCalibWeight.m_ui32CalibADCForWeight;
            break;
        default :
            break;
    }
}   



#endif	/* VM_IBC_PROCESS_M1_H */

