// Lab10 Arcade Style Game
// GameControl.c
// Runs on LM4F120 or TM4C123
// Brad Gray bg22946,  Feyzullah Kalyoncu fk2787
// Put the date here or look very silly

// Game Logic on Kentec EB-LM4F120-L35 for Reversi Game

#include "../inc/tm4c123gh6pm.h"
#include "SSD2119.h"
#include "RevArt.h"
#include "GameControl.h"
#include "ADC.h"
#include "IO.h"
#include "Song.h"
int direction[8][2]={{0,1},{1,0},{-1,0},{0,-1},{1,1},{-1,-1},{1,-1},{-1,1}};
int positiony[8]={R1,R2,R3,R4,R5,R6,R7,R8};
int positionx[8]={C1,C2,C3,C4,C5,C6,C7,C8};
int color,t1,t2,t3,t4,t5,top,DEPTH,gcc[10][10];
struct node
{
	int ar[10][10];
	int results;		
}a;
struct node U;

typedef struct selectedOption{
	int short difficutly;
  const unsigned char* Player1Color;
	const unsigned char* Player2Color;
	unsigned short Player1ColorNum; //orange = 1; white = 2 
	unsigned short Player2ColorNum; //orange = 1; white = 2 
	unsigned char Mode;
}Settings;

typedef struct rowAndCol{
	unsigned short RowPos;
  unsigned char RowNum;	
  unsigned short ColPos;	
	unsigned char ColNum;
	}rowColInfo;


typedef struct gameVariables{
		unsigned short numOrange;
		unsigned short numBlack;
		unsigned char gameFinished;
		int results;
		int takenMoves[8][8];
	  unsigned char oScore;
	  unsigned char wScore;
	}stats;	

void ShowMenuScreen1(void);
void ShowMenuScreen2(void);
void ControlMenuScreen1(void);
void ControlMenuScreen2(void);
void ControlMenuScreen0(void);
void ShowMenuScreen0(void);
void ShowTitleScreen(void);
void Run_Setup(void );
void Input_Button_Reset(void);
void Play_Board(void);
void Board_Setup(void);
void Player1_Turn(void);
void Player2_Turn(void);
struct rowAndCol TranslatePosition_to_Row(unsigned short position);
unsigned char Slider_Button_Input(void);
struct rowAndCol  TranslatePosition_to_Col(unsigned short position);
unsigned char ValidMove(unsigned int Row, unsigned int Col);
void ResetTakenMoves(void);
struct rowAndCol Find_Empty_Board_Pos(void);
unsigned short ConvRowIndextoRowPos(unsigned char index);
unsigned short ConvColIndextoColPos(unsigned char index);
void WriteBoard(void);
void ComputersPlay(void);
int max(struct node s,int depth);
int min(struct node s,int depth);
unsigned char Legal_Move(unsigned int Row, unsigned int Col);
void UpdateScoreDisplay(void);
void play(int a,int b,int row);
void makeitequal2(void);
void makeitequal1(void);
void PlayersPlay(int c,int b);
void GameOver(void);


stats gameStats;
Settings playerSettings;
unsigned short gameState=1; 
 

#define INT_MAX 0xFFFFFFFF
#define INT_MIN -0x7FFFFFFF
#define veryEasy 1
#define Easy 2
#define Medium 3
#define Hard 4
#define veryHard 5
#define Impossible 6
#define titleScreen 1
#define MenuScreen0 2
#define MenuScreen1 3
#define MenuScreen2 4
#define GameBoard   5
#define TRUE 1
#define FALSE 0
#define PVP 0
#define PVC 1



void Run_Reversi(void){
			
			PlayBackgroundMusic();
			gameState=1;
			Run_Setup();
			Play_Board();
}	

void Run_Setup(void){
	switch (gameState){
		case titleScreen: 
			ShowTitleScreen();
			break;
		case MenuScreen0:
			ControlMenuScreen0();
			break;
		case MenuScreen1: 
			ControlMenuScreen1();
			break;
		case MenuScreen2: 
			ControlMenuScreen2();
			break;
	}	
}			

