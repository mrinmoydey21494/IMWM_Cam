/* 
 * File:   viIMWIOManager.h
 * Author: OEM
 *
 * Created on April 29, 2018, 8:24 PM
 */

#ifndef VI_IMW_IO_MANAGER_H
#define	VI_IMW_IO_MANAGER_H

#include "viLdCDispModbusConfig.h"
#include "viCommon.h"
#include "viShiftRegister.h"
//#include "viCalibrationConfig.h"
//#include "viToleranceConfig.h"


#define FLAG_SHIPPER_REJECTION_ON         1
#define FLAG_SHIPPER_REJECTION_OFF        0

#define FLAG_WEIGHT_PASSED                1
#define FLAG_WEIGHT_FAILED                0

#define BEST_MEAN_SHIFT_VALUES_PERCENTAGE 30

////////////////////////////////////////////////////
#define STATE_WEIGHING_1 0
#define STATE_WEIGHING_2 4
#define STATE_WEIGHING_3 6
#define STATE_WEIGHING_4 2
#define STATE_WEIGHING_5 3
#define STATE_WEIGHING_6 1
#define STATE_WEIGHING_7 5
#define STATE_WEIGHING_8 7
    
#define FORCE_REJECTION_ON  1
#define FORCE_REJECTION_OFF 0


unsigned int8 g_ui8WeighingCurState  = STATE_WEIGHING_6;
unsigned int8 g_ui8WeighingPrevState = STATE_WEIGHING_6;
unsigned int8 g_ui8WeighingNextState = STATE_WEIGHING_1;

BYTE g_fForceRejection = FORCE_REJECTION_OFF;
/////////////////////////////////////////////////////////////////////


typedef struct _ST_MEAN_SHIFT_VALUE
{
	unsigned int8 ui8ActualIndex ;
	unsigned int8 ui8MeanShiftValue;
}ST_MEAN_SHIFT_VALUE;



BYTE g_fRejectionFlagStatus    = FLAG_SHIPPER_REJECTION_ON;
BYTE g_fShipperRejectionStatus = FLAG_WEIGHT_PASSED;
UN_SHIPPER_WEIGHT g_unShipperWeight;


//unsigned int16 g_ui16ArrMeanShiftVal[SIZE_LDCDISP_VALUE] = {0};
unsigned int8  g_ui8ArrSortedTemp[SIZE_LDCDISP_VALUE] = {0};
ST_MEAN_SHIFT_VALUE g_ui16ArrMeanShiftVal[SIZE_LDCDISP_VALUE] = {0};
ST_MEAN_SHIFT_VALUE g_ui16ArrSortedMeanShiftVal[SIZE_LDCDISP_VALUE] = {0};
//unsigned int16 g_ui16ArrSortedMeanShiftVal[SIZE_LDCDISP_VALUE] = {0};
unsigned int8  g_ui8SortedValueCount = 0;
int16 g_i16PrevMean = 999; // TESTING*****************************************************************


unsigned int16 GetMeanWeight()
{
    // Currently we are doing mean weight calculation
    // Later we will device some statistical measure to calculate the most stable weight
    
    unsigned int8  ui8LoopCount = 0;
    unsigned int16 ui16prevSum  = 0;				 
    unsigned int32 ui32Sum      = 0;
    unsigned int32 ui32Mean     = 0;
    unsigned int16 ui16Mean     = 0;
    
    // If 
    if (SIZE_LDCDISP_VALUE != g_ui8CountLdCDValue)
    {
        g_ui8CountLdCDValue = g_ui8WtLdCDIdx;
    }
    
    // Check valid entries are there or not
    if (!g_ui8CountLdCDValue)
    {
        // No such entry found
        // That might be due to the communication failure between MCU and the Load Cell Display Unit
        //return 0;
    }
    
    //ui16Mean = g_ui16ArrCurLdCDValue[0];
    // Calculate Mean
    for (ui8LoopCount = 0; ui8LoopCount < g_ui8CountLdCDValue; ui8LoopCount++) 
    {
	    
	        // Calculate sum.
        	ui32Sum = ui32Sum + g_ui16ArrCurLdCDValue[ui8LoopCount];
        
    }
    // Calculate Mean with left sum value if any.			   						//(unsigned int16)g_ui8CountLdCDValue)//***************************
	ui32Mean = (ui32Sum / g_ui8CountLdCDValue);
	ui16Mean = (unsigned int16) (ui32Mean & 0x0000FFFF);
    return ui16Mean;
}


