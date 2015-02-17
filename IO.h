// Lab10 Arcade Style Game
// GameControl.c
// Runs on LM4F120 or TM4C123
// Brad Gray bg22946,  Feyzullah Kalyoncu fk2787
// Put the date here or look very silly

// Game Controller Inputs 

#ifndef IO_H
#define IO_H


extern unsigned long Slider_ADCMail;
extern unsigned char ADCMailStatus;
extern unsigned long ADCData;


//----------Input_Button_Init--------------//
// Sets up main input Button
// Input on PF2
void Input_Button_Init(void);


//----------Main_Button_Input--------------//
// Returns button input as 1 (pressed) or 0 (not pressed)
// Input on PF2
unsigned char Main_Button_Input(void);


//----------Timer2_Init--------------//
// Inits Timer2 for slider input
// Input from slider on PE2
void Timer2_Init(void);

//----------Input_Button_Reset--------------//
// Causes busy wait until Input button no longer pressed
// used to keep inputs from being read too quickly
void Input_Button_Reset(void);


//----------Slider_Button_Input--------------//
// Returns input from slider button
// Slider button used to change direction of slider
unsigned char Slider_Button_Input(void);

//----------EdgeCounter_Init--------------//
// Inits Edge Counter Interrupt for PF4
void EdgeCounter_Init(void);


//----------Slider_Direction--------------//
// Returns 1 or 0 to use slider in both directions
unsigned char Slider_Direction(void);

#endif