void Play_Board(void){
	
	int temp;
	const unsigned char* P1Color = playerSettings.Player1Color;
	const unsigned char* P2Color = playerSettings.Player2Color;
	unsigned char playMode = playerSettings.Mode;
	Board_Setup();
	
	ResetTakenMoves();

	gameStats.gameFinished=0;
	gameStats.oScore=2;
	gameStats.wScore=2;
		
	
	
	
	while (1){ 
  	Input_Button_Reset();	
   	makeitequal1();
		playerSettings.Player1Color = P1Color;
		playerSettings.Player2Color = P2Color;
    Player1_Turn();
		makeitequal2();
		UpdateScoreDisplay();
		Input_Button_Reset();
    WriteBoard();
		for(temp=0xFFFFFF;temp>0;temp--){};

	if (playMode == PVP){		
	 	Player2_Turn();
	}
	else {
				makeitequal1();		
				playerSettings.Player1Color = P1Color;
				playerSettings.Player2Color = P2Color;
			  ComputersPlay();
				makeitequal2();
 
	}	
	WriteBoard();	
    UpdateScoreDisplay();			
	}	
}	
void WriteBoard(void)
{
	int i,j=0;
	for(i=0;i<8;i++)
	{
		for(j=0;j<8;j++)
		{
			if(gameStats.takenMoves[i][j]==1)
						LCD_DrawBMP(Orange_Tile,positionx[j],positiony[i]);	
			else if(gameStats.takenMoves[i][j]==2)
						LCD_DrawBMP(White_Tile,positionx[j],positiony[i]);
		}
	}
}

void Player1_Turn(){
	unsigned short rawPosition;
	rowColInfo cursorInfo; //struct to store row  and col position for display and number for use
	rowColInfo tempInfo;
	unsigned char prevCursorRowNum;
	unsigned char prevCursorColNum;
	unsigned int prevCursorRowPos;
	unsigned int prevCursorColPos;
	unsigned char dir;
	unsigned int delay;
	
	
	
	 cursorInfo = Find_Empty_Board_Pos();
   prevCursorRowNum = cursorInfo.RowNum;
   prevCursorColNum = cursorInfo.ColNum;	
   prevCursorRowPos = cursorInfo.RowPos;
   prevCursorColPos = cursorInfo.ColPos;	
	 Input_Button_Reset();
	
	//flash player cursor to get attention 
	LCD_DrawBMP(playerSettings.Player1Color, cursorInfo.ColPos, cursorInfo.RowPos);
	 for(delay=0xFFFFF; delay>0; delay--){};
	LCD_DrawFilledRect(cursorInfo.ColPos,prevCursorRowPos,25,27,BLACK); 
		for(delay=0xFFFFF; delay>0; delay--){}; 
	 LCD_DrawBMP(playerSettings.Player1Color, cursorInfo.ColPos, cursorInfo.RowPos);
		for(delay=0xFFFFF; delay>0; delay--){};	
	LCD_DrawFilledRect(cursorInfo.ColPos,prevCursorRowPos,25,27,BLACK); 
		for(delay=0xFFFFF; delay>0; delay--){};	
	 LCD_DrawBMP(playerSettings.Player1Color, cursorInfo.ColPos, cursorInfo.RowPos);
		for(delay=0xFFFFF; delay>0; delay--){};	
	LCD_DrawFilledRect(cursorInfo.ColPos,prevCursorRowPos,25,27,BLACK);
		for(delay=0xFFFFF; delay>0; delay--){};	
	 LCD_DrawBMP(playerSettings.Player1Color, cursorInfo.ColPos, cursorInfo.RowPos);
	 //end flash
		
		while (Main_Button_Input() == 0){
		 while (ADCMailStatus == 1){
			rawPosition = Slider_ADCMail;	
			dir=Slider_Direction();
			 switch(dir){ //Direction = 0: move up/down; 1: left/right
				case 0:	tempInfo = TranslatePosition_to_Row (rawPosition);	 //returns struct with both lcd position and number
//								if(Legal_Move(tempInfo.RowNum,tempInfo.ColNum)==TRUE){
								cursorInfo.RowNum = tempInfo.RowNum;
								cursorInfo.RowPos = tempInfo.RowPos;
								if ((cursorInfo.RowNum != prevCursorRowNum) && (ValidMove(cursorInfo.RowNum, cursorInfo.ColNum)==TRUE ) && (ValidMove(prevCursorRowNum, cursorInfo.ColNum)==TRUE)) {
										LCD_DrawBMP(playerSettings.Player1Color, cursorInfo.ColPos, cursorInfo.RowPos);									
										LCD_DrawFilledRect(prevCursorColPos,prevCursorRowPos,25,27,BLACK); 
										prevCursorRowNum = cursorInfo.RowNum;	
									  prevCursorRowPos = cursorInfo.RowPos;
								}	
//							}
							break;
				case 1:	tempInfo = TranslatePosition_to_Col(rawPosition);
	//						  if(Legal_Move(tempInfo.RowNum,tempInfo.ColNum)==TRUE){
								cursorInfo.ColNum = tempInfo.ColNum;
								cursorInfo.ColPos = tempInfo.ColPos;
								if ((cursorInfo.ColNum != prevCursorColNum)&& (ValidMove(cursorInfo.RowNum, cursorInfo.ColNum)==TRUE)&&(ValidMove(cursorInfo.RowNum, prevCursorColNum)==TRUE)){
										LCD_DrawBMP(playerSettings.Player1Color, cursorInfo.ColPos, cursorInfo.RowPos);
										LCD_DrawFilledRect(prevCursorColPos, prevCursorRowPos,25,27,BLACK); 
										prevCursorColPos = cursorInfo.ColPos;
										prevCursorColNum = cursorInfo.ColNum;
								}	
		//					}
							break;				
		}	
			
		ADCMailStatus = 0;	
			}
		}
 
	LCD_DrawBMP(playerSettings.Player1Color, cursorInfo.ColPos, cursorInfo.RowPos); 
	gameStats.takenMoves[cursorInfo.RowNum-1][cursorInfo.ColNum-1] = playerSettings.Player1ColorNum;	 
//	gameStats=play(gameStats,cursorInfo.RowNum-1,cursorInfo.ColNum-1,playerSettings.Player1ColorNum);		
  PlayersPlay(cursorInfo.RowNum,cursorInfo.ColNum);
}	