int16 ErrorFactorCalculation(void)
{
	int16 i16ErrorFactor = 0;
	
	// Check for selector switch pin.
	// For 5240 gm shipper.
	if(PIN_HIGH == g_unDIState.m_stAllDIBits.m_ui1HWDICn03)
	{
		if(g_unCalibTable.m_stCalibTable.m_stArrTable[0].m_i16ErrorFactor > 0x8000)
		{
			i16ErrorFactor = (g_unCalibTable.m_stCalibTable.m_stArrTable[0].m_i16ErrorFactor & 0x7FFF);
			g_fErrorfactorStatus = NEGATIVE_ERROR_FACTOR;
		}
		else
		{
			i16ErrorFactor = g_unCalibTable.m_stCalibTable.m_stArrTable[0].m_i16ErrorFactor;
			g_fErrorfactorStatus = POSITIVE_ERROR_FACTOR;
		}
		
	}
	// For 7340 gm shipper.
	else if(PIN_HIGH == g_unDIState.m_stAllDIBits.m_ui1HWDICn05)
	{
		if(g_unCalibTable.m_stCalibTable.m_stArrTable[1].m_i16ErrorFactor > 0x8000)
		{
			i16ErrorFactor = (g_unCalibTable.m_stCalibTable.m_stArrTable[1].m_i16ErrorFactor & 0x7FFF);
			g_fErrorfactorStatus = NEGATIVE_ERROR_FACTOR;
		}
		else
		{
			i16ErrorFactor = g_unCalibTable.m_stCalibTable.m_stArrTable[1].m_i16ErrorFactor;
			g_fErrorfactorStatus = POSITIVE_ERROR_FACTOR;
		}
	}
	// For variety 3;
	else if(PIN_HIGH == g_unDIState.m_stAllDIBits.m_ui1HWDICn06)
	{
		if(g_unCalibTable.m_stCalibTable.m_stArrTable[2].m_i16ErrorFactor > 0x8000)
		{
			i16ErrorFactor = (g_unCalibTable.m_stCalibTable.m_stArrTable[2].m_i16ErrorFactor & 0x7FFF);
			g_fErrorfactorStatus = NEGATIVE_ERROR_FACTOR;
		}
		else
		{
			i16ErrorFactor = g_unCalibTable.m_stCalibTable.m_stArrTable[2].m_i16ErrorFactor;
			g_fErrorfactorStatus = POSITIVE_ERROR_FACTOR;
		}
	}
	// For variety 4;
	else if(PIN_HIGH == g_unDIState.m_stAllDIBits.m_ui1HWDICn07)
	{
		if(g_unCalibTable.m_stCalibTable.m_stArrTable[3].m_i16ErrorFactor > 0x8000)
		{
			i16ErrorFactor = (g_unCalibTable.m_stCalibTable.m_stArrTable[3].m_i16ErrorFactor & 0x7FFF);
			g_fErrorfactorStatus = NEGATIVE_ERROR_FACTOR;
		}
		else
		{
			i16ErrorFactor = g_unCalibTable.m_stCalibTable.m_stArrTable[3].m_i16ErrorFactor;
			g_fErrorfactorStatus = POSITIVE_ERROR_FACTOR;
		}
	}
	// For variety 5;
	 else if(PIN_HIGH == g_unDIState.m_stAllDIBits.m_ui1HWDICn08)
	 {
	 	if(g_unCalibTable.m_stCalibTable.m_stArrTable[4].m_i16ErrorFactor > 0x8000)
		{
			i16ErrorFactor = (g_unCalibTable.m_stCalibTable.m_stArrTable[4].m_i16ErrorFactor & 0x7FFF);
			g_fErrorfactorStatus = NEGATIVE_ERROR_FACTOR;
		}
		else
		{
			i16ErrorFactor = g_unCalibTable.m_stCalibTable.m_stArrTable[4].m_i16ErrorFactor;
			g_fErrorfactorStatus = POSITIVE_ERROR_FACTOR;
		}
	 }
	 
	 return i16ErrorFactor;
}

