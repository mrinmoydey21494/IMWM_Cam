#ifndef _VI_TOLERANCE_CONFIG_H_
#define _VI_TOLERANCE_CONFIG_H_

#include "viEEPROMConfig.h"
#include "viConveyorControl.h"

#define SIZE_TOLERANCE_ENTRY            4
#define SIZE_TOLERANCE_ENTRY_BYTE_ARR   (SIZE_TOLERANCE_ENTRY*3*2)
#define EEPROM_START_ADDR_TOL_TABLE     0




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

//***********************************************************************
//********** Above Data Structure Implementation ************************
//***********************************************************************


//*****************************************************************************
//******* Below Global Variables Declaration **********************************
//*****************************************************************************

UN_TOLERANCE_TABLE g_unToleranceTable;

//*******************************************************************************
//********** Above Global Variable **********************************************
//*******************************************************************************


//*******************************************************************************
//********* Function Implementation for Tolerance *******************************
//*******************************************************************************

void PopulateToleranceTable()
{
	int16 i16StartOffset = EEPROM_START_ADDR_TOL_TABLE;
	int8 i16Count = 0;
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

//TESTING
void ManipulateToleranceTable(void)
{
	g_unToleranceTable.m_stToleranceTable.m_stArrTable[0].m_ui16Value = 2100;
	g_unToleranceTable.m_stToleranceTable.m_stArrTable[0].m_ui16PosTolerence = 35;
	g_unToleranceTable.m_stToleranceTable.m_stArrTable[0].m_ui16NegTolerence = 35;
	g_unToleranceTable.m_stToleranceTable.m_stArrTable[1].m_ui16Value = 4200;
	g_unToleranceTable.m_stToleranceTable.m_stArrTable[1].m_ui16PosTolerence = 35;
	g_unToleranceTable.m_stToleranceTable.m_stArrTable[1].m_ui16NegTolerence = 35;
	g_unToleranceTable.m_stToleranceTable.m_stArrTable[2].m_ui16Value = 4870;
	g_unToleranceTable.m_stToleranceTable.m_stArrTable[2].m_ui16PosTolerence = 29;
	g_unToleranceTable.m_stToleranceTable.m_stArrTable[2].m_ui16NegTolerence = 29;
	g_unToleranceTable.m_stToleranceTable.m_stArrTable[3].m_ui16Value = 6720;
	g_unToleranceTable.m_stToleranceTable.m_stArrTable[3].m_ui16PosTolerence = 40;
	g_unToleranceTable.m_stToleranceTable.m_stArrTable[3].m_ui16NegTolerence = 40;
}
//END

void ToleranceCheckingandDecisionMaking(unsigned int16 ui16CalibratedWeight)
{
	unsigned int16 ui16MeasuredTolerance = 0;
	
	//Fetch tolerance table from EEPROM.
	PopulateToleranceTable();
	
	// Tolerance checking for good day butter of 35gm  of 60 packets CBB. 
	if(input(GD_BB_35GM_60PCKTS_CBB_SELECTOR_SWITCH) == HIGH)
	{
		// Comparing standard carton weight with calibrated weight.
		if(g_unToleranceTable.m_stToleranceTable.m_stArrTable[0].m_ui16Value > ui16CalibratedWeight) // For Negative Tolerance.
		{
			// measuring tolerance.
			ui16MeasuredTolerance = (g_unToleranceTable.m_stToleranceTable.m_stArrTable[0].m_ui16Value - ui16CalibratedWeight);
			// checking tolerance's permisible range.
			if(ui16MeasuredTolerance >= g_unToleranceTable.m_stToleranceTable.m_stArrTable[0].m_ui16NegTolerence)// tolerance not in permisible range.
			{
				for(unsigned int8 ui8LoopCount = 0; ui8LoopCount < MAX_FIVE_CARTONS; ui8LoopCount++)
				{
					// Finding carton number for which decision is set.
					if(g_stLastFivePassedCartonInfoBuffer[ui8LoopCount].m_ui16CartonNumber == g_ui16EnteredCartonThroughSensorCounter)
					{
						// set flag denoting this carton has to reject.
						g_stLastFivePassedCartonInfoBuffer[ui8LoopCount].m_ui8WeightTestingStatus = 1;
				    }   
				}
			}
			else // tolerance is within permissible range.
			{
				for(unsigned int8 ui8LoopCount = 0; ui8LoopCount < MAX_FIVE_CARTONS; ui8LoopCount++)
				{
					// Finding carton number for which decision is set.
					if(g_stLastFivePassedCartonInfoBuffer[ui8LoopCount].m_ui16CartonNumber == g_ui16EnteredCartonThroughSensorCounter)
					{
						// reset flag denoting this carton is accepted. 
						g_stLastFivePassedCartonInfoBuffer[ui8LoopCount].m_ui8WeightTestingStatus = 0;
				    }   
				}		
			}
			
		}
		else // For Positive tolerance.
		{
			// measuring tolerance.
			ui16MeasuredTolerance = (ui16CalibratedWeight - g_unToleranceTable.m_stToleranceTable.m_stArrTable[0].m_ui16Value);
			// checking tolerance's permisible range.
			if(ui16MeasuredTolerance >= g_unToleranceTable.m_stToleranceTable.m_stArrTable[0].m_ui16PosTolerence)// tolerance not in permisible range.
			{
				for(unsigned int8 ui8LoopCount = 0; ui8LoopCount < MAX_FIVE_CARTONS; ui8LoopCount++)
				{
					// Finding carton number for which decision is set.
					if(g_stLastFivePassedCartonInfoBuffer[ui8LoopCount].m_ui16CartonNumber == g_ui16EnteredCartonThroughSensorCounter)
					{
						// set flag denoting this carton has to reject.
						g_stLastFivePassedCartonInfoBuffer[ui8LoopCount].m_ui8WeightTestingStatus = 1;
				    }   
				}	
			}
			else // tolerance is within permissible range.
			{
				for(unsigned int8 ui8LoopCount = 0; ui8LoopCount < MAX_FIVE_CARTONS; ui8LoopCount++)
				{
					// Finding carton number for which decision is set.
					if(g_stLastFivePassedCartonInfoBuffer[ui8LoopCount].m_ui16CartonNumber == g_ui16EnteredCartonThroughSensorCounter)
					{
						// reset flag denoting this carton is accepted.
						g_stLastFivePassedCartonInfoBuffer[ui8LoopCount].m_ui8WeightTestingStatus = 0;
				    }   
				}	
			}
		}
	}
	
	// Tolerance checking for good day butter of 35gm  of 120 packets CBB.
	if(input(GD_BB_35GM_120PCKTS_CBB_SELECTOR_SWITCH) == HIGH)
	{
		// Comparing standard carton weight with calibrated weight.
		if(g_unToleranceTable.m_stToleranceTable.m_stArrTable[1].m_ui16Value > ui16CalibratedWeight) // For Negative Tolerance.
		{
			// measuring tolerance.
			ui16MeasuredTolerance = (g_unToleranceTable.m_stToleranceTable.m_stArrTable[1].m_ui16Value - ui16CalibratedWeight);
			// checking tolerance's permisible range.
			if(ui16MeasuredTolerance >= g_unToleranceTable.m_stToleranceTable.m_stArrTable[1].m_ui16NegTolerence)// tolerance not in permisible range.
			{
				for(unsigned int8 ui8LoopCount = 0; ui8LoopCount < MAX_FIVE_CARTONS; ui8LoopCount++)
				{
					// Finding carton number for which decision is set.
					if(g_stLastFivePassedCartonInfoBuffer[ui8LoopCount].m_ui16CartonNumber == g_ui16EnteredCartonThroughSensorCounter)
					{
						// set flag denoting this carton has to reject.
						g_stLastFivePassedCartonInfoBuffer[ui8LoopCount].m_ui8WeightTestingStatus = 1;
				    }   
				}
			}
			else // tolerance is within permissible range.
			{
				for(unsigned int8 ui8LoopCount = 0; ui8LoopCount < MAX_FIVE_CARTONS; ui8LoopCount++)
				{
					// Finding carton number for which decision is set.
					if(g_stLastFivePassedCartonInfoBuffer[ui8LoopCount].m_ui16CartonNumber == g_ui16EnteredCartonThroughSensorCounter)
					{
						// reset flag denoting this carton is accepted. 
						g_stLastFivePassedCartonInfoBuffer[ui8LoopCount].m_ui8WeightTestingStatus = 0;
				    }   
				}		
			}
			
		}
		else // For Positive tolerance.
		{
			// measuring tolerance.
			ui16MeasuredTolerance = (ui16CalibratedWeight - g_unToleranceTable.m_stToleranceTable.m_stArrTable[1].m_ui16Value);
			// checking tolerance's permisible range.
			if(ui16MeasuredTolerance >= g_unToleranceTable.m_stToleranceTable.m_stArrTable[1].m_ui16PosTolerence)// tolerance not in permisible range.
			{
				for(unsigned int8 ui8LoopCount = 0; ui8LoopCount < MAX_FIVE_CARTONS; ui8LoopCount++)
				{
					// Finding carton number for which decision is set.
					if(g_stLastFivePassedCartonInfoBuffer[ui8LoopCount].m_ui16CartonNumber == g_ui16EnteredCartonThroughSensorCounter)
					{
						// set flag denoting this carton has to reject.
						g_stLastFivePassedCartonInfoBuffer[ui8LoopCount].m_ui8WeightTestingStatus = 1;
				    }   
				}	
			}
			else // tolerance is within permissible range.
			{
				for(unsigned int8 ui8LoopCount = 0; ui8LoopCount < MAX_FIVE_CARTONS; ui8LoopCount++)
				{
					// Finding carton number for which decision is set.
					if(g_stLastFivePassedCartonInfoBuffer[ui8LoopCount].m_ui16CartonNumber == g_ui16EnteredCartonThroughSensorCounter)
					{
						// reset flag denoting this carton is accepted.
						g_stLastFivePassedCartonInfoBuffer[ui8LoopCount].m_ui8WeightTestingStatus = 0;
				    }   
				}	
			}
		}
	}
	
	// Tolerance checking for good day cashew of 29 gm  of 168 packets CBB.
	if(input(GD_CB_29GM_168PCKTS_CBB_SELECTOR_SWITCH) == HIGH)
	{
		// Comparing standard carton weight with calibrated weight.
		if(g_unToleranceTable.m_stToleranceTable.m_stArrTable[2].m_ui16Value > ui16CalibratedWeight)// For negative tolerance.
		{
			// measuring tolerance.
			ui16MeasuredTolerance = (g_unToleranceTable.m_stToleranceTable.m_stArrTable[2].m_ui16Value - ui16CalibratedWeight);
			// checking tolerance's permisible range.
			if(ui16MeasuredTolerance >= g_unToleranceTable.m_stToleranceTable.m_stArrTable[2].m_ui16NegTolerence)// tolerance not in permisible range.
			{
				for(unsigned int8 ui8LoopCount = 0; ui8LoopCount < MAX_FIVE_CARTONS; ui8LoopCount++)
				{
					// Finding carton number for which decision is set.
					if(g_stLastFivePassedCartonInfoBuffer[ui8LoopCount].m_ui16CartonNumber == g_ui16EnteredCartonThroughSensorCounter)
					{
						// set flag denoting this carton has to reject.
						g_stLastFivePassedCartonInfoBuffer[ui8LoopCount].m_ui8WeightTestingStatus = 1;
				    }   
				}
			}
			else // tolerance  is within permisible range.
			{
				for(unsigned int8 ui8LoopCount = 0; ui8LoopCount < MAX_FIVE_CARTONS; ui8LoopCount++)
				{
					// Finding carton number for which decision is set.
					if(g_stLastFivePassedCartonInfoBuffer[ui8LoopCount].m_ui16CartonNumber == g_ui16EnteredCartonThroughSensorCounter)
					{
						// reset flag denoting this carton is accepted. 
						g_stLastFivePassedCartonInfoBuffer[ui8LoopCount].m_ui8WeightTestingStatus = 0;
				    }   
				}		
			}
		}
		else // For Positive tolerance.
		{
			// measuring tolerance.
			ui16MeasuredTolerance = (ui16CalibratedWeight - g_unToleranceTable.m_stToleranceTable.m_stArrTable[2].m_ui16Value);
			// checking tolerance's permisible range.
			if(ui16MeasuredTolerance >= g_unToleranceTable.m_stToleranceTable.m_stArrTable[2].m_ui16PosTolerence)// tolerance not in permisible range.
			{
				for(unsigned int8 ui8LoopCount = 0; ui8LoopCount < MAX_FIVE_CARTONS; ui8LoopCount++)
				{
					// Finding carton number for which decision is set.
					if(g_stLastFivePassedCartonInfoBuffer[ui8LoopCount].m_ui16CartonNumber == g_ui16EnteredCartonThroughSensorCounter)
					{
						// set flag denoting this carton has to reject.
						g_stLastFivePassedCartonInfoBuffer[ui8LoopCount].m_ui8WeightTestingStatus = 1;
				    }   
				}	
			}
			else // tolerance is within permissible range.
			{
				for(unsigned int8 ui8LoopCount = 0; ui8LoopCount < MAX_FIVE_CARTONS; ui8LoopCount++)
				{
					// Finding carton number for which decision is set.
					if(g_stLastFivePassedCartonInfoBuffer[ui8LoopCount].m_ui16CartonNumber == g_ui16EnteredCartonThroughSensorCounter)
					{
						// reset flag denoting this carton is accepted.
						g_stLastFivePassedCartonInfoBuffer[ui8LoopCount].m_ui8WeightTestingStatus = 0;
				    }   
				}	
			}
		}
	}	
	
	// Tolerance checking for good day butter of 40 gm  of 168 packets CBB.
	if(input(GD_BB_40GM_168PCKTS_CBB_SELECTOR_SWITCH) == HIGH)
	{
		// Comparing standard carton weight with calibrated weight.
		if(g_unToleranceTable.m_stToleranceTable.m_stArrTable[3].m_ui16Value > ui16CalibratedWeight)// For negative tolerance.
		{
			// measuring tolerance.
			ui16MeasuredTolerance = (g_unToleranceTable.m_stToleranceTable.m_stArrTable[3].m_ui16Value - ui16CalibratedWeight);
			// checking tolerance's permisible range.
			if(ui16MeasuredTolerance >= g_unToleranceTable.m_stToleranceTable.m_stArrTable[3].m_ui16NegTolerence)// tolerance not in permisible range.
			{
				for(unsigned int8 ui8LoopCount = 0; ui8LoopCount < MAX_FIVE_CARTONS; ui8LoopCount++)
				{
					// Finding carton number for which decision is set.
					if(g_stLastFivePassedCartonInfoBuffer[ui8LoopCount].m_ui16CartonNumber == g_ui16EnteredCartonThroughSensorCounter)
					{
						// set flag denoting this carton has to reject.
						g_stLastFivePassedCartonInfoBuffer[ui8LoopCount].m_ui8WeightTestingStatus = 1;
				    }   
				}
			}
			 else // tolerance is within permissible range.
		   	{
				for(unsigned int8 ui8LoopCount = 0; ui8LoopCount < MAX_FIVE_CARTONS; ui8LoopCount++)
				{
					// Finding carton number for which decision is set.
					if(g_stLastFivePassedCartonInfoBuffer[ui8LoopCount].m_ui16CartonNumber == g_ui16EnteredCartonThroughSensorCounter)
					{
						// reset flag denoting this carton is accepted. 
						g_stLastFivePassedCartonInfoBuffer[ui8LoopCount].m_ui8WeightTestingStatus = 0;
				    }   
				}		
			}				
		}
		
		else // For Positive tolerance.
		{
			// measuring tolerance.
			ui16MeasuredTolerance = (ui16CalibratedWeight - g_unToleranceTable.m_stToleranceTable.m_stArrTable[3].m_ui16Value);
			// checking tolerance's permisible range.
			if(ui16MeasuredTolerance >= g_unToleranceTable.m_stToleranceTable.m_stArrTable[3].m_ui16PosTolerence)// tolerance not in permisible range.
			{
				for(unsigned int8 ui8LoopCount = 0; ui8LoopCount < MAX_FIVE_CARTONS; ui8LoopCount++)
				{
					// Finding carton number for which decision is set.
					if(g_stLastFivePassedCartonInfoBuffer[ui8LoopCount].m_ui16CartonNumber == g_ui16EnteredCartonThroughSensorCounter)
					{
						// set flag denoting this carton has to reject.
						g_stLastFivePassedCartonInfoBuffer[ui8LoopCount].m_ui8WeightTestingStatus = 1;
				    }   
				}	
			}
			else // tolerance is within permissible range.
			{
				for(unsigned int8 ui8LoopCount = 0; ui8LoopCount < MAX_FIVE_CARTONS; ui8LoopCount++)
				{
					// Finding carton number for which decision is set.
					if(g_stLastFivePassedCartonInfoBuffer[ui8LoopCount].m_ui16CartonNumber == g_ui16EnteredCartonThroughSensorCounter)
					{
						// reset flag denoting this carton is accepted.
						g_stLastFivePassedCartonInfoBuffer[ui8LoopCount].m_ui8WeightTestingStatus = 0;
				    }   
				}	
			}
	    }
	}
}
#endif /* _VI_TOLERANCE_CONFIG_H_ */