void Player2_Turn(){
	unsigned short rawPosition;
	rowColInfo cursorInfo; //struct to store row  and col position for display and number for use
	rowColInfo tempInfo;
	unsigned int prevCursorRowPos;
	unsigned int prevCursorColPos;
	unsigned char prevCursorRowNum;
	unsigned char prevCursorColNum;

	
	
	 cursorInfo = Find_Empty_Board_Pos();
   prevCursorRowPos = cursorInfo.RowPos;
   prevCursorColPos = cursorInfo.ColPos;
   prevCursorRowNum = cursorInfo.RowNum;
   prevCursorColNum = cursorInfo.ColNum;	
	 Input_Button_Reset();
	 while (Main_Button_Input() == 0){
		 while (ADCMailStatus == 1){
			rawPosition = Slider_ADCMail;	
			
			 switch(Slider_Direction()){ //Direction = 0: move up/down; 1: left/right
				case 0:	tempInfo = TranslatePosition_to_Row (rawPosition);	 //returns struct with both lcd position and number
//								if(Legal_Move(tempInfo.RowNum,tempInfo.ColNum)==TRUE){
								cursorInfo.RowNum = tempInfo.RowNum;
								cursorInfo.RowPos = tempInfo.RowPos;
								if ((cursorInfo.RowNum != prevCursorRowNum) && (ValidMove(cursorInfo.RowNum, cursorInfo.ColNum)==TRUE ) && (ValidMove(prevCursorRowNum, cursorInfo.ColNum)==TRUE)) {
										LCD_DrawBMP(playerSettings.Player1Color, cursorInfo.ColPos, cursorInfo.RowPos);									
										LCD_DrawFilledRect(prevCursorColPos,prevCursorRowPos,25,27,BLACK); 
										prevCursorRowNum = cursorInfo.RowNum;	
									  prevCursorRowPos = cursorInfo.RowPos;
								}	
//							}
							break;
				case 1:	tempInfo = TranslatePosition_to_Col(rawPosition);
	//						  if(Legal_Move(tempInfo.RowNum,tempInfo.ColNum)==TRUE){
								cursorInfo.ColNum = tempInfo.ColNum;
								cursorInfo.ColPos = tempInfo.ColPos;
								if ((cursorInfo.ColNum != prevCursorColNum)&& (ValidMove(cursorInfo.RowNum, cursorInfo.ColNum)==TRUE)&&(ValidMove(cursorInfo.RowNum, prevCursorColNum)==TRUE)){
										LCD_DrawBMP(playerSettings.Player1Color, cursorInfo.ColPos, cursorInfo.RowPos);
										LCD_DrawFilledRect(prevCursorColPos, prevCursorRowPos,25,27,BLACK); 
										prevCursorColPos = cursorInfo.ColPos;
										prevCursorColNum = cursorInfo.ColNum;
								}	
		//					}
							break;				
		}	
			
		ADCMailStatus = 0;	
			}
		}
	LCD_DrawBMP(playerSettings.Player1Color, cursorInfo.ColPos, cursorInfo.RowPos); 

	gameStats.takenMoves[cursorInfo.RowNum-1][cursorInfo.ColNum-1] = playerSettings.Player2ColorNum;
	color=3-color;
  PlayersPlay(cursorInfo.RowNum,cursorInfo.ColNum);
	color=3-color;
		
		
	 
}

