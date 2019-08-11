/* 
 * File:   vmADCManager.h
 * Author: Amit
 *
 * Created on December 12, 2018, 11:38 PM
 */

#ifndef VM_ADCMANAGER_H
#define	VM_ADCMANAGER_H

#include "vmCalibration.h"
#include "vmM0ADCModuleCommon.h"
#include "vmAverageWeightCalculation.h"
#include "vmEEPROMConfig_Britania.h"

#define LOAD_CELL_FACTOR       818
//#define ADC_SCANNING_START     0
//#define ADC_SCANNING_RUNNING   1
//#define ADC_SCANNING_STOP      2
#define ADC_SCAN_ON            1
#define ADC_SCAN_OFF           0

#define TIMER_IGNORE_ON  1
#define TIMER_IGNORE_OFF 0

int8 g_fIgnore_TIMER = TIMER_IGNORE_OFF;


#define COLLECT_DYNAMIC_TARE_DATA      3
#define COLLECT_WEIGHING_DATA          4
#define COLLECT_NOTHING                5

#define SIZE_DATA_QUEUE              200
#define MAX_TARE_MULTIPLAYER         100

#define ADC_DATA_NOT_READY             0
#define ADC_DATA_READY                 1

#define ADC_DATA_Q_VALUES_TRIMMING_PERCNTG     30

#define ADC_MEAN_CALCULATION_START    0
#define ADC_MEAN_CALCULATION_STOP     1
#define ADC_MEAN_CALCULATION_RUNNING  2
#define ADC_MEAN_CALCULATION_DONE     3

unsigned int8 g_ui8ADCMeanCalState = ADC_MEAN_CALCULATION_STOP;
unsigned int32 g_ui32ADCCurMeanValue = 0;
unsigned int32 g_ui32ADCCurSum       = 0;
unsigned int16 g_ui16ADCSumLoopCount = 0;
int16          g_i16LoadCellFactor   = 0;



typedef struct _ST_ADC_DATA_QUEUE
{
    unsigned int32 m_ui32Arr[SIZE_DATA_QUEUE];
    int16          m_i16Rear;
    unsigned int8  m_ui8Multiplier;
    unsigned int32 m_ui32Sum;
    int8           m_i8CollectionState;
} ST_ADC_DATA_QUEUE;

typedef struct _ST_MEAN_SHIFT_VALUE
{
	unsigned int8 ui8ActualIndex ;
	unsigned int8 ui8MeanShiftValue;
}ST_MEAN_SHIFT_VALUE;

unsigned int8  g_ui8ADCScanningStatus   = ADC_SCAN_OFF;
unsigned int32 g_ui32ADCValue           = 0;
unsigned int8  g_ui8DataCollectionState = COLLECT_NOTHING;
unsigned int8  g_fADCDataReadyStatus    = ADC_DATA_NOT_READY;
unsigned int8 g_ui8CurMaxDataCount    = 0;

ST_ADC_DATA_QUEUE g_stADCDataQ;
unsigned int16    g_ui16NoOfADCData   = 0;
unsigned int32    g_ui32ADCAvg        = 0;

//unsigned int8       g_ui8ArrSortedTemp[SIZE_DATA_QUEUE]          = {0};
//ST_MEAN_SHIFT_VALUE g_ui16ArrMeanShiftVal[SIZE_DATA_QUEUE]       = {0};
//ST_MEAN_SHIFT_VALUE g_ui16ArrSortedMeanShiftVal[SIZE_DATA_QUEUE] = {0};

void ResetADCDataQ(int8 i8CollectionState);
unsigned int32 GetADCMeanValue();
unsigned int32 GetTareData();
int16          GetWeighingData(unsigned int32);
//void           IBCSendWeighingData();
//void           IBCSendTareData();
void           IBCSendTareData(unsigned int32);

void ResetADCDataQ(int8 i8CollectionState)
{
    g_stADCDataQ.m_i16Rear           = -1;
    g_stADCDataQ.m_ui32Sum           = 0;
    g_stADCDataQ.m_ui8Multiplier     = 0;
    g_stADCDataQ.m_i8CollectionState = i8CollectionState;
    
    //CHANGE_RUNNING_AVG START
    g_ui16NoOfADCData = 0;
    g_ui32ADCAvg      = 0;
    //CHANGE_RUNNING_AVG END
}

unsigned int32 GetADCMeanValue()
{
    unsigned int8  ui8LoopCount     = 0;
    unsigned int32 ui32DataSum      = 0;
    //unsigned int32 ui32ADCMean      = 0;
    
    // There is no Data then return 0
    if (-1 == g_stADCDataQ.m_i16Rear)
    {
        return 0;
    }
    // Check the value of the Multiplier
    if (g_stADCDataQ.m_ui8Multiplier)
    {
        g_ui8CurMaxDataCount = SIZE_DATA_QUEUE;
    }
    else
    {
        g_ui8CurMaxDataCount = g_stADCDataQ.m_i16Rear + 1;
    }
    //output_high(PIN_OUT_DO_02);
    // Get the mean Value
    for (ui8LoopCount = 0; ui8LoopCount < g_ui8CurMaxDataCount; ui8LoopCount++)
    {
        ui32DataSum += g_stADCDataQ.m_ui32Arr[ui8LoopCount];
        //ui32DataSum += 65500;
        //output_toggle(PIN_OUT_DO_02);
    }
    //output_low(PIN_OUT_DO_02);
    // TESTING Start
    // Reset Data Queue
    //ResetADCDataQ(COLLECT_NOTHING);  //TODO It should not be here, Reset should be done from State Machine
    // TESTING End
    // Now return Mean Value
    return(ui32DataSum / g_ui8CurMaxDataCount);
}

