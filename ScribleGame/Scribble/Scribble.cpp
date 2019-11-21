#include "Scribble.h"

int InitWordsFromFile(Word_Details ioarrwWords[],
					  const char*  FILE_NAME)
{
	FILE*  fp = fopen(FILE_NAME, "rt");
	int    nWordIndex = 0;
	int	   nLength;

	// check if the file opening succeeded
	if (!fp) 
	{ 
		printf("There is problem with loading the reading file..\n");
		getchar();
		exit(1);
	}

	while (!feof(fp))
	{
		// put every word(and the value) in different index in the words array
		fscanf(fp, "%s %d", ioarrwWords[nWordIndex].word, &(ioarrwWords[nWordIndex].value));
		nLength = strlen(ioarrwWords[nWordIndex].word);
		// place null terminator in the last index of the word to be a string
		ioarrwWords[nWordIndex].word[nLength] = '\0';
		// reset 'placed' value
		ioarrwWords[nWordIndex].was_placed = 0;
		// go for next word index in the array
		nWordIndex++; 
	}
	fclose(fp);
	// returning wordIndex to send the size of the array
	return nWordIndex ;
}

void WriteResultsToFile(char		ioarrcMatrix[][GAME_MATRIX_SIZE], 
						int			ionTotalPoints, 
						const char* FILE_NAME)
{
	FILE * fp = fopen(FILE_NAME, "wt");
	// check there is no error loading the file
	if (!fp)
	{
		printf("There is problem with loading the writing file..\n");
		getchar();
		exit(1);
	}
	// goes through all the matrix and print every char
	for (int nRow = 1; nRow <= MATRIX_SIZE; nRow++)
	{
		for (int nCol = 1; nCol <= MATRIX_SIZE; nCol++)
		{
			fprintf(fp, " %c ", ioarrcMatrix[nRow][nCol]);
		}
		fprintf(fp, "\n");
	}
	//printing results in the last line
	fprintf(fp, "Total Points : %d", ionTotalPoints);

	//closing the FILE pointer
	fclose(fp);
}

void InitMatrix(char ioarrcMatrix[][GAME_MATRIX_SIZE])
{
	// goes through all the indexes in the array and put '-'
	// apart from edges, where it put '*' for wall secure
	for (int nRow = 0; nRow < GAME_MATRIX_SIZE; nRow++)
	{
		for (int nCol = 0; nCol < GAME_MATRIX_SIZE; nCol++)
		{
			//wall secure, if it's the edge of the board
			if (nRow == 0 || nRow == GAME_MATRIX_SIZE - 1 || nCol == 0 || nCol == GAME_MATRIX_SIZE - 1)
			{
				ioarrcMatrix[nRow][nCol] = '*';
			}
			// if its not the edge, put empty value
			else
			{
				ioarrcMatrix[nRow][nCol] = '-';
			}
		}
	}
}

void FirstWord(char			ioarrcMatrix[][GAME_MATRIX_SIZE],
			   Word_Details ioarrwWords[],
			   int*			ionTotalPoints)
{
	WORD_TYPE word;
	int nMid_Word = (MATRIX_SIZE / 2) + 1;
	// the first word - the most valued word
	strcpy(word, ioarrwWords[0].word);
	// copying the word letters to the board from the middle
	for (int nIndexRow = 0; nIndexRow < strlen(word); nIndexRow++)
	{
		ioarrcMatrix[nMid_Word][nMid_Word + nIndexRow] = word[nIndexRow];
	}
	// add the word value to the total points
	*ionTotalPoints = ioarrwWords[0].value;
	// mark the word as used
	ioarrwWords[0].was_placed = 1;
	PrintMatrix(ioarrcMatrix);
}

BOOL PlaceAll(
	char		 ioarrcMatrix[][GAME_MATRIX_SIZE],
	Word_Details ioarrwWords[],
	int			 inWordsNum)
{
	BOOL bIsPlaced = 1;
	int nTotalPoints = 0; // returning value of all points of the words that the function placed
	while (!isPlacedAll(ioarrwWords, inWordsNum) && bIsPlaced)
	{
		bIsPlaced = HandleTurn(ioarrcMatrix, ioarrwWords, inWordsNum, &nTotalPoints);
	}
	return nTotalPoints; 
}

int isPlacedAll(Word_Details ioarrcMatrix[WORDS_NUM], int iWordsNum)
{
	BOOL bIsPlacedAll = 1;

	for (int nIndex = 0; nIndex < iWordsNum && bIsPlacedAll; nIndex++)
	{
		if (ioarrcMatrix[nIndex].was_placed == 0)
			bIsPlacedAll = 0;
	}
	
	return bIsPlacedAll;
}

