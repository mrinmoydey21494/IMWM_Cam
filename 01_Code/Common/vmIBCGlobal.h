#ifndef _VM_IBC_GOBAL_H_
#define _VM_IBC_GOBAL_H_

#define IBC_SEND_LOCK                   1
#define IBC_SEND_UNLOCK                 0

typedef enum _EN_IBC_COMMANDS
{
    IBC_CMD_MODE_HOME                   = 100,
    // Calibration Mode
    IBC_CMD_MODE_CALIB                  = 101,
    IBC_CMD_GET_CALIB_POINT             = 102,
    IBC_CMD_RESET_CALIB_POINT_TARE      = 103,
    IBC_CMD_RESET_CALIB_POINT           = 104,
    IBC_CMD_RESET_CALIB_POINT_WEIGHT    = 105, 
    IBC_CMD_TARE_REQUEST                = 106,
    IBC_CMD_SET_LOAD_CELL_FACTOR        = 115,        

    // Pre-Production Mode
    IBC_CMD_MODE_PRE_PROD               = 107,
    IBC_CMD_PRE_PROD_AVG_WEIGHT         = 108,
    IBC_CMD_PRE_PROD_TOLERANCE          = 109,
    IBC_CMD_MODE_AUTO_CALIB             = 110,
    IBC_CMD_MODE_FIXED_CALIB            = 111,


    // Production Mode
    IBC_CMD_MODE_PROD                   = 112,
    IBC_CMD_START_PROD                  = 113,
    IBC_CMD_PROD_WEIGH_DATA             = 114,

    IBC_CMD_PROD_TARE_DATA              = 118,
    IBC_CMD_PROD_AVG_WT_DATA            = 119,
            
    IBC_CMD_PROD_INTMD_DATA             = 122
            
            
} EN_IBC_COMMANDS;

// Mode type
enum boardModeType
{
    BOARD_MODE_TYPE_NOTHING         = 0,
    BOARD_MODE_TYPE_HOME            = 1,
    BOARD_MODE_TYPE_PRE_PRODUCTION  = 2,
	BOARD_MODE_TYPE_PRODUCTION      = 3,
    BOARD_MODE_TYPE_CONFIGURATION   = 4,
    BOARD_MODE_TYPE_CALIBRATION     = 5
};

// Mode action on Hardware
enum manualModeActionHW {
    MANUAL_MODE_ACTION_ON_NOTHING               = 0,
    MANUAL_MODE_ACTION_ON_COIL_STOPPER_UNIT_1   = 1,
    MANUAL_MODE_ACTION_ON_COIL_STOPPER_UNIT_2   = 2,
    MANUAL_MODE_ACTION_ON_GRIPPER_ARM_1         = 3,
    MANUAL_MODE_ACTION_ON_GRIPPER_ARM_2         = 4,
    MANUAL_MODE_ACTION_ON_ARCH_CHUTE            = 5,
    MANUAL_MODE_ACTION_ON_SERVO                 = 6,

	MANUAL_MODE_ACTION_ON_LA1                   = 7,
    MANUAL_MODE_ACTION_ON_LA2                   = 8,
    MANUAL_MODE_ACTION_ON_VFD                   = 9,
    MANUAL_MODE_ACTION_ON_PNEUMATIC_DRIVE       = 10,
    
    MANUAL_MODE_ACTION_ON_COIL_TRANSFER         = 11,
    MANUAL_MODE_ACTION_ON_TUNION                = 12

};

//typedef struct _ST_IBC_PROD_M_WEIGHT_DATA
//{
//    int16 m_i16BoxCount;
//    int16 m_i16BoxWeight;
//} ST_IBC_PROD_M_WEIGHT_DATA;
typedef struct _ST_IBC_PROD_M_WEIGHT_DATA
{
    unsigned int16 m_ui16BoxCount;
    int16 m_i16BoxWeight;
} ST_IBC_PROD_M_WEIGHT_DATA;


typedef union _UN_IBC_PROD_M_WEIGHT_DATA
{
    ST_IBC_PROD_M_WEIGHT_DATA m_stWeighingData;
    unsigned int8             m_ui8Arr[4];
} UN_IBC_PROD_M_WEIGHT_DATA;

typedef struct _ST_IBC_PROD_M_TARE_DATA
{
    int16 m_i16Padding;
    int16 m_i16TareData;
} ST_IBC_PROD_M_TARE_DATA;

typedef union _UN_IBC_PROD_M_TARE_DATA
{
    ST_IBC_PROD_M_TARE_DATA m_stTareData;
    unsigned int8           m_ui8Arr[4];
} UN_IBC_PROD_M_TARE_DATA;