int16 DynamicErrorCalculation(void)
{
	 int32 i32Mean     = 0;
	 //int16 i16Mean     = 0;//*****************************************************
	 //int16 i16PrevMean = 0;
	 int32 i32Sum      = 0;
	 int16 i16PrevSum  = 0;
	 
	 int16 i16Temp     = 0;
	 int32 i32Temp     = 0;
	
	// Check for Valid entries.
	if(!g_ui8DECLdCDReceivedByteCount)
	{
		// No such valid entries found.
		return 0;
	}
	
	// valid entries found.
	// Check for sufficient number of data entry.
	// If suuficient data is not there take previous mean as current mean.
	if(g_ui8DECLdCDReceivedByteCount < ((DEC_LDCD_ARR_SIZE * 80) / 100))
	{
		//i16Mean = g_i16PrevMean;
		g_i16Mean = g_i16PrevMean;
	}
	
	else // Sufficient data is present.
	{
		// Calculate Mean.
		// If Current DEC array is not full calculate mean upto count value.
		if(DEC_LDCD_RECEIVED_BYTE_COUNT_GREATER_THAN_100 == g_fDECLdCDReceivedByteStatus)
		{
			for(int8 i8LoopCount = 0; i8LoopCount < DEC_LDCD_ARR_SIZE; i8LoopCount++)
			{
			    i16Temp = (int16)g_i16ArrCurrLdCDValuesDEC[i8LoopCount];
		        //i32Temp = (int32)i16Temp;
		        
				// Check for 16-bit signed integer overflow.
	        	if(i16Temp <= 0x7FFF)
	        	{
		        	i32Temp = (int32)i16Temp & 0x0000FFFF;
		        	// Calculate sum.
	        		i32Sum = i32Sum + i32Temp;
	        	}
	        	else
	        	{
		        	i16Temp = (~i16Temp + 1); //& 0x7FFF;
		        	i32Temp = (int32)i16Temp & 0x0000FFFF;
		        	//g_i16ArrCurrLdCDValuesDEC[i8LoopCount] &= 0x7FFF;
		        	i32Sum = i32Sum - i32Temp;
		        }
			}
			// Calculate Mean with left sum value if any.
			if (i32Sum > 0x80000000)
			{
				// So it is a negative value
				i32Sum = ~i32Sum +1;// & 0x7FFFFFFF;
				i32Mean = (i32Sum/(int32)(DEC_LDCD_ARR_SIZE));
				
				g_i16Mean = (int16)(i32Mean & 0x00007FFF);
				g_i16Mean = -g_i16Mean;
			}
			else
			{
				//i16Mean = (int16)(i32Mean & 0x00007FFF);
				
				// So it is a positive value
				//i32Sum = i32Sum & 0x7FFFFFFF;
				i32Mean = (i32Sum/(int32)(DEC_LDCD_ARR_SIZE));
				
				g_i16Mean = (int16)(i32Mean & 0x00007FFF);
			}		
		}
		
		else // Current DEC array is full so calculate mean of whole array; (array size -1).
		{
			for(int8 i8LoopCount = 0; i8LoopCount < g_ui8DECLdCDReceivedArrIndex; i8LoopCount++)
			{
			    i16Temp = (int16)g_i16ArrCurrLdCDValuesDEC[i8LoopCount];
		        //i32Temp = (int32)i16Temp;
		        
				// Check for 16-bit signed integer overflow.
	        	if(i16Temp <= 0x7FFF)
	        	{
		        	i32Temp = (int32)i16Temp & 0x0000FFFF;
		        	// Calculate sum.
	        		i32Sum = i32Sum + i32Temp;
	        	}
	        	else
	        	{
		        	i16Temp = (~i16Temp + 1);// & 0x7FFF;
		        	i32Temp = (int32)i16Temp & 0x0000FFFF;
		        	//g_i16ArrCurrLdCDValuesDEC[i8LoopCount] &= 0x7FFF;
		        	i32Sum = i32Sum - i32Temp;
		        }
		    }
			// Calculate Mean with left sum value if any.
			if (i32Sum > 0x80000000)
			{
				// So it is a negative value
				i32Sum = ~i32Sum +1;// & 0x7FFFFFFF;
				i32Mean = (i32Sum/(int32)(g_ui8DECLdCDReceivedArrIndex));
				
				g_i16Mean = (int16)(i32Mean & 0x00007FFF);
				g_i16Mean = -g_i16Mean;
			}
			else
			{
				//i16Mean = (int16)(i32Mean & 0x00007FFF);
				
				// So it is a positive value
				//i32Sum = i32Sum & 0x7FFFFFFF;
				i32Mean = (i32Sum/(int32)(g_ui8DECLdCDReceivedArrIndex));
				
				g_i16Mean = (int16)(i32Mean & 0x00007FFF);
			}
		}	
	}

	
	// Reset Current DEC array. 
	memset(g_i16ArrCurrLdCDValuesDEC , 0, (2 * DEC_LDCD_ARR_SIZE));
	// Reset Counter.
	g_ui8DECLdCDReceivedByteCount = 0;
	// Reset Index.
	g_ui8DECLdCDReceivedArrIndex = 0;
	// Reset Flag.
	g_fDECLdCDReceivedByteStatus = DEC_LDCD_RECEIVED_BYTE_COUNT_LESS_THAN_100;
	
	// Update previous Mean.
	g_i16PrevMean = g_i16Mean;
		
	// Total Dynamic Error value is ready, so send it. 
	return g_i16Mean;

}

