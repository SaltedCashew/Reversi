// Lab10 Arcade Style Game
// main.c
// Runs on LM4F120 or TM4C123
// Put your name here or look very silly
// Put the date here or look very silly

// Graphic display on Kentec EB-LM4F120-L35
// Sound output to DAC (Lab 7)
// Analog Input connected to PE2=ADC1, 1-D joystick (Lab 8)
// optional: UART link to second board for two-player mode
// Switch input

#include "../inc/tm4c123gh6pm.h"
#include "SSD2119.h"
#include "PLL.h"
#include "random.h"
     // audio waveforms
#include "tm4c123gh6pm.h"
#include "ADC.h"
#include "GameControl.h"
#include "Song.h"
#include "Sound.h"
#include "dac.h"
#include "IO.h"





void EnableInterrupts(void);
void Timer2_Init(void);
void Delay(unsigned int delayCount);
void Init_All(void);
void LCD_Setup(void);
void Input_Button_Init(void);


#define PF2       (*((volatile unsigned long *)0x40025010))

unsigned long TimerCount;
unsigned long playerPosition;  // 32-bit fixed-point 0.001 cm

unsigned long Data;      // 12-bit ADC
unsigned long startingPeriod = 0x00FFFFFF;


int main(void){
	
	Init_All();
	
	
	while(1){
	  Run_Reversi();
		}	

}		
	

void Init_All(void){
	
	Input_Button_Init();
	PLL_Init();  // Set the clocking to run at 80MHz from the PLL.
  LCD_Init();  // Initialize LCD
	
	Init_ADC();
	Timer2_Init();  //Used for ADC
  EdgeCounter_Init();	//Used for slider direction button
	//Touch_Init();
	Sound_Init(startingPeriod); //Initialize sound driver with starting period defined above
	SongPlay_Init(); //Initialize driver to play our song
	EnableInterrupts();
	//EnableSliders();
}




