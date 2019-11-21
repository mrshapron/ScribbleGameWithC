
//-----------------------------------------------------------------------------
// 				  Scribble
// 				-----------
//
// General : the program simulates the game Scribble, 
//			 it tries to place words and to get points.
//
// Input : you give to the program a reading file that contain vocabulary 
//		   and the value of each word. it syntax needs to be like this 'WORD'(space)'VALUE'(new line)
//																	   'WORD' 'VALUE'
//
// Process : the program at the start put the most valued word in the center of the board,
//			 then it search words that can be fit with the existing letters
//			 it always try to place the valued words
//
// Output : the program writing the finished board into a writing file
//			in addition, it writes the total points.
//
//-----------------------------------------------------------------------------
// Programmer : Sharon Pais
// Student No : 31864169-3
// Date : 1.1.2017
//-----------------------------------------------------------------------------


#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD_LENGTH   15
#define MAX_WORD_FILE    200
#define WORDS_NUM         30
#define MATRIX_SIZE       15
#define WALL_SIZE		   1
#define GAME_MATRIX_SIZE MATRIX_SIZE + WALL_SIZE * 2

typedef char WORD_TYPE[MAX_WORD_LENGTH + 1];
typedef int  BOOL;

// the word struct, it contain the word(word type), 
// the value of the word and boolean variable to know if it used
struct Word_Details_Struct
{
	WORD_TYPE word;
	int	      value;
	BOOL	  was_placed;
};
typedef struct Word_Details_Struct Word_Details;






int InitWordsFromFile(Word_Details ioarrwWords[],
					  const char*  FILE_NAME);
//-----------------------------------------------------------------------------
// 					InitWordsFromFile
// 					-----------------
//
// General : This function initiates the words vector from a file.
//
// Parameters :
//			ioarrwWords - the words vector (IO)
//			FILE_NAME   - the name of the file (CONST)
//
// Return Value :  the number of words read
//
//-----------------------------------------------------------------------------

void InitMatrix(char ioarrcMatrix[][GAME_MATRIX_SIZE]);
//-----------------------------------------------------------------------------
// 						InitMatrix
// 					-----------------
//
// General : This procedure initialize the matrix with an empty sign value
//
// Parameters :
//			ioarrcMatrix - the board (IO)
//
// Return Value : none.
//
//-----------------------------------------------------------------------------



void FirstWord(char			ioarrcMatrix[][GAME_MATRIX_SIZE],
			   Word_Details ioarrwWords[],
			   int*			ionTotalPoints);
//-----------------------------------------------------------------------------
// 						FirstWord
// 					-----------------
//
// General : this function put the most valued word in 
//			 the center of the board to start the game
//
// Parameters :
//			ioarrcMatrix   - the board (IO)
//			ioarrwWords	   - the words vector (IO)
//			ionTotalPoints - the total points of the game (POINTER)
//
// Return Value : none.
//
//-----------------------------------------------------------------------------




BOOL PlaceAll(
	char		 ioarrcMatrix[][GAME_MATRIX_SIZE],
	Word_Details ioarrwWords[],
	int			 inWordsNum);
//-----------------------------------------------------------------------------
// 						PlaceAll
// 					-----------------
//
// General : This function operates the game until it is over .
//
//
// Parameters :
//			ioarrcMatrix - the board (IO)
//			ioarrwWords  - the words vector (IO)
//			inWordsNum   - the number of words in 'ioarrwWords' (I)
//
// Return Value : the total points amount.
//
//-----------------------------------------------------------------------------


BOOL HandleTurn(
	char		 ioarrcMatrix[][GAME_MATRIX_SIZE],
	Word_Details ioarrwWords[],
	int			 inWordsNum,
	int*		 ionTotalPoints);
//-----------------------------------------------------------------------------
// 						HandleTurn
// 					-----------------
//
// General :  This is the main function. It tries to place a word in the matrix and returns  
//				whether a word  was placed.
//
// Parameters :
//			ioarrcMatrix   - the board (IO)
//			ioarrwWords    - the words vector (IO)
//			inWordsNum	   - the number of words in 'ioarrwWords' (I)
//			ionTotalPoints - the total points of the game (POINTER)
//
// Return Value : bool if it placed a word 1 else 0.
//
//-----------------------------------------------------------------------------
	

void WriteResultsToFile(char		ioarrcMatrix[][GAME_MATRIX_SIZE],
						int			ionTotalPoints,
						const char* FILE_NAME);	
//-----------------------------------------------------------------------------
// 						WriteResultsToFile
// 					-------------------------
//
// General : This procedure writes the result matrix into a file
//
// Parameters :
//			ioarrcMatrix   - the board (IO)
//			FILE_NAME	   - the name of the file (CONST)
//			ionTotalPoints - the total points of the game (POINTER)
//
// Return Value : none.
//
//-----------------------------------------------------------------------------

// the function try to place the word in the index you give to it
BOOL TryToPlaceSouth(
	char		  ioarrcMatrix[][GAME_MATRIX_SIZE],
	Word_Details* ioWord,
	int			  nRow,
	int			  nCol,
	int			  iIndexLetter);

//-----------------------------------------------------------------------------
// 						TryToPlaceSouth
// 					----------------------
//
// General : the function try to place the word in the index you give to it (South Vector)
//
// Parameters :
//			ioarrcMatrix   - the board (IO)
//			ioWord	       - the selected word (IO) (CALL BY REFERENCE)
//		    nRow		   - the row index of the board where the func try to place the board
//		    nCol		   - the col index of the board where the func try to place the board
//			iIndexLetter   - it is location index of the word, where it twits with the other word
//
// Return Value : 1 if it succedded else 0.
//
//-----------------------------------------------------------------------------

BOOL TryToPlaceEast(
	char		  ioarrcMatrix[][GAME_MATRIX_SIZE],
	Word_Details* ioWord,
	int			  nRow,
	int			  nCol,
	int			  iIndexLetter);
//-----------------------------------------------------------------------------
// 						TryToPlaceEast
// 					----------------------
//
// General : the function try to place the word in the index you give to it (East Vector)
//
// Parameters :
//			ioarrcMatrix   - the board (IO)
//			ioWord	       - the selected word (IO) (CALL BY REFERENCE)
//		    nRow		   - the row index of the board where the func try to place the board
//		    nCol		   - the col index of the board where the func try to place the board
//			iIndexLetter   - it is location index of the word, where it twits with the other word
//
// Return Value : 1 if it succedded else 0.
//
//-----------------------------------------------------------------------------

void PrintMatrix(char iarrcMatrix[][GAME_MATRIX_SIZE]);
//-----------------------------------------------------------------------------
// 						PrintMatrix
// 					----------------------
//
// General : the function try to place the word in the index you give to it (East Vector)
//
// Parameters :
//			ioarrcMatrix   - the board (I)
//
// Return Value : none.
//
//-----------------------------------------------------------------------------


int isPlacedAll(Word_Details ioarrcMatrix[], int iWordsNum);
//-----------------------------------------------------------------------------
// 						isPlacedAll
// 					-----------------
//
// General : the function check if all the words was placed
//
// Parameters :
//			ioarrcMatrix   - the board (IO)
//			ioarrwWords	   - the words vector (IO)
//			iWordsNum      - the number words in 'ioarrwWords' vector
//
// Return Value : 1 if all the words placed else 0.
//
//-----------------------------------------------------------------------------

// end program.