unsigned int8 StartADCSplitAvg()
{
    g_ui32ADCCurMeanValue = 0;
    g_ui32ADCCurSum       = 0;
    g_ui8CurMaxDataCount  = 0;
    g_ui16ADCSumLoopCount = 0;
    
    // There is no Data then return 0
    if (-1 == g_stADCDataQ.m_i16Rear)
    {
        g_ui8ADCMeanCalState  = ADC_MEAN_CALCULATION_DONE;
        return;
    }
    // Check the value of the Multiplier
    if (g_stADCDataQ.m_ui8Multiplier)
    {
        g_ui8CurMaxDataCount = SIZE_DATA_QUEUE;
    }
    else
    {
        g_ui8CurMaxDataCount = g_stADCDataQ.m_i16Rear + 1;
    }
    
    g_ui8ADCMeanCalState  = ADC_MEAN_CALCULATION_RUNNING;
////    if(g_stADCDataQ.m_i8CollectionState == COLLECT_DYNAMIC_TARE_DATA)
////    {
////        g_ui16ADCSumLoopCount = 20;
////    }
}

void GetADCSplitSum()
{
 
    if (g_ui16ADCSumLoopCount < g_ui8CurMaxDataCount)
    {
        g_ui32ADCCurSum += g_stADCDataQ.m_ui32Arr[g_ui16ADCSumLoopCount];
        // Increment Loop Count
        g_ui16ADCSumLoopCount++;
    }
    else if (g_ui16ADCSumLoopCount >= g_ui8CurMaxDataCount)
    {
        // Calculate the mean
        g_ui32ADCCurMeanValue = g_ui32ADCCurSum / g_ui8CurMaxDataCount;
        
        // Update State
        g_ui8ADCMeanCalState  = ADC_MEAN_CALCULATION_DONE;
    }
////    else
////    {
////        // This will occur whe
////        g_ui32ADCCurMeanValue = 0;
////    }    
    
}

unsigned int32 GetADCSplitAvg()
{
    
}

void SortADCDataQ(void)
{
    unsigned int8 ui8LoopCount = 0;
    unsigned int8 ui8LoopCount1 = 0;
    unsigned int8 ui8CurrMaxDataCount = 0;
    unsigned int32 ui32Temp = 0;
    
    // There is no Data then return 0
    if (-1 == g_stADCDataQ.m_i16Rear)
    {
        return ;
    }
    // Check the value of the Multiplier
    if (g_stADCDataQ.m_ui8Multiplier)
    {
        ui8CurrMaxDataCount = SIZE_DATA_QUEUE;
    }
    else
    {
        ui8CurrMaxDataCount = g_stADCDataQ.m_i16Rear + 1;
    }
    
    // Loop through the array to sort ADC data..Using Bubble Sort.
    for(ui8LoopCount = 0; ui8LoopCount < (ui8CurrMaxDataCount - 1); ui8LoopCount++)
    {
        for(ui8LoopCount1 = ui8LoopCount + 1; ui8LoopCount1 < ui8CurrMaxDataCount; ui8LoopCount1++)
        {
            if(g_stADCDataQ.m_ui32Arr[ui8LoopCount] > g_stADCDataQ.m_ui32Arr[ui8LoopCount1])
            {
                ui32Temp = g_stADCDataQ.m_ui32Arr[ui8LoopCount1];
                g_stADCDataQ.m_ui32Arr[ui8LoopCount1] = g_stADCDataQ.m_ui32Arr[ui8LoopCount];
                g_stADCDataQ.m_ui32Arr[ui8LoopCount] = ui32Temp;
            }
        }
    }    
    
}

unsigned int32 GetADCMeanT(void)
{
    unsigned int32 ui8CountL = 0;
    unsigned int32 ui8CountH = 0;
    unsigned int8 ui8CurrMaxDataCount = 0;
    unsigned int8 ui8LoopCount =  0;
    unsigned int32 ui32DataSum = 0;
    
    // Check the value of the Multiplier
    if (g_stADCDataQ.m_ui8Multiplier)
    {
        ui8CurrMaxDataCount = SIZE_DATA_QUEUE;
    }
    else
    {
        ui8CurrMaxDataCount = g_stADCDataQ.m_i16Rear + 1;
    }
    
    ui8CountL = ((ui8CurrMaxDataCount * ADC_DATA_Q_VALUES_TRIMMING_PERCNTG) / 100);
    ui8CountH = (ui8CurrMaxDataCount - ui8CountL);
    
    // Get the mean Value
    for (ui8LoopCount = ui8CountL; ui8LoopCount < ui8CountH; ui8LoopCount++)
    {
        ui32DataSum += g_stADCDataQ.m_ui32Arr[ui8LoopCount];
    }
    
    return (ui32DataSum / (ui8CountH - ui8CountL));
    
}