void Board_Setup(void){
	LCD_ColorFill(BLACK);
	LCD_Goto(0,5);
	LCD_SetTextColor(255,127,0);
	printf("O:");
	LCD_Goto(1,7);
	LCD_SetTextColor(255,127,0);
	printf("2");
	LCD_Goto(48,5);
	LCD_SetTextColor(240,248,255);
	printf("W:");
	LCD_Goto(49,7);
	LCD_SetTextColor(240,248,255);
	printf("2");

	
  LCD_DrawLine(40,5,280,5,BLUE);
//	LCD_DrawLine(40,238,280,238,BLUE);
	LCD_DrawLine(40,5,40,240,BLUE);
	LCD_DrawLine(280,5,280,238,BLUE);
	
	LCD_DrawLine(41,35,279,35,GREEN);
	LCD_DrawLine(41,65,279,65,GREEN);
	LCD_DrawLine(41,95,279,95,GREEN);
	LCD_DrawLine(41,125,279,125,GREEN);
  LCD_DrawLine(41,154,279,154,GREEN);
	LCD_DrawLine(41,185,279,185,GREEN);
	LCD_DrawLine(41,213,279,213,GREEN);
	
	
	LCD_DrawLine(70,6,70,237,GREEN);
	LCD_DrawLine(100,6,100,237,GREEN);
	LCD_DrawLine(130,6,130,237,GREEN);
	LCD_DrawLine(160,6,160,237,GREEN);
	LCD_DrawLine(190,6,190,237,GREEN);
	LCD_DrawLine(220,6,220,237,GREEN);
	LCD_DrawLine(250,6,250,237,GREEN);
	
	
	
	LCD_DrawBMP(White_Tile, C4,R4);
	LCD_DrawBMP(Orange_Tile, C4, R5);
	LCD_DrawBMP(White_Tile, C5,R5);
	LCD_DrawBMP(Orange_Tile, C5, R4);
	
	
	}

void ShowTitleScreen(void){
	LCD_ColorFill(WHITE);	
	LCD_DrawBMP(title_graphic2, C4, R4);	
	LCD_DrawBMP(reversi_title2, C3, R2);
  
	LCD_Goto(11,20);
	LCD_SetTextColor(255,0,255); 
	LCD_PrintString("Brad Gray & Feyzullah Kalyoncu");
	LCD_Goto(14,22);
	LCD_PrintString("Press Button to Start");
	Input_Button_Reset();
	Input_Button_Reset();
	Input_Button_Reset();
	while (Main_Button_Input() != 1){};
	gameState++;
	Run_Setup();
		
		}	

void ShowMenuScreen0(void){
	LCD_ColorFill(BLACK);
	LCD_DrawLine(10,16,310,16,BLUE);
	LCD_DrawLine(10,222,310,222,BLUE);
	LCD_DrawLine(10,16,10,222,BLUE);
	LCD_DrawLine(310,16,310,222,BLUE);
	LCD_Goto(15,6);
	LCD_SetTextColor(0,255,255); 
	LCD_PrintString("Select Game Type");
	LCD_DrawLine(90,68,190,68,GREEN);
	
	LCD_Goto(15,9);	
	printf("Player Vs Player");	
	LCD_Goto(15,11);	
	printf("Player Vs Computer");	
	LCD_Goto(15,13);	
	printf("Cancel");	
	
	}	

void ControlMenuScreen0(void){

	unsigned short rawPosition;
	typedef struct cursor{
		unsigned short xPos, yPos;
		unsigned short prevXPos, prevYPos;
		} cursorData;	
	
	cursorData menuCursor;
	ShowMenuScreen0();
  menuCursor.xPos = 77;
	menuCursor.yPos = 96;
	menuCursor.prevYPos = 96;
	Input_Button_Reset();
  Input_Button_Reset();
  		
	
		while (Main_Button_Input() == 0){
			while (ADCMailStatus == 1){
				rawPosition = Slider_ADCMail;	
				if(rawPosition>2000){
					menuCursor.yPos = 77;
				}
				else if((rawPosition<=2000)&& (rawPosition>1000)){
					menuCursor.yPos = 96;
				}
				else {
					menuCursor.yPos = 116;
				}
				if (menuCursor.yPos != menuCursor.prevYPos)
					
				{	LCD_DrawFilledRect(menuCursor.xPos, menuCursor.prevYPos,10,15,BLACK); 
					LCD_DrawBMP(cursor, menuCursor.xPos, menuCursor.yPos);
					menuCursor.prevYPos=menuCursor.yPos;
				}
				
				ADCMailStatus = 0;
		  }
		}
		if (Main_Button_Input() ==1){
			switch (menuCursor.yPos){
				case  (77):
					playerSettings.Mode= PVP;
				  gameState=MenuScreen2;
					break;
				case  (96):
					playerSettings.Mode = PVC;
				  gameState++;
					break;
				case  (116):
					playerSettings.difficutly= PVP;
					gameState--;
					break;
			}	
		}
  Run_Setup();		
}			
	
