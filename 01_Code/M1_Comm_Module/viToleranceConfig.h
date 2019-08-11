#ifndef _VI_TOLERANCE_CONFIG_H_
#define _VI_TOLERANCE_CONFIG_H_

#include "viEEPROMConfig.h"
//#include "viConveyorControl.h"
#include "viIMWMsgConfig.h"

#define SIZE_TOLERANCE_ENTRY            6
#define SIZE_TOLERANCE_ENTRY_BYTE_ARR   (SIZE_TOLERANCE_ENTRY*3*2)
#define EEPROM_START_ADDR_TOL_TABLE     0
#define SIZE_MSG_TOLERANCE_TABLE        39
#define NO_TOLERANCE_VARITY_SET         0xFF



//*****************************************************************
//********* Below Data Structure Implementation *******************
//*****************************************************************


typedef struct _ST_TOLERANCE_NODE
{
	unsigned int16 m_ui16Value;
	unsigned int16 m_ui16PosTolerence;
	unsigned int16 m_ui16NegTolerence;
} ST_TOLERANCE_NODE;

typedef struct _ST_TOLERANCE_TABLE
{
	ST_TOLERANCE_NODE m_stArrTable[SIZE_TOLERANCE_ENTRY];
} ST_TOLERANCE_TABLE;

typedef union _UN_TOLERANCE_TABLE
{
	ST_TOLERANCE_TABLE m_stToleranceTable;
	unsigned int8      m_ui8Arr[SIZE_TOLERANCE_ENTRY_BYTE_ARR];
} UN_TOLERANCE_TABLE;

//////////////////////// Amit ////////////////

//***********************************************************************
//********** Above Data Structure Implementation ************************
//***********************************************************************


//*****************************************************************************
//******* Below Global Variables Declaration **********************************
//*****************************************************************************

UN_TOLERANCE_TABLE g_unToleranceTable;
ST_TOLERANCE_NODE  g_stCurrentToleranceNode;
BYTE               g_bCurrentVariety = NO_TOLERANCE_VARITY_SET;

//*******************************************************************************
//********** Above Global Variable **********************************************
//*******************************************************************************


//*******************************************************************************
//********* Function Implementation for Tolerance *******************************
//*******************************************************************************
void PopulateToleranceTable()
{
	int16 i16StartOffset = EEPROM_START_ADDR_TOL_TABLE;
	int16 i16Count = 0;
	// Read the EEPROM for Tolerance Table Value and populate the Table in RAM
	for (i16Count = 0; i16Count < SIZE_TOLERANCE_ENTRY_BYTE_ARR; i16Count++)
	{
		g_unToleranceTable.m_ui8Arr[i16Count] = ReadEEPROMInt8(i16StartOffset + i16Count);
	}	
}


void UpdateToleranceTable()
{
	int16 i16StartOffset = EEPROM_START_ADDR_TOL_TABLE;
	int16 i16Count = 0;
	// Write the EEPROM for Tolerance Table Value and update the Table in RAM
	for (i16Count = 0; i16Count < SIZE_TOLERANCE_ENTRY_BYTE_ARR; i16Count++)
	{
		 WriteEEPROMInt8((i16StartOffset + i16Count), g_unToleranceTable.m_ui8Arr[i16Count]);
	}	
}

// This is the Receive Proc for Tolerance Table
void RcvProcToleranceTable(BYTE* pArrMSG)
{
    // Null Pointer Checking
    if (!pArrMSG)
    {
        return;
    }
    
    // pArrMSg contains the Table coming from PC end
    // Step 1: Copy the Array to RAM Tolerance Table
    memcpy(g_unToleranceTable.m_ui8Arr, pArrMSG, SIZE_TOLERANCE_ENTRY_BYTE_ARR);
    
    // Step 2: Copy the Tolerance Table from RAM to EEPROM
    UpdateToleranceTable();
    
    // Step 3: Copy Tolerance table from EEPROM to RAM for further use.
    PopulateToleranceTable();
    
    // Step 4:
    // Send Acknowledgment to UART2
    SendUART2(0, CMD_PC_MCU_SET_TOLERANCE_TABLE, 0, CRC_NOT_REQUIRED);
    
    // Step 5: Restart MCU for good health.
    //SetLEDsOnPowerOff();
    while(1);
}