BOOL HandleTurn(
	char		 ioarrcMatrix[][GAME_MATRIX_SIZE],
	Word_Details ioarrwWords[],
	int			 inWordsNum, 
	int*		 ionTotalPoints)
{
	BOOL bIsPlaced = 0; // check if the word is placed
	BOOL bIsTried = 0; // if a certain word have 2 letters that are the same, it check only the first one

	// this double for check every index in the matrix (not the walls) until it places a word
	for (int nRow = 1; nRow <= MATRIX_SIZE && !bIsPlaced; nRow++)
	{
		for (int nCol = 1; nCol <= MATRIX_SIZE && !bIsPlaced; nCol++)
		{
			// if it founds letter, it tries to check if there is word that contain this letter 
			if (ioarrcMatrix[nRow][nCol] >= 'A' &&
				ioarrcMatrix[nRow][nCol] <= 'Z')
			{
				// check from the most valued word to the lowest if the word fit in the board
				for (int nIndexWord = 0; nIndexWord < inWordsNum && !bIsPlaced; nIndexWord++)
				{
					bIsTried = 0;
					// check if the word was not used
					if (!ioarrwWords[nIndexWord].was_placed)
					{ 
						//search in every letter of the word
						for (int nIndexLetter = 0;
								 nIndexLetter < strlen(ioarrwWords[nIndexWord].word)&& !bIsPlaced && !bIsTried; nIndexLetter++)
						{
							// check if the word contain this letter
							if (ioarrcMatrix[nRow][nCol] == ioarrwWords[nIndexWord].word[nIndexLetter])
							{
								bIsTried = 1;
								// try to place the word in South vector and return the bool value to bIsPlaced if it placed
								bIsPlaced = TryToPlaceSouth(ioarrcMatrix, &ioarrwWords[nIndexWord], nRow, nCol, nIndexLetter);

								// if the selcted word can't be placed South vector, it try to place it to East
								if (!bIsPlaced)
								{
									bIsPlaced = TryToPlaceEast(ioarrcMatrix, &ioarrwWords[nIndexWord], nRow, nCol, nIndexLetter);
									// it try to place to east vector, if it placed bIsPlaced will be 1(True) and the loop would end
									// else the loop countiues
								}
								if (bIsPlaced)
								{
									*ionTotalPoints += ioarrwWords[nIndexWord].value;
									// if the word was placed, it add the value of the word to the the total points
									PrintMatrix(ioarrcMatrix);
								}
							}
						}
					}
				}
			}
		}
	}
	return bIsPlaced;
}


BOOL TryToPlaceSouth(
	char		  ioarrcMatrix[][GAME_MATRIX_SIZE],
	Word_Details* ioWord,
	int			  nRow,
	int			  nCol,
	int			  iIndexLetter)
{
	BOOL bCanBePlaced = 1;  // the returned value, if the word is placed 1 else 0
	int  nStartPointRow;    // The Col index that the word suppose to start
	int  nEndPointRow;      // the Col index that the word suppose to end
	int  nPosRow;

	nStartPointRow = nRow - iIndexLetter;
	nEndPointRow = nStartPointRow + strlen(ioWord->word) - 1;

	// check if the word can be placed in the board
	// without getting out of the limits
	if (nStartPointRow >= 1 && nEndPointRow <= MATRIX_SIZE)
	{
		for (int nIndexRow = 0; nIndexRow < strlen(ioWord->word) &&
			bCanBePlaced; nIndexRow++)
		{
			nPosRow = nStartPointRow + nIndexRow;

			// check if there is at least one space between every letter in the word where it supposed to be placed 
			// (doesn't count the wall)
			if (nPosRow == nStartPointRow || nPosRow == nEndPointRow)
				// this 'if' statement, check if there is a letter that up or down to the word(only in edges)
			{
				if (nPosRow == nStartPointRow)
				{
					if (ioarrcMatrix[nPosRow - 1][nCol] != '-' && ioarrcMatrix[nPosRow - 1][nCol] != '*')
					{
						bCanBePlaced = 0;
					}
				}
				if (nPosRow == nEndPointRow)
				{
					if (ioarrcMatrix[nPosRow + 1][nCol] != '-' && ioarrcMatrix[nPosRow + 1][nCol] != '*')
					{
						bCanBePlaced = 0;
					}
				}
			}

			if (bCanBePlaced && 
				((ioarrcMatrix[nPosRow][nCol] != '-'	) ||
				(ioarrcMatrix[nPosRow][nCol - 1] != '-' && ioarrcMatrix[nPosRow][nCol - 1] != '*') ||
				(ioarrcMatrix[nPosRow][nCol + 1] != '-' && ioarrcMatrix[nPosRow][nCol + 1] != '*')))
				// this 'if' statement check if there is letter in the sides of the word indexes
			{
				if (nIndexRow != iIndexLetter)
					// check if it's the twist index (integrades with the word)
				{
					bCanBePlaced = 0;
					// if its not the twist index and , it means that it can't be placed
				}
			}
		}

		if (bCanBePlaced)
			// if it can be placed, it places the word
		{
			for (int nIndexRow = 0; nIndexRow < strlen(ioWord->word); nIndexRow++)
			{
				nPosRow = nStartPointRow + nIndexRow;
				// put the letters of the selected word in the board
				ioarrcMatrix[nPosRow][nCol] = ioWord->word[nIndexRow];
			}
			ioWord->was_placed = 1; // mark the word as 'placed'
		}

	}
	else
	{
		bCanBePlaced = 0;
	}
	return bCanBePlaced;
}