unsigned int16 GetStatisticalWeight()
{
    unsigned int8  ui8LoopCount         = 0;
    unsigned int8  ui8LoopCount1        = 0;
    unsigned int16 ui16CurMean          = GetMeanWeight();
    unsigned int8  ui8CurArrLength      = 0;
    unsigned int16 ui16TempValue        = 0;
    unsigned int8 ui8TempIndex            = 0;
    unsigned int16 ui16CurSmallestShift = 0;
    unsigned int16  ui16BestMeanShiftValues = 0;
    int16    i16DynamicErrorValue         = 0;
    unsigned int16 ui16Temp               = 0;
    unsigned int16 ui16prevSum        	  = 0;				 
    unsigned int32 ui32Sum      		  = 0;
    unsigned int32 ui32Mean      		  = 0;
    unsigned int16 ui16LoopInit           = 0; 
    
    // 
    if (SIZE_LDCDISP_VALUE != g_ui8CountLdCDValue)
    {
        g_ui8CountLdCDValue = g_ui8WtLdCDIdx;
    }
    
    do
    {
        // Create the Mean Shift Array.
        for (ui8LoopCount = 0; ui8LoopCount < g_ui8CountLdCDValue; ui8LoopCount++) 
        {
            if (ui16CurMean >= g_ui16ArrCurLdCDValue[ui8LoopCount])
            {
                g_ui16ArrMeanShiftVal[ui8LoopCount].ui8MeanShiftValue = ui16CurMean - g_ui16ArrCurLdCDValue[ui8LoopCount];
                g_ui16ArrMeanShiftVal[ui8LoopCount].ui8ActualIndex = ui8LoopCount;
            }
            else
            {
                g_ui16ArrMeanShiftVal[ui8LoopCount].ui8MeanShiftValue = g_ui16ArrCurLdCDValue[ui8LoopCount] - ui16CurMean;
                g_ui16ArrMeanShiftVal[ui8LoopCount].ui8ActualIndex = ui8LoopCount;
            }
        }

		// copy mean shift array into another array for sorting.
		memcpy(g_ui16ArrSortedMeanShiftVal, g_ui16ArrMeanShiftVal, SIZE_LDCDISP_VALUE * sizeof(ST_MEAN_SHIFT_VALUE)); 
		
        // Now Sort This Mean Difference Table
        for (ui8LoopCount = 0; ui8LoopCount < (g_ui8CountLdCDValue - 1); ui8LoopCount++) 
        {
            for (ui8LoopCount1 = ui8LoopCount + 1; ui8LoopCount1 < g_ui8CountLdCDValue; ui8LoopCount1++) 
            {
                if (g_ui16ArrSortedMeanShiftVal[ui8LoopCount].ui8MeanShiftValue > g_ui16ArrSortedMeanShiftVal[ui8LoopCount1].ui8MeanShiftValue )
                { 
                    // For error checking of smallest number.
                    ui8TempIndex++;
                    
                    // Upgrade the Sort Table
                    //g_ui8ArrSortedTemp[ui8LoopCount] = ui8LoopCount1;
                    
                    // swaping the numbers in ascending order.
                    ui16TempValue = g_ui16ArrSortedMeanShiftVal[ui8LoopCount1].ui8MeanShiftValue;
                    ui8TempIndex  = g_ui16ArrSortedMeanShiftVal[ui8LoopCount1].ui8ActualIndex;
                    g_ui16ArrSortedMeanShiftVal[ui8LoopCount1].ui8MeanShiftValue = g_ui16ArrSortedMeanShiftVal[ui8LoopCount].ui8MeanShiftValue;
                    g_ui16ArrSortedMeanShiftVal[ui8LoopCount1].ui8ActualIndex    = g_ui16ArrSortedMeanShiftVal[ui8LoopCount].ui8ActualIndex;
                    g_ui16ArrSortedMeanShiftVal[ui8LoopCount].ui8MeanShiftValue  = ui16TempValue;
                    g_ui16ArrSortedMeanShiftVal[ui8LoopCount].ui8ActualIndex     = ui8TempIndex;
                }
            }

        }
        // Now we are having the Mean Shift sorted Array
        // Now we will take best 30% of total fetched weights from mean shift array.
        
        ui16BestMeanShiftValues = ((unsigned int16)g_ui8CountLdCDValue * LDCD_FILTER_MEAN);     //((unsigned int16)g_ui8CountLdCDValue)
        ui16BestMeanShiftValues = (ui16BestMeanShiftValues / 100);
        //ui16CurMean = g_ui16ArrCurLdCDValue[g_ui8ArrSortedTemp[0]];
        //ui8CurArrLength--;
        ui16LoopInit = (SIZE_LDCDISP_VALUE - ui16BestMeanShiftValues) / 2; // We are trying to get the middle portion
        ui16LoopInit = 0;
        // Calculate Mean of best 30% values deviated from mean.
        for (ui8LoopCount = ui16LoopInit; ui8LoopCount < (ui16LoopInit + ui16BestMeanShiftValues); ui8LoopCount++)
        {
        	// Calculate sum.
       		ui32Sum = ui32Sum + g_ui16ArrCurLdCDValue[g_ui16ArrSortedMeanShiftVal[ui8LoopCount].ui8ActualIndex];
        }
        // Calculate Mean with left sum value if any.
    	 ui32Mean = (ui32Sum / ui16BestMeanShiftValues);
    	 ui16CurMean = (unsigned int16) (ui32Mean & 0x00007FFF);
          
    } while (0); // Do it once.																		
    
    // Flush Load Cell Weighing Value Array.
    memset (g_ui16ArrCurLdCDValue, 0, (2 * SIZE_LDCDISP_VALUE)); 
    // Reset Counter.
    g_ui8CountLdCDValue = 0;
    g_ui8WtLdCDIdx      = 0;
    
    // Eliminate Dynamic Error.
    // Fetch Dynamic error value.
    i16DynamicErrorValue = DynamicErrorCalculation();
    
    // Check if positive error is there or negative.//************************************************
    if(i16DynamicErrorValue < 0x8000)
    {
	    // For positive error substract dynamic error from current mean.
    	ui16CurMean = (ui16CurMean - i16DynamicErrorValue);
    }
    else
    {
	    // For negative error add dynamic error with current mean.
    	ui16CurMean = ui16CurMean + (~i16DynamicErrorValue + 1);
    }
    
    
    // So last mean Value is the actual value.
    return ui16CurMean;
}


