/* 
 * File:   vmM0ADCModuleCommon.h
 * Author: OEM
 *
 * Created on December 13, 2018, 4:59 PM
 */

#ifndef VM_M0_ADC_MODULE_COMMON_H
#define	VM_M0_ADC_MODULE_COMMON_H

#define BOX_ACCEPTED               1
#define BOX_REJECTED               0
//#define BOX_REJECTED_UNDR_WT       0
//#define BOX_REJECTED_OVR_WT        2

#define SYS_MODE_NOTHING   0
#define SYS_MODE_HOME      1
#define SYS_MODE_CALIB     2
#define SYS_MODE_PRE_PROD  3
#define SYS_MODE_PROD      4
#define SYS_MODE_POST_PROD 5

#define IBC_INTERMEDIATE_DATA_SEND_ON  1
#define IBC_INTERMEDIATE_DATA_SEND_OFF 0

#define ADC_MEAN_DATA_INTERMEDIATE_WEIGHT   31
#define ADC_MEAN_DATA_INTERMEDIATE_TARE     32
#define ADC_MEAN_DATA_FINAL_WEIGHT          33
#define ADC_MEAN_DATA_FINAL_TARE            34
#define ADC_MEAN_DATA_CALIB_WEIGHT          35
#define ADC_MEAN_DATA_NOTHING               36

unsigned int8 g_ui8ADCMeanDataState = ADC_MEAN_DATA_NOTHING;

#define FLAG_WT_CALCULATION_DONE     1
#define FLAG_WT_CALCULATION_NOT_DONE 0

unsigned int8 g_fWtCalculationState = FLAG_WT_CALCULATION_NOT_DONE;

#define IBC_RESET_CALIB_OFF    0
#define IBC_RESET_CALIB_ON     1

#define HMI_REJECTION_AUTO_CALIB_MODE   0
#define HMI_REJECTION_FIXED_CALIB_MODE  1


unsigned int16 g_ui16CurrentBoxCount   = 0;     // Total Box Passed Sofar
unsigned int16 g_ui16RejectionStatus   = BOX_REJECTED;
unsigned int8  g_ui8SystemMode         = SYS_MODE_NOTHING;
unsigned int8  g_fIntermediateDataSend = IBC_INTERMEDIATE_DATA_SEND_OFF;
unsigned int16 g_ui16DynamicTareValue   = 0;
unsigned int32 g_ui32DynamicTareADCValue   = 0;

unsigned int16 g_ui16PrevDynamicTareValue = 0;
int32          g_i32StaticTareValue    = 0;  // TODO Should keep in EEPROM
unsigned int8  g_fResetCalibStatus     = IBC_RESET_CALIB_OFF;
int16          g_i16CalibResetValue     = 0;
int16          g_i16CalibResetSumValue  = 0;
int32          g_i32ADCSumValue         = 0;
int32          g_i32ADCAvgValue         = 0;
unsigned int16 g_ui16ADCCalibValueEEPROM = 0;
unsigned int16 g_ui16CalibPointEEPROM    = 0;
int16          g_i16CalibResetPrevValue = 0;
int16          g_i16DynamicErrValue     = 0;
int16          g_i16CurBoxWeight        = 0;
int8           g_fRejectionMode         = HMI_REJECTION_FIXED_CALIB_MODE;

typedef struct _ST_CUR_BATCH_INFO
{
    int16 m_i16AvgWeight;
    int16 m_i16PositiveTol;
    int16 m_i16NegativeTol;
} ST_CUR_BATCH_INFO;

ST_CUR_BATCH_INFO g_stCurBatchInfo;

#endif	/* VM_M0_ADC_MODULE_COMMON_H */