BOOL TryToPlaceEast(
	char		  ioarrcMatrix[][GAME_MATRIX_SIZE],
	Word_Details* ioWord,
	int			  nRow,
	int			  nCol,
	int			  iIndexLetter)
{
	BOOL bCanBePlaced = 1;  // the returned value, if the word is placed 
	int  nStartPointCol;    // The Row index that the word suppose to start
	int  nEndPointCol;      // the Row index that the word suppose to end
	int  nPosCol;

	nStartPointCol = nCol - iIndexLetter;
	nEndPointCol = nStartPointCol + strlen(ioWord->word) - 1;

	// check if the word can be placed in the board 
	// without getting out of the limits
	if (nStartPointCol >= 1 && nEndPointCol <= MATRIX_SIZE)
	{
		for (int nIndexCol = 0; nIndexCol < strlen(ioWord->word) &&
			bCanBePlaced; nIndexCol++)
		{
			nPosCol = nStartPointCol + nIndexCol;

			// check if there is at least one space between every letter in the word where it supposed to be placed 
			// and it doesn't override other words
			// (doesn't count the wall)
			if (nPosCol == nStartPointCol || nPosCol == nEndPointCol)
				// this 'if' statement, check if there is a letter that right or left to the word(only in edges)
			{
				if (nPosCol == nStartPointCol)
				{
					if (ioarrcMatrix[nRow][nPosCol - 1] != '-' && ioarrcMatrix[nPosCol - 1][nCol] != '*')
					{
						bCanBePlaced = 0;
					}
				}
				if (nPosCol == nEndPointCol)
				{
					if (ioarrcMatrix[nRow][nPosCol + 1] != '-' && ioarrcMatrix[nPosCol + 1][nCol] != '*')
					{
						bCanBePlaced = 0;
					}
				}
			}

			if (bCanBePlaced &&
				((ioarrcMatrix[nRow][nPosCol]    != '-') ||
				(ioarrcMatrix[nRow - 1][nPosCol] != '-' && ioarrcMatrix[nRow - 1][nPosCol] != '*') ||
				(ioarrcMatrix[nRow + 1][nPosCol] != '-' && ioarrcMatrix[nRow + 1][nPosCol] != '*')))
				//// this 'if' statement, check if there is a letter that up or down to the word
			{
			    // check if it's the twist index (integrades with the word)
				if (nIndexCol != iIndexLetter)
				{
					bCanBePlaced = 0; // if its not twist index and there is letter 
									  // near that index it can't be placed
				}
			}
		}

		if (bCanBePlaced)
			// if it can be placed, it places the word
		{
			for (int nIndexCol = 0; nIndexCol < strlen(ioWord->word); nIndexCol++)
			{
				nPosCol = nStartPointCol + nIndexCol;
				// put the letters of the selected word in the board
				ioarrcMatrix[nRow][nPosCol] = ioWord->word[nIndexCol];
			}
			ioWord->was_placed = 1; // mark the word as 'placed'
		}

	}
	else
	{
		//if the word would be longer than the limits, it can't be placed
		bCanBePlaced = 0;
	}
	return bCanBePlaced;
}

void PrintMatrix(char iarrcMatrix[][GAME_MATRIX_SIZE])
{
	// for loop that pass all the matrix and print it
	system("cls");
	for (int nRow = 0; nRow <= GAME_MATRIX_SIZE - 1; nRow++)
	{
		for (int nCol = 0; nCol <= GAME_MATRIX_SIZE - 1; nCol++)
		{
			printf(" %c ", iarrcMatrix[nRow][nCol]);
		}
		printf("\n");
	}
	getchar();
	getchar();
}




