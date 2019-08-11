#ifndef VM_ADS1131_H
#define VM_ADS1131_H

#include "vmPINConfig.h"



unsigned int32 read_ext_adc()
{
   int i;
   unsigned int32 data;
   unsigned int8 ui8Test = 5;

   data=0;
   //output_low(ADS8320_CS);
   
   ui8Test = input(PIN_ADC_DOUT_DRDY);

   for(i=0;i<18;++i) {           // send sample over spi
      output_high(PIN_ADC_SCLK);
      delay_us(1);
      shift_left(&data,3,input(PIN_ADC_DOUT_DRDY));
      output_low(PIN_ADC_SCLK);
      delay_us(1);
   }
   
   ui8Test = input(PIN_ADC_DOUT_DRDY);
           
   // Apply 19th clock pulse to make DOUT PIN high i.e.data retrieve done.
   delay_us(1);
   output_high(PIN_ADC_SCLK);
   delay_us(1);
   output_low(PIN_ADC_SCLK);
   //delay_us(90);
   
   ui8Test = input(PIN_ADC_DOUT_DRDY);
   
   //output_high(ADS8320_CS);
   return(data);
}

float convert_to_volts(long data) {
   return ((float)data*5.0/0xffff);
}

#endif /* VM_ADS1131_H */