void SetShipperWeightVariety(void)
{
	 //For VARIETY 1.
    if(PIN_HIGH == g_unDIState.m_stAllDIBits.m_ui1HWDICn03)
    {
    	g_bCurrentVariety = VARITY_1;
    	SetCurrentToleranceNode();
    	g_i16ErrorFactor = ErrorFactorCalculation();
    }
    // For VARIETY 2.
    else if(PIN_HIGH == g_unDIState.m_stAllDIBits.m_ui1HWDICn05)
    {
    	g_bCurrentVariety = VARITY_2;
    	SetCurrentToleranceNode();
		g_i16ErrorFactor = ErrorFactorCalculation();
    }
    // For VARIETY 3.
    else if(PIN_HIGH == g_unDIState.m_stAllDIBits.m_ui1HWDICn06)
    {
    	g_bCurrentVariety = VARITY_3;
    	SetCurrentToleranceNode();
    	g_i16ErrorFactor = ErrorFactorCalculation();
    }
    // For VARIETY 4.
    else if(PIN_HIGH == g_unDIState.m_stAllDIBits.m_ui1HWDICn07)
    {
    	g_bCurrentVariety = VARITY_4;
    	SetCurrentToleranceNode();
    	g_i16ErrorFactor = ErrorFactorCalculation();
    }
    //For VARIETY 5.
    else if(PIN_HIGH == g_unDIState.m_stAllDIBits.m_ui1HWDICn08)
    {
    	g_bCurrentVariety = VARITY_5;
    	SetCurrentToleranceNode();
    	g_i16ErrorFactor = ErrorFactorCalculation();
    }	

}