//unsigned int16 GetStatisticalWeight()
//{
//    unsigned int8  ui8LoopCount         = 0;
//    unsigned int8  ui8LoopCount1        = 0;
//    unsigned int16 ui16CurMean          = GetADCMeanValue();
//    unsigned int8  ui8CurArrLength      = 0;
//    unsigned int16 ui16TempValue        = 0;
//    unsigned int8 ui8TempIndex            = 0;
//    unsigned int16 ui16CurSmallestShift = 0;
//    unsigned int16  ui16BestMeanShiftValues = 0;
//    int16    i16DynamicErrorValue         = 0;
//    unsigned int16 ui16Temp               = 0;
//    unsigned int16 ui16prevSum        	  = 0;				 
//    unsigned int32 ui32Sum      		  = 0;
//    unsigned int32 ui32Mean      		  = 0;
//    unsigned int16 ui16LoopInit           = 0; 
//    
//    // 
//    if (SIZE_LDCDISP_VALUE != g_ui8CountLdCDValue)
//    {
//        g_ui8CountLdCDValue = g_ui8WtLdCDIdx;
//    }
//    
//    do
//    {
//        // Create the Mean Shift Array.
//        for (ui8LoopCount = 0; ui8LoopCount < g_ui8CountLdCDValue; ui8LoopCount++) 
//        {
//            if (ui16CurMean >= g_ui16ArrCurLdCDValue[ui8LoopCount])
//            {
//                g_ui16ArrMeanShiftVal[ui8LoopCount].ui8MeanShiftValue = ui16CurMean - g_ui16ArrCurLdCDValue[ui8LoopCount];
//                g_ui16ArrMeanShiftVal[ui8LoopCount].ui8ActualIndex = ui8LoopCount;
//            }
//            else
//            {
//                g_ui16ArrMeanShiftVal[ui8LoopCount].ui8MeanShiftValue = g_ui16ArrCurLdCDValue[ui8LoopCount] - ui16CurMean;
//                g_ui16ArrMeanShiftVal[ui8LoopCount].ui8ActualIndex = ui8LoopCount;
//            }
//        }
//
//		// copy mean shift array into another array for sorting.
//		memcpy(g_ui16ArrSortedMeanShiftVal, g_ui16ArrMeanShiftVal, SIZE_LDCDISP_VALUE * sizeof(ST_MEAN_SHIFT_VALUE)); 
//		
//        // Now Sort This Mean Difference Table
//        for (ui8LoopCount = 0; ui8LoopCount < (g_ui8CountLdCDValue - 1); ui8LoopCount++) 
//        {
//            for (ui8LoopCount1 = ui8LoopCount + 1; ui8LoopCount1 < g_ui8CountLdCDValue; ui8LoopCount1++) 
//            {
//                if (g_ui16ArrSortedMeanShiftVal[ui8LoopCount].ui8MeanShiftValue > g_ui16ArrSortedMeanShiftVal[ui8LoopCount1].ui8MeanShiftValue )
//                { 
//                    // For error checking of smallest number.
//                    ui8TempIndex++;
//                    
//                    // Upgrade the Sort Table
//                    //g_ui8ArrSortedTemp[ui8LoopCount] = ui8LoopCount1;
//                    
//                    // swaping the numbers in ascending order.
//                    ui16TempValue = g_ui16ArrSortedMeanShiftVal[ui8LoopCount1].ui8MeanShiftValue;
//                    ui8TempIndex  = g_ui16ArrSortedMeanShiftVal[ui8LoopCount1].ui8ActualIndex;
//                    g_ui16ArrSortedMeanShiftVal[ui8LoopCount1].ui8MeanShiftValue = g_ui16ArrSortedMeanShiftVal[ui8LoopCount].ui8MeanShiftValue;
//                    g_ui16ArrSortedMeanShiftVal[ui8LoopCount1].ui8ActualIndex    = g_ui16ArrSortedMeanShiftVal[ui8LoopCount].ui8ActualIndex;
//                    g_ui16ArrSortedMeanShiftVal[ui8LoopCount].ui8MeanShiftValue  = ui16TempValue;
//                    g_ui16ArrSortedMeanShiftVal[ui8LoopCount].ui8ActualIndex     = ui8TempIndex;
//                }
//            }
//
//        }
//        // Now we are having the Mean Shift sorted Array
//        // Now we will take best 30% of total fetched weights from mean shift array.
//        
//        ui16BestMeanShiftValues = ((unsigned int16)g_ui8CountLdCDValue * LDCD_FILTER_MEAN);     //((unsigned int16)g_ui8CountLdCDValue)
//        ui16BestMeanShiftValues = (ui16BestMeanShiftValues / 100);
//        //ui16CurMean = g_ui16ArrCurLdCDValue[g_ui8ArrSortedTemp[0]];
//        //ui8CurArrLength--;
//        ui16LoopInit = (SIZE_LDCDISP_VALUE - ui16BestMeanShiftValues) / 2; // We are trying to get the middle portion
//        ui16LoopInit = 0;
//        // Calculate Mean of best 30% values deviated from mean.
//        for (ui8LoopCount = ui16LoopInit; ui8LoopCount < (ui16LoopInit + ui16BestMeanShiftValues); ui8LoopCount++)
//        {
//        	// Calculate sum.
//       		ui32Sum = ui32Sum + g_ui16ArrCurLdCDValue[g_ui16ArrSortedMeanShiftVal[ui8LoopCount].ui8ActualIndex];
//        }
//        // Calculate Mean with left sum value if any.
//    	 ui32Mean = (ui32Sum / ui16BestMeanShiftValues);
//    	 ui16CurMean = (unsigned int16) (ui32Mean & 0x00007FFF);
//          
//    } while (0); // Do it once.																		
//    
//    // Flush Load Cell Weighing Value Array.
//    memset (g_ui16ArrCurLdCDValue, 0, (2 * SIZE_LDCDISP_VALUE)); 
//    // Reset Counter.
//    g_ui8CountLdCDValue = 0;
//    g_ui8WtLdCDIdx      = 0;
//    
//    // Eliminate Dynamic Error.
//    // Fetch Dynamic error value.
//    i16DynamicErrorValue = DynamicErrorCalculation();
//    
//    // Check if positive error is there or negative.//************************************************
//    if(i16DynamicErrorValue < 0x8000)
//    {
//	    // For positive error substract dynamic error from current mean.
//    	ui16CurMean = (ui16CurMean - i16DynamicErrorValue);
//    }
//    else
//    {
//	    // For negative error add dynamic error with current mean.
//    	ui16CurMean = ui16CurMean + (~i16DynamicErrorValue + 1);
//    }
//    
//    
//    // So last mean Value is the actual value.
//    return ui16CurMean;
//}

