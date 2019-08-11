/* 
 * File:   vmIBCProcessM0.h
 * Author: Amit
 *
 * Created on December 9, 2018, 6:17 PM
 */

#ifndef VM_IBC_PROCESS_M0_H
#define	VM_IBC_PROCESS_M0_H

#include "../Common/vmIBCConfig.h"
#include "vmM0ADCModuleCommon.h"
#include "vmAverageWeightCalculation.h"
#include "vmEEPROMConfig_Britania.h"

//// Get Calib Point Parsing
//typedef struct _ST_GET_CALIB_POINT
//{
//    
//} ST_GET_CALIB_POINT;

// IBC Message Process Proc
//void ProcessIBCMsg()
//{
//    int8 i8LoopCount = 0;
//
//    // There is some valid message in IBC Receive Buffer
//    // Try to process that message
//    switch (g_stIBCRcvData.g_ui8ArrIBCData[0])
//    {
//        case IBC_CMD_MODE_HOME:
//            g_ui8SystemMode = SYS_MODE_HOME;
//            break;
//        case IBC_CMD_MODE_CALIB:
//            g_ui8SystemMode = SYS_MODE_CALIB;
//            break;
//        case IBC_CMD_RESET_CALIB_POINT:
//            // Get the Reset Point Value
//            // Copy the Message for Parsing
//            for (i8LoopCount = 1; i8LoopCount < IBC_MSG_BYTE_COUNT; i8LoopCount++)
//            {
//                g_unIBCResetCalib.m_ui8Arr[i8LoopCount -1] = g_stIBCRcvData.g_ui8ArrIBCData[i8LoopCount];
//            }
//            g_fResetCalibStatus = IBC_RESET_CALIB_ON;
//            // Reset Calib Reset Acknowledgment Node
//            g_unIBCResetCalibAck.m_stIBCResetCalibPoint.m_i16PassCount  = 0;
//            g_unIBCResetCalibAck.m_stIBCResetCalibPoint.m_i16CalibValue = 0;
//            break;
//        case IBC_CMD_RESET_CALIB_POINT_ACK:
//            for (i8LoopCount = 1; i8LoopCount < IBC_MSG_BYTE_COUNT; i8LoopCount++)
//            {
//                g_unIBCAvgNode.m_ui8Arr[i8LoopCount -1] = g_stIBCRcvData.g_ui8ArrIBCData[i8LoopCount];
//            }
//            g_stCurBatchInfo.m_i16AvgWeight = g_unIBCAvgNode.m_stIBCAvgNode.m_i16AvgWt;
//            break;
//            
//        case IBC_CMD_MODE_PRE_PROD:
//            g_ui8SystemMode = SYS_MODE_PRE_PROD;
//            break;
//        case IBC_CMD_MODE_PROD:
//            g_ui8SystemMode = SYS_MODE_PROD;
//            break;
//        case IBC_CMD_GET_CALIB_POINT:
//            break;
////        case IBC_CMD_RESET_CALIB_POINT:
////            break;
//        case IBC_CMD_TARE_REQUEST:
//            break;
//        case IBC_CMD_PRE_PROD_AVG_WEIGHT:
//            // Copy the Message for Parsing
//            for (i8LoopCount = 1; i8LoopCount < IBC_MSG_BYTE_COUNT; i8LoopCount++)
//            {
//                g_unIBCAvgNode.m_ui8Arr[i8LoopCount -1] = g_stIBCRcvData.g_ui8ArrIBCData[i8LoopCount];
//            }
//            g_stCurBatchInfo.m_i16AvgWeight = g_unIBCAvgNode.m_stIBCAvgNode.m_i16AvgWt;
//            break;
//        case IBC_CMD_PRE_PROD_TOLERANCE:
//            // Copy the Message for Parsing
//            for (i8LoopCount = 1; i8LoopCount < IBC_MSG_BYTE_COUNT; i8LoopCount++)
//            {
//                g_unIBCTolNode.m_ui8Arr[i8LoopCount - 1] = g_stIBCRcvData.g_ui8ArrIBCData[i8LoopCount];
//            }
//            g_stCurBatchInfo.m_i16PositiveTol = g_unIBCTolNode.m_stIBCTolNode.m_i16PositiveTol;
//            g_stCurBatchInfo.m_i16NegativeTol = g_unIBCTolNode.m_stIBCTolNode.m_i16NegativeTol;
//            break;
//        case IBC_CMD_START_PROD:
//            break;
//        default:
//            break;
//    }
//}

