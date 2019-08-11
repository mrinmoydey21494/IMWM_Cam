#ifndef VM_IMW_COMMON_H
#define VM_IMW_COMMON_H

#define MODE_PRODUCTION                  0 
#define MODE_CALIBRATION                 1

#define HIGHEST_16_BIT_UNSIGNED_INTERGER 65535

#define FLAG_LDCDISP_DATA_FETCH_ON       1
#define FLAG_LDCDISP_DATA_FETCH_OFF      0

#define SIZE_LDCDISP_VALUE               100

#define PIN_HIGH                         0  
#define PIN_LOW                          1 

#define VARITY_1                         1
#define VARITY_2                         2
#define VARITY_3                         3
#define VARITY_4                         4
#define VARITY_5                         5
#define VARITY_6                         6

#define READY_TO_FEED_DELAY_ON			 12
#define READY_TO_FEED_DELAY_OFF			 13

#define READY_TO_FEED_DELAY_TIMEOUT      1000

#define POSITIVE_ERROR_FACTOR            17
#define NEGATIVE_ERROR_FACTOR  			 18
#define NO_ERROR_FACTOR					 19


//**************** For Dynamic Error Calculation ************************
	
#define DEC_ACTIVE_MODE                  45
#define DEC_DEACTIVE_MODE                46	

#define DEC_LDCD_ARR_SIZE                200
#define DEC_LDCD_RECEIVED_BYTE_COUNT_GREATER_THAN_100     12
#define DEC_LDCD_RECEIVED_BYTE_COUNT_LESS_THAN_100        13  

#define ERROR_FACTOR_FOR_2000GM							  15
#define ERROR_FACTOR_FOR_5240GM							  19
#define ERROR_FACTOR_FOR_7340GM							  19

//************************** End ****************************************

//****************  For Dynamic Speed Calculation ***********************

#define DSC_ACTIVE_MODE                  67
#define DSC_DECATIVE_MODE                68
#define DSC_CALCULATIVE_MODE             69

#define DISTANCE_MEASUREMENT_FOR_DSC     100

//*************************** End ***************************************

//******************************************************
//************* Shift Register Delays ******************
//******************************************************

#define REJECTION_SOLENOID_BACK_TIMEOUT  470
#define PC_COMM_TX_LED_OFF_TIMEOUT       200
#define PC_COMM_RX_LED_OFF_TIMEOUT		 200
#define LDCD_TX_LED_OFF_TIMEOUT          200
#define LDCD_RX_LED_OFF_TIMEOUT          200

#define SOLENOID_BACK_START_TIME_COUNT    1
#define SOLENOID_BACK_STOP_TIME_COUNT     0

#define PC_TX_LED_OFF_START_TIME_COUNT       20
#define PC_TX_LED_OFF_STOP_TIME_COUNT        21

#define PC_RX_LED_OFF_START_TIME_COUNT       40
#define PC_RX_LED_OFF_STOP_TIME_COUNT        41

#define LDCD_TX_LED_OFF_START_TIME_COUNT     60
#define LDCD_TX_LED_OFF_STOP_TIME_COUNT      61

#define LDCD_RX_LED_OFF_START_TIME_COUNT     80
#define LDCD_RX_LED_OFF_STOP_TIME_COUNT      81

#define LDCD_TX_LED_ON						 30
#define LDCD_TX_LED_OFF						 31

#define LDCD_RX_LED_ON					     35
#define LDCD_RX_LED_OFF					     36

#define PC_COMM_RX_LED_ON                    45
#define PC_COMM_RX_LED_OFF					 46

#define REJECTION_UNIT_ENGAGE				 75
#define REJECTION_UNIT_FREE					 76

//*******************************************************
//************** End ************************************
//*******************************************************

#define LDCD_FILTER_TOLERANCE 5   // 50% Tolerance for Filter Value (Base is 10, Means 10 = 100%)
#define LDCD_FILTER_MEAN      50  // 80% Max is 100

