/* 
 * File:   vmGSMM66Config.h
 * Author: mrinmoy
 *
 * Created on January 18, 2019, 5:10 PM
 */

#ifndef VM_GSM_M66_CONFIG_H
#define	VM_GSM_M66_CONFIG_H

#include "vmGSMM66_GPRSConfig.h"
#include "vmTimerConfig.h"

#define   SIZE_SMS_RESPONSE_BUFF                60
#define   GPRS_MODULE_HEALTH_OK                 44
#define   GPRS_MODULE_HEALTH_MODULE_IS_NOT_OK   45

// *****************************************************************************
// ***************** Below -> Global Variable Declarations *********************
// *****************************************************************************
unsigned int8  g_ui8M66SMSResponseDataBuffer[SIZE_SMS_RESPONSE_BUFF]      = {0};
unsigned int8  g_ui8RcvPhoneNumber[13]                                    = {0};
unsigned int8  g_ui8Recp1PhoneNumber[13]                                  = {'+','9','1','7','0','4','4','3','4','1','4','1','4'};
unsigned int8  g_ui8Recp2PhoneNumber[13]                                  = {'+','9','1','8','9','8','1','7','8','3','6','0','9'};
unsigned int8  g_ui8Recp3PhoneNumber[13]                                  = {'+','9','1','9','8','0','4','6','6','8','1','2','2'};
unsigned int8  g_fIsGPRSConnected                                         = GPRS_NOT_CONNECTED;
unsigned int8  g_fIsSIM900ModuleOK                                        = GPRS_MODULE_HEALTH_MODULE_IS_NOT_OK;
// *****************************************************************************
// ***************** Above -> Global Variable Declarations *********************
// *****************************************************************************


void InitM66RxInterrupt(void)
{
    // Enable SIM900 interrupt.
    clear_interrupt(INT_RDA);
	enable_interrupts(INT_RDA);
}



void Delay_Milli(unsigned int16 ui16DelayTime)
{
    unsigned int16 ui16TimeCounter = 0;
    while(ui16TimeCounter < ui16DelayTime)
    {
        delay_ms(1);
        ui16TimeCounter++;
    }
}   

#INT_RDA
void GSMM66_Rx_ISR(void)
{
     // Read the received byte from stream and store it in Rx buffer 
    g_ui8M66RxBuffer[g_ui16M66RxByteCount] = fgetc(GSM_M66);
    
    // Increase receive byte count
    g_ui16M66RxByteCount++;
    
    // Check if receive byte count reached receive buffer max size
    if(SIZE_GSMM66_RX_BUFFER == g_ui16M66RxByteCount)
    {
        // Make it circular buffer
        g_ui16M66RxByteCount = 0;
 
    }
    
    // Clear interrupt.
    clear_interrupt(INT_RDA);
}

void SetM66BasicSMSConfiguration(void)
{
//    fputs("AT\r",GSM_M66);
//    //delay_ms(1000);
//    Delay_Milli(2000);
    
    // Set GSM module Echo disable. 
    fputs("ATE0\r",GSM_M66);
   delay_ms(1000);

   // Set Message service type as GSM..Compatible with SMS AT Commands   
   fputs("AT+CSCS=\"GSM\"\r",GSM_M66);
   delay_ms(1000);
   
    // Set gsm modem preferred memory storage.
    fputs("AT+CPMS=\"SM\"\r",GSM_M66);
    delay_ms(1000);
    
    // Set Incoming SMS Alert on.
    fputs("AT+CNMI=2,2,0,0,0\r",GSM_M66);
    delay_ms(1000);
    
    // set the GSM modem in text mode.
    fputs("AT+CMGF=1\r",GSM_M66);
    delay_ms(1000);
    
    ResetGSMM66RxBuffer();
}

unsigned int8 SendAtCommand(void)
{
    unsigned int8 ui8fIsOKFound = 0;
    unsigned int8 ui8SendCount  = 0;
    g_fIsTimedOut = 0;
    
    while(ui8SendCount < 5)
    {
	    // Reset Rx Buffer.
	    ResetGSMM66RxBuffer();
	    // Send AT Command.
	    fputs("AT\r",GSM_M66);
	    // Set Timeout.	
	    SetTimeout(1);
	    // Check for 'OK' response.
	     while(g_fIsTimedOut == 0 && ui8fIsOKFound == 0)
	     {
	     	for(unsigned int16 ui16ByteCount = 0; ui16ByteCount < g_ui16M66RxByteCount; ui16ByteCount++)
	     	{
	     		// Check for "OK".
	             if(g_ui8M66RxBuffer[(ui16ByteCount - 0)] == 0x0A &&
	                g_ui8M66RxBuffer[(ui16ByteCount - 1)] == 0x0D &&
	                g_ui8M66RxBuffer[(ui16ByteCount - 2)] == 'K' &&
	                g_ui8M66RxBuffer[(ui16ByteCount - 3)] == 'O')
	                {
                        // Set flag denoting 'OK' is found.
                        ui8fIsOKFound = 1;
                        ui8SendCount = 5;
                        break;
	                }
	     	}
	     
	     }
	        
		ui8SendCount++;
	}
	
    
    return ui8fIsOKFound;
}

