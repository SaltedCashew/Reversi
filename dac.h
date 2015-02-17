// dac.h
// This software configures DAC output
// Runs on LM4F120 or TM4C123
// Program written by:  Brad Gray bg22946,  Feyzullah Kalyoncu fk2787
// Date Created: 8/25/2013 
// Last Modified: 10/9/2013 
// Section 4-5pm     TA: Saugata Bhattacharyya
// Lab number: 6


// **************DAC_Init*********************
// Initialize 4-bit DAC, called once 
// Input: none
// Output: none
void DAC_Init(void);


// **************DAC_Out*********************
// output to DAC
// Input: 4-bit data, 0 to 15 
// Output: none
void DAC_Out(char sineStep);