void ShowMenuScreen1(void){
	LCD_ColorFill(BLACK);
	LCD_DrawLine(10,16,310,16,BLUE);
	LCD_DrawLine(10,222,310,222,BLUE);
	LCD_DrawLine(10,16,10,222,BLUE);
	LCD_DrawLine(310,16,310,222,BLUE);
	LCD_Goto(15,6);
	LCD_SetTextColor(0,255,255); 
	LCD_PrintString("Select Difficulty");
	LCD_DrawLine(90,68,190,68,GREEN);
	
	LCD_Goto(15,9);	
	printf("Easy");	
	LCD_Goto(15,11);	
	printf("Normal");	
	LCD_Goto(15,13);	
	printf("Cancel");	
	
	}

void ControlMenuScreen1(void){

	unsigned short rawPosition;
	
	typedef struct cursor{
		unsigned short xPos, yPos;
		unsigned short prevXPos, prevYPos;
		} cursorData;	
	
	cursorData menuCursor;
	ShowMenuScreen1();
  menuCursor.xPos = 77;
	menuCursor.yPos = 96;
	menuCursor.prevYPos = 96;
	Input_Button_Reset();
  Input_Button_Reset();			
	
		while (Main_Button_Input() == 0){
			while (ADCMailStatus == 1){
				rawPosition = Slider_ADCMail;	
				if(rawPosition>2000){
					menuCursor.yPos = 77;
				}
				else if((rawPosition<=2000)&& (rawPosition>1000)){
					menuCursor.yPos = 96;
				}
				else {
					menuCursor.yPos = 116;
				}
				if (menuCursor.yPos != menuCursor.prevYPos)
					
				{	LCD_DrawFilledRect(menuCursor.xPos, menuCursor.prevYPos,10,15,BLACK); 
					LCD_DrawBMP(cursor, menuCursor.xPos, menuCursor.yPos);
					menuCursor.prevYPos=menuCursor.yPos;
				}
				
				ADCMailStatus = 0;
		  }
		}
		if (Main_Button_Input() ==1){
			switch (menuCursor.yPos){
				case  (77):
					playerSettings.difficutly= veryEasy;
				  DEPTH =1;
					gameState++;
					break;
				case  (96):
					playerSettings.difficutly= Easy;
					gameState++;
					DEPTH =2;
					break;
				case  (116):
					playerSettings.difficutly= Medium;
					gameState++;
					DEPTH =3;
					break;
				case  (187):
					playerSettings.difficutly= veryEasy;
					gameState=MenuScreen0;
					DEPTH =1;
					break;
			}	
		 
		 Run_Setup();
		}	
	
}		

void ShowMenuScreen2(void){
	LCD_ColorFill(BLACK);
	LCD_DrawLine(10,16,310,16,BLUE);
	LCD_DrawLine(10,222,310,222,BLUE);
	LCD_DrawLine(10,16,10,222,BLUE);
	LCD_DrawLine(310,16,310,222,BLUE);
	LCD_Goto(15,6);
	LCD_PrintString("Select Player 1 Tile Color");
	LCD_DrawLine(90,68,190,68,GREEN);
	
	LCD_Goto(15,9);
  LCD_SetTextColor(255,127,0);	
	printf("Orange");	
	LCD_Goto(15,11);	
	LCD_SetTextColor(240,248,255);
	printf("White");
  LCD_Goto(15,14);	
	LCD_SetTextColor(0,255,0);
	printf("Cancel");		
	

	}

	void ControlMenuScreen2(void){

	unsigned short rawPosition;
	
	typedef struct cursor{
		unsigned short xPos, yPos;
		unsigned short prevXPos, prevYPos;
		} cursorData;	
	
	cursorData menuCursor;
	ShowMenuScreen2();
  menuCursor.xPos = 77;
	menuCursor.yPos = 90;
	menuCursor.prevYPos = 90;
	Input_Button_Reset();	
	Input_Button_Reset();		
	
		while (Main_Button_Input() == 0){
			while (ADCMailStatus == 1){
				rawPosition = Slider_ADCMail;	
				if(rawPosition>2000){
					menuCursor.yPos = 77;
				}
				else if(rawPosition>1000 && rawPosition<=2000){
					menuCursor.yPos = 96;
				}
				else{
					menuCursor.yPos = 122;
				}
				
				
				if (menuCursor.yPos != menuCursor.prevYPos)
				{	LCD_DrawFilledRect(menuCursor.xPos, menuCursor.prevYPos,10,15,BLACK); 
					LCD_DrawBMP(cursor, menuCursor.xPos, menuCursor.yPos);
					menuCursor.prevYPos=menuCursor.yPos;
				}
				
				ADCMailStatus = 0;
		  }
		}
		if (Main_Button_Input() == 1){
			switch (menuCursor.yPos){
				case  (77):
					color=2;
					playerSettings.Player1ColorNum=1;
				  playerSettings.Player2ColorNum=2;
					playerSettings.Player1Color= Orange_Tile;
				  playerSettings.Player2Color= White_Tile;
					gameState++;
					break;
				case  (96):
					color=1;
					playerSettings.Player1ColorNum=2;
					playerSettings.Player2ColorNum=1;
					playerSettings.Player1Color= White_Tile;
				  playerSettings.Player2Color= Orange_Tile;
					gameState++;
					break;
				case  (122):
					gameState=MenuScreen0;
					break;
			}	
			Run_Setup();
		}	
	
}		



