// ADC.c
// Runs on LM4F120/TM4C123
// Provide functions that initialize ADC0 SS3 to be triggered by
// software and trigger a conversion, wait for it to finish,
// and return the result.
// Brad Gray bg22946,  Feyzullah Kalyoncu fk2787
// Last modification date: 6 April 2014


#include "../inc/tm4c123gh6pm.h"

// This initialization function 
// Max sample rate: <=125,000 samples/second
// SS3 triggering event: software trigger
// SS3 1st sample source: Ain1 (PE2)
// SS3 interrupts: flag set on completion but no interrupt requested
void Init_ADC(void){ 
		volatile unsigned long delay;
		SYSCTL_RCGC2_R |= 0x10;			//start clock for port E
		delay = SYSCTL_RCGC2_R;
		GPIO_PORTE_AFSEL_R |= 0x4;
	  GPIO_PORTE_AMSEL_R |= 0x4;
	  GPIO_PORTE_DATA_R &= ~0x4;
		GPIO_PORTE_DIR_R  &= ~0x4;
		GPIO_PORTE_DEN_R  &= ~0x4;  		 //disable digital engable - use as analog
	  SYSCTL_RCGC0_R |= 0x00010000; 	 //activate ADC0
		delay = SYSCTL_RCGC2_R;
		SYSCTL_RCGC0_R &= ~0x00000300; 	 //configure for 125k samples - 00 in bits 8 and 9
		delay = SYSCTL_RCGC2_R;
	  ADC0_SSPRI_R = 0x0123;           //Sequencer 3 is highest priority
	  ADC0_ACTSS_R &= ~0x0008;       	 //disable sample sequencer 3 by clearing bit 3 during programming - will turn on later
    ADC0_EMUX_R &= ~0xF000;          //seq3 is software trigger	
    ADC0_SSMUX3_R &= ~0x000F;        //clear SS3 field
		ADC0_SSMUX3_R += 1;              //set channel Ain1 (PE2)
		ADC0_SSCTL3_R = 0x0006;          //no TS0 D0, yes IE0 END0
		ADC0_SAC_R = ADC_SAC_AVG_64X;		 //enable 64 times average before obtaining result
		ADC0_ACTSS_R |= 0x0008;          //enable sample sequencer 3
	
}

//------------ADC_In------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
unsigned long ADC_In(void){  
	unsigned long result;
  ADC0_PSSI_R = 0x0008;            // 1) initiate SS3
  while((ADC0_RIS_R&0x08)==0){};   // 2) wait for conversion done
  result = ADC0_SSFIFO3_R&0xFFF;   // 3) read result
  ADC0_ISC_R = 0x0008;             // 4) acknowledge completion
  return result;
}


