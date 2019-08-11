/* 
 * File:   vmSIM900Config.h
 * Author: Mrinmoy
 *
 * Created on December 12, 2018, 12:40 PM
 */

#ifndef VM_SIM900_CONFIG_H
#define	VM_SIM900_CONFIG_H

#include "vmSIM900_GPRSConfig.h"

#define   MAX_GPRS_FAILED_ATTEMPT_COUNT         5
#define   MAX_BAKING_DATA_SEND_ATTEMPT_COUNT    3


// *****************************************************************************
// ***************** Below -> Global Variable Declarations *********************
// *****************************************************************************


// *****************************************************************************
// ***************** Above -> Global Variable Declarations *********************
// *****************************************************************************


void InitSIM900RxInterrupt(void)
{
    // Enable SIM900 interrupt.
    clear_interrupt(INT_RDA);
	enable_interrupts(INT_RDA);
}

#INT_RDA
void SIM900_Rx_ISR(void)
{
    // Read the received byte from stream and store it in Rx buffer 
    g_ui8SIM900RxBuffer[g_ui16SIM900RxByteCount] = fgetc(SIM900);
    
    // Increase receive byte count
    g_ui16SIM900RxByteCount++;
    
    // Check if receive byte count reached receive buffer max size
    if(SIM900_RX_BUFF_SIZE == g_ui16SIM900RxByteCount)
    {
        // Make it circular buffer
        g_ui16SIM900RxByteCount = 0;
    }
    
   // Clear interrupt.
	clear_interrupt(INT_RDA);
}


//void SetAutomaticSMSRecvOn(void)
//{
//    fputs("ATE0\r",SIM900);
//    delay_ms(2000);
////    // set the GSM modem in text mode.
////    fputs("AT+CIPSPRT=0\r",SIM900);
////    delay_ms(2000);
////    // Set gsm modem preferred memory storage.
////    fputs("AT+CPMS=\"SM\"\r",SIM900);
////    delay_ms(2000);
//    // Set Incoming SMS Alert on.
//    fputs("AT+CNMI=2,2,0,0,0\r",SIM900);
//    delay_ms(2000);
//    // set the GSM modem in text mode.
//    fputs("AT+CMGF=1\r",SIM900);
//    delay_ms(2000);
//    
//
//    ResetSIM900RxBuffer();
//}




unsigned int8 SendATCommand(void)
{
    unsigned int8 ui8fIsOKFound = 0;
    unsigned int8 ui8SendCount  = 0;
    g_fIsTimedOut               = 0;
    
    while(ui8SendCount < 5)
    {
        // Reset Rx Buffer.
	    ResetSIM900RxBuffer();
        // Send AT Command.
	    fputs("AT\r",SIM900);
        // Set Timeout.	
	    SetTimeout(1);
        // Check for "OK" response
        while(g_fIsTimedOut == 0 && ui8fIsOKFound == 0)
        {
            // Loop through whole SIM900 Rx buffer
            for(unsigned int16 ui16LoopCount = 0; ui16LoopCount < g_ui16SIM900RxByteCount; ui16LoopCount++)
            {
                // Check for "OK".
	             if(g_ui8SIM900RxBuffer[(ui16LoopCount - 0)] == 0x0A &&
	                g_ui8SIM900RxBuffer[(ui16LoopCount - 1)] == 0x0D &&
	                g_ui8SIM900RxBuffer[(ui16LoopCount - 2)] == 'K' &&
	                g_ui8SIM900RxBuffer[(ui16LoopCount - 3)] == 'O')
	                {
                        // Set flag denoting 'OK' is found.
                        ui8fIsOKFound = 1;
                        ui8SendCount = 5;
                        break;
	                }
            }
        }
        
        // Increase send count
        ui8SendCount++;
    }
    
    return ui8fIsOKFound;
}



unsigned int8 ConnectGPRS(void)
{
	unsigned int8 ui8ReturnVal = 0;
	
	do
	{
		// Reset Receive Buffer.
		ResetSIM900RxBuffer();
		// Set connection type.
		ui8ReturnVal = SetConnectionType();
		
		// Check return value.
        if(ui8ReturnVal != 0)
        {
            break;
        }

		// Reset Receive Buffer.
		ResetSIM900RxBuffer();
		// Set APN.
		ui8ReturnVal = SetAPN();
		
		// Check return value.
        if(ui8ReturnVal != 0)
        {
            break;
        }
        
        // Reset Receive Buffer.
		ResetSIM900RxBuffer();
		// Enable GPRS.
		ui8ReturnVal = EnableGPRS();
		
		// Check return value.
        if(ui8ReturnVal != 0)
        {
            break;
        }
        
        // Reset Receive Buffer.
		ResetSIM900RxBuffer();
		// Query if GPRS Connection is ok.
        ui8ReturnVal = CheckGPRSConnection();
        
        // Check return value.
        if(ui8ReturnVal != 0)
        {
            break;
        }
		
	}while(0);
	
	return ui8ReturnVal;

}


unsigned int8 SendHTTPData(void)
{
    unsigned int8 ui8ReturnVal = 0;
    
    do
    {
        // Reset Rx Buffer.
		ResetSIM900RxBuffer();
		// Enable HTTP mode.
		ui8ReturnVal = EnableHttpMode();  /* Step 1: Enable HTTP mode */
		
		// Check return value.
        if(ui8ReturnVal != 0)
        {
            break;
        }
        
//        // Reset Rx Buffer.
//        ResetSIM900RxBuffer();
//        // Enable HTTP Secure mode.
//        ui8ReturnVal = EnableSecureHttpMode(); /* Step 2: Enable HTTPS mode */
//        
//        // Check return value.
//        if(ui8ReturnVal != 0)
//        {
//            break;
//        }
        
        // Reset Rx Buffer.
		ResetSIM900RxBuffer();
		// Set HTTP bearer Profile Identifier.
		ui8ReturnVal = SetHTTPBearerProfileID();  /* Step 3: Set the type of action for web service URL */
		
		// Check return value.
        if(ui8ReturnVal != 0)
        {
            break;
        }
        
        // Reset Rx Buffer.
		ResetSIM900RxBuffer();
        // Set the value to Web Service URL.
		ui8ReturnVal = SetWebServiceURL();   /* Step 4: Set the URL and parameters to send data to server */
		
		// Check return value.
        if(ui8ReturnVal != 0)
        {
            break;
        }
        
        // Reset Rx Buffer.
		ResetSIM900RxBuffer();
		// Start HTTP POST Session.
		ui8ReturnVal = StartHTTPGETSession(); /* Step 5; Hit URL to dump data */
		
		// Check return value.
        if(ui8ReturnVal != 0)
        {
            break;
        }
    
    }while(0);
    
    // Terminate HTTP Session.
    TerminateHttpSession();
    
    return ui8ReturnVal;
}


#endif	/* VM_SIM900_CONFIG_H */