void StateMachine_1_Action();
void StateMachine_2_Action();
void StateMachine_3_Action();
void StateMachine_4_Action();
void StateMachine_5_Action();
void StateMachine_6_Action();

void StateMachine_1_Action()
{
		// Step 1: Start Load Cell Display Data fetching Mode ON.
		//g_fLdCDispDataFetchState = FLAG_LDCDISP_DATA_FETCH_ON;
		
		// Step 1 : Conveyor is ready to take another shipper; Ready to Feed LED On.
		g_fReadyToFeedON = READY_TO_FEED_DELAY_ON;
		
		// Step 2: Start fetching data for Dynamic Error Calculation.
		g_fDynamicErrorCalculationStatus = DEC_ACTIVE_MODE;
		
		// Step 3: Send Modbus Command to Load Cell Display Unit.
//		FetchLoadCellData();
}

void StateMachine_2_Action()
{
    // Tare Data fetching off and calculate the Tare Value
    
    // Step 1: Stop LdCDisp Recurring Data Fetching Mode.
    //g_fLdCDispDataFetchState = FLAG_LDCDISP_DATA_FETCH_OFF;

    // Step 2: Stop fetching data for Dynamic Error Calculation.
    g_fDynamicErrorCalculationStatus = DEC_DEACTIVE_MODE;
    
    // Step 3: Shipper  already feeded; Don't feed another. Ready to Feed LED Off.
    g_unDOState.m_stAllDOBits.m_ui1HWDOCn03 = PIN_LOW;

}

void StateMachine_3_Action()
{
    // Major portion of the Box is on the Conveyor

    // Step 1: Raise flag to start time counting for Dynamic Speed Calculation.
    g_fDynamicSpeedCalculationStatus = DSC_ACTIVE_MODE;
}

void StateMachine_4_Action()
{
    // The complete box in weighing Conveyor
    // Start fetching Weighing Data from Load Cell Display
    
    // Step 1: Start Load Cell Display (LdCDisp) Recurring Data fetching Mode ON
    g_fLdCDispDataFetchState = FLAG_LDCDISP_DATA_FETCH_ON;

    // Step 2: Send Modbus Command to Load Cell Display Unit
//    FetchLoadCellData();
}