typedef struct _ST_IBC_AVG_WT_NODE
{
    int16 m_i16AvgWt;
    int16 m_i16Padding;
} ST_IBC_AVG_WT_NODE;

typedef union _UN_IBC_AVG_WT_NODE
{
    ST_IBC_AVG_WT_NODE m_stIBCAvgNode;
    unsigned int8      m_ui8Arr[4];
} UN_IBC_AVG_WT_NODE;

typedef struct _ST_IBC_TOL_NODE
{
    int16 m_i16PositiveTol;
    int16 m_i16NegativeTol;
} ST_IBC_TOL_NODE;

typedef union _UN_IBC_TOLERANCE_NODE
{
    ST_IBC_TOL_NODE m_stIBCTolNode;
    unsigned int8   m_ui8Arr[4];
}UN_IBC_TOLERANCE_NODE;


typedef struct _ST_IBC_RESET_CALIB_POINT
{
    int16         m_i16CalibPoint;
    int16         m_i16CurrCalibPoint;
} ST_IBC_RESET_CALIB_POINT;

typedef union _UN_IBC_RESET_CALIB_POINT
{
    ST_IBC_RESET_CALIB_POINT m_stIBCResetCalibPoint;
    unsigned int8            m_ui8Arr[4];
} UN_IBC_RESET_CALIB_POINT;


typedef struct _ST_IBC_RESET_CALIB_POINT_ACK
{
    int16         m_i16PassCount;
    int16         m_i16CalibValue;
} ST_IBC_RESET_CALIB_POINT_ACK;

typedef union _UN_IBC_RESET_CALIB_POINT_ACK
{
    ST_IBC_RESET_CALIB_POINT_ACK m_stIBCResetCalibPoint;
    unsigned int8                m_ui8Arr[4];
} UN_IBC_RESET_CALIB_POINT_ACK;

typedef struct _ST_IBC_RESET_CALIB_POINT_TARE
{
    unsigned int32 m_ui32CalibADCForTare;
}ST_IBC_RESET_CALIB_POINT_TARE;

typedef union _UN_IBC_RESET_CALIB_POINT_TARE
{
    ST_IBC_RESET_CALIB_POINT_TARE m_stIBCResetCalibTare;
    unsigned int8                 m_ui8Arr[4];
}UN_IBC_RESET_CALIB_POINT_TARE;

typedef struct _ST_IBC_RESET_CALIB_POINT_WEIGHT
{
    unsigned int32 m_ui32CalibADCForWeight;
}ST_IBC_RESET_CALIB_POINT_WEIGHT;

typedef union _UN_IBC_RESET_CALIB_POINT_WEIGHT
{
    ST_IBC_RESET_CALIB_POINT_WEIGHT m_stIBCResetCalibWeight;
    unsigned int8                 m_ui8Arr[4];
}UN_IBC_RESET_CALIB_POINT_WEIGHT;

// Send Current Average Weight from MCU0 to MCU1, for IBC_CMD_PROD_AVG_WT_DATA
typedef struct _ST_IBC_PROD_MODE_CUR_AVG
{
    int16 m_i16CurAvg;
    int16 m_i16Padding;
} ST_IBC_PROD_MODE_CUR_AVG;

typedef union _UN_IBC_PROD_MODE_CUR_AVG
{
    ST_IBC_PROD_MODE_CUR_AVG m_stAvgData;
    unsigned int8            m_ui8Arr[4];
} UN_IBC_PROD_MODE_CUR_AVG;

typedef struct _ST_IBC_LOAD_CELL_FACTOR
{
    unsigned int16 m_ui16CurrLoadCellFactor;
    unsigned int16 m_ui16Padding;
    
}ST_IBC_LOAD_CELL_FACTOR;

typedef union _UN_IBC_LOAD_CELL_FACTOR
{
    ST_IBC_LOAD_CELL_FACTOR     m_stLoadCellFactor;
    unsigned int8               m_ui8Arr[4];
}UN_IBC_LOAD_CELL_FACTOR;

//*****************************************************************************
// Below -> Global Variable Declaration ***************************************
//***************************************************************************** 
boardModeType           g_boardModeType;
boardModeType           g_boardModeTypePre;
manualModeActionHW      g_manualModeActionOnHw;
unsigned int16          g_ui16DIStatus;
unsigned int16          g_ui16DOStatus;
unsigned int16          g_ui16ErrorStatus;
unsigned int8           g_ui8IncrementIndex;
int8                    g_fIBCSendLockStatus = IBC_SEND_UNLOCK;

