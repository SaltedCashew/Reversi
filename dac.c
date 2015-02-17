// dac.c
// This software configures DAC output
// Runs on LM4F120 or TM4C123
// Program written by:  Brad Gray bg22946,  Feyzullah Kalyoncu fk2787
// Date Created: 8/25/2014 
// Last Modified: 4/26/2014 
// Section 4-5pm     TA: Saugata Bhattacharyya
// Lab number: 6
// Hardware connections: Outputs on PC4-7
// Initializes ports for Digital to analog conversion circuit.
// Changes output to DAC according to input 

#include "tm4c123gh6pm.h"



/* **************DAC_Init*********************
/ Initialize DAC, called once 
/ Initially setting up using 4 bit PC4-7. 
/ Input: none
/ Output: none
*/
void DAC_Init(void){
	volatile unsigned long NOP = 0;
	SYSCTL_RCGC2_R |= 0x04;
	NOP =  2;
	GPIO_PORTC_DIR_R |= 0xF0;
	GPIO_PORTC_AFSEL_R &= 0x0;
	GPIO_PORTC_AMSEL_R &= 0x0;
	GPIO_PORTC_DATA_R &= 0x0;
	GPIO_PORTC_DEN_R |= 0xF0;

}

// **************DAC_Out*********************
// output to DAC
// Input: 4-bit data, 0 to 15 
// Output: none
void DAC_Out(char sineStep){
	GPIO_PORTC_DATA_R = sineStep*8;
	}
