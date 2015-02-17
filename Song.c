// Program written by:  Brad Gray bg22946,  Feyzullah Kalyoncu fk2787
// Date Created: 3/08/2014 
// Last Modified: 3/19/2014 
// Section 4-5pm     TA: Saugata Bhattacharyya
// Lab number: 6
// Driver to play a song
// Input: Song data structure in a -1 terminated array. Elements of the array are desired notes.






#include "tm4c123gh6pm.h"
#include "Sound.h"
#include "Song.h"
#include "ReversiMusic.h"


#define NVIC_EN0_INT19          0x00080000  // Interrupt 19 enable

unsigned char songPlayFlag;   // Global songPlayFlag to allow other functions to proceed or wait depending on state of song
int *songToPlay;
unsigned char songIndex; 
int t; 

long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value




void PlayBackgroundMusic(){
	
TIMER0_CTL_R |= TIMER_CTL_TAEN;  // enable timer0A
songPlayFlag = 1;
songIndex = 0;					//global index to count through the array for playing the song
songToPlay = TestSong; //songToPlay is a pointer to starting memory location of song array.
	
}

void End_Song(){
TIMER0_CTL_R &= ~TIMER_CTL_TAEN;	//disable timer0 to stop timer isr0 and stop song
songPlayFlag = 0;	
songIndex=0;
}	

void Timer0A_Handler (void){
		
				  
			
	int *nextNote = songToPlay + songIndex;	  //starting song memory location plus offset incremeneted with each timer0 ISR
	int freq = *nextNote;

	if (freq >= 0){   //-1 terminated array
		Sound_Play(freq);
		songIndex ++;
	}
	else {
		 PlayBackgroundMusic();
  }
	
	
	
	
	TIMER0_ICR_R = TIMER_ICR_TATOCINT; //acknowledge interrupt
}

void SongPlay_Init(void){
	//initialize timer0 to set tempo for song play	
	//Assuming 80 MHz clock = 12.5 ns per cycle - 80,000,000 cycles per second. 
	long sr; volatile unsigned long NOP;
	songPlayFlag = 0;
	t=0;
  sr = StartCritical();  //prohibit interrupts
  SYSCTL_RCGC1_R |= SYSCTL_RCGC1_TIMER0; // 0) activate timer0
	NOP = 2; 
  TIMER0_CTL_R &= ~TIMER_CTL_TAEN; // 1) disable timer0A during setup  (TAEN = Timer A Enable)                            
  TIMER0_CFG_R = TIMER_CFG_32_BIT_TIMER;  // 2) configure for 32-bit timer mode
  TIMER0_TAMR_R = TIMER_TAMR_TAMR_PERIOD;  // 3) configure for periodic mode, default down-count settings   
	TIMER0_TAILR_R = 0x005FFFFF; //initial reload value
  TIMER0_ICR_R = TIMER_ICR_TATOCINT;  // 5) clear timer0A timeout flag
  TIMER0_IMR_R |= TIMER_IMR_TATOIM;// 6) arm timeout interrupt
  NVIC_PRI4_R = (NVIC_PRI4_R&0x00FFFFFF)|0x40000000; // 7) priority 2
  NVIC_EN0_R = NVIC_EN0_INT19;     // 8) enable interrupt 19 in NVIC
  // timer left disabled to enable elsewhere
  EndCritical(sr);  //allow interrupts again
}

