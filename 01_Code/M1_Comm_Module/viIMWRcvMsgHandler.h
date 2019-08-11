/* 
 * File:   viIMWRcvMsgHandler.h
 * Author: OEM
 *
 * Created on April 27, 2018, 6:43 PM
 */

#ifndef VI_IMW_RCV_MSG_HANDLER_H
#define	VI_IMW_RCV_MSG_HANDLER_H

#include "viIMWMsgConfig.h"
#include "viCRCConfig.h"

#define MSG_CUR_STATUS_UNDEFINED_MSG      1
#define MSG_CUR_STATUS_ETX_FOUND          2
#define MSG_CUR_STATUS_FIRST_STX_FOUND    3
#define MSG_CUR_STATUS_ALL_STX_FOUND      4
#define MSG_CUR_STATUS_ALL_STX_NOT_FOUND  5
#define MSG_CUR_STATUS_FRONT_AT_MSG_START 6
#define MSG_CUR_STATUS_QUEUE_EMPTY        7
#define MSG_CUR_STATUS_CRC_PASSED         8
#define MSG_CUR_STATUS_CRC_FAILED         9

BYTE g_bArrUART2MSGTemp[SIZE_UART2_MSG_BUFFER] = {0};
BYTE g_bCurMSGStatus = MSG_CUR_STATUS_UNDEFINED_MSG;