struct rowAndCol TranslatePosition_to_Row(unsigned short position){
	
	
	
	rowColInfo translatedRowInfo;
		
	if(position>3500){
					translatedRowInfo.RowPos = R8;
					translatedRowInfo.RowNum = 8;
				}
	else if(position<3500&&position>=3000){
					translatedRowInfo.RowPos = R7;
					translatedRowInfo.RowNum = 7;
				}
	else if(position<3000&&position>=2500){
					translatedRowInfo.RowPos = R6;
					translatedRowInfo.RowNum = 6;
				}
	else if(position<2500&&position>=2000){
					translatedRowInfo.RowPos = R5;
					translatedRowInfo.RowNum = 5;
				}
	else if(position<2000&&position>=1500){
					translatedRowInfo.RowPos = R4;
					translatedRowInfo.RowNum = 4;
				}
	else if(position<1500&&position>=1000){
					translatedRowInfo.RowPos = R3;
					translatedRowInfo.RowNum = 3;
				}
		else if(position<1000&&position>=500){
					translatedRowInfo.RowPos = R2;
					translatedRowInfo.RowNum = 2;
				}
	else {
					translatedRowInfo.RowPos = R1;
					translatedRowInfo.RowNum = 1;
				}
	return 	translatedRowInfo;	
}


 
struct rowAndCol TranslatePosition_to_Col(unsigned short position){
	rowColInfo translatedColInfo;
	
	if(position>3500){
					translatedColInfo.ColPos = C8;
					translatedColInfo.ColNum = 8;
				}
		else if(position<3500&&position>=3000){
					translatedColInfo.ColPos = C7;
					translatedColInfo.ColNum = 7;
				}
	else if(position<3000&&position>=2500){
					translatedColInfo.ColPos = C6;
					translatedColInfo.ColNum = 6;
				}
	else if(position<2500&&position>=2000){
					translatedColInfo.ColPos = C5;
					translatedColInfo.ColNum = 5;
				}
	else if(position<2000&&position>=1500){
					translatedColInfo.ColPos = C4;
					translatedColInfo.ColNum =4;
				}
	else if(position<1500&&position>=1000){
					translatedColInfo.ColPos = C3;
					translatedColInfo.ColNum = 3;
				}
	else if(position<1000&&position>=500){
					translatedColInfo.ColPos = C2;
					translatedColInfo.ColNum = 2;
				}
	else {
					translatedColInfo.ColPos = C1;
					translatedColInfo.ColNum = 1;
				}
	return 	translatedColInfo;	
}

unsigned char ValidMove(unsigned int Row, unsigned int Col){
	unsigned char result;	
	if (gameStats.takenMoves[Row-1][Col-1] != 0){
	result=FALSE;	
	}	
  else result=TRUE;
	return result;
}	

void ResetTakenMoves(void){
	unsigned char index, index2;
	for(index=0; index<8; index++){
		for (index2=0; index2<8; index2++){
			gameStats.takenMoves[index][index2] = 0;
		}
	}	
	gameStats.takenMoves[4-1][4-1] = 2;  //row 4, substracting 1 to put in index 2; etc.
	gameStats.takenMoves[4-1][5-1] = 1;
	gameStats.takenMoves[5-1][4-1] = 1;
	gameStats.takenMoves[5-1][5-1] = 2;

}	