UN_IBC_PROD_M_WEIGHT_DATA    g_unIBCProdModeWtData;
UN_IBC_PROD_M_TARE_DATA      g_unIBCProdModeTrData;
UN_IBC_AVG_WT_NODE           g_unIBCAvgNode;
UN_IBC_TOLERANCE_NODE        g_unIBCTolNode;
UN_IBC_RESET_CALIB_POINT     g_unIBCResetCalib;
UN_IBC_RESET_CALIB_POINT_ACK g_unIBCResetCalibAck;
UN_IBC_PROD_MODE_CUR_AVG     g_unIBCProdModeAvg;
UN_IBC_LOAD_CELL_FACTOR      g_unIBCCurrLoadCellFactor;
UN_IBC_RESET_CALIB_POINT_TARE g_unIBCResetCalibTare;
UN_IBC_RESET_CALIB_POINT_WEIGHT g_unIBCResetCalibWeight;

//*****************************************************************************
// Above -> Global Variable Declaration ***************************************
//*****************************************************************************

// function for set mode as Nothing
void SetModeNothing(void)
{
    g_boardModeTypePre  = g_boardModeType;
    g_boardModeType     = BOARD_MODE_TYPE_NOTHING;
}

// function for check mode is Nothing or not
int8 IsModeNothing(void)
{
    int8 bRetVal = 0;
    
    if(g_boardModeType == BOARD_MODE_TYPE_NOTHING) 
    {
        bRetVal = 1;
    }
    
    return bRetVal;
}

// function for set mode as WakeUp
void SetModeHome(void)
{
    g_boardModeTypePre  = g_boardModeType;
    g_boardModeType     = BOARD_MODE_TYPE_HOME;
}

// function for check mode is WakeUp or not
int8 IsModeHome(void)
{
    int8 bRetVal = 0;
    
    if(g_boardModeType == BOARD_MODE_TYPE_HOME) 
    {
        bRetVal = 1;
    }
    
    return bRetVal;
}

// function for set mode as manual
void SetModeCalibration(void)
{
    g_boardModeTypePre  = g_boardModeType;
    g_boardModeType     = BOARD_MODE_TYPE_CALIBRATION;
}

// function for check mode is manual or not
int8 IsCalibrationMode(void)
{
    int8 bRetVal = 0;
    
    if(g_boardModeType == BOARD_MODE_TYPE_CALIBRATION) 
    {
        bRetVal = 1;
    }
    
    return bRetVal;
}

// function for set mode as pre production
void SetModePreProduction()
{
    g_boardModeTypePre  = g_boardModeType;
    g_boardModeType     = BOARD_MODE_TYPE_PRE_PRODUCTION;
}

// function for check mode is pre production or not
int8 IsPreProductionMode(void)
{
    int8 bRetVal = 0;
    
    if(g_boardModeType == BOARD_MODE_TYPE_PRE_PRODUCTION) 
    {
        bRetVal = 1;
    }
    
    return bRetVal;
}

// function for set mode as production
void SetModeProduction()
{
    g_boardModeTypePre  = g_boardModeType;
    g_boardModeType     = BOARD_MODE_TYPE_PRODUCTION;
}

// function for check mode is production or not
int8 IsProductionMode(void)
{
    int8 bRetVal = 0;
    
    if(g_boardModeType == BOARD_MODE_TYPE_PRODUCTION) 
    {
        bRetVal = 1;
    }
    
    return bRetVal;
}

//// function for set mode as post production
//void SetModePostProduction()
//{
//    g_boardModeTypePre  = g_boardModeType;
//    g_boardModeType     = BOARD_MODE_TYPE_POST_PRODUCTION;
//}
//
//// function for check mode is post production or not
//int8 IsPostProductionMode(void)
//{
//    int8 bRetVal = 0;
//    
//    if(g_boardModeType == BOARD_MODE_TYPE_POST_PRODUCTION) 
//    {
//        bRetVal = 1;
//    }
//    
//    return bRetVal;
//}

// function for set mode as configuration
void SetModeConfiguration(void)
{
    g_boardModeTypePre  = g_boardModeType;
    g_boardModeType     = BOARD_MODE_TYPE_CONFIGURATION;
}

// function for check mode is configuration or not
int8 IsConfigurationMode(void)
{
    int8 bRetVal = 0;
    
    if(g_boardModeType == BOARD_MODE_TYPE_CONFIGURATION) 
    {
        bRetVal = 1;
    }
    
    return bRetVal;
}

#endif /* _VM_IBC_GOBAL_H_ */