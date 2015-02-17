// Lab10 Arcade Style Game
// GameControl.h
// Runs on TM4C123
// Brad Gray bg22946,  Feyzullah Kalyoncu fk2787
// Put the date here or look very silly

// Game Control on Kentec EB-LM4F120-L35 for Reversi Game

#ifndef GAMECTRL_H
#define GAMECTRL_H

//Definitions to Translate Kentec LCD  position to game board position
#define C1 43
#define C2 73
#define C3 103
#define C4 133
#define C5 163
#define C6 194
#define C7 224
#define C8 252
#define R1 6
#define R2 36
#define R3 66
#define R4 96
#define R5 126
#define R6 156
#define R7 186
#define R8 216



//----------Run_Reversi--------------//
// Sets up Graphics
// Controls Menu and Game Board
// Controls Game Logic
void Run_Reversi(void);



#endif
