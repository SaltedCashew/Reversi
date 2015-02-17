// This software plays a stored song
// Runs on LM4F120 or TM4C123
// Program written by:  Brad Gray bg22946,  Feyzullah Kalyoncu fk2787
// Date Created: 8/25/2013 
// Last Modified: 10/9/2013 
// Section 4-5pm     TA: Saugata Bhattacharyya
// Lab number: 6
//Song is here. Definitions below designed to allow song writing in more common notation.
//Designed to take data structure provided and convert for audio output

 extern unsigned char songPlayFlag; //flag to tell other system that a song is playing

/*#define C6    1046 //Hz			
#define B5    988 			
#define BF5   933 			
#define A5    880 			
#define AF5   831 			
#define G5    784 			
#define GF5   740 			
#define F5    699 			
#define E5    659 			
#define EF5   622 			
#define D5    587 			
#define DF5   554 			
#define C5   523 			
#define B4   494 			
#define BF4  466 			
#define A4   440 			
#define AF4  415 			
#define G4   392 			
#define GF4  370 			
#define F4   349 			
#define E4   329 			
#define EF4  311 			
#define D4   293 			
#define DF4  277 			
#define C4  262
 			
#define B3   494 			
#define BF3  466 			
#define A3   440 			
#define AF3  415 			
#define G3   392 			
#define GF3  370 			
#define F3   349 			
#define E3   329 			
#define EF3  311 			
#define D3   293 			
#define DF3  277 			

#define C_3  262 			*/
#define B3   246 			
#define BF3  233 			
#define A3   220 			
#define AF3  208 			
#define G3   196 			
#define GF3  185 			
#define F3   175 			
#define E3   165 			
#define EF3  156 			
#define D3   147 			
#define DF3  139 			
#define C_3  131			
#define B2   123 			
#define BF2  117 			
#define A2   110 			
#define AF2  104 			
#define G2   98 			
#define GF2  93 			
#define F2   87 			
#define E2   82 			
#define EF2  78 			
#define D2   73 			
#define DF2  69 			
#define C_2   65
#define B1   61 			
#define BF1  58 			
#define A1   55 			
#define AF1  52 			
#define G1   49 			
#define GF1  46 			
#define F1   43 			
#define E1   41 			
#define EF1  39 			
#define D1   37 			
#define DF1  35 			
#define C_1  33 			
#define B0   31 			
#define BF0  29 			
#define A0   28 			

 			


 			
		
// **************Play_Song*********************
// Plays a song
// Input: song data stored in array terminated by -1 
// Output: none
void PlayBackgroundMusic(void);


// **************SongPlay_Init*********************
// Intialializes Timer 0 for tempo use in song play and sets initial timer value. 
// Does not enable timer
// Input: none 
// Output: none
void SongPlay_Init(void);

// **************End_Song*********************
// Ends song when playing
// Input: none 
// Output: none
void End_Song(void);