void SendAllRecipients(unsigned int8 ui8DataLength)
{
    
    unsigned int8 ui8IsOKFound = 0;
    g_fIsTimedOut = 0;
    unsigned int8 ui8LoopCount = 0;
    
    
    // Check if recipient 1's phone number is valid.
    if(g_ui8Recp1PhoneNumber[0] == '+')
    {
        // Set the GSM modem in text mode.
        fputs("AT+CMGF=1\r",GSM_M66);
        delay_ms(500);
        // Reset M66 receive buffer
        ResetGSMM66RxBuffer();
        
        // Prepare phone number at which SMS needs to send.
        fprintf(GSM_M66, "AT+CMGS=\"%c%c%c%c%c%c%c%c%c%c%c%c%c\"\r",
                                    g_ui8Recp1PhoneNumber[0],
                                    g_ui8Recp1PhoneNumber[1],
                                    g_ui8Recp1PhoneNumber[2],
                                    g_ui8Recp1PhoneNumber[3],
                                    g_ui8Recp1PhoneNumber[4],
                                    g_ui8Recp1PhoneNumber[5],
                                    g_ui8Recp1PhoneNumber[6],
                                    g_ui8Recp1PhoneNumber[7],
                                    g_ui8Recp1PhoneNumber[8],
                                    g_ui8Recp1PhoneNumber[9],
                                    g_ui8Recp1PhoneNumber[10],
                                    g_ui8Recp1PhoneNumber[11],
                                    g_ui8Recp1PhoneNumber[12]);
        
        // Set timeout.
        SetTimeout(2);
        // wait for response.
        while(0 == g_fIsTimedOut && ui8IsOKFound == 0)
        {
            // Loop through entire receive buffer response.
            for(unsigned int16 ui16ByteCount = 0; ui16ByteCount < g_ui16M66RxByteCount; ui16ByteCount++)
            {
                // If 'prompt' received
                if(g_ui8M66RxBuffer[ui16ByteCount - 0] == '>')
                {
                    // Set flag denoting response received.
                    ui8IsOKFound = 1;
                }
            }
        }
        
        // Check if response is received within time.
        if(!g_fIsTimedOut)
        {
            // put data.
            for(ui8LoopCount = 0; ui8LoopCount < ui8DataLength; ui8LoopCount++)
            {
                fputc(g_ui8M66SMSResponseDataBuffer[ui8LoopCount], GSM_M66);
            }
            
            delay_ms(500);
            fputc(0x1A,GSM_M66); // Ctrl-Z indicates end of SMS.
            delay_ms(4000);
            // Reset M66 receive buffer
            ResetGSMM66RxBuffer();
        }
    }
    
    // Reset variables.
    ui8IsOKFound = 0;
    g_fIsTimedOut = 0;
    
    // Check if recipient 2's phone number is valid.
    if(g_ui8Recp2PhoneNumber[0] == '+')
    {
        // Reset M66 receive buffer
        ResetGSMM66RxBuffer();
        
        // Prepare phone number at which SMS needs to send.
        fprintf(GSM_M66, "AT+CMGS=\"%c%c%c%c%c%c%c%c%c%c%c%c%c\"\r",
                                    g_ui8Recp2PhoneNumber[0],
                                    g_ui8Recp2PhoneNumber[1],
                                    g_ui8Recp2PhoneNumber[2],
                                    g_ui8Recp2PhoneNumber[3],
                                    g_ui8Recp2PhoneNumber[4],
                                    g_ui8Recp2PhoneNumber[5],
                                    g_ui8Recp2PhoneNumber[6],
                                    g_ui8Recp2PhoneNumber[7],
                                    g_ui8Recp2PhoneNumber[8],
                                    g_ui8Recp2PhoneNumber[9],
                                    g_ui8Recp2PhoneNumber[10],
                                    g_ui8Recp2PhoneNumber[11],
                                    g_ui8Recp2PhoneNumber[12]);
        
        // Set timeout.
        SetTimeout(2);
        // wait for response.
        while(0 == g_fIsTimedOut && ui8IsOKFound == 0)
        {
            // Loop through entire receive buffer response.
            for(unsigned int16 ui16ByteCount = 0; ui16ByteCount < g_ui16M66RxByteCount; ui16ByteCount++)
            {
                // If 'prompt' received
                if(g_ui8M66RxBuffer[ui16ByteCount - 0] == '>')
                {
                    // Set flag denoting response received.
                    ui8IsOKFound = 1;
                }
            }
        }
        
        // Check if response is received within time.
        if(!g_fIsTimedOut)
        {
            // put data.
            for(ui8LoopCount = 0; ui8LoopCount < ui8DataLength; ui8LoopCount++)
            {
                fputc(g_ui8M66SMSResponseDataBuffer[ui8LoopCount], GSM_M66);
            }
            
            delay_ms(500);
            fputc(0x1A,GSM_M66); // Ctrl-Z indicates end of SMS.
            delay_ms(4000);
            // Reset M66 receive buffer
            ResetGSMM66RxBuffer();
        }
    }
    
    // Reset variables.
    ui8IsOKFound = 0;
    g_fIsTimedOut = 0;
    
    // Check if recipient 3's phone number is valid.
    if(g_ui8Recp3PhoneNumber[0] == '+')
    {
        // Reset M66 receive buffer
        ResetGSMM66RxBuffer();
        
        // Prepare phone number at which SMS needs to send.
        fprintf(GSM_M66, "AT+CMGS=\"%c%c%c%c%c%c%c%c%c%c%c%c%c\"\r",
                                    g_ui8Recp3PhoneNumber[0],
                                    g_ui8Recp3PhoneNumber[1],
                                    g_ui8Recp3PhoneNumber[2],
                                    g_ui8Recp3PhoneNumber[3],
                                    g_ui8Recp3PhoneNumber[4],
                                    g_ui8Recp3PhoneNumber[5],
                                    g_ui8Recp3PhoneNumber[6],
                                    g_ui8Recp3PhoneNumber[7],
                                    g_ui8Recp3PhoneNumber[8],
                                    g_ui8Recp3PhoneNumber[9],
                                    g_ui8Recp3PhoneNumber[10],
                                    g_ui8Recp3PhoneNumber[11],
                                    g_ui8Recp3PhoneNumber[12]);
        
        // Set timeout.
        SetTimeout(2);
        // wait for response.
        while(0 == g_fIsTimedOut && ui8IsOKFound == 0)
        {
            // Loop through entire receive buffer response.
            for(unsigned int16 ui16ByteCount = 0; ui16ByteCount < g_ui16M66RxByteCount; ui16ByteCount++)
            {
                // If 'prompt' received
                if(g_ui8M66RxBuffer[ui16ByteCount - 0] == '>')
                {
                    // Set flag denoting response received.
                    ui8IsOKFound = 1;
                }
            }
        }
        
        // Check if response is received within time.
        if(!g_fIsTimedOut)
        {
            // put data.
            for(ui8LoopCount = 0; ui8LoopCount < ui8DataLength; ui8LoopCount++)
            {
                fputc(g_ui8M66SMSResponseDataBuffer[ui8LoopCount], GSM_M66);
            }
            
            delay_ms(500);
            fputc(0x1A,GSM_M66); // Ctrl-Z indicates end of SMS.
            delay_ms(4000);
            // Reset M66 receive buffer
            ResetGSMM66RxBuffer();
        }
    }
    
    // Reset SMS response data buffer.
    memset(g_ui8M66SMSResponseDataBuffer, 0 , SIZE_SMS_RESPONSE_BUFF);
}