unsigned int32 GetStatisticalMean()
{
    
}

int16 GetCalibratedWeight(unsigned int32 ui32ADCMean)
{
    int16 i16ActualWeight = 0;
    int32    i32Temp = 0;
    unsigned int32 ui32DiffValue = 0;
    
    // Check Which one is greater
    if (g_ui32DynamicTareADCValue > ui32ADCMean)
    {
        // Should not be
        ui32DiffValue = g_ui32DynamicTareADCValue - ui32ADCMean;
        i32Temp = (int32)ui32DiffValue * (int32)g_i16LoadCellFactor;
        i32Temp /= 1000;
        i16ActualWeight = (int16)i32Temp;
        i16ActualWeight = ~i16ActualWeight + 1;
    }
    else
    {
        ui32DiffValue = ui32ADCMean - g_ui32DynamicTareADCValue;
        i32Temp = (int32)ui32DiffValue * (int32)g_i16LoadCellFactor;
        i32Temp /= 1000;
        i16ActualWeight = (int16)i32Temp;
        
    }
    
    return i16ActualWeight;
}

//int16 GetCalibratedWeight(unsigned int32 ui32ADCMean)
//{
//    int16 i16ActualWeight = 0;
//    int8  i8LoopCount     = 0;
//    int16    i16Temp = 0;
//    int32    i32Temp = 0;
//    unsigned int16 ui16DiffValue = 0;
//    unsigned int16 ui16Factor = 0; // Usually 1000, just to get better value we are using scale of 100
//    
//    
//    //g_stCalibTable[SIZE_CALIB_TABLE];
//    // Check if the Value falls within the Calibration Table or not
//    if (ui32ADCMean < g_stCalibTable[0].m_ui32CalibValue)
//    {
//
//        // Value is in negative range
//        ui16DiffValue = (g_stCalibTable[1].m_ui32CalibValue - g_stCalibTable[0].m_ui32CalibValue);
//        // Get the Factor
//        ui16Factor = 10000 / ui16DiffValue;
//
//        i16Temp = ((g_stCalibTable[0].m_ui32CalibValue - ui32ADCMean) * ui16Factor) - 
//                  (g_stCalibTable[0].m_i16CalibPoint * 10);
//        i16ActualWeight = (int16)i16Temp/10;
//        i16ActualWeight = ~i16ActualWeight;  // As this is a negative value
//        
//
//
//    }
//    if (ui32ADCMean > g_stCalibTable[SIZE_CALIB_TABLE - 1].m_ui32CalibValue)
//    {
//        // Value is greater than Calibtable range
//        ui16DiffValue = (g_stCalibTable[SIZE_CALIB_TABLE - 1].m_ui32CalibValue - g_stCalibTable[SIZE_CALIB_TABLE - 2].m_ui32CalibValue);
//        ui16Factor = 10000 / ui16DiffValue;
//        
//        i32Temp = (int32)(g_stCalibTable[SIZE_CALIB_TABLE - 1].m_i16CalibPoint) * (int32)(10);
//        i32Temp += ((ui32ADCMean - g_stCalibTable[SIZE_CALIB_TABLE - 1].m_ui32CalibValue) * (int32)(ui16Factor));
//                   
//        i32Temp /= 10;
//        i16ActualWeight = (int16)i32Temp;
//        
//    }
//    else
//    {
//        // Value within Calibration Table Range
//        // Seek Table
//        for (i8LoopCount = 1; i8LoopCount < SIZE_CALIB_TABLE; i8LoopCount++)
//        {
//            if (g_stCalibTable[i8LoopCount].m_ui32CalibValue == ui32ADCMean)
//            {
//                i16ActualWeight = g_stCalibTable[i8LoopCount].m_i16CalibPoint;
//                break;
//            }
//            else if (g_stCalibTable[i8LoopCount].m_ui32CalibValue > ui32ADCMean)
//            {
//                break;
//            }
//        }
//
//        if (!i16ActualWeight)
//        {
//            // Calculate weight
//            ui16DiffValue = (g_stCalibTable[i8LoopCount].m_ui32CalibValue - g_stCalibTable[i8LoopCount - 1].m_ui32CalibValue);
//            ui16Factor = 10000/ui16DiffValue;
//
//            i32Temp  = (int32)(g_stCalibTable[i8LoopCount - 1].m_i16CalibPoint * (int32)(10));
//            i32Temp  += ((ui32ADCMean - g_stCalibTable[i8LoopCount - 1].m_ui32CalibValue)*(int32)ui16Factor);
//            i32Temp /= 10;
//            i16ActualWeight = (int16)i32Temp;
//        }
//        
//    }
//    
//    return i16ActualWeight;
//}