//typedef union _UN_SHIPPER_WEIGHT
//{
//    unsigned int16 m_ui16ShipperWeight;
//    BYTE           m_bArr[2];
//} UN_SHIPPER_WEIGHT;
//
//
////BYTE g_bCurMode  = MODE_PRODUCTION; 
////BYTE g_fLdCDispDataFetchState = FLAG_LDCDISP_DATA_FETCH_OFF;
//
////*************************************************************************************
//// *********************** For Collection Of Running Weight ***************************
////*************************************************************************************
//#define SIZE_RUNNING_WEIGHT_ARRAY                      200
//typedef struct _ST_RUNNING_WEIGHT_ARRAY
//{
//    int16           m_i16ArrRunningWeight[SIZE_RUNNING_WEIGHT_ARRAY];
//    int16           m_i16Rear;
//    unsigned int8   m_fRunningWeightArrayFull;
//} ST_RUNNING_WEIGHT_ARRAY;
//
//ST_RUNNING_WEIGHT_ARRAY g_stRunningWeightArray;
//
//// ****************************** End ************************************************* 
//// ************************************************************************************
////************** Shift Register  Global Variables *************************************
////*************************************************************************************
//
////unsigned int16 g_fSolenoidBackTimeCountStart = SOLENOID_BACK_STOP_TIME_COUNT;
////
////unsigned int8  g_fPCCommTxLEDOffTimeCountStart = PC_TX_LED_OFF_STOP_TIME_COUNT;
////unsigned int8  g_fPCCommRxLEDOffTimeCountStart = PC_RX_LED_OFF_STOP_TIME_COUNT;
////unsigned int8  g_fLdCDTxLEDOffTimeCountStart   = LDCD_TX_LED_OFF_STOP_TIME_COUNT;
////unsigned int8  g_fLdCDRxLEDOffTimeCountStart   = LDCD_RX_LED_OFF_STOP_TIME_COUNT;
////unsigned int8  g_fLdCDTxLEDStatus              = LDCD_TX_LED_OFF;
////unsigned int8  g_fLdCDRxLEDStatus    		   = LDCD_RX_LED_OFF;
////unsigned int8  g_fPCCommRxLEDStatus			   = PC_COMM_RX_LED_OFF;
//
////*************************************************************************************
////**************************** End ****************************************************
////*************************************************************************************
//
////unsigned int16        g_ui16ArrCurLdCDValue[SIZE_LDCDISP_VALUE];  // Here we are holding the Load Cell Display Value
////unsigned int8         g_ui8CountLdCDValue = 0;                    // This variable holds how many entries are
////unsigned int8         g_ui8WtLdCDIdx      = 0;
////                                                 
////unsigned int8 g_fDynamicSpeedCalculationStatus      = DSC_DECATIVE_MODE;   
////unsigned int16 g_ui16DynamicSpeedCalculationCounter = 0;     
////unsigned int8 g_ui8ConveyorDynamicSpeed             = 0; 
////BYTE g_fReadyToFeedON                               =  READY_TO_FEED_DELAY_OFF;
////BYTE g_fErrorfactorStatus 							=  NO_ERROR_FACTOR;
////BYTE g_fCurrRejectionUnitStatus						=  REJECTION_UNIT_FREE;
//
////*************************************************************************************
//// *********************** For Dynamic Error Calculation ******************************
////*************************************************************************************
//
////unsigned int8 g_fDynamicErrorCalculationStatus = DEC_DEACTIVE_MODE;
//// 
////int16 g_i16ArrCurrLdCDValuesDEC[DEC_LDCD_ARR_SIZE] = {0};
////unsigned int8 g_ui8DECLdCDReceivedArrIndex      = 0;
////unsigned int8 g_ui8DECLdCDReceivedByteCount     = 0;
////BYTE  g_fDECLdCDReceivedByteStatus              = DEC_LDCD_RECEIVED_BYTE_COUNT_LESS_THAN_100;
//
//// ****************************** End *************************************************      
//                                                                  
//BYTE g_fProcessModbusData = 0;
//
//int16 g_i16LdCDValueMax = 0;
//int16 g_i16LdCDValueMin = 0;
//
//void DELAY(unsigned int16 time)
//{
//	unsigned int16 ui16Counter = 0;
//	while(ui16Counter < time)
//	{
//		delay_ms(1);
//		ui16Counter++;
//	}
//}
//
//
////TESTING
//int16 g_i16Mean = 0;
//
//int16 g_i16ErrorFactor = 0;
//
//// AK - start
//// Init running weight array
//void InitRunningWeightArray(void)
//{
//    g_stRunningWeightArray.m_fRunningWeightArrayFull = 0;
//    g_stRunningWeightArray.m_i16Rear = -1;
//}
//
//// Is any data in running weight array
//int8 HasRunningWeightArrayData(void)
//{
//    int8 i8HasData = (g_stRunningWeightArray.m_i16Rear) > 0 ? 1 : 0;
//    return i8HasData;
//}
//
//// Push Running data in data structure
//void PushRunningWeightData(int16 i16Data)
//{
//    g_stRunningWeightArray.m_i16Rear++;
//
//    if(g_stRunningWeightArray.m_i16Rear >= SIZE_RUNNING_WEIGHT_ARRAY)
//    {
//       g_stRunningWeightArray.m_i16Rear = 0; 
//       g_stRunningWeightArray.m_fRunningWeightArrayFull = 1;
//    }
//
//    g_stRunningWeightArray.m_i16ArrRunningWeight[g_stRunningWeightArray.m_i16Rear] = i16Data;
//}
//
//// Make running weight average
//int16 GetRunningWeightDataAverage()
//{
//    int16 i16DataAvg = 0;
//    int32 i32DataAvg = 0;
//    int32 i32DataSum = 0;
//    int32 i32DataSize = 0;
//    int32 i32Index    = 0;
//    
//    if(HasRunningWeightArrayData() <= 0) 
//    {
//        return i16DataAvg;
//    }
//    
//    i32DataSize = g_stRunningWeightArray.m_i16Rear + 1;
//    
//    if(g_stRunningWeightArray.m_fRunningWeightArrayFull == 1)
//    {
//       i32DataSize = SIZE_RUNNING_WEIGHT_ARRAY; 
//    }
//
//    for(i32Index = 0; i32Index < i32DataSize; i32Index++)
//    {
//        i32DataSum += g_stRunningWeightArray.m_i16ArrRunningWeight[i32Index];
//    }
//    
//    if(i32DataSize > 0)
//    {
//        i32DataAvg = (int32)(i32DataSum / i32DataSize);
//    }
//    i16DataAvg = (int16)i32DataAvg;
//    
//    return i16DataAvg;
//}
//
//// Process running weight
//unsigned int16 ProcessWeightData(unsigned int16 ui16WeightData)
//{
//    unsigned int16 i16RunningAvgWt = 0;
//    PushRunningWeightData(ui16WeightData);
//    i16RunningAvgWt = GetRunningWeightDataAverage();
//    return i16RunningAvgWt;
//}
//// AR - end

#endif // VM_IMW_COMMON_H