// Sets Tolerance Node of the current Variety
void SetCurrentToleranceNode()
{
    int8 i8TableIndex = -1;
    
    // Get the Current Pin Settings
    switch (g_bCurrentVariety)
    {
        case VARITY_1:
            // Set the Table Index
            i8TableIndex = 0;
            // Set Load Cell Display Data Value Filter
            g_i16LdCDValueMax = g_unToleranceTable.m_stToleranceTable.m_stArrTable[0].m_ui16Value + ((g_unToleranceTable.m_stToleranceTable.m_stArrTable[0].m_ui16Value * LDCD_FILTER_TOLERANCE)/10);
            g_i16LdCDValueMin = g_unToleranceTable.m_stToleranceTable.m_stArrTable[0].m_ui16Value - ((g_unToleranceTable.m_stToleranceTable.m_stArrTable[0].m_ui16Value * LDCD_FILTER_TOLERANCE)/10);
            break;
        case VARITY_2:
            // Set the Table Index
            i8TableIndex = 1;
            // Set Load Cell Display Data Value Filter.
            g_i16LdCDValueMax = g_unToleranceTable.m_stToleranceTable.m_stArrTable[1].m_ui16Value + ((g_unToleranceTable.m_stToleranceTable.m_stArrTable[1].m_ui16Value *LDCD_FILTER_TOLERANCE )/ 10);
            g_i16LdCDValueMin = g_unToleranceTable.m_stToleranceTable.m_stArrTable[1].m_ui16Value - ((g_unToleranceTable.m_stToleranceTable.m_stArrTable[1].m_ui16Value *LDCD_FILTER_TOLERANCE )/ 10);
            break;
        case VARITY_3:
            // Set the Table Index
            i8TableIndex = 2;
            // Set Load Cell Display Data Value Filter.
            g_i16LdCDValueMax = g_unToleranceTable.m_stToleranceTable.m_stArrTable[2].m_ui16Value + ((g_unToleranceTable.m_stToleranceTable.m_stArrTable[2].m_ui16Value *LDCD_FILTER_TOLERANCE )/ 10);
            g_i16LdCDValueMin = g_unToleranceTable.m_stToleranceTable.m_stArrTable[2].m_ui16Value - ((g_unToleranceTable.m_stToleranceTable.m_stArrTable[2].m_ui16Value *LDCD_FILTER_TOLERANCE )/ 10);
            break;
        case VARITY_4:
            // Set the Table Index
            i8TableIndex = 3;
            // Set Load Cell Display Data Value Filter.
            g_i16LdCDValueMax = g_unToleranceTable.m_stToleranceTable.m_stArrTable[3].m_ui16Value + ((g_unToleranceTable.m_stToleranceTable.m_stArrTable[3].m_ui16Value *LDCD_FILTER_TOLERANCE )/ 10);
            g_i16LdCDValueMin = g_unToleranceTable.m_stToleranceTable.m_stArrTable[3].m_ui16Value - ((g_unToleranceTable.m_stToleranceTable.m_stArrTable[3].m_ui16Value *LDCD_FILTER_TOLERANCE )/ 10);
            break;
        case VARITY_5:
        	// Set the table Index.
        	i8TableIndex = 4;
        	 // Set Load Cell Display Data Value Filter.
        	 g_i16LdCDValueMax = g_unToleranceTable.m_stToleranceTable.m_stArrTable[4].m_ui16Value + ((g_unToleranceTable.m_stToleranceTable.m_stArrTable[4].m_ui16Value *LDCD_FILTER_TOLERANCE )/ 10);
        	 g_i16LdCDValueMax = g_unToleranceTable.m_stToleranceTable.m_stArrTable[4].m_ui16Value + ((g_unToleranceTable.m_stToleranceTable.m_stArrTable[4].m_ui16Value *LDCD_FILTER_TOLERANCE )/ 10);
        	 break;
        default:
            break;
    }
    
    if (i8TableIndex >= 0)
    {
        g_stCurrentToleranceNode.m_ui16Value        = g_unToleranceTable.m_stToleranceTable.m_stArrTable[i8TableIndex].m_ui16Value;
        g_stCurrentToleranceNode.m_ui16PosTolerence = g_unToleranceTable.m_stToleranceTable.m_stArrTable[i8TableIndex].m_ui16PosTolerence;
        g_stCurrentToleranceNode.m_ui16NegTolerence = g_unToleranceTable.m_stToleranceTable.m_stArrTable[i8TableIndex].m_ui16NegTolerence;
    }
    else
    {
        // No valid Variety found
    }
}


// Verify Tolerance Table
// Will Return MSG_STATUS_DATA_LOGG_PASSED or MSG_STATUS_DATA_LOGG_REJECTED
BYTE VerifyTolerance(unsigned int16 ui16ShipperWeight)
{
    // If the shipper weight is between tolerance level
    if(ui16ShipperWeight < (g_stCurrentToleranceNode.m_ui16Value + g_stCurrentToleranceNode.m_ui16PosTolerence) &&
       ui16ShipperWeight > (g_stCurrentToleranceNode.m_ui16Value - g_stCurrentToleranceNode.m_ui16NegTolerence))
    {
        // The Shipper is passed
        return MSG_STATUS_DATA_LOGG_PASSED;
    }
    else
    {
        return MSG_STATUS_DATA_LOGG_REJECTED;
    }
}

//TESTING
void ManipulateToleranceTable(void)
{
	g_unToleranceTable.m_stToleranceTable.m_stArrTable[0].m_ui16Value = 2100;
	g_unToleranceTable.m_stToleranceTable.m_stArrTable[0].m_ui16PosTolerence = 35;
	g_unToleranceTable.m_stToleranceTable.m_stArrTable[0].m_ui16NegTolerence = 35;
	g_unToleranceTable.m_stToleranceTable.m_stArrTable[1].m_ui16Value = 4200;
	g_unToleranceTable.m_stToleranceTable.m_stArrTable[1].m_ui16PosTolerence = 35;
	g_unToleranceTable.m_stToleranceTable.m_stArrTable[1].m_ui16NegTolerence = 35;
	g_unToleranceTable.m_stToleranceTable.m_stArrTable[2].m_ui16Value = 5240;
	g_unToleranceTable.m_stToleranceTable.m_stArrTable[2].m_ui16PosTolerence = 60;
	g_unToleranceTable.m_stToleranceTable.m_stArrTable[2].m_ui16NegTolerence = 60;
	g_unToleranceTable.m_stToleranceTable.m_stArrTable[3].m_ui16Value = 7310;
	g_unToleranceTable.m_stToleranceTable.m_stArrTable[3].m_ui16PosTolerence = 70;
	g_unToleranceTable.m_stToleranceTable.m_stArrTable[3].m_ui16NegTolerence = 70;
}
//END



#endif /* _VI_TOLERANCE_CONFIG_H_ */