int16 GetWeighingData(unsigned int32 ui32ADCMean)
{
    int16 i16CalibratedWeight = 0;
    unsigned int16 ui16TempTareWeight   = 0;
    unsigned int16 ui16TempDynErrValue  = 0;    
    
    i16CalibratedWeight = GetCalibratedWeight(ui32ADCMean);
    
////////    // Deduct Dynamic Tare Value
////////    ui16TempTareWeight = g_ui16DynamicTareValue;
////////    if (g_ui16DynamicTareValue > 0x8000)
////////    {
//////////        ui16TempTareWeight = ui16TempTareWeight << 1;
//////////        ui16TempTareWeight = ui16TempTareWeight >> 1;
////////        ui16TempTareWeight = ~ui16TempTareWeight + 1;
////////        
////////        i16CalibratedWeight += (int16)ui16TempTareWeight;
////////    }
////////    else
////////    {
////////        i16CalibratedWeight -= (int16)ui16TempTareWeight;
////////    }

//AMIT
////////    // Deduct Dynamic Error Value
////////    ui16TempDynErrValue = (unsigned int16)g_i16DynamicErrValue;
////////    if (g_i16DynamicErrValue > 0x8000)
////////    {
//////////        ui16TempDynErrValue = ui16TempDynErrValue << 1;
//////////        ui16TempDynErrValue = ui16TempDynErrValue >> 1;
////////        ui16TempDynErrValue = ~ui16TempDynErrValue + 1;
////////        
////////        i16CalibratedWeight += (int16)ui16TempDynErrValue;
////////    }
////////    else
////////    {
////////        i16CalibratedWeight -= (int16)ui16TempDynErrValue;
////////    }
    
    return i16CalibratedWeight;
}

//int16 GetWeighingData(unsigned int32 ui32ADCMean)
//{
//    int16 i16CalibratedWeight = 0;
//    unsigned int16 ui16TempTareWeight   = 0;
//    unsigned int16 ui16TempDynErrValue  = 0;    
//    
//    //unsigned int32 ui32ADCMean = GetADCMeanValue();
//    i16CalibratedWeight = GetCalibratedWeight(ui32ADCMean);
//    
//    // Deduct Dynamic Tare Value
//    ui16TempTareWeight = g_ui16DynamicTareValue;
//    if (g_ui16DynamicTareValue > 0x8000)
//    {
////        ui16TempTareWeight = ui16TempTareWeight << 1;
////        ui16TempTareWeight = ui16TempTareWeight >> 1;
//        ui16TempTareWeight = ~ui16TempTareWeight + 1;
//        
//        i16CalibratedWeight += (int16)ui16TempTareWeight;
//    }
//    else
//    {
//        i16CalibratedWeight -= (int16)ui16TempTareWeight;
//    }
//
//    // Deduct Dynamic Error Value
//    ui16TempDynErrValue = (unsigned int16)g_i16DynamicErrValue;
//    if (g_i16DynamicErrValue > 0x8000)
//    {
////        ui16TempDynErrValue = ui16TempDynErrValue << 1;
////        ui16TempDynErrValue = ui16TempDynErrValue >> 1;
//        ui16TempDynErrValue = ~ui16TempDynErrValue + 1;
//        
//        i16CalibratedWeight += (int16)ui16TempDynErrValue;
//    }
//    else
//    {
//        i16CalibratedWeight -= (int16)ui16TempDynErrValue;
//    }
//    
//    return i16CalibratedWeight;
//}