void CheckETXandProcessData(void)
{
    BYTE bData            = 0;
	BYTE ui8ETXIndex      = 0;
	BYTE ui8LoopCount     = 0;
	int8 i8STXCountTemp   = 0;
	BYTE ui8CommandNumber = 0;
	unsigned int8 ui8DataArr[100] = {0};
	unsigned int16 ui16CRCValue = 0;
    int8 iLoopCount                  = 0;
    BYTE bIsQueueEmpty    = MSG_QUEUE_EMPTY;
    
    BYTE bMSGLength     = 0;
    BYTE bMSGCMD        = 0;
    BYTE bMSGStatusByte = 0;
    int16 i16CRCValue   = 0;
	
	// checking for End Text Count.
	if(COUNT_ETX == g_stUART2RcvBuff.m_bCountETX)
	{
        g_bCurMSGStatus = MSG_CUR_STATUS_ETX_FOUND;   // Only ETX Found

		// Traverse through entire UART2 Receive buffer queue.
		// Start Checking STX from Front of the Queue upto Rear of Queue
		do
		{
            bData = GetQueue(&bIsQueueEmpty);
            
            if (MSG_QUEUE_EMPTY == bIsQueueEmpty)
            {
                // Invalid data Possible only when Queue is empty
                // Reset Queue info
                g_stUART2RcvBuff.m_bCountETX = 0;
                
                // And Return
                return;
            }
            
			// Remove junk if STX is not present
			while (STX != bData)
			{
                // Go for the next Byte from Queue
                bData = GetQueue(&bIsQueueEmpty);
                if (MSG_QUEUE_EMPTY == bIsQueueEmpty)
                {
                    break;
                }
			}
            if (MSG_QUEUE_EMPTY == bIsQueueEmpty)
            {
                break;
            }	
            g_bCurMSGStatus = MSG_CUR_STATUS_FIRST_STX_FOUND;
            // Now Check for three consecutive STX as per protocol
            bData = GetQueue(&bIsQueueEmpty);
            if (MSG_QUEUE_EMPTY == bIsQueueEmpty)
            {
                break;
            }
            if (STX == bData)
            {
                bData = GetQueue(&bIsQueueEmpty);
                if (MSG_QUEUE_EMPTY == bIsQueueEmpty)
                {
                    break;
                }
                if (STX == bData)
                {
                    // This is a Valid MSG Start
                    g_bCurMSGStatus = MSG_CUR_STATUS_ALL_STX_FOUND;
                }
                else
                {
                    // Not a Valid MSG Start
                    g_bCurMSGStatus = MSG_CUR_STATUS_ALL_STX_NOT_FOUND;
                }
            }
            else
            {
                // Not a Valid MSG Start
                g_bCurMSGStatus = MSG_CUR_STATUS_ALL_STX_NOT_FOUND;
            }
            
            // If a Valid MSG Start Found
            if (MSG_CUR_STATUS_ALL_STX_FOUND == g_bCurMSGStatus)
            {
                // Start Putting this message to the Temporary Message Array
                bMSGLength = g_bArrUART2MSGTemp[0] = GetQueue(&bIsQueueEmpty);  // MSG Length
                if (MSG_QUEUE_EMPTY == bIsQueueEmpty)
                {
                    g_bCurMSGStatus = MSG_CUR_STATUS_QUEUE_EMPTY;
                    break;
                }
                bMSGCMD = g_bArrUART2MSGTemp[1] = GetQueue(&bIsQueueEmpty);  // MSG Command
                if (MSG_QUEUE_EMPTY == bIsQueueEmpty)
                {
                    g_bCurMSGStatus = MSG_CUR_STATUS_QUEUE_EMPTY;
                    break;
                }
                bMSGStatusByte = g_bArrUART2MSGTemp[2] = GetQueue(&bIsQueueEmpty);  // MSG Status Byte
                if (MSG_QUEUE_EMPTY == bIsQueueEmpty)
                {
                    g_bCurMSGStatus = MSG_CUR_STATUS_QUEUE_EMPTY;
                    break;
                }
                
                // Start filling the message Data
                for (iLoopCount = 0; iLoopCount < bMSGLength; iLoopCount++)
                {
                    g_bArrUART2MSGTemp[iLoopCount + 3] = GetQueue(&bIsQueueEmpty);
                    if (MSG_QUEUE_EMPTY == bIsQueueEmpty)
                    {
                        g_bCurMSGStatus = MSG_CUR_STATUS_QUEUE_EMPTY;
                        break;
                    }
                }
                
                // Now Get the CRC Bytes
                
                g_bArrUART2MSGTemp[3 + bMSGLength] = GetQueue(&bIsQueueEmpty);
                //i16CRCValue <<= 8;
                g_bArrUART2MSGTemp[3 + bMSGLength + 1] = GetQueue(&bIsQueueEmpty);
                //bData = GetQueue(&bIsQueueEmpty);
                
                
                // Check the CRC
                g_bCurMSGStatus = MSG_CUR_STATUS_CRC_FAILED;
                if (bMSGLength > 0)
                {
                    // CRC Calculation process.
                    ui16CRCValue = calculateCrc(g_bArrUART2MSGTemp + 3, bMSGLength);
                    // If Valid CRC then
                    // Checking calculated CRC, with the CRC received from PC end.
                    if((ui16CRCValue >> 8) == g_bArrUART2MSGTemp[3 + bMSGLength + 1] &&
                       (ui16CRCValue & 0x00FF) == g_bArrUART2MSGTemp[3 + bMSGLength])
                       {
                           g_bCurMSGStatus = MSG_CUR_STATUS_CRC_PASSED;
                       }
                }
                else  // bMSGLength == 0; Messages with no data
                {
                    // NO CRC Check required
                    g_bCurMSGStatus = MSG_CUR_STATUS_CRC_PASSED;
                }
                
                if (MSG_CUR_STATUS_QUEUE_EMPTY == g_bCurMSGStatus)
                {
                    break;
                }
                
                // Remove three ETX from message 
                //GetQueue();GetQueue();GetQueue();

            }
            
            else
            {
                // Valid Start MSG Not found
                // Might be a corrupt message, just leave it, will be truncated while we get next ETX
                
            }
            
            // Reset ETX Count
            g_stUART2RcvBuff.m_bCountETX -= COUNT_ETX;
            
		} while (0);
		
//        if (MSG_QUEUE_EMPTY == bIsQueueEmpty)
//        {
//            // Invalid data Possible only when Queue is empty
//            // Reset Queue info
//            g_stUART2RcvBuff.m_bCountETX = 0;
//
//            // And Return
//            return;
//        }
		//if (1)
        // Check Message status
        if (MSG_CUR_STATUS_CRC_PASSED == g_bCurMSGStatus)  
        {
            // If a valid message is there in the temporary message buffer
            switch (bMSGCMD)
            {
                case 20:
                	// This Command will send weighted & Calibrated carton data for PC Log.
                    break;
                case 30:
                    // This command means it receives Tolerance Table
                    // Send it to Tolerance Table Receive Proc
                    RcvProcToleranceTable(g_bArrUART2MSGTemp + 3);
                    
                    break;
                case 41:
                    // This Command means PC is Asking for Complete Calibration Table from MCU if Status Byte is 0xFF
                    if (0xFF == bMSGStatusByte)
                    {
                        RcvProcCalibTableRequest();
                    }
				    else if (0 == bMSGStatusByte)
                    {
                        // This is the PC Acknowledgment for Successful Calibration Table Transfer
                    }
                    else if(0x01 == bMSGStatusByte) // This is for CRC failure; resend the calib table.
                    {
                        RcvProcCalibTableRequest();
                    }
                    else // For other status.
                    {
                    	// Unhandled.
                    }
                    break;

                case 42:
                    // PC has sent one Calibration Set point
                    RcvProcSetCalibPoint(g_bArrUART2MSGTemp + 3);
                    break;

                case 43:
                    // This Command is to set the Complete Calibration table to EEPROM
                    RcvProcSetCalibTable(g_bArrUART2MSGTemp + 3);

                    break;	
                default :
                    break;
            }
            
            // Reset all MSG Parameters involves in this message retrieval process
            memset(g_bArrUART2MSGTemp, 0, SIZE_UART2_MSG_BUFFER);
            
        }
        else
        {
            // Not a valid message.
            // Send Failure Acknowledgement On CRC Fail.
            // This is for Tolerance table updation failure; Request for resend.
            if(bMSGCMD == 0x1E)
            {
	            // Stuff 0 as data.
				g_bArrSendBufferUART2[SIZE_MSG_HEADER] = 0x00;
				g_bArrSendBufferUART2[SIZE_MSG_HEADER + 1] = 0x00;
            	SendUART2(0, CMD_PC_MCU_SET_TOLERANCE_TABLE, 255, CRC_NOT_REQUIRED);
            }
            // This is for calibration table updation failure; Request for resend.
            else if(bMSGCMD == 0x2B)
            {
	            // Stuff 0 as data.
				g_bArrSendBufferUART2[SIZE_MSG_HEADER] = 0x00;
				g_bArrSendBufferUART2[SIZE_MSG_HEADER + 1] = 0x00;
            	SendUART2(0, CMD_PC_MCU_SET_CALIB_TABLE, 255, CRC_NOT_REQUIRED);																		//CMD_PC_MCU_SET_CALIB_TABLE
            }
            else
            {
            	// Do Nothing.
            }
            
           	// Reset all MSG Parameters involves in this message retrieval process
            memset(g_bArrUART2MSGTemp, 0, SIZE_UART2_MSG_BUFFER);
                 
        }

	}
	else   // If No ETX Found
	{
		
	}	
}


#endif	/* VI_IMW_RCV_MSG_HANDLER_H */

