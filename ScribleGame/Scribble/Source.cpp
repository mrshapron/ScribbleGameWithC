#include "Scribble.h"

int main()
{
	Word_Details arrwWordsVec[WORDS_NUM];							 // array of all the words
	char		 arrcGameMatrix[GAME_MATRIX_SIZE][GAME_MATRIX_SIZE]; // matrix of the board
	int			 nTotalPoints;										 // total points
	int			 nNumOfWords;								         // number of words

	char		 arrcReadFileName[MAX_WORD_FILE];				     // the name of the reading file
	char		 arrcWriteFileName[MAX_WORD_FILE];					 // the name of the writing file

	printf("\nPlease Enter Reading File Name\n");
	scanf("%s", arrcReadFileName);

	printf("\nPlease Enter Writing File Name\n");
	scanf("%s", arrcWriteFileName);

	// the function init the arrcGameMatrix from the file and return 
	// to nNumofWords the number of the words
	nNumOfWords = InitWordsFromFile(arrwWordsVec, arrcReadFileName);

	// the function reset the board and build a security wall
	InitMatrix(arrcGameMatrix);

	// the function put the first word in the board to start the game
	FirstWord(arrcGameMatrix, arrwWordsVec, &nTotalPoints);
	
	// the function place all the words that can be placed and return
	// to nTotalPoints the points it collected
	nTotalPoints += PlaceAll(arrcGameMatrix, arrwWordsVec, nNumOfWords);


	// the function writes the results in the writing file
	WriteResultsToFile(arrcGameMatrix, nTotalPoints, arrcWriteFileName);


	getchar();
	getchar();
	return 0;
}