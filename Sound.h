// Sound.h
// This module contains the SysTick ISR that plays sound
// Runs on LM4F120 or TM4C123
// Program written by:  Brad Gray bg22946,  Feyzullah Kalyoncu fk2787
// Date Created: 8/25/2013 
// Last Modified: 10/9/2013 
// Section 4-5pm     TA: Saugata Bhattacharyya
// Lab number: 6


#ifndef SOUND_H
#define SOUND_H



// **************Sound_Init*********************
// Initialize Systick periodic interrupts
// Called once, with sound initially off
// Input: interrupt period
//           Units to be determined by YOU
//           Maximum to be determined by YOU
//           Minimum to be determined by YOU
// Output: none
void Sound_Init(unsigned long period);


// **************Sound_Play*********************
// Start sound output, and set Systick interrupt period 
// Input: interrupt period
//           Units to be determined by YOU
//           Maximum to be determined by YOU
//           Minimum to be determined by YOU
//         input of zero disable sound output
// Output: none
void Sound_Play(unsigned long note);

#endif

