// Lab10 Arcade Style Game
// GameControl.c
// Runs on LM4F120 or TM4C123
// Brad Gray bg22946,  Feyzullah Kalyoncu fk2787
// Put the date here or look very silly

// Game Logic on Kentec EB-LM4F120-L35 for Reversi Game

#include "../inc/tm4c123gh6pm.h"
#include "SSD2119.h"
#include "IO.h"
#include "ADC.h"

unsigned long Slider_ADCMail;
unsigned char ADCMailStatus;
unsigned long ADCData; 
unsigned char sliderDirection;


void Input_Button_Init(void){
	//use PF2for input from main button
		unsigned long volatile delay;
	  SYSCTL_RCGC2_R |= 0x20;			//start clock for port F
		delay = SYSCTL_RCGC2_R;
		GPIO_PORTF_AFSEL_R &= ~0xE;
	  GPIO_PORTF_DATA_R &= ~0x4;
		GPIO_PORTF_DIR_R  &= ~0x4;
		GPIO_PORTF_DEN_R  |=  0x4;  

}

unsigned char Main_Button_Input(void){
		unsigned char result = (GPIO_PORTF_DATA_R & 0x04)>>2;
	result = result;
	  return result;
}

void Timer2_Init(void){ 
  unsigned long volatile delay;
 SYSCTL_RCGC1_R |= SYSCTL_RCGC1_TIMER2;  // 0) activate timer2
  delay = SYSCTL_RCGCTIMER_R;
  TIMER2_CTL_R &= ~TIMER_CTL_TAEN;    // 1) disable timer2A during setup
  TIMER2_CFG_R = TIMER_CFG_32_BIT_TIMER;    // 2) configure for 32-bit mode
  TIMER2_TAMR_R = TIMER_TAMR_TAMR_PERIOD;   // 3) configure for periodic mode, default down-count settings
  TIMER2_TAILR_R = 1000000;    // 4) reload value; need sampling rate of 40 Hz
  TIMER2_TAPR_R = 0;            // 5) bus clock resolution
  TIMER2_ICR_R = TIMER_ICR_TATOCINT;    // 6) clear timer2A timeout flag
  TIMER2_IMR_R |= TIMER_IMR_TATOIM;    // 7) arm timeout interrupt
  NVIC_PRI5_R = (NVIC_PRI5_R&0x00FFFFFF)|0x80000000; // 8) priority 4
// interrupts enabled in the main program after all devices initialized
// vector number 39, interrupt number 23
  NVIC_EN0_R = 1<<23;           // 9) enable IRQ 23 in NVIC
  TIMER2_CTL_R |= TIMER_CTL_TAEN;    // 10) enable timer2A
	ADCMailStatus=0;
}

void Timer2A_Handler(void){ 
  TIMER2_ICR_R = 0x00000001;   // acknowledge timer2A timeout
  ADCData = ADC_In();
	Slider_ADCMail = ADCData;
  ADCMailStatus = 1; // mail flag
}



void Input_Button_Reset(void){
//wait for button to not be pressed before continuing
		while ((GPIO_PORTF_DATA_R & 0x04) != 0) {};
			
}	
unsigned char Slider_Button_Input(void){
	unsigned char result;
	result = 	(GPIO_PORTF_DATA_R & 0x08)>>4;
	return result;	
}	


	
void EdgeCounter_Init(void){
		SYSCTL_RCGC2_R |= 0x020;
	  sliderDirection = 0;		
	  GPIO_PORTF_DIR_R &= ~0x08; //set PF3 as input
	  GPIO_PORTF_AFSEL_R &= ~0x08;
	  GPIO_PORTF_DEN_R |= 0x08;
	  GPIO_PORTF_AMSEL_R &= ~0x08;
	  GPIO_PORTF_IS_R &= ~0x08;			//PF3 is edge sensitive
	  GPIO_PORTF_IBE_R &= ~0x08;    //not both edges
		GPIO_PORTF_IEV_R &= ~0x08;		//set to falling edge
		GPIO_PORTF_ICR_R &= 0x08;     //clear flag
	  GPIO_PORTF_IM_R |= 0x08;			//arm interrupt on PF3
	  NVIC_PRI7_R = (NVIC_PRI7_R&0xFF00FFFF) | 0x00A00000; //priority 5
	  NVIC_EN0_R = 0x40000000;   //enable interupt 30 on NVIC
}	

void GPIOPortF_Handler(void){
	GPIO_PORTF_ICR_R = 0x08;		//ack flag
	sliderDirection ^= 0x1;	
	
}

unsigned char Slider_Direction(void){
	//return 1 or 0
	return sliderDirection;
}