struct rowAndCol Find_Empty_Board_Pos(void){
	unsigned char rowIndex, colIndex;
	rowColInfo EmptySpot;
  EmptySpot.ColNum =1;
	EmptySpot.ColPos = 43;
	EmptySpot.RowNum = 1;
	EmptySpot.RowPos = 6;
	
	for(rowIndex=0; rowIndex<8; rowIndex++){
		for (colIndex=0; colIndex<8; colIndex++){
			 if ((gameStats.takenMoves[rowIndex][colIndex] == 0)&& (Legal_Move(rowIndex+1, colIndex+1)==TRUE)){
				 EmptySpot.RowPos=ConvRowIndextoRowPos(rowIndex);
				 EmptySpot.RowNum=rowIndex+1;
				 EmptySpot.ColPos=ConvColIndextoColPos(colIndex);
				 EmptySpot.ColNum=colIndex+1;
				 return EmptySpot;
			 } 
		}
	}
if (ValidMove(EmptySpot.RowNum, EmptySpot.ColNum) == FALSE) {
	GameOver();
}

 return EmptySpot;	
}	

unsigned short ConvRowIndextoRowPos(unsigned char index){
	unsigned int result;
	switch (index){
				case 0: result=R1;
								break;
				case 1: result=R2;
								break; 
				case 2: result=R3;
								break; 
				case 3: result=R4;
								break;
				case 4: result=R5;
								break; 	
				case 5: result=R6;
								break;
				case 6: result=R7;
								break;
		    case 7: result=R8;
				default: result=R1;
								break;

	}
	return result;
}	

unsigned short ConvColIndextoColPos(unsigned char index){
	unsigned int result;
	switch (index){
				case 0: result=C1;
								break;
				case 1: result=C2;
								break; 
				case 2: result=C3;
								break; 
				case 3: result=C4;
								break;
				case 4: result=C5;
								break; 	
				case 5: result=C6;
								break;
				case 6: result=C7;
								break;
		    case 7: result=C8;
								break; 	

				default: result=C1;
								break;

	}
	return result;
}	

unsigned char Legal_Move(unsigned int Row, unsigned int Col){
	unsigned char rowIndex, colIndex, result;
	rowIndex = Row-1;
	colIndex= Col-1;

			 if ((gameStats.takenMoves[rowIndex+1][colIndex] != 0) || (gameStats.takenMoves[rowIndex-1][colIndex] != 0) ||
				 (gameStats.takenMoves[rowIndex][colIndex+1] != 0)||(gameStats.takenMoves[rowIndex][colIndex-1] != 0)||
					(gameStats.takenMoves[rowIndex-1][colIndex-1] != 0)||(gameStats.takenMoves[rowIndex+1][colIndex+1] != 0)||
					(gameStats.takenMoves[rowIndex-1][colIndex+1] != 0)||(gameStats.takenMoves[rowIndex+1][colIndex-1] != 0))	{
				 result = TRUE;
			 } 
	
		
			else result=FALSE;

 return result;	
}		

void UpdateScoreDisplay(void){
	unsigned char index, index2, emptyCount;
	gameStats.oScore=0;
	gameStats.wScore=0;
	emptyCount = 0;
	for(index=0; index<8; index++){
		for (index2=0; index2<8; index2++){
			if 	(gameStats.takenMoves[index][index2] == 1){
				gameStats.oScore++; 
			}
			else if (gameStats.takenMoves[index][index2] == 2){	
		     gameStats.wScore++;
			}
			else emptyCount++;
	  }	
   } 
  index=gameStats.oScore;
	index2=gameStats.wScore; 
	LCD_DrawFilledRect(4,60,20,15,BLACK); 
	LCD_DrawFilledRect(289,60,20,15,BLACK); 
	LCD_Goto(1,7);
	LCD_SetTextColor(255,127,0);
	printf(" %d ", index);
	LCD_Goto(49,7);
	LCD_SetTextColor(240,248,255);
	printf(" %d ", index2);
	if (emptyCount == 0){
	   GameOver();	
	}	
 }