void IBCSendWeighingData(int16 i16BoxWeight, unsigned int16 ui16BoxCount)
{
    ST_IBC_DATA_NODE stIBCDataNode;
    int8             i8LoopCount         = 0;
    int16            i16AvgWeight        = 0;

    // Check for Rejection in Fixed Calib Mode
    if (HMI_REJECTION_FIXED_CALIB_MODE == g_fRejectionMode)
    {
        i16AvgWeight = g_stCurBatchInfo.m_i16AvgWeight;
    }
    else
    {
        i16AvgWeight = g_stAvgWtQueue.m_i16CurAvg;
    }
    
    // Now Check if the Weight is within tolerance limit or not
    if (i16BoxWeight > (i16AvgWeight + g_stCurBatchInfo.m_i16PositiveTol) || 
        i16BoxWeight < (i16AvgWeight - g_stCurBatchInfo.m_i16NegativeTol))
    {
        g_ui16RejectionStatus = BOX_REJECTED;
        // Set Rejection State ON
        g_unDIState.m_stAllDIBits.m_ui1GDICn01 = BOX_REJECTED;
    }
    else
    {
        g_ui16RejectionStatus = BOX_ACCEPTED;
        // Set Rejection State OFF
        g_unDIState.m_stAllDIBits.m_ui1GDICn01 = BOX_ACCEPTED;
    }
    
//    // Now Check if the Weight is within tolerance limit or not.
//    // The under weight and over weight rejection is also segregated here and accordingly send the status to server.
//    if (i16BoxWeight > (i16AvgWeight + g_stCurBatchInfo.m_i16PositiveTol)) 
//        
//    {
//        g_ui16RejectionStatus = BOX_REJECTED_OVR_WT;
//        // Set Rejection State ON
//        g_unDIState.m_stAllDIBits.m_ui1GDICn01 = BOX_REJECTED_UNDR_WT; // Here under weight is used only to maintain #define value to actuate rejection DO.
//    }
//    else if(i16BoxWeight < (i16AvgWeight - g_stCurBatchInfo.m_i16NegativeTol))
//    {
//        g_ui16RejectionStatus = BOX_REJECTED_UNDR_WT;
//        // Set Rejection State ON
//        g_unDIState.m_stAllDIBits.m_ui1GDICn01 = BOX_REJECTED_UNDR_WT;
//    }
//    else
//    {
//        g_ui16RejectionStatus = BOX_ACCEPTED;
//        // Set Rejection State OFF
//        g_unDIState.m_stAllDIBits.m_ui1GDICn01 = BOX_ACCEPTED;
//    }
    
    // Prepare IBC Data Node
    g_unIBCProdModeWtData.m_stWeighingData.m_ui16BoxCount = ui16BoxCount;
    g_unIBCProdModeWtData.m_stWeighingData.m_ui16BoxCount <<= 1;
    g_unIBCProdModeWtData.m_stWeighingData.m_ui16BoxCount
            = g_unIBCProdModeWtData.m_stWeighingData.m_ui16BoxCount | g_ui16RejectionStatus;
    
    g_unIBCProdModeWtData.m_stWeighingData.m_i16BoxWeight = i16BoxWeight;
    
    
//    // Prepare IBC Data Node
//    g_unIBCProdModeWtData.m_stWeighingData.m_ui16BoxCount = ui16BoxCount;
//    g_unIBCProdModeWtData.m_stWeighingData.m_ui16BoxCount <<= 2;
//    g_unIBCProdModeWtData.m_stWeighingData.m_ui16BoxCount
//            = g_unIBCProdModeWtData.m_stWeighingData.m_ui16BoxCount | g_ui16RejectionStatus;
//    
//    g_unIBCProdModeWtData.m_stWeighingData.m_i16BoxWeight = i16BoxWeight;
    
    // Stuff IBC Command
    stIBCDataNode.g_ui8ArrIBCData[0] = IBC_CMD_PROD_WEIGH_DATA;
    for (i8LoopCount = 1; i8LoopCount < IBC_MSG_BYTE_COUNT; i8LoopCount++)
    {
        stIBCDataNode.g_ui8ArrIBCData[i8LoopCount] = g_unIBCProdModeWtData.m_ui8Arr[i8LoopCount - 1];
    }
    
    // Send to IBC Insert
    //InsertSendQueue(&stIBCDataNode);
    InsertIBCMsgQueue(&g_stIBCSendQueue, &stIBCDataNode);
    InsertIBCMsgQueue(&g_stIBCSendQueue, &stIBCDataNode);
    //InsertIBCMsgQueue(&g_stIBCSendQueue, &stIBCDataNode);
}

void IBCSendWtInterMidData(int16 i16BoxWeight)
{
    ST_IBC_DATA_NODE stIBCDataNode;
    int8             i8LoopCount         = 0;
    int16            i16AvgWeight        = 0;
   
    
    // Prepare IBC Data Node
    g_unIBCProdModeWtData.m_stWeighingData.m_ui16BoxCount = 0;
    
    g_unIBCProdModeWtData.m_stWeighingData.m_i16BoxWeight = i16BoxWeight;
    
    // Stuff IBC Command
    stIBCDataNode.g_ui8ArrIBCData[0] = IBC_CMD_PROD_INTMD_DATA;
    for (i8LoopCount = 1; i8LoopCount < IBC_MSG_BYTE_COUNT; i8LoopCount++)
    {
        stIBCDataNode.g_ui8ArrIBCData[i8LoopCount] = g_unIBCProdModeWtData.m_ui8Arr[i8LoopCount - 1];
    }
    
    // Send to IBC Insert
    //InsertSendQueue(&stIBCDataNode);
    InsertIBCMsgQueue(&g_stIBCSendQueue, &stIBCDataNode);
}

void IBCSendTareData(unsigned int32 ui32ADCMean)
{
    ST_IBC_DATA_NODE stIBCDataNode;
    int8 i8LoopCount = 0;
    int16  i16TempDynTare = 0;

    i16TempDynTare = GetCalibratedWeight(ui32ADCMean);
        
//    // Update Dynamic Tare only if there are more than 200 data available in DataQ
//    if (ADC_MEAN_DATA_FINAL_TARE  == g_ui8ADCMeanDataState && g_stADCDataQ.m_ui8Multiplier > 0)
//    {
//        g_ui16DynamicTareValue   = i16TempDynTare;
//        g_ui32DynamicTareADCValue = ui32ADCMean;
//    }
    
    // Update Dynamic Tare only if there are more than 200 data available in DataQ
    if (g_stADCDataQ.m_ui8Multiplier > 0)
    {
        g_ui16DynamicTareValue   = i16TempDynTare;
        g_ui32DynamicTareADCValue = ui32ADCMean;
    }
    // Prepare IBC Data Node
    g_unIBCProdModeTrData.m_stTareData.m_i16Padding  = 0;
    g_unIBCProdModeTrData.m_stTareData.m_i16TareData = i16TempDynTare;
    
    // Stuff IBC Command
    stIBCDataNode.g_ui8ArrIBCData[0] = IBC_CMD_PROD_TARE_DATA;

    for (i8LoopCount = 1; i8LoopCount < IBC_MSG_BYTE_COUNT; i8LoopCount++)
    {
        stIBCDataNode.g_ui8ArrIBCData[i8LoopCount] = g_unIBCProdModeTrData.m_ui8Arr[i8LoopCount - 1];
    }

    // Send to IBC Insert
    //InsertSendQueue(&stIBCDataNode);
    InsertIBCMsgQueue(&g_stIBCSendQueue, &stIBCDataNode);
}