void ProcessIBCMsg()
{
    int8 i8LoopCount = 0;
    ST_IBC_DATA_NODE stIBCRcvNode;
    
    // Get Node from IBC Receive Message Queue
    DeleteIBCMsgQueue(&g_stIBCRcvQueue, &stIBCRcvNode);
    // There is some valid message in IBC Receive Buffer
    // Try to process that message
    switch (stIBCRcvNode.g_ui8ArrIBCData[0])
    {
        case IBC_CMD_MODE_HOME:
            g_ui8SystemMode = SYS_MODE_HOME;
            break;
        case IBC_CMD_MODE_CALIB:
            g_ui8SystemMode = SYS_MODE_CALIB;
            break;
        case IBC_CMD_RESET_CALIB_POINT:
            // Get the Reset Point Value
            // Copy the Message for Parsing
            for (i8LoopCount = 1; i8LoopCount < IBC_MSG_BYTE_COUNT; i8LoopCount++)
            {
                g_unIBCResetCalib.m_ui8Arr[i8LoopCount -1] = stIBCRcvNode.g_ui8ArrIBCData[i8LoopCount];
            }
            g_fResetCalibStatus = IBC_RESET_CALIB_ON;
            
            break;
//        case IBC_CMD_RESET_CALIB_POINT_ACK:
//            for (i8LoopCount = 1; i8LoopCount < IBC_MSG_BYTE_COUNT; i8LoopCount++)
//            {
//                g_unIBCAvgNode.m_ui8Arr[i8LoopCount -1] = stIBCRcvNode.g_ui8ArrIBCData[i8LoopCount];
//            }
//            g_stCurBatchInfo.m_i16AvgWeight = g_unIBCAvgNode.m_stIBCAvgNode.m_i16AvgWt;
//            break;
            
        case IBC_CMD_SET_LOAD_CELL_FACTOR:
            // Request to update new calibration table.
            //UpdateNewCalibTable();
           
            // Receive current load cell factor.
            for (i8LoopCount = 1; i8LoopCount < IBC_MSG_BYTE_COUNT; i8LoopCount++)
            {
                g_unIBCCurrLoadCellFactor.m_ui8Arr[i8LoopCount - 1] = stIBCRcvNode.g_ui8ArrIBCData[i8LoopCount];
            }
            
            //g_stCurBatchInfo.m_i16AvgWeight = g_unIBCAvgNode.m_stIBCAvgNode.m_i16AvgWt;
            // Store current LCF(Load Cell Factor) in RAM.
            g_i16LoadCellFactor = (int16)g_unIBCCurrLoadCellFactor.m_stLoadCellFactor.m_ui16CurrLoadCellFactor;
            
            // Store the LCF in EEPROM to retrieve data on system restart.
            WriteLCFToEEPROM(g_i16LoadCellFactor);
            
            break;
            
        case IBC_CMD_MODE_PRE_PROD:
            g_ui8SystemMode = SYS_MODE_PRE_PROD;
            break;
        case IBC_CMD_MODE_PROD:
            g_ui8SystemMode = SYS_MODE_PROD;
            break;
        case IBC_CMD_GET_CALIB_POINT:
            break;
//        case IBC_CMD_RESET_CALIB_POINT:
//            break;
        case IBC_CMD_TARE_REQUEST:
            ResetCalibTable(g_ui16DynamicTareValue);
            break;
        case IBC_CMD_PRE_PROD_AVG_WEIGHT:
            // Copy the Message for Parsing
            for (i8LoopCount = 1; i8LoopCount < IBC_MSG_BYTE_COUNT; i8LoopCount++)
            {
                g_unIBCAvgNode.m_ui8Arr[i8LoopCount -1] = stIBCRcvNode.g_ui8ArrIBCData[i8LoopCount];
            }
            g_stCurBatchInfo.m_i16AvgWeight = g_unIBCAvgNode.m_stIBCAvgNode.m_i16AvgWt;
            break;
        case IBC_CMD_PRE_PROD_TOLERANCE:
            // Copy the Message for Parsing
            for (i8LoopCount = 1; i8LoopCount < IBC_MSG_BYTE_COUNT; i8LoopCount++)
            {
                g_unIBCTolNode.m_ui8Arr[i8LoopCount - 1] = stIBCRcvNode.g_ui8ArrIBCData[i8LoopCount];
            }
            g_stCurBatchInfo.m_i16PositiveTol = g_unIBCTolNode.m_stIBCTolNode.m_i16PositiveTol;
            g_stCurBatchInfo.m_i16NegativeTol = g_unIBCTolNode.m_stIBCTolNode.m_i16NegativeTol;
            break;
        case IBC_CMD_START_PROD:
            break;
        case IBC_CMD_MODE_AUTO_CALIB:
            // Copy the Data Length
            g_stAvgWtQueue.m_i8Length = stIBCRcvNode.g_ui8ArrIBCData[1];
            // Change the Rejection Mode to Auto Calib Mode
            g_fRejectionMode = HMI_REJECTION_AUTO_CALIB_MODE;
            break;
        case IBC_CMD_MODE_FIXED_CALIB:
            // Mrinmoy - start
            // Copy the Data Length
            g_stAvgWtQueue.m_i8Length = stIBCRcvNode.g_ui8ArrIBCData[1];
            // Mrinmoy - end
            // Change the Rejection Mode to Fixed Calib Mode
            g_fRejectionMode = HMI_REJECTION_FIXED_CALIB_MODE;
            break;
        default:
            break;
    }
}

void UpdateCalibTableToMCU1()
{
    
}
#endif	/* VM_IBC_PROCESS_M0_H */