void StateMachine_5_Action()
{
	
	unsigned int16 ui16Temp = 0;
    BYTE bMSGStatus = g_bCurrentVariety;
	
    // Step 1: Stop LdCDisp Recurring Data Fetching Mode
    g_fLdCDispDataFetchState = FLAG_LDCDISP_DATA_FETCH_OFF;

    // Step 2: Set flag to start actual Dynamic Speed Calculation.
    g_fDynamicSpeedCalculationStatus = DSC_CALCULATIVE_MODE;

    // Step 3: Calculate the Statistical Value for this particular weight
    ui16Temp = GetStatisticalWeight();
    
    // Step 4: Check If it is in Production Mode
    if (MODE_PRODUCTION == g_bCurMode)
    {
	    
	    // Step 1: Take care of error factor.
	    // If positive error factor then subtract it.
	    if(POSITIVE_ERROR_FACTOR == g_fErrorfactorStatus)
	    {
    		ui16Temp = ui16Temp - g_i16ErrorFactor;
		}
		// For negative error factor add it.
		else
		{
			ui16Temp = ui16Temp + g_i16ErrorFactor;
		}
		
    	// Step 2: Get the Calibrated Value
    	g_unShipperWeight.m_ui16ShipperWeight = ui16Temp; 
    	
        // Step 3: Refer Tolerance Table to verify the permissible weight for the current variety
        g_fShipperRejectionStatus = VerifyTolerance(g_unShipperWeight.m_ui16ShipperWeight); 

        // Step 4: Set the Rejection Flag
        if (MSG_STATUS_DATA_LOGG_PASSED == g_fShipperRejectionStatus)
        {
            g_fRejectionFlagStatus = FLAG_SHIPPER_REJECTION_OFF;
            // Update rejection unit status.
            g_fCurrRejectionUnitStatus = REJECTION_UNIT_ENGAGE; 
            // Shipper accepted denoting GREEN LED On.
    		SetSystemLedState(LED_BOX_PASSED,LOGICAL_ON);
            // Set MSG Status
            bMSGStatus = g_bCurrentVariety;
        }
        else
        {
            g_fRejectionFlagStatus = FLAG_SHIPPER_REJECTION_ON;
            // Update rejection unit status.
            g_fCurrRejectionUnitStatus = REJECTION_UNIT_ENGAGE; 
            // Shipper Rejected denoting RED LED On.
	    	SetSystemLedState(LED_BOX_REJECTED,LOGICAL_ON);
            // Set MSG Status
            bMSGStatus = g_bCurrentVariety + MSG_STATUS_DATA_LOGG_REJECTION_OFSET;
        }
        
        //  Prepare the Data for PC to send
        g_bArrSendBufferUART2[SIZE_MSG_HEADER ] = g_unShipperWeight.m_bArr[0];
        g_bArrSendBufferUART2[SIZE_MSG_HEADER + 1] = g_unShipperWeight.m_bArr[1];
        
        // Send data to PC
        SendUART2(2, CMD_MCU_PC_DATA_LOGGING_KL_MODE, bMSGStatus, CRC_NOT_REQUIRED);
	}	
    else if (MODE_CALIBRATION == g_bCurMode)
    {
	    // Step 1: Get the calibrated value.
	    g_unShipperWeight.m_ui16ShipperWeight = ui16Temp;
	    
        // Step 2: Check if Calibration Point is Set by PC. 
       	if (g_unCurrentCalibPointValue.m_stCalibValue.m_ui16CalibPoint)										
         {
	          // Prepare the data
	          g_unCurrentCalibPointValue.m_stCalibValue.m_ui16CalibValue = g_unShipperWeight.m_ui16ShipperWeight;
	
	          // Copy the data to temporary message buffer
	          g_bArrSendBufferUART2[SIZE_MSG_HEADER] = g_unCurrentCalibPointValue.m_bArr[0];
	          g_bArrSendBufferUART2[SIZE_MSG_HEADER + 1] = g_unCurrentCalibPointValue.m_bArr[1];
	          g_bArrSendBufferUART2[SIZE_MSG_HEADER + 2] = g_unCurrentCalibPointValue.m_bArr[2];
	          g_bArrSendBufferUART2[SIZE_MSG_HEADER + 3] = g_unCurrentCalibPointValue.m_bArr[3];
	
	          // Send data to PC
	          SendUART2(4, CMD_PC_MCU_SET_CALIB_POINT, ++g_ui8CalibPointValueSequence, CRC_NOT_REQUIRED);
	      }
    }
	
}	
void StateMachine_6_Action()
{
    //Box is Leaving.
    
    // Nothing to do
    
}

void StateMachine(void)
{
	int8 i8Temp = -1;
	
    do
    {
        // If this new state is expected?
        if (g_ui8WeighingCurState != g_ui8WeighingNextState)
        {
            // Forced Rejection On
            g_fForceRejection      = FORCE_REJECTION_ON;
            
            // Shipper Rejected denoting RED LED On.
	    	SetSystemLedState(LED_BOX_REJECTED,LOGICAL_ON);
	    	
            g_ui8WeighingNextState = STATE_WEIGHING_1;
            
            break;
        }

        switch (g_ui8WeighingCurState)
        {
            case STATE_WEIGHING_1:
                 // While Nothing is on the Conveyor.
                StateMachine_1_Action();
                
                g_ui8WeighingNextState = STATE_WEIGHING_2;
                break;
            case STATE_WEIGHING_2:
                // Box Just Entered
                StateMachine_2_Action();
                
                g_ui8WeighingNextState = STATE_WEIGHING_3;
                break;
            case STATE_WEIGHING_3:
                // Major Portion of the Box on Weighing Conveyor
                StateMachine_3_Action();
                
                g_ui8WeighingNextState = STATE_WEIGHING_4;
                break;
            case STATE_WEIGHING_4:
                // The complete box in weighing Conveyor
                StateMachine_4_Action();
                
                g_ui8WeighingNextState = STATE_WEIGHING_5;
                break;
            case STATE_WEIGHING_5:
                // Box is about to leave weighing Conveyor
                StateMachine_5_Action();
                
                g_ui8WeighingNextState = STATE_WEIGHING_6;
                break;
            case STATE_WEIGHING_6:
                // Box is Leaving
                StateMachine_6_Action();
                
                g_ui8WeighingNextState = STATE_WEIGHING_1;
                break;
            case STATE_WEIGHING_7:
            case STATE_WEIGHING_8:
                // Box Overlapped in weighing Conveyor

                g_ui8WeighingNextState = STATE_WEIGHING_1;
                break;
            default:
                break;

        }
    } while (0);
}