int calculate(int ar[][10])
{
	int i,j,s1=0,s2=0,s3;
	for(i=0;i<8;i++)
		for(j=0;j<8;j++)
		{
			s3=1;
			{if(((i==0 || i==7) && (j==0 || j==7)))
				s3=7;
			else if((i==0 || i==7) || (j==0 || j==7))
				s3=3;}
			if(ar[i][j]==color) 	s1+=s3;
			if(ar[i][j]==3-color)	s2+=s3;
		}
	return s1-s2;	
}
void play(int a,int b,int row)
{
	int i,t=0;
	int k,l,x,y;
	for(i=0;i<8;i++)
	{
		x=direction[i][0]; y=direction[i][1];
		k=a+x; l=b+y;
		for(;k>=0 && l>=0 && k<8 && l<8 && gcc[k][l];k+=x,l+=y)
			if(gcc[k][l]==row) break;
		if(k>=0 && l>=0 && k<8 && l<8 && gcc[k][l]==row)
			for(k=a+x,l=b+y;gcc[k][l]!=row;k+=x,l+=y){ gcc[k][l]=row; t=1;}
	}
	if(t) gcc[a][b]=row;
}

int min(struct node s,int depth)
{
	int i,j,results,t,l,m;
	struct node ge;
	for(i=0;i<8;i++)
		for(j=0;j<8;j++)
				ge.ar[i][j]=s.ar[i][j];
	if(!depth)return calculate(s.ar);  // control depth
	results=INT_MAX; 
	for(i=0;i<8;i++)
		for(j=0;j<8;j++)
			if(!s.ar[i][j]) // try every possible move 
			{
				for(l=0;l<8;l++)
						for(m=0;m<8;m++)
								gcc[l][m]=s.ar[l][m];
				play(i,j,3-color); 
				for(l=0;l<8;l++)
						for(m=0;m<8;m++)
								ge.ar[l][m]=gcc[l][m];		
				if(ge.ar[i][j])
				{
					t=max(ge,depth-1);	// send new board to max function in order to get min result		
					if(t<=results)
						results=t;
				}				
			}
	return results;
}
int max(struct node s,int depth)
{
	int i,j,results,t,l,m;
	struct node ge;
	for(i=0;i<8;i++)
		for(j=0;j<8;j++)
				ge.ar[i][j]=s.ar[i][j];
	if(!depth)return calculate(s.ar); // control depth
	results=INT_MIN; 
	for(i=0;i<8;i++)
		for(j=0;j<8;j++)
			if(!s.ar[i][j]) // try every possible move
			{
				for(l=0;l<8;l++)
						for(m=0;m<8;m++)
								gcc[l][m]=s.ar[l][m];
				play(i,j,color);
				for(l=0;l<8;l++)
						for(m=0;m<8;m++)
								ge.ar[l][m]=gcc[l][m];						
				if(ge.ar[i][j])
				{
					t=min(ge,depth-1);		// send new board to min function in order to get max result
					if(t>=results)  
					{
						results=t;
						if(depth==DEPTH){t1=i;t2=j;} // if we reach DEPTH specified, keep computer's best move
					}
				}
			}
	return results;
}

void PlayersPlay(int c,int b)
{
	int l,m;
	U.ar[c-1][b-1]=3-color;
	for(l=0;l<8;l++)
			for(m=0;m<8;m++)
					gcc[l][m]=U.ar[l][m];				
	play(c-1,b-1,3-color);
	for(l=0;l<8;l++)
			for(m=0;m<8;m++)
					U.ar[l][m]=gcc[l][m];				
	
}
void ComputersPlay(void)
{
	int l,m;
	max(U,DEPTH); // call max function to maximize computer's score
	U.ar[t1][t2]=color; // choosing t1,t2 is best move from computer's point of view
//	for(l=0;l<8;l++)
//			for(m=0;m<8;m++)
//					gcc[l][m]=U.ar[l][m];				
	play(t1,t2,color);
	for(l=0;l<8;l++)
			for(m=0;m<8;m++)
					U.ar[l][m]=gcc[l][m];				
	
}


void makeitequal1(void)
{
	int i,j;
	for(i=0;i<8;i++)
		for(j=0;j<8;j++)
				U.ar[i][j]=gameStats.takenMoves[i][j];
}
void makeitequal2(void)
{
	int i,j;
	for(i=0;i<8;i++)
		for(j=0;j<8;j++)
				gameStats.takenMoves[i][j]=U.ar[i][j];
}

void GameOver(void){
	gameStats.gameFinished=1;
  LCD_ColorFill(BLACK);
	LCD_Goto(10,10);
	LCD_SetTextColor(255,127,0);
	if (gameStats.oScore>gameStats.wScore){
	printf("Orange Wins");
	}
  else {
		LCD_SetTextColor(240,248,255);
		printf ("White Wins");	
	}
	LCD_Goto(20,15);
	LCD_SetTextColor(100,100,100);
	printf("Press Main Button");
	Input_Button_Reset();
	while (Main_Button_Input() == 0){}	
  Run_Reversi();
}
   		
