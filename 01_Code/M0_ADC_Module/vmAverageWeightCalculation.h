/* 
 * File:   vmAverageWeightCalculation.h
 * Author: OEM
 *
 * Created on December 20, 2018, 11:50 AM
 */

#ifndef VM_AVERAGE_WEIGHT_CALCULATION_H
#define	VM_AVERAGE_WEIGHT_CALCULATION_H

#define SIZE_AVG_WT_ARR                 50
#define MAX_DEVIATIION_FROM_AVG_WT      1000  // We are considering +- 1000gm weight deviation
#define MAX_DEVIATION_FOR_BATCH_CHANGE  850   // If Current weight 850gm deviated from last average weight then we consider batch must be changed.
                                              // In that case we reset the queue.

typedef struct _ST_AVG_WT_QUEUE
{
    int16 m_i16Arr[SIZE_AVG_WT_ARR];
    int8  m_i8Rear;
    int8  m_i8Length;
    int8  m_i8QueueFull;
    int16 m_i16CurAvg;
    int16 m_i16LstAvg;
} ST_AVG_WT_QUEUE;

ST_AVG_WT_QUEUE g_stAvgWtQueue;

void InitializeAvgWtQueue()
{
    unsigned int8 ui8LoopCount = 0;
    g_stAvgWtQueue.m_i8Rear     = -1;
    g_stAvgWtQueue.m_i8Length   = SIZE_AVG_WT_ARR;
    g_stAvgWtQueue.m_i16CurAvg  = 0;
    g_stAvgWtQueue.m_i8QueueFull = 0;
    
    // Initialize array member.
    for(ui8LoopCount = 0; ui8LoopCount < SIZE_AVG_WT_ARR; ui8LoopCount++)
    {
        g_stAvgWtQueue.m_i16Arr[ui8LoopCount] = 0;
    }
}

void ResetAvgWtQueue(int8 i8Length)
{
    if (i8Length <= SIZE_AVG_WT_ARR)
    {
        g_stAvgWtQueue.m_i8Length = i8Length;
    }
    else
    {
        g_stAvgWtQueue.m_i8Length = SIZE_AVG_WT_ARR;
    }
}

void InsertAvgWtQueue(int16 i16LastWeight)
{
    int8  i8LoopCount = 0;
    int8  i8MaxEntry  = 0;
    int32 i32Sum      = 0;
    
    // Check the weight deviation from Selected SKU Average Weight
    if (i16LastWeight > (g_stCurBatchInfo.m_i16AvgWeight + MAX_DEVIATIION_FROM_AVG_WT) ||
        i16LastWeight < (g_stCurBatchInfo.m_i16AvgWeight - MAX_DEVIATIION_FROM_AVG_WT))
    {
        // The weight is out of range, we are not inserting this weight into Average weight queue 
        return;
    }
    
    // Increment Rear
    g_stAvgWtQueue.m_i8Rear++;
    
    // Check if the current weight is a new batch weight.
    if(i16LastWeight >= g_stAvgWtQueue.m_i16LstAvg + MAX_DEVIATION_FOR_BATCH_CHANGE)
    {
        g_stAvgWtQueue.m_i8Rear = 0;
    }
    
    // Check queue length according to auto/fixed calibration mode.
    // If in auto calibration mode then data will be inserted upto user input stored avg queue length.
    if(HMI_REJECTION_AUTO_CALIB_MODE == g_fRejectionMode)
    {
        if (g_stAvgWtQueue.m_i8Rear >= g_stAvgWtQueue.m_i8Length)
        {
            g_stAvgWtQueue.m_i8Rear      = 0;
            // Update state Queue Full
            g_stAvgWtQueue.m_i8QueueFull = 1;
        }
    }
    // Otherwise in fixed calibration mode data will be inserted upto queue's original length.
    else
    {
        if (g_stAvgWtQueue.m_i8Rear >= SIZE_AVG_WT_ARR)
        {
            g_stAvgWtQueue.m_i8Rear      = 0;
            // Update state Queue Full
            g_stAvgWtQueue.m_i8QueueFull = 1;
        }
    }
    
    // Insert into Queue
    g_stAvgWtQueue.m_i16Arr[g_stAvgWtQueue.m_i8Rear] = i16LastWeight;
    
    // Get the Max entry in queue
    if (1 == g_stAvgWtQueue.m_i8QueueFull)
    {
        if(HMI_REJECTION_AUTO_CALIB_MODE == g_fRejectionMode)
        {    
            // Set maximum counted data for average calculation.
            i8MaxEntry = g_stAvgWtQueue.m_i8Length;
            
            // Reset queue full flag.
            g_stAvgWtQueue.m_i8QueueFull = 0;
        }
        else
        {
            // Set maximum counted data for average calculation.
            i8MaxEntry = SIZE_AVG_WT_ARR;
            
            // Reset queue full flag.
            g_stAvgWtQueue.m_i8QueueFull = 0;
        }
    }
    else
    {
        i8MaxEntry = g_stAvgWtQueue.m_i8Rear + 1;
    }
    
    // Get the summation of queue values   
    for (i8LoopCount = 0; i8LoopCount < i8MaxEntry; i8LoopCount++)
    {
        i32Sum += g_stAvgWtQueue.m_i16Arr[i8LoopCount];
    }
    
    g_stAvgWtQueue.m_i16CurAvg = (int16)(i32Sum / (int32)i8MaxEntry);
    
    // Store the current average in last average of all CBBs.
    g_stAvgWtQueue.m_i16LstAvg = g_stAvgWtQueue.m_i16CurAvg;
}

void IBCSendProdModeAvgData()
{
    ST_IBC_DATA_NODE stIBCDataNode;
    int8 i8LoopCount = 0;
    

    // Prepare IBC Data Node
    g_unIBCProdModeAvg.m_stAvgData.m_i16Padding  = 0;
    g_unIBCProdModeAvg.m_stAvgData.m_i16CurAvg   = g_stAvgWtQueue.m_i16CurAvg;
    
    // Stuff IBC Command
    stIBCDataNode.g_ui8ArrIBCData[0] = IBC_CMD_PROD_AVG_WT_DATA;
    for (i8LoopCount = 1; i8LoopCount < IBC_MSG_BYTE_COUNT; i8LoopCount++)
    {
        stIBCDataNode.g_ui8ArrIBCData[i8LoopCount] = g_unIBCProdModeAvg.m_ui8Arr[i8LoopCount - 1];
    }
    
    // Send to IBC Insert
    //InsertSendQueue(&stIBCDataNode);
    InsertIBCMsgQueue(&g_stIBCSendQueue, &stIBCDataNode);
}
#endif	/* VM_AVERAGE_WEIGHT_CALCULATION_H */