//void IBCSendTareData(unsigned int32 ui32ADCMean)
//{
//    ST_IBC_DATA_NODE stIBCDataNode;
//    int8 i8LoopCount = 0;
//    unsigned int32 ui32TempDynTare = 0;
//    // Sort the values of data queue.
//    //SortADCDataQ();
//    
//    //ui32ADCMean = GetADCMeanValue();
//     
//    ui32TempDynTare = GetCalibratedWeight(ui32ADCMean);
//        
//    // Update Dynamic Tare
//    if (ADC_MEAN_DATA_FINAL_TARE  == g_ui8ADCMeanDataState)
//    {
//        g_ui16DynamicTareValue   = ui32TempDynTare;
//        g_ui32DynamicTareADCValue = ui32ADCMean;
//    }
//    // Prepare IBC Data Node
//    g_unIBCProdModeTrData.m_stTareData.m_i16Padding  = 0;
//    g_unIBCProdModeTrData.m_stTareData.m_i16TareData = ui32TempDynTare;
//    
//    // Stuff IBC Command
//    stIBCDataNode.g_ui8ArrIBCData[0] = IBC_CMD_PROD_TARE_DATA;
//
//    for (i8LoopCount = 1; i8LoopCount < IBC_MSG_BYTE_COUNT; i8LoopCount++)
//    {
//        stIBCDataNode.g_ui8ArrIBCData[i8LoopCount] = g_unIBCProdModeTrData.m_ui8Arr[i8LoopCount - 1];
//    }
//
//    // Send to IBC Insert
//    //InsertSendQueue(&stIBCDataNode);
//    InsertIBCMsgQueue(&g_stIBCSendQueue, &stIBCDataNode);
//}

void IBCSendCalibResetData(unsigned int32 ui32ADCMean)
{
    ST_IBC_DATA_NODE stIBCDataNode;
    int8 i8LoopCount = 0;
    
    //CHANGE_RUNNING_AVG START
    
    // 1. Send Dynamic Tare ADC Value : g_ui32DynamicTareADCValue
    // 2. Send Current Dynamic Weight : ui32ADCMean
    
    // Set dynamic tare ADC value and weight ADC value in variables.
    g_unIBCResetCalibTare.m_stIBCResetCalibTare.m_ui32CalibADCForTare = g_ui32DynamicTareADCValue;
    g_unIBCResetCalibWeight.m_stIBCResetCalibWeight.m_ui32CalibADCForWeight = ui32ADCMean;
    
    // Stuff IBC Command.
    // 1. Dynamic Tare ADC value.
    stIBCDataNode.g_ui8ArrIBCData[0] = IBC_CMD_RESET_CALIB_POINT_TARE;
    for (i8LoopCount = 1; i8LoopCount < IBC_MSG_BYTE_COUNT; i8LoopCount++)
    {
        stIBCDataNode.g_ui8ArrIBCData[i8LoopCount] = g_unIBCResetCalibTare.m_ui8Arr[i8LoopCount - 1];
    }
    
    // Insert in IBC queue.
    InsertIBCMsgQueue(&g_stIBCSendQueue, &stIBCDataNode);
    
    // 2. Current Dynamic Weight ADC value.
    stIBCDataNode.g_ui8ArrIBCData[0] = IBC_CMD_RESET_CALIB_POINT_WEIGHT;
    for (i8LoopCount = 1; i8LoopCount < IBC_MSG_BYTE_COUNT; i8LoopCount++)
    {
        stIBCDataNode.g_ui8ArrIBCData[i8LoopCount] = g_unIBCResetCalibWeight.m_ui8Arr[i8LoopCount - 1];
    }
    
    // Insert in IBC queue.
    InsertIBCMsgQueue(&g_stIBCSendQueue, &stIBCDataNode);
    
    // Reset start calibration flag.
    g_fResetCalibStatus = IBC_RESET_CALIB_OFF;

}

//void IBCSendCalibResetData(unsigned int32 ui32ADCMean)
//{
//    ST_IBC_DATA_NODE stIBCDataNode;
//    int8 i8LoopCount = 0;
//    
//    //CHANGE_RUNNING_AVG START
//    // Sort the values of data queue.
//    //SortADCDataQ();
//    
//    //unsigned int32 ui32ADCMean = GetADCMeanT();
//    
//    //CHANGE_RUNNING_AVG END
//    
//    // Update Calib Reset Value
//    g_i16CalibResetValue = GetCalibratedWeight(ui32ADCMean);
//    
//    // Deduct the Dynamic Tare Value
//    g_i16CalibResetValue -= g_ui16DynamicTareValue;
//    
//    // Prepare IBC Data Node
//    g_i16CalibResetSumValue += g_i16CalibResetValue;
//    g_unIBCResetCalibAck.m_stIBCResetCalibPoint.m_i16PassCount++;
//    g_unIBCResetCalibAck.m_stIBCResetCalibPoint.m_i16CalibValue =
//            (g_i16CalibResetSumValue/g_unIBCResetCalibAck.m_stIBCResetCalibPoint.m_i16PassCount);
//    
//    // Calculate Calib value for particular calib pont
//    g_i32ADCSumValue += ui32ADCMean;
//    g_i32ADCAvgValue = (g_i32ADCSumValue/g_unIBCResetCalibAck.m_stIBCResetCalibPoint.m_i16PassCount);
//
//    // Stuff IBC Command
//    stIBCDataNode.g_ui8ArrIBCData[0] = IBC_CMD_RESET_CALIB_POINT_ACK;
//    for (i8LoopCount = 1; i8LoopCount < IBC_MSG_BYTE_COUNT; i8LoopCount++)
//    {
//        stIBCDataNode.g_ui8ArrIBCData[i8LoopCount] = g_unIBCResetCalibAck.m_ui8Arr[i8LoopCount - 1];
//    }
//    
//    // Send to IBC Insert
//    //InsertSendQueue(&stIBCDataNode);
//    InsertIBCMsgQueue(&g_stIBCSendQueue, &stIBCDataNode);
//    
//    if (MAX_CALIB_RESET_PASS == g_unIBCResetCalibAck.m_stIBCResetCalibPoint.m_i16PassCount)
//    {
//        // Set Dynamic Err Adjustment Value
//        g_i16DynamicErrValue = g_unIBCResetCalibAck.m_stIBCResetCalibPoint.m_i16CalibValue -
//                g_unIBCResetCalib.m_stIBCResetCalibPoint.m_i16CalibPoint;
//        
//
//        // Set Dynamic Error Value to EEPROM
//        //WriteDynamicErrToEEPROM(g_i16DynamicErrValue);
//
//        // Reset all temp values
//        g_i16CalibResetSumValue     = 0;
//        g_i16CalibResetValue        = 0;
//        g_i32ADCSumValue            = 0;
//        g_i32ADCAvgValue            = 0;
//        g_ui16CalibPointEEPROM      = 0;
//        g_ui16ADCCalibValueEEPROM   = 0;
//        g_unIBCResetCalibAck.m_stIBCResetCalibPoint.m_i16CalibValue = 0;
//        g_unIBCResetCalibAck.m_stIBCResetCalibPoint.m_i16PassCount  = 0;
//        
//        g_fResetCalibStatus = IBC_RESET_CALIB_OFF;
//    }
//}

