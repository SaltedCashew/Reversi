// Sound.c, 
// This module contains the SysTick ISR that plays sound
// Also contains the Debug heartbeat initialization and toggle functions.
// Systick Handler calls to DAC_Out to play given frequency. 
//Interrupt times determined by desired frequency of note, system clock, and wavelength of sine wave array.
// Runs on LM4F120 or TM4C123
// Program written by:  Brad Gray bg22946,  Feyzullah Kalyoncu fk2787
// Date Created: 8/25/2013 
// Last Modified: 10/9/2013 
// Section 4-5pm     TA: Saugata Bhattacharyya
// Lab number: 10

#include "tm4c123gh6pm.h"
#include "dac.h"
#include "song.h"
#include "sounds.h"
#include "Sound.h"

const unsigned char Bassoon[32] = {8,8,8,8,7,7,9,15,10,1,1,4,8,11,10,6,3,2,6,10,8,5,5,5,6,7,7,9,8,8,8,7};  
const unsigned char Trumpet[32] = {10,11,1,12,10,8,3,1,8,15,15,11,10,10,11,10,10,10,10,10,10,10,10,11,11,11,11,11,11,10,10,10};
const unsigned char Flute[32] = {6,9,10,12,13,15,15,15,13,12,10,8,7,6,5,5,4,4,4,3,3,2,1,1,0,0,0,1,1,2,3,4};
const unsigned char SineWave [32] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0};
const unsigned char Drum [32] = {15,5,14,7,13,7,10,5,8,4,7,3,1,2,2, 15,5,14,7,13,7,10,5,8,4,7,3,1,2,2};

#define SystemClock 80000000 // 80 MHz clock
#define WaveArrayLength 32   //array used to build the audio sine wave

void SysTick_Init(unsigned long period);



char sineIndex; unsigned long delayPeriod;


// **************Sound_Init*********************
// Initialize Systick periodic interrupts
// Input: Initial interrupt period
// Output: none
void Sound_Init(unsigned long period){
	DAC_Init();
	SysTick_Init(period);
	sineIndex=0;
	
}

// **************Sound_Play*********************
// Start sound output, and set Systick interrupt period 
// Input: desired note frequency
//         input of zero sets delay period to zero.
// Output: none
void Sound_Play(unsigned long frequency){
	/*input is sound frequency desired. System clock set above in #define statement. Assuming 80 MHz to start.
		12.5 ns per cycle. period = 1/f. e.g. 440 HZ = 2.27 ms. 
	*/
	if (frequency != 0){
	delayPeriod = (SystemClock/WaveArrayLength)/frequency; //proper delay period to achieve desired note freq

		NVIC_ST_RELOAD_R = delayPeriod;// initialize to given period
	}	
	else delayPeriod = 0;
}



// Interrupt service routine
// Executed periodically, the actual period
// determined by the current Reload.
void SysTick_Handler(void){
	unsigned char instrument = 0;

	if (delayPeriod == 0){
		DAC_Out (0);
	}
	else {
		switch (instrument){
		case 0: DAC_Out (Drum[sineIndex]);
						break;
		case 1: DAC_Out (fastinvader1[sineIndex]);
						break;
		case 2: DAC_Out (Flute[sineIndex]);
						break;
		case 3: DAC_Out (Trumpet[sineIndex]);
						break;

		}
	}	
	sineIndex = (sineIndex+1) & 0x1F; 
	
	
}

// **************SysTick_Init*********************
// Initialize SysTick periodic interrupts, setting initial reload value to max
// Input: None
// Output: None
void SysTick_Init(unsigned long period){
  
  NVIC_ST_CTRL_R = 0;         // disable SysTick during setup
  NVIC_ST_RELOAD_R = period;// initialize to given period
  NVIC_ST_CURRENT_R = 0;      // any write to current clears it
  NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x40000000; // priority 2
                              // enable SysTick with core clock and interrupts
  NVIC_ST_CTRL_R = 0x07;

}