void PopulateAndSendTestSMS(void)
{
    // generate test SMS to send.
    sprintf(g_ui8M66SMSResponseDataBuffer, "THIS IS A TEST MESSAGE FROM QUECTEL");
    
    // Send test message to all recipients.
    SendAllRecipients(35);
}

unsigned int8 ConnectGPRS(void)
{
    unsigned int8 ui8ReturnVal = 0;
    
    do
    {
        // Reset Rx buffer.
        ResetGSMM66RxBuffer();
        // Define PDP context.
        ui8ReturnVal = SetPDPContext();
        
       // Check return value.
        if(ui8ReturnVal != 0)
        {
            break;
        }
        
        // Reset Rx buffer.
        ResetGSMM66RxBuffer();
        // Activate PDP context.
        ui8ReturnVal = ActivatePDP();
        
        // Check return value.
        if(ui8ReturnVal != 0)
        {
            break;
        }
        
        // Reset Rx buffer.
        ResetGSMM66RxBuffer();
        // Show activated IP.
        ui8ReturnVal = ShowActivatedAddress();
        
        // Check return value.
        if(ui8ReturnVal != 0)
        {
            break;
        }
 
    }while(0);
    
    return ui8ReturnVal;
}

unsigned int8 SendData(void)
{
    unsigned int8 ui8RetVal = 0;
    
    do
    {
        // Reset Rx buffer.
        ResetGSMM66RxBuffer();
        // Set web service URL.
        ui8RetVal = SetHTTPURL();
        
        // Check return value.
        if(ui8RetVal != 0)
        {
            break;
        }
        
        // Reset Rx buffer.
        ResetGSMM66RxBuffer();
        // Start HTTP GET session.
        ui8RetVal = StartHTTPGETSession();
        
        // Check return value.
        if(ui8RetVal != 0)
        {
            break;
        }
                
    }while(0);
    
    return ui8RetVal;
}

#endif	/* VM_GSM_M66_CONFIG_H */