void UpdateNewCalibTable(void)
{
    int8  i8LoopCount = 0;
    unsigned int16 ui16CalibValueEEPROM = 0;
    unsigned int16 ui16CalibPointEEPROM = 0;
    
    // Loop through the entire calibration table
    for (i8LoopCount = 0; i8LoopCount < SIZE_CALIB_TABLE; i8LoopCount++)
    {
        // Update in RAM
        g_stCalibTable[i8LoopCount].m_i16CalibPoint = g_stTempCalibTable[i8LoopCount].m_i16CalibPoint;
        g_stCalibTable[i8LoopCount].m_ui32CalibValue = g_stTempCalibTable[i8LoopCount].m_ui32CalibValue;
        
        // Update in EEPROM
        ui16CalibValueEEPROM = (unsigned int16)g_stTempCalibTable[i8LoopCount].m_ui32CalibValue;
        ui16CalibPointEEPROM = (unsigned int16)g_stTempCalibTable[i8LoopCount].m_i16CalibPoint;
        //WriteDataToEEPROM(ui16CalibPointEEPROM, ui16CalibValueEEPROM);
    }
}


void AfterADCMeanCalculationDone()
{
    // Intermediate Data Send for Weight
    if (ADC_MEAN_DATA_INTERMEDIATE_WEIGHT == g_ui8ADCMeanDataState)
    {
        // ADC Mean Calculation Done
        g_i16CurBoxWeight = GetWeighingData(g_ui32ADCCurMeanValue);
        IBCSendWtInterMidData(g_i16CurBoxWeight);
    }
    // Intermediate Data Send for Tare
    if (ADC_MEAN_DATA_INTERMEDIATE_TARE == g_ui8ADCMeanDataState)
    {
        //output_toggle(PIN_OUT_DO_02);
        // ADC Mean Calculation Done
        IBCSendTareData(g_ui32ADCCurMeanValue);
        //output_toggle(PIN_OUT_DO_02);
    }

    // Final Weight Calculation
    if (ADC_MEAN_DATA_FINAL_WEIGHT == g_ui8ADCMeanDataState)
    {
        // ADC Mean Calculation Done
        // Step 3: Send Final Tare data to IBC
        //output_toggle(PIN_OUT_DO_02);
        g_i16CurBoxWeight = GetWeighingData(g_ui32ADCCurMeanValue);
        //output_toggle(PIN_OUT_DO_02);
        IBCSendWeighingData(g_i16CurBoxWeight, g_ui16CurrentBoxCount);

        // Step 4: Insert Last Box Data to Average Queue
        InsertAvgWtQueue(g_i16CurBoxWeight);

        // Step 5: Send Current average Weight through IBC
        IBCSendProdModeAvgData();

        // Step 6: Reset ADC Data Queue
        ResetADCDataQ(COLLECT_NOTHING);

        g_fWtCalculationState = FLAG_WT_CALCULATION_DONE;
    }

    // Final Tare Value Set
    if (ADC_MEAN_DATA_FINAL_TARE  == g_ui8ADCMeanDataState)
    {
        // ADC Mean Calculation Done
        IBCSendTareData(g_ui32ADCCurMeanValue);
        ResetADCDataQ(COLLECT_NOTHING);
    }
    
    if (ADC_MEAN_DATA_CALIB_WEIGHT  == g_ui8ADCMeanDataState)
    {
        // ADC Mean Calculation Done
        IBCSendCalibResetData(g_ui32ADCCurMeanValue);
        ResetADCDataQ(COLLECT_NOTHING);
    }
    
    // Reset Flags
    g_ui8ADCMeanCalState  = ADC_MEAN_CALCULATION_STOP;
    g_ui8ADCMeanDataState = ADC_MEAN_DATA_NOTHING;
}
#endif	/* VM_ADCMANAGER_H */