void CheckDIStateChange()
{
    // Step 1: Check Which Mode is Selected now
    // Checking for Calibration Mode.
    if (PIN_HIGH == g_unDIState.m_stAllDIBits.m_ui1HWDICn10) 
    {
	    // This is in Calibration Mode
	    g_bCurMode = MODE_CALIBRATION;
	    SetSystemLedState(LED_PROD_MODE,LOGICAL_OFF); 
	    SetSystemLedState(LED_CALIB_MODE,LOGICAL_ON);
    }
    else
    {
	    // This is in Production mode.       
	    g_bCurMode = MODE_PRODUCTION; 
	    SetSystemLedState(LED_CALIB_MODE,LOGICAL_OFF); 
	    SetSystemLedState(LED_PROD_MODE,LOGICAL_ON);
    }
	

    
    // Step 2: Check for Weighing State
    g_ui8WeighingCurState = (!g_unDIState.m_stAllDIBits.m_ui1HWDICn01 * 1) +
                            (!g_unDIState.m_stAllDIBits.m_ui1HWDICn00 * 2) +
                            (!g_unDIState.m_stAllDIBits.m_ui1HWDICn11 * 4);
    
    // If There is a change found in the Weighing State
    if (g_ui8WeighingCurState != g_ui8WeighingPrevState)
    {
        // Update Previous State
        g_ui8WeighingPrevState = g_ui8WeighingCurState;
        
        // Check State Machine
        StateMachine();
    }
    

    // Step 3: check if shipper exit sensor is high.
    if(PIN_HIGH == g_unDIState.m_stAllDIBits.m_ui1HWDICn02)
    {
	    // Update current  rejection unit status.
	    g_fCurrRejectionUnitStatus = REJECTION_UNIT_FREE;
	    
	    // Checking for rejection.
    	if(FLAG_SHIPPER_REJECTION_ON == g_fRejectionFlagStatus ||
    	   FORCE_REJECTION_ON == g_fForceRejection)
    	{	    	
	    	// Opearate solenoid to reject shipper.
    		g_unDOState.m_stAllDOBits.m_ui1HWDOCn00 = PIN_HIGH;

    		// Set flag to start counter for solenoid back.
    		g_fSolenoidBackTimeCountStart = SOLENOID_BACK_START_TIME_COUNT;
            
            // Check rejection flag status.
            if(FLAG_SHIPPER_REJECTION_OFF == g_fRejectionFlagStatus)
            {
            	g_fRejectionFlagStatus = FLAG_SHIPPER_REJECTION_ON;
            }
    		
    		// Update Force rejection status.
    		if (FORCE_REJECTION_ON == g_fForceRejection)
                {
                    g_fForceRejection = FORCE_REJECTION_OFF;
                }
                
    		//After shipper passed RED LED Off.
        	SetSystemLedState(LED_BOX_REJECTED,LOGICAL_OFF);
    		
    	}
	
    	else
    	{
	    	
    		
    		//After shipper passed GREEN LED Off.
			SetSystemLedState(LED_BOX_PASSED,LOGICAL_OFF);
    	} 
    }
    else // If shipper exit sensor is low.
    {  
	    if(FLAG_SHIPPER_REJECTION_OFF == g_fRejectionFlagStatus &&
	       REJECTION_UNIT_FREE == g_fCurrRejectionUnitStatus)
	    {
    		// Update Rejection Flag.
    		g_fRejectionFlagStatus = FLAG_SHIPPER_REJECTION_ON;
    	}	
    }
    
    
     //Step 4: check current shipper weight variety.
    SetShipperWeightVariety();

}
    





#endif	/* VI_IMW_IO_MANAGER_H */

