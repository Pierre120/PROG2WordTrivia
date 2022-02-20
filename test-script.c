#include "trivia.h"
#include <stdlib.h>

#define HD 196 // horizontal divider 
#define VD 179 // vertical divider 
#define C 197   //cross
#define V 186 // vertical lines for gameboard
#define H 205 // horizontal lines for gameboard
#define UR 187 // upper-right corner for gameboard
#define LR 188 // lower-right corner for gameboard
#define UL 201 // upper-left corner for gameboard
#define LL 200 // lower-left corner for gameboard

#define FALSE 0
#define TRUE 1

/* 
NOTES: 
> "~" for string input cancelation
> middle character for my screen is the 74th character
*/

/* You can add other data structures here.
   
   You are expected to implement other functions not listed
   in the trivia.h here. Do not forget to include internal 
   documentation. For functions with long (many lines) of code,
   make sure to also include inline comments to discuss your
   algorithm.
*/

//====================================================== UNIVERSAL FUNCTIONS
// CAN BE REUSED/CALLED FOR MOST PART
// ESSENTIALS FOR THE OTHER FUNCTIONS TO WORK

// prints a single character in a single line repeating format
void displayFloatingIcons(int nMax, char cIcon, int nSpacing)
{
	int x;
 	
 	for(x = 1; x <= nMax && nSpacing > 0; x++)
 	{
    // prints on and every nSpacing from the last printed character
 		if(x % nSpacing == 0)
 			printf("%c", cIcon);
 			
 		else 
 			printf(" ");
	}
} // P


// prints the underline of headers
void displayHeaderUnderline()
{
	// 74th char middle
	printf("%57c", UR);
	displayFloatingIcons(33, H, 1);
	printf("%c\n", UL);		//total: 35 characters	
} // P


// scan divider when getting inputs from user
void displayScanDivider()
{
	// 74th char middle
	printf("%57c", LR);
	displayFloatingIcons(33, H, 1);	//33 characters
	printf("%c\n", LL);
	printf("%88s", "Enter letter/number of action: ");
} // P


// prints the lower border of the choice box
void displayUpperUniversalChoiceBox()
{
	printf("%60c", UR);
	displayFloatingIcons(27, H, 1); //27 characters
	printf("%c\n", UL);	//total: 29 characters
} // P


// prints the lower border of the choice box
void displayLowerUniversalChoiceBox()
{
	printf("%60c", LR);
	displayFloatingIcons(27, H, 1); //27 characters
	printf("%c\n", LL);	//total: 29 characters
} // P


// scans a single/1st character from user and avoids buffer overflow
void getSingleChar(char *cChar1)
{
	char cScan;
	int bFirstChar = TRUE;
	*cChar1 = '\0';
	
	// continuously scans from the input buffer until newline char is reached
	while(cScan != '\n') 
	{
		scanf("%c", &cScan);
		
		// only first non-newline character will be stored
		if(bFirstChar && cScan != '\n')
		{
			*cChar1 = cScan;
			bFirstChar = FALSE;
		}
	}
} // P


/* Gets the string without buffer overflow. Returns 0 if user input is
	an empty string or greater than the expected string length. Returns 1
	if user input is within the expected string length or is a cancel 
	character that cancels string input.
*/
int getString(char *sString, int nLength, char *cCancelChar)
{
	char sScan;
	int nCounter = 0;
	
	// initializes all characters of sString to null characters
	// if it contains non-null characters
	if(strlen(sString))
		memset(sString, '\0', nLength + 1); // sets to empty string
	
	while(sScan != '\n')
	{
		scanf("%c", &sScan);
		
		// stores the characters until max length is reached
		// or newline character is reached
		if(nCounter < nLength && sScan != '\n')
		{
			sString[nCounter] = sScan;
			nCounter++;
		}	
	}
	
	// for invalid input
	if(strlen(sString) == 0) // user entered an enter key only
		return 0; // returns indication that string input is invalid
	
	// user cancels string input
	if(!strcmp(sString, cCancelChar)) // (strcmp(sString, cCancelChar) == 0)
	{
		sString[0] = '\0';
		return -1; // returns indication that string input is canceled
	}
	
	return 1; // returns indication that string input is valid
} // P


// generates a set of random that is less than the max limit
int generateRandomNum(int nMaxLim)
{
   // srand(time(0));
   int nRandNum;
	
	if(nMaxLim <= 0)
		return -1;
	
   do
   {
      nRandNum = rand() % nMaxLim;
   } while(nRandNum < 0 || nRandNum >= nMaxLim);
   //makes sure generated random number is >= 0 and less than nMaxLim

   return nRandNum;
} // P


void delay(int nSeconds)
{
	time_t Start = time(0); // gets the starting time
	
	// do nothing until difference of the start and current is equal to nSeconds
	while(time(0) - Start <= nSeconds && nSeconds >= 0);
} // P


void getWordEntry(strWord sChosenWord, int *bValid, char *sEntryFor)
{
	do
	{
		if(strcmp(sEntryFor, "Play") == 0)
			printf("%32s", "Answer: ");
		
		if(strcmp(sEntryFor, "Add") == 0) // for Add Word and Add Trivia
			printf("%74s", "Enter word: ");
		
		if(strcmp(sEntryFor, "ModifyWord") == 0) // for Modify Word
			printf("%74s", "Word to modify: ");
		
		if(strcmp(sEntryFor, "DeleteWord") == 0) // for Delete Word
			printf("%74s", "Word/Entry to delete: ");
		
		if(strcmp(sEntryFor, "DeleteClue") == 0) // for Delete Clue
			printf("%74s", "Delete a clue of: ");
		
		if(strcmp(sEntryFor, "ViewClue") == 0) // for View Clue
			printf("%74s", "View clues of: ");
		
		if(strcmp(sEntryFor, "File") == 0) // for Import and Export
			printf("%74s", "Enter file name: ");
		
		*bValid = getString(sChosenWord, LEN1, "~");  // 0 - invalid; 1 - valid; -1 - cancel
		
		// notifies user that string input is invalid
		if(*bValid == 0)
		{
			printf("%80s\n%91s\n\n", "INVALID INPUT", "INPUT MUST BE AT LEAST 1 CHARACTER");
			delay(3); // pauses program for 3 seconds
		}
	}while(*bValid == 0); // continuously asks for string input until string is either valid or cancelation
} // P

//==========================================================================================


//====================================================== INIT BOARD
// HANDLES THE INITILIZATION OF THE WORDS AND LETTERS TO BE USED FOR THE PLAY PHASE


// checks if produced random number is unique in the aRandInd
int isUniqueLetter(char gameboardRow[], int nCols, int nRandNum, wordList aEntries)
{
   int col, bUnique = TRUE; // assumes that Entry is unique for the gameboard
	
	// only when nCols is the starting index [0]
	if(nCols == 0 && aEntries[nRandNum].use > 0) 
		bUnique = FALSE;
	
   for(col = 0; col < nCols && bUnique; col++)
      if(aEntries[nRandNum].use > 0 || aEntries[nRandNum].answer[0] == gameboardRow[col])
         bUnique = FALSE; // indicates that current word is already in use
							 // or first letter already exists in the current row of the gameboard
   
   return bUnique;
} // P


int checkLettersLeft(wordList aEntries, int nElem)
{
	int nLettersLeft = 0, w, bFound;
	char cLetter;
	
	for(cLetter = 'A'; cLetter <= 'z'; cLetter++)
	{
		bFound = FALSE;
		for(w = 0; w < nElem && !bFound; w++)
			if(aEntries[w].answer[0] == cLetter && aEntries[w].use == 0)
			{
				nLettersLeft++; // counts the number of alphabetical letters that are present in the current entry list 
				bFound = TRUE; 
			}
			
		if(cLetter == 'Z') // adds an additional ASCII value to start checking from 'a' 
			cLetter += 6;   // after all the capital letters have been checked
	}
	
	return nLettersLeft;
} // P

//==========================================================================================

//====================================================== PLAY
// DISPLAYS AND FUNCTIONS THAT WILL BE USED IN THE PLAY PHASE


// prints the outer upper and lower border of the gameboard
void printOuterGamboard(int nCol, char cLeft, char cRight)
{
   printf("%25c", cLeft); // left corner of border
   
	// printed line border length will vary on the number of nCol
   displayFloatingIcons(nCol * 3 + nCol - 1, H, 1);
   
   printf("%c\n", cRight); // rigth corner of border
}

// prints the horizontal divider after/before every gameboard content
void printGameboardDivi(int nCol)
{
   int j;
   
   printf("%25c", V);
   
   for(j = 1; j <= nCol; j++)
   {
      printf("%c%c%c", HD, HD, HD);
      
      if(j != nCol) // j < nCol
         printf("%c", C);
   }
   
   printf("%c\n", V);
}

// prints the content with vertical divider per row
void printGameboardContent(char aGameLetters[], int nCol)
{
   int y;
   
   printf("%c", V);
   
   for(y = 0; y < nCol; y++)
   {
      printf(" %c ", aGameLetters[y]); // gameboard contents/letters
      
      if(y < nCol - 1)
         printf("%c", VD); // vertical dividers for contents/letters
   }
   
   printf("%c\n", V);
}

// prints the whole gameboard
void printGameboard(arrMatrix GameLetters, int nRow, int nCol, int nCurrentRow)
{
   int row, nSpacing;
	
   printf("\n\n");
   
   printOuterGamboard(nCol, UL, UR); // upper border
   
   for(row = 0; row < nRow; row++)
   {
		nSpacing = 25;
		
		if(row == nCurrentRow) // indicates the current row of player
			nSpacing--;
		
		displayFloatingIcons(24, '>', nSpacing); // prints row indicator for player's current row
		
      printGameboardContent(GameLetters[row], nCol); // contents
      
      if(row < nRow - 1)
         printGameboardDivi(nCol); // divider
   }
   
   printOuterGamboard(nCol, LL, LR); // lower border
   
   printf("\n");
}

void checkGameboard(char gameboardRow[], wordList aEntries, int nElem, int *nIndex, 
int *nCurrCol, char cChoice, int nCurrentRow, int nCol)
{
	int w, bFound = FALSE;
	*nIndex = -1; // assumes that word is not in the entry
	*nCurrCol = -1; // assumes that letter is not in the current row of the gameboard
	
	for(w = 0; w < nCol && !bFound; w++)
		if(cChoice == gameboardRow[w]) // entered cChoice is in the gameboard
		{
			bFound = TRUE;
			*nCurrCol = w;
		}
	
	for(w = 0; w < nElem && bFound; w++) // when cChoice position in the gameboard is found
		if(aEntries[w].use == nCurrentRow && aEntries[w].answer[0] == cChoice)
			*nIndex = w; // gives the index of the entry that has the same first letter
							 // and same row usage number or row where it is used with the current row number
} // P

//==========================================================================================


//====================================================== MAINTENANCE PHASE MENU

void displayMaintenanceContent(int nNum, char * String)
{
	printf("%60c%6d.%14s%7c\n", V, nNum, String, V);	//29 characters
}


void displayMaintenanceChoices()
{
	int y;
	// choices for maintenance ohase actions
	char aChoices[9][13] = {"ADD WORD", "ADD TRIVIA",
									"MODIFY ENTRY", 
									"DELETE WORD", "DELETE CLUE", 
									"VIEW WORDS", "VIEW CLUES",
									"EXPORT", "IMPORT"};
	
	// prints all the actions/choices for maintenance phase
	for(y = 0; y < 9; y++)
	{
		displayUpperUniversalChoiceBox();
		displayMaintenanceContent(y + 1, aChoices[y]);
		displayLowerUniversalChoiceBox();
	}
}

void initializeEntries(wordList aEntries)
{
	int y, z;
	
	for(y = 0; y < CAP; y++)
	{
		memset(aEntries[y].answer, '\0', 21); // sets to empty string
		aEntries[y].numClues = 0; // sets number of clues to 0
		aEntries[y].use = 0; // sets entry usage to 0
		
		// sets the trivias(relation and relations value) to empty strings
		for(z = 0; z < 10; z++)
		{
			memset(aEntries[y].clueList[z].relation, '\0', 31);
			memset(aEntries[y].clueList[z].relValue, '\0', 31);
		}
	}
} // P

//==========================================================================================

//====================================================== LIST WORDS
// BORDERS FOR THE VIEWING FRAME OF LIST WORDS

void printViewBorders(char cHeaderOrFooter) // 'H' - Header ; 'F' - Footer
{
	if(cHeaderOrFooter == 'H') // display for header
	{
		printf("%45c", UR);
		displayFloatingIcons(24, H, 1);
		printf(" WORDS ");
		displayFloatingIcons(24, H, 1);
		printf("%c\n", UL);
	} // total: 57 char
	
	if(cHeaderOrFooter == 'F') // display for footer
	{
		printf("%45c", LR);
		displayFloatingIcons(55, H, 1);
		printf("%c\n", LL);
	} // total: 57 char
}

//==========================================================================================

//====================================================== ADD WORD 
// HEADER DISPLAY FOR THE ADD WORD

void displayAddWordHeader()
{
	printf("\t[~] EXIT/CANCEL"); // cancel character for string input in Add Word
	printf("\n\n%78s\n", "ADD WORD");
   displayHeaderUnderline();
   printf("\n\n\n\n");
}

//==========================================================================================

//====================================================== ADD TRIVIA 
// DISPLAYS AND FUNCTIONS FOR ADD TRIVIA

void displayAddTriviaHeader()
{
	printf("\t[~] EXIT/CANCEL"); // cancel character for string input in Add Trivia
	printf("\n\n%79s\n", "ADD TRIVIA");
   displayHeaderUnderline();
   printf("\n\n\n");
}


void addTriviaMenu(wordList aEntries, int nElem)
{
	int nIndex = -2, bValid = 0;
	strWord strWordToAddTrivia;
	
	system("cls");
	displayAddTriviaHeader();
	
	getWordEntry(strWordToAddTrivia, &bValid, "Add"); // get string input for Add trivia
	
	// checks if word entered exists
	if(bValid == 1)
		nIndex = searchWord(strWordToAddTrivia, aEntries, nElem);
	
	// calls addTrivia() if entered word exists
	if(nIndex > -1)
	{
		printf("\n\n");
		addTrivia(&aEntries[nIndex]);
	}
	
	// word entered doesn't exist
	if(nIndex == -1)
	{
		printf("%81s\n\n", "WORD NOT FOUND"); // notifies user that entered word was not found in the current entry
		delay(3); // pauses program for 3 seconds
	}
} // P

//==========================================================================================

//====================================================== MODIFY ENTRY

// -------------------------------------------------------- WORDS
// FUNCTIONS AND DISPLAYS FOR MODFIYING THE WORD OF AN ENTRY

void displayModifyWordHeader()
{
	printf("\t[~] EXIT/CANCEL"); // cancel character for string input in Modify Word
	printf("\n\n%79s\n", "MODIFY WORD");
   displayHeaderUnderline();
   printf("\n\n");
}

// modifies the word of chosen oneEntry
void modifyWord(struct triviaTag * oneEntry, wordList aEntries, int nElem)
{
	strWord sModifiedWord;
	int bValid = 0, bUnique;
	
	// continues to ask user for a modified word until 
	// user decides to cancel
	while(bValid != -1 )
	{	
		bUnique = 0;
		
		system("cls");
		displayModifyWordHeader();
		printf("%74s%s\n\n", "Word: ", oneEntry->answer); // current word being modified
		
		printf("%74s", "Modify word to: ");
		bValid = getString(sModifiedWord, LEN1, "~"); // 0 - invalid; 1 - valid; -1 - cancel
		
		if(bValid == 1) // verifies if new modified word is unique in the entry list
			bUnique = searchWord(sModifiedWord, aEntries, nElem);
		
		if(bUnique == -1) // new modified word is unique
			strcpy(oneEntry->answer, sModifiedWord);
		
		if(bValid == 0) // user enter an invalid input
		{
			printf("%80s\n%91s\n\n", "INVALID INPUT", "INPUT MUST BE AT LEAST 1 CHARACTER");
			delay(3); // pauses program for 3 seconds
		}
		
		if(bUnique != -1 && bValid != -1) // modified word already exist in the entry list
		{
			printf("%83s\n\n", "WORD ALREADY EXISTS");
			delay(3); // pauses program for 3 seconds
		}
	}
} // P


// -------------------------------------------------------- CLUES
// DISPLAYS AND FUNCTIONS FOR MODIFYIING THE CLUES OF AN ENTRY/WORD

void displayModifyCluesHeader()
{
	printf("\n\n%80s\n", "MODIFY CLUES");
   displayHeaderUnderline();
   printf("\n\n");
}


void displayCluesChoices(struct triviaTag oneEntry, int nNumClues, int toModify)
{
	int q;
	
	if(toModify) 
		printf("%88s", "Which do you want to modify?");
	
	printf("\n\n");
	
	for(q = 0; q < nNumClues; q++) // prints the all the clues that the oneEntry currently have
		printf("%45c%2d]\t%s: %s\n", '[', q + 1, oneEntry.clueList[q].relation, oneEntry.clueList[q].relValue);
	
	printf("%48s\t%s\n\n", "[ 0]", "Exit/Cancel"); // cancel character for choice input
	displayScanDivider();
} //P


void displayModifyTriviaChoices()
{
	printf("%88s\n\n", "Which do you want to modify?");
	
	// choices for clue modification
	printf("%65s\tRelation\n", "[1]");
	printf("%65s\tRelation Value\n", "[2]");
	printf("%65s\tExit/Cancel\n\n", "[0]");
	displayScanDivider();
}


void getClueModification(struct clueTag * oneTrivia, phrase sClueToModify, char * sTypeOfClue)
{
	phrase sModifiedPhrase;
	int bValid = 0;
	
	while(bValid != -1)
	{
		system("cls");
		printf("\t[~] Cancel"); // cancel character for string input
		displayModifyCluesHeader();
		printf("%76s\n%72s: %s\n\n", "CLUE:", oneTrivia->relation, oneTrivia->relValue);
		
		printf("%72s: ", sTypeOfClue);
		bValid = getString(sModifiedPhrase, LEN2, "~");  // 0 - invalid; 1 - valid; -1 - cancel
		
		if(bValid == 1) // user entered a valid string input
			strcpy(sClueToModify, sModifiedPhrase); // copies new modified clue to the sClueToModify
		
		if(bValid == 0) // notifies user that input is invalid
		{
			printf("%80s\n%91s\n\n", "INVALID INPUT", "INPUT MUST BE AT LEAST 1 CHARACTER");
			delay(3); // pauses program for 3 seconds
		}
	}
} // P

void modifyClue(struct clueTag * oneTrivia)
{
	char cChoice;
	
	do
	{
		system("cls");
		displayModifyCluesHeader();
		printf("%76s\n%72s: %s\n\n", "CLUE:", oneTrivia->relation, oneTrivia->relValue); // display clue under modification
		displayModifyTriviaChoices();
		getSingleChar(&cChoice); // get modification choice from user
		
		switch(cChoice)
		{
			case '1': // user picks to modify relation member field of the current clue under modification
				getClueModification(oneTrivia, oneTrivia->relation, "Relation");
				break;
				
			case '2': // user picks to modify relation value member field of the current clue under modification
				getClueModification(oneTrivia, oneTrivia->relValue, "Relation Value");
				break;
				
			default: // user either picks to exit/cancel or entered an invalid input
				if(cChoice != '0')
				{	// notifies user if input is not within the choices
					printf("%80s\n%91s\n\n", "INVALID INPUT", "INPUT MUST BE AT LEAST 1 CHARACTER");
					delay(3); // pasues program for 3 seconds
				}
				break;
		}
	}while(cChoice != '0'); // continuously modify clue until user decides to stop
} // P
// --------------------------------------------------------- MAIN MODIFY MENU
// DISPLAYS AND FUNCTIONS FOR THE MAIN ENTRY MODIFICATION MENU 

void displayModifyEntryHeader()
{
	printf("\n\n%80s\n", "MODIFY ENTRY");
   displayHeaderUnderline();
   printf("\n\n");
}

void displayModificationChoices()
{
	printf("%88s\n\n", "Which do you want to modify?");
	
	// main choices of modification per entry
	displayUpperUniversalChoiceBox();
	printf("%60c%16s%12c\n", V, "W. WORD", V);
	displayLowerUniversalChoiceBox();
	
	displayUpperUniversalChoiceBox();
	printf("%60c%17s%11c\n", V, "C. CLUES", V);
	displayLowerUniversalChoiceBox();
	
	displayUpperUniversalChoiceBox();
	printf("%60c%16s%11c\n", V, "X. EXIT", V); // cancel character for choice input
	displayLowerUniversalChoiceBox();
	
	printf("\n\n");
	displayScanDivider();
}

void executeModification(wordList aEntries, int nElem)
{
	int nIndex = -2, bValid;
	strWord sChosenWord;
	char cChoice = '~';
	
	system("cls");
	printf("\t[~] EXIT/CANCEL"); // cancel character for string input
	displayModifyEntryHeader();
	listWords(aEntries, nElem); // list of words in the current entry list in alphabetical order
	
	getWordEntry(sChosenWord, &bValid, "ModifyWord"); // gets string input
	
	if(bValid == 1)
		nIndex = searchWord(sChosenWord, aEntries, nElem); // check if entered word exists
	
	// modification will not stop until player chooses to stop/cancel/exit
	while(bValid == 1 && nIndex > -1 && cChoice != 'X')
	{
		system("CLS");
		displayModifyEntryHeader();
		printf("%70s%s\n", "ENTRY: ", aEntries[nIndex].answer);
		displayModificationChoices();
		getSingleChar(&cChoice);
		
		switch(cChoice)
		{
			case 'W': // modify word
				modifyWord(&aEntries[nIndex], aEntries, nElem);
				break; 
				
			case 'C': // modify word
				modifyEntry(&aEntries[nIndex]);
				break;
				
			default: // cancel character or invalid input
				if(cChoice != 'X')
				{
					printf("%80s", "INVALID INPUT"); // invalid input note to user
					delay(3); // pauses program for 3 seconds
				}
				break;
		}
	}
	
	if(nIndex == -1) // notifies user that entered word is not in the current entry list
	{
		printf("%81s\n\n", "WORD NOT FOUND");
		delay(3);
	}
} // P

//==========================================================================================

//====================================================== DELETE WORD
// DISPLAYS AND FUNCTIONS FOR DELETE WORD

void displayDeleteWordHeader()
{
	printf("\t[~] EXIT/CANCEL"); // cancel character for string input
	printf("\n\n%79s\n", "DELETE WORD");
   displayHeaderUnderline();
   printf("\n\n");
}

void clearLastEntry(struct triviaTag *oneEntry)
{
	int c;
	
	memset(oneEntry->answer, '\0', 21); // sets last entries answer member field to empty string
	
	// sets all the clues/trivia to empty strings
	for(c = 0; c < oneEntry->numClues; c++)
	{
		memset(oneEntry->clueList[c].relation, '\0', 31);
		memset(oneEntry->clueList[c].relValue, '\0', 31);
	}
	oneEntry->numClues = 0; // signifies that there are no clues left in the oneEntry
} // P 

//==========================================================================================

//====================================================== DELETE CLUE
// DISPLAYS AND FUNCTIONS FOR DELETE CLUE

void displayDeleteClueHeader()
{
	printf("\n\n%79s\n", "DELETE CLUE");
   displayHeaderUnderline();
   printf("\n\n");
}

void executeClueDeletion(arrClues aCluesList, int *nNumClues, int nDeleteIndex)
{
	int y;
	
	// deletes the clue and moves the placement of the clues
	for(y = nDeleteIndex; y + 1 < *nNumClues; y++)
		aCluesList[y] = aCluesList[y + 1];
	
	// clears the last clue
	memset(aCluesList[y].relation, '\0', LEN2 + 1);
	memset(aCluesList[y].relValue, '\0', LEN2 + 1);
	
	*nNumClues -= 1; // updates that 1 clue has been deleted
} // P

void deleteClue(wordList aEntries, int nElem)
{
	int nChoice = 1, nIndex = -2, bValid;
	strWord sWordForClue;
	char cDmp;
	
	system("CLS");
	printf("\t[~] EXIT/CANCEL");
	displayDeleteClueHeader();
	listWords(aEntries, nElem); // list of words in the current entry list in aplhabetical order
	
	getWordEntry(sWordForClue, &bValid, "DeleteClue"); // gets string for which word would be deleted a clue with
	
	if(bValid == 1)
		nIndex = searchWord(sWordForClue, aEntries, nElem); // checks if entered word exists in the current entry list
	
	// continuously asks user which clues to delete until min capacity is reached or user decides to stop
	// this will be skipped if chosen entry is already at min clue capacity or entered word doesn't exist
	while(bValid == 1 && nIndex > -1 && nChoice != 0 && aEntries[nIndex].numClues > 1)
	{
		system("CLS");
		displayDeleteClueHeader();
		displayCluesChoices(aEntries[nIndex], aEntries[nIndex].numClues, FALSE);
		scanf(" %d%c", &nChoice, &cDmp);
		
		if(nChoice > 0 && nChoice <= aEntries[nIndex].numClues)
			executeClueDeletion(aEntries[nIndex].clueList, &aEntries[nIndex].numClues, nChoice - 1);
	}
	
	if(aEntries[nIndex].numClues == 1 && bValid != -1)
	{	// notifies user that chosen entry has reached its min capacity of trivias/clues (1)
		printf("%89s\n%86s\n\n", "NUMBER OF CLUES AT MIN CAPACITY", "CAN'T DELETE ANOTHER CLUE");
		delay(3); // pauses program for 3 seconds
	}
	
	if(nIndex == -1) // notifies user that the entered word is not in the current entry list
	{
		printf("%81s\n\n", "WORD NOT FOUND");
		delay(3);
	}
} // P

//==========================================================================================

//====================================================== VIEW WORDS
// HEADER DISPLAY FOR VIEW WORDS

void displayViewWordsHeader()
{
	printf("\n\n%79s\n", "VIEW WORDS");
   displayHeaderUnderline();
   printf("\n\n");
}

//==========================================================================================

//====================================================== VIEW CLUES
// DISPLAYS AND FUNCTIONS FOR VIEW CLUES

void displayViewCluesHeader()
{
	printf("\n\n%79s\n", "VIEW CLUES");
   displayHeaderUnderline();
   printf("\n\n");
}

void viewClues(wordList aEntries, int nElem)
{
	int bValid, nIndex = -2;
	strWord strViewClues;
	char cChoice = '\0';
	
	system("CLS");
	printf("\t[~] EXIT/CANCEL"); // cancel character for string input
	displayViewCluesHeader();
	listWords(aEntries, nElem); // list of words in the current entry list in alphabetical order
	
	getWordEntry(strViewClues, &bValid, "ViewClue"); // get string input
	
	if(bValid == 1 && nElem > 0) // checks if word is in the entry list
		nIndex = searchWord(strViewClues, aEntries, nElem);
	
	// displays the clues of the entered word if entered word exists
	while(nIndex > -1 && cChoice != 'X' && nElem > 0)
	{
		system("CLS");
		displayViewCluesHeader();
		printViewBorders('H');
		viewEntry(aEntries[nIndex]);
		printViewBorders('F');
		printf("%70s", "Exit [X]: "); // cancel/exit character for input
		getSingleChar(&cChoice);
	}
	
	if(nElem == 0 && bValid == 1) // entry list is empty
	{
		printf("%81s\n\n", "NO ENTRIES YET");
		delay(3); // pauses the program for 3 seconds
	}
	
	if(nIndex == -1) // entered word doesn't exist
	{
		printf("%81s\n\n", "WORD NOT FOUND");
		delay(3); // pauses the program for 3 seconds
	}
}

//==========================================================================================

//====================================================== SORT
// SWAPPING FUNCTION FOR SORTING ENTRIES

void swapEntries(struct triviaTag *sDestination, struct triviaTag *sSource)
{
	struct triviaTag sTemp; // temporary storage for struct triviaTag variables
	
	sTemp = *sDestination;
	*sDestination = *sSource;
	*sSource = sTemp;
}

//==========================================================================================

//====================================================== IMPORT & EXPORT
// HEADER FOR BOTH IMPORT AND EXPORT

void displayExportImportHeader(char * sExportorImport)
{
	printf("\t[~] EXIT/CANCEL"); // cancel character for string input in import or export
	printf("\n\n%77s\n", sExportorImport);
   displayHeaderUnderline();
   printf("\n\n");
}

//==========================================================================================

/* Implement the functions listed in trivia.h here. 
   Read through the comments to know what the function is
   supposed to accomplish.  Note that the function names
   may not exactly be performing the whole task indicated
   in the mp specs.  For example, some may not have the 
   getting of input be part of the function, instead it 
   could be somewhere else and was just passed to the 
   function as parameter.
   
   Provided below are examples of the comments before 
   each function.  
*/

/* Given the filename stored in fname, this function
   loads the entries of the text file and ADDS them to
   the array aEntries. The number of elements should be
   updated in pElem. For each trivia word in the file 
   that already exists in aEntries, the user is asked if 
   he wants to keep the one in the array or replace it 
   with the one from the text file.
 
   @param fname - the string filename, including the extension
   @param aEntries - the array of words with the corresponding clues
   @param pElem - the address where the number of elements are 
                  to be updated
*/
void
import(strWord fname, wordList aEntries, int * pElem)
{
	int bEnd, bFirst = TRUE, bExist, nInd;
	struct triviaTag sTempStorage;
	char cChoice;
	phrase strTemp2;
	FILE *fpImport;
	
	sTempStorage.use = 0;
	
	fpImport = fopen(fname, "r");
	
	// notifies user if file entered doesn't exist
	if(fpImport == NULL)
		printf("%83s\n\n", "FILE DOES NOT EXIST");
	
	// scanning input from file until EOF or maximum capacity of entries reached
	while(!feof(fpImport) && *pElem < CAP && fpImport != NULL)
	{
		cChoice = '~';
		sTempStorage.numClues = 0;
		sTempStorage.use = 0;
		
		// start input from file
		if(bFirst)
		{
			fscanf(fpImport, "%s", strTemp2);
			bFirst = FALSE;
		}
		
		// gets the answer memeber field
		fscanf(fpImport, " %[^\n]%*c", sTempStorage.answer);
		
		// checks if the entry from file already exists
		bExist = searchWord(sTempStorage.answer, aEntries, *pElem);
		
		// prompts user on wether to overwrite or not the existing entry with the one from file
		if(bExist != -1)
		{
			printf("\n%60s \"%s\" %s\n", "The entry", sTempStorage.answer, "already exists in your list.");
			printf("%91s", "Do you want to overwrite it? [Y/y/N/n]: ");
			getSingleChar(&cChoice);
		}
		
		bEnd = FALSE;
		nInd = *pElem;
		
		// heads to the index of the entry to be overwritten
		if(cChoice == 'Y' || cChoice == 'y')
			nInd = bExist;
		
		// gets all the clues of current entry in the file
		// until to the last clue or EOF reached
		while(!bEnd && !feof(fpImport) && cChoice != 'N' && cChoice != 'n')
		{
			fscanf(fpImport, "%[^:]%*c", strTemp2);
			
			// flags the end of current entry or EOF
			if(strTemp2[0] == '\n')
				bEnd = TRUE;
			
			//stores all the clues if it is not end of current entry or EOF
			if(!bEnd)
			{
				strcpy(sTempStorage.clueList[sTempStorage.numClues].relation, strTemp2);
				fscanf(fpImport, " %[^\n]%*c", sTempStorage.clueList[sTempStorage.numClues].relValue);
				sTempStorage.numClues++;
			}
		}
		
		// doesn't add or replace the entry if player chooses not to overwrite it
		if(cChoice != 'N' && cChoice != 'n')
			aEntries[nInd] = sTempStorage;
		
		// pElem increments if it is a unique entry
		if(bExist == -1)
			*pElem += 1;
	}
	
	fclose(fpImport);
} // P

/* This function initializes the gameboard with nRows by
   nCols number of characters based on the contents of 
   aEntries.  The use member field is also updated to
   signify that this entry is already used in the board
   for the current game.  The function returns 1 if the
   board can be fully initialized with unique entries based
   on the needed dimension. Otherwise, the function returns 0.

   @param gameboard - 2D array of characters containing the
                      first letters of the answer
   @param nRows - the number rows to be initialized in gameboard
   @param nCols - the number columns to be initialized in gameboard
   @param aEntries - the array of words with the corresponding clues
   @param nElem - the number of elements in aEntries
   @return 1 if successful initialization of the gameboard, 0 otherwise
*/
int 
initBoard(arrMatrix gameboard, int nRows, int nCols,
              wordList aEntries, int nElem)
{
	int row, col, nRandNum, bInit = TRUE, nNeeded = nRows * nCols, nLettersLeft;	
	
	// number of entries needed is greater than the number of entries the player or file has
   if(nNeeded > nElem)
      bInit = FALSE;
	
	// initializes all use member field to 0
	for(row = 0; row < nElem && bInit; row++)
		aEntries[row].use = 0;
   
	// initializes the content of the gameboard
   for(row = 0; row < nRows && bInit; row++)
      for(col = 0; col < nCols; col++)
      {
         do {
            nRandNum = generateRandomNum(nElem); // generate a random index
         } while(!isUniqueLetter(gameboard[row], col, nRandNum, aEntries));
				// checks if that index is already in use or its 1st letter is not unique for the current row
         
			// generated index of entry is unique
         gameboard[row][col] = aEntries[nRandNum].answer[0]; 
			aEntries[nRandNum].use = row + 1;
			nNeeded -= 1; // checks the number of entries needed
			nLettersLeft = checkLettersLeft(aEntries, nElem); 
				// counts the number of unique letters left in that are not yet used
			
			// unique letters left from the unused entries are less than the column count
			// or less than what the gameboard still needs
			if(nLettersLeft < nCols && nLettersLeft < nNeeded)
				bInit = FALSE; // initialization failed
      }
		
	if(!bInit) // not initialized
		return 0;
	
	return 1; // initialized
} // P

/* This function facilitates the game play of the user,
   including determining when the game ends. 
   @param gameboard - the 2D array of characters
   @param nRows - the number of rows in gameboard
   @param nCols - the number of columns in gameboard
   @param aEntries - the array of words with the corresponding clues
   @param nElem - the number of elements in aEntries   
*/   
void 
play(arrMatrix gameboard, int nRows, int nCols,
          wordList aEntries, int nElem)
{ 
   char cPlyrInpt;
   strWord sPlyrAns;
   int nPlyrChance = nCols, nCurrentRow = 0, nInd = -1, nClue = -1, nGBCol, bCancel, bGame = TRUE;
   
	while(nCurrentRow < nRows && bGame && bCancel != -1)
	{
		system("color 3f");
		cPlyrInpt = '0';
		
		system("cls");
		printf("\t[~] EXIT/CANCEL\n\n");
		printf("%43s\n\n", "+=| WORD TRIVIA |=+");
		printf("%38s%d", "Chances left: ", nPlyrChance);
		printGameboard(gameboard, nRows, nCols, nCurrentRow);
		
		// gameover when all answers in a row is wrong
		if(nPlyrChance == 0)
		{
			printf("\n%48s\n\n", "GAMEOVER! 0 CHANCES LEFT");
			delay(3);
			bGame = FALSE;
		}
		
		while(bGame && cPlyrInpt == '0') // gets input while invalid input by user
		{
			printf("%38s", "Enter letter: ");
			getSingleChar(&cPlyrInpt); // get letter choice for current row
		}

		if(cPlyrInpt == '~') // player cancels game
			bGame = FALSE;
		
		// checks if game is still on play
		if(bGame)
			checkGameboard(gameboard[nCurrentRow], aEntries, nElem, &nInd, &nGBCol, cPlyrInpt, nCurrentRow + 1, nCols);
			// checks input if it exists on the gameboard and in the entries
			
		// player entered a valid input but choice doesn't exist in the current row
		if(bGame && nInd == -1)
		{
			printf("%64s\n\n", "CHOICE DOES NOT EXIST IN THE CURRENT ROW");
			delay(3);
		}
			
		// choice exists in the entry
		if(bGame && nInd > -1)
		{
			//randomizes a clue/trivia to display to the user
			nClue = generateRandomNum(aEntries[nInd].numClues);
			// displays clue to the user
			printf("\n%29s\n%27s\t%s: %s\n\n", "CLUE:", "->", aEntries[nInd].clueList[nClue].relation, aEntries[nInd].clueList[nClue].relValue);
			
			getWordEntry(sPlyrAns, &bCancel, "Play"); // gets the answer of the user
			
			// if answer is wrong
			if(strcmp(sPlyrAns, aEntries[nInd].answer) != 0 && bCancel != -1)
			{
				gameboard[nCurrentRow][nGBCol] = '-';
				aEntries[nInd].use = 0;
				// nInd = -1;
				// nClue = -1;
				nPlyrChance--;
				printf("\n%30s\n\n", "WRONG!");
				system("color cf");
				system("color f4");
				system("color cf");
				delay(3);
			}
			
			// if answer is right
			if(strcmp(sPlyrAns, aEntries[nInd].answer) == 0 && bCancel != -1)
			{
				gameboard[nCurrentRow][nGBCol] = '*';
				aEntries[nInd].use = 0;
				nPlyrChance = nCols;
				cPlyrInpt = '\0';
				// nInd = -1;
				// nClue = -1;
				nCurrentRow++;
				printf("\n%32s\n\n", "CORRECT!");
				system("color e0");
				system("color f6");
				system("color e0");
				delay(3);
			}
		}
	}
   
   if(nCurrentRow == nRows) // player answered each row correctly
	{
		printf("%49s\n\n", "CONGRATULATIONS! YOU WIN!");
		delay(3);
	}
} // P

/* This function shows the menu options in the maintenance
   phase as well as call the functions (like listWords(), 
   addWord(), etc.) that are relevant to the chosen option.   
*/
void maintenance()
{
   wordList aEntries;
   int nElem = 0, bValid;
   char cChoice;
	strWord sFileName;
	
	initializeEntries(aEntries);
	system("color 0f");
	
   do
   {
      system("cls");
		printf("\t[X] EXIT/CANCEL");
      printf("\n\n%82s\n", "MAINTENANCE PHASE");
      displayHeaderUnderline();
      printf("\n");
	
      displayMaintenanceChoices();
      displayScanDivider();
      getSingleChar(&cChoice); // get user action/choice
		
      switch(cChoice)
      {
         case '1': // Add Word
            addWord(aEntries, &nElem);
            break;
				
         case '2': // Add Trivia
            addTriviaMenu(aEntries, nElem);
            break;
			
         case '3': // Modify Entry
				sortEntries(aEntries, nElem); // sort entries in alphabetical order
            executeModification(aEntries, nElem);
            break;
			
         case '4': // Delete word
				sortEntries(aEntries, nElem); // sort entries in alphabetical order
            deleteWord(aEntries, &nElem);
            break;
			
         case '5': // Delete clue
				sortEntries(aEntries, nElem); // sort entries in alphabetical order
            deleteClue(aEntries, nElem);
            break;
			
         case '6': // View words
				sortEntries(aEntries, nElem); // sort entries in alphabetical order
            viewWords(aEntries, nElem);
            break;
			
         case '7': // View clues
				sortEntries(aEntries, nElem); // sort entries in alphabetical order
            viewClues(aEntries, nElem);
            break;
			
         case '8': // Export
				system("cls");
				displayExportImportHeader("EXPORT");
            getWordEntry(sFileName, &bValid, "File"); // gets file name
				if(bValid == 1)
					export(sFileName, aEntries, nElem);
            break;
			
         case '9': // Import
				system("cls");
				displayExportImportHeader("IMPORT");
            getWordEntry(sFileName, &bValid, "File"); // gets file name
				if(bValid == 1)
					import(sFileName, aEntries, &nElem);
            break;
				
         default:
            if(cChoice != 'X')
				{
               printf("%80s", "INVALID INPUT");
					delay(3);
				}
            system("cls");
            break;
			
      }
   }while(cChoice != 'X');
} // P

/* This function returns the index where the key was found
   in aEntries. If not found, the function returns -1.
   @param key - the word or phrase to look for
   @param aEntries - the array of words with the corresponding clues
   @param nElem - the number of elements in aEntries
   @return the index where key was found, -1 otherwise
*/
int 
searchWord(strWord key, wordList aEntries, int nElem)
{
	int i, bFound = FALSE;
	
	for(i = 0; i < nElem && !bFound; i++)
		if(strcmp(aEntries[i].answer, key) == 0)
			bFound = TRUE; // key exists in the list of entries
	
	if(bFound)
		return i - 1; // returns index
	
	return -1; // not found
} // P

/* This function displays the list of words (without the clues)
   in aEntries. 
   @param aEntries - the array of words with the corresponding clues
   @param nElem - the number of elements in aEntries
*/ 
void 
listWords(wordList aEntries, int nElem)
{
	int z, nMid = nElem / 2, nSpace;
	
	if(nElem % 2 == 1) // nElem is odd
		nMid += 1;
	
	printViewBorders('H');
	
	// two column listing
	for(z = 0; z < nMid && nElem > 0; z++)
	{
		displayFloatingIcons(47, '!', 48);
		printf("%s", aEntries[z].answer); // 1st half of list
		
		nSpace = 35 - (int)strlen(aEntries[z].answer);
		displayFloatingIcons(nSpace, '!', nSpace + 1);
		
		if(nMid + z < nElem) // 2nd half of list
			printf("%s", aEntries[nMid + z].answer);
		
		printf("\n");
	}
	
	if(nElem == 0) // no entries yet
		printf("%85s\n", "NO EXISTING ENTRIES YET");
	
	printViewBorders('F');
	
	//ALTERNATIVE:
	// one column listing
	/*
	printf("%63c", UR);
	displayFloatingIcons(6, H, 1);
	printf(" Words ");
	displayFloatingIcons(6, H, 1);
	printf("%c\n", UL);
	
	for(z = 0; z < nElem; z++)
	{
		displayFloatingIcons(65, '!', 66);
		printf("%s\n", aEntries[z].answer);
	}
	
	printf("%63c", LR);
	displayFloatingIcons(19, H, 1);
	printf("%c\n", LL);
	*/
	
} // P

/* This function will ask the user to input the word, calls
   searchWord() to check if the word is unique.  If unique, this
   word is added, the addTrivia() is called, and the
   number of elements is updated. Other error checking (as 
   indicated in the MP specs requirements) are also done.

   @param aEntries - the array of words with the corresponding clues
   @param pElem - the address where the number of elements are 
                  to be updated
*/
void 
addWord(wordList aEntries, int * pElem)
{
	int bUnique, bValid;
	strWord strNewWord;
	
	system("cls");
	displayAddWordHeader();
	
	getWordEntry(strNewWord, &bValid, "Add"); // gets string input / word
	
	// checks if valid entered word is unique
	if(bValid == 1)
		bUnique = searchWord(strNewWord, aEntries, *pElem);
	
	// adds the new word to the aEntries if unique
	if(bValid == 1 && bUnique == -1)
	{
		strcpy(aEntries[*pElem].answer, strNewWord);
		printf("\n\n");
		addTrivia(&aEntries[*pElem]);
		*pElem += 1;
	}
	
	// when word already exists
	if(bValid == 1 && bUnique != -1)
		printf("%97s\n", "This word already exists in your current entry.");
}

/* This function will ask the user for the relation and
   relation value as store it in the clueList member field
   in *oneEntry, as long as the array can still accommodate
   more data and as long as the user still wants to add
   more entries. If there are existing clues already, new
   entries are added (not overwritten). The member field
   numClues is also updated.

   @param oneEntry - address to the structure containing the
                     array of clues and the number of clues,
					 among other data
*/
void 
addTrivia(struct triviaTag * oneEntry)
{
	int bAdd = TRUE, bHasRel = 0, bHasRelVal = -1;
	
	printf("%65s%s\n\n", "Trivia for: ", oneEntry->answer); // current word where trivia is being added
	
	// loop stops if player wants to cancel trivia input or if max capacity is reached
	// it also loop if user hasn't added at least 1 trivia to the word chosen
	while((bAdd && oneEntry->numClues < 10) || oneEntry->numClues == 0)
	{
		// skips this part if user hasn't entered a valid relation member field.
		// displays the recent valid entered relation member field.
		if(bHasRel == 1)
			printf("%67s%s\n", "Trivia Relation: ", oneEntry->clueList[oneEntry->numClues].relation);
		
		// skips this input part if user hasn't entered a -
		// - valid relation value pair for the recent entered relation
		// asks the user for the relation member field
		if(bHasRel == 0)
		{
			// input for relation member field
			printf("%67s", "Trivia Relation: ");
			bHasRel = getString(oneEntry->clueList[oneEntry->numClues].relation, LEN2, "~"); 
			 // ^^^  0 - invalid; 1 - valid; -1 - cancel
		}
		
		// asks for the relation value if user entered a valid relation member field
		if(bHasRel == 1)
		{
			// input for relation value member field
			printf("%67s", "Trivia Relation Value: ");
			bHasRelVal = getString(oneEntry->clueList[oneEntry->numClues].relValue, LEN2, "~");
			 // ^^^  0 - invalid; 1 - valid; -1 - cancel
		}
		
		// user cancels input for trivia
		if(bHasRel == -1 && oneEntry->numClues > 0)
		{
			// if ever player inputs a relation, then cancels the add trivia
			// then it will erase the relation input since it doesn't have a relation value pair
			memset(oneEntry->clueList[oneEntry->numClues].relation, '\0', 31);
			bAdd = FALSE;
		}
		
		// user entered an invalid input
		if(bHasRel == 0 || bHasRelVal == 0)
			printf("%99s\n\n", "Invalid input. Input must be at least 1 character.");
		
		// user entered a valid pair of relation and relation value memeber fields
		if(bHasRel == 1 && bHasRelVal == 1)
		{
			oneEntry->numClues++;
			bHasRelVal = -1;
			bHasRel = 0; // refreshes the input for relation member field
		}
		
		// user tries to cancel trivia input even it doesn't have at least 1 trivia
		if(bHasRel == -1 && oneEntry->numClues == 0)
		{
			printf("%92s\n\n", "A WORD MUST HAVE AT LEAST ONE TRIVIA");
			bHasRel = 0;
		}
	}
	
	// pairs of clues or trivia is at max capacity
	if(oneEntry->numClues == 10)
		printf("%89s\n%85s\n\n", "NUMBER OF CLUES AT MAX CAPACITY", "CAN'T ADD ANOTHER CLUE");
} // P

/* This function will ask the user for which part of the
   information in *oneEntry that will be modified (word or
   clue). The required input, including appropriate error
   checking as indicated in the specs, is done in this
   function.  The user is allowed to modify multiple parts
   of *oneEntry, until the user chooses to finish the 
   editing. For the clues, only modification can be done.
   That is, deletion or addition is not allowed here.

   @param oneEntry - address to the structure containing the
                     words, array of clues and the number of clues,
					 among other data
*/
void 
modifyEntry(struct triviaTag * oneEntry)
{
	int nNumChoice;
	char cDmp;
	
	do
	{
		system("CLS");
		displayModifyCluesHeader();
		displayCluesChoices(*oneEntry, oneEntry->numClues, TRUE); // clue choices
		scanf("%d%c", &nNumChoice, &cDmp); // number of choice for clue modification
		
		// valid choice
		if(nNumChoice > 0 && nNumChoice <= oneEntry->numClues)
			modifyClue(&oneEntry->clueList[nNumChoice - 1]);
		
		// invalid choice
		if(nNumChoice < 0 || nNumChoice > oneEntry->numClues)
		{
			printf("\n%81s\n\n", "INVALID CHOICE");
			delay(3); // pauses program for 3 seconds
		}
	}while(nNumChoice != 0); // until player decided to stop
} // P

/* This function will ask the user to input the word (or phrase)
   that will be deleted.  Calls to other functions, like 
   searchWord(), as needed by the function, will be done.
   If deletion is successful (i.e., the word was found), the
   number of elements in pElem should also be updated.

   @param aEntries - the array of words with the corresponding clues
   @param pElem - the address where the number of elements are 
                  to be updated
*/
void 
deleteWord(wordList aEntries, int *pElem)
{
	int nIndex = -2, bValid = -1, g;
	strWord sWordToDelete;
	
	system("CLS");
	displayDeleteWordHeader();
	listWords(aEntries, *pElem);
	
	getWordEntry(sWordToDelete, &bValid, "DeleteWord"); // get word to delete from user
	
	if(bValid == 1 && *pElem > 0) // checks if word entered exists
		nIndex = searchWord(sWordToDelete, aEntries, *pElem);
	
	if(nIndex > -1) // clears the word if it exist
	{
		for(g = nIndex; g + 1 < *pElem; g++)
			aEntries[g] = aEntries[g + 1];
		
		clearLastEntry(&aEntries[g]);
		
		*pElem -= 1; // updates the nElem 
	}
	
	if(*pElem == 0 && bValid == 1) // player enters a word even though there are no words to delete
		printf("%83s\n\n", "NO WORDS TO DELETE");
	
	if(nIndex == -1) // entered word is not in the current entry list
		printf("%81s\n\n", "WORD NOT FOUND.");
}

/* This function displays all information per element in aEntries.
   Part of the solution is to call the other function/s, like 
   viewEntry(), as needed by the function.
 
   @param aEntries - the array of words with the corresponding clues
   @param nElem - the number of elements in aEntries
*/
void 
viewWords(wordList aEntries, int nElem)
{
	int e = 0, nLastElem = nElem - 1;
	char cChoice;
	
	do
	{
		system("CLS");
		printf("\tX - Exit");
		displayViewWordsHeader();
		printViewBorders('H');
		if(nElem > 0)
			viewEntry(aEntries[e]);
		if(nElem == 0)
			printf("%85s\n", "NO EXISITING ENTRIES YET");
		printViewBorders('F');
		
		// displays for next and previous options
		if(e == 0 && nElem > 1) // when it is on the first entry
			printf("%78s\n","Next - >");
		
		if(e > 0 && e < nLastElem) // when entry is between the first and last
			printf("%65s%29s\n","< - Previous", "Next - >");
		
		if(e == nLastElem && nElem > 1) // when it is on the last entry
			printf("%80s\n","< - Previous");
		
		printf("%74s","Option[ < / > / X ]: ");
		getSingleChar(&cChoice);
		
		// next and previous option execution
		if(e >= 0 && e < nLastElem && cChoice == '>') // next
			e++;
		
		if(e > 0 && e <= nLastElem && cChoice == '<') // previous
			e--;
		
	}while(cChoice != 'X'); // exit
} // P

/* This function displays all information in entry.
   Part of the solution is to call the other function/s, like 
   displayAllClues(), as needed by the function.
 
   @param entry - structure containing the word and the clues, 
                  among other data
*/
void
viewEntry(struct triviaTag entry)
{
	displayFloatingIcons(57, '!', 61); // indent
	printf("Object: %s\n", entry.answer); // answer field of current entry
	
	// displays all the clues the entry currently has
	displayAllClues(entry.clueList, entry.numClues);
}

/* This function displays numClues number of elements from aList.

   @param aList - array of relation and relation values
   @param numClues - number of elements in aList
*/
void
displayAllClues(arrClues aList, int numClues)
{
	int c;
	
	for(c = 0; c < numClues; c++) // displays all the clues the entry currently has
	{
		displayFloatingIcons(59, '!', 66); // indent
		printf("%s: %s\n", aList[c].relation, aList[c].relValue);
	}
}

/* Given the filename stored in fname, this function
   overwrites the contents of the text file with the contents 
   if the array aEntries. Contents of aEntries and nElem will 
   not change in this function.
 
   @param fname - the string filename, including the extension
   @param aEntries - the array of words with the corresponding clues
   @param nElem - the number of elements in aEntries
*/
void
export(strWord fname, wordList aEntries, int nElem)
{
	int f, c;
	FILE *fpExport;
	fpExport = fopen(fname, "w");
	
	for(f = 0; f < nElem; f++)
	{
		fprintf(fpExport, "Object: %s\n", aEntries[f].answer); // prints in the file the answer member field
		
		for(c = 0; c < aEntries[f].numClues; c++) // prints in the file all the clues the current entry has
			fprintf(fpExport, "%s: %s\n", aEntries[f].clueList[c].relation, aEntries[f].clueList[c].relValue);
		
		fprintf(fpExport, "\n");
	}
	
	fclose(fpExport);
} // P

/* This function sorts the contents of aEntries in increasing
   order (based on ASCII value). The solution to this function
   should be an IMPLEMENTATION of a sorting algorithm, NOT by
   calling a sort function from some C library.
   
   @param aEntries - the array of words with the corresponding clues
   @param nElem - the number of elements in aEntries
*/
void 
sortEntries(wordList aEntries, int nElem)
{
	int t, u, nMinIndex;
	
	for(t = 0; t < nElem - 1; t++)
	{
		nMinIndex = t; // initialize current element as minimum ASCII
		
		for(u = t + 1; u < nElem; u++)
			if(strcmp(aEntries[nMinIndex].answer, aEntries[u].answer) > 0)
				nMinIndex = u; // a lesser ASCII value string is found
		
		// swaps the entry with the minimum ASCII value to the current one
		swapEntries(&aEntries[t], &aEntries[nMinIndex]);
	}
}


int main()
{
   wordList aEntries;
   arrMatrix gameboard;
   int nElem = 0, bValid;
   char cChoice;
	strWord sFileName;
   srand(time(0));
   
   // listWords()
   /*
   import("INIT1.txt", aEntries, &nElem);
   sortEntries(aEntries, nElem);
   listWords(aEntries, nElem);
   */
   
   // displayFloatingIcons()
   /*
   displayFloatingIcons(5, '!', 0);
   printf("~");
   printf("\n");
   displayFloatingIcons(0, '!', 5);
   printf("~");
   printf("\n");
   displayFloatingIcons(11, '!', 7);
   printf("~");
   printf("\n");
   displayFloatingIcons(10, '!', 11);
   printf("~");
   printf("\n");
   */
   
   // getSingleChar()
   /*
   do{
      printf("Enter char: ");
      getSingleChar(&cChoice);
      printf("%c", cChoice);
   }while(cChoice != '~');
   printf("End\n");
   */
   
   // getString()
   /*
   do {
      printf("Enter string: ");
      bValid = getString(sFileName, 20, "~");
      
      printf("Valid: %d\nString length: %d\n%s", bValid, (int)strlen(sFileName), sFileName);
   }while(bValid != -1);
   */
   
   // generateRandomNum()
   /*
   do{
      printf("Enter Max: ");
      scanf("%d", &bValid);
      // nElem = generateRandomNum(bValid);
      printf("%d\n", generateRandomNum(bValid));
   }while(bValid != -1);
   printf("End\n");
   */
   
   // delay()
   /*
   do{
      printf("Seconds: ");
      scanf("%d%c", &nElem, &cChoice);
      delay(nElem);
   }while(nElem != -2);
   printf("End\n");
   */
   
   // getWordEntry()
   /*
   getWordEntry(sFileName, &bValid, "Add");
   printf("Valid: %d\n%s\n", bValid, sFileName);
   getWordEntry(sFileName, &bValid, "ModifyWord");
   printf("Valid: %d\n%s\n", bValid, sFileName);
   getWordEntry(sFileName, &bValid, "DeleteWord");
   printf("Valid: %d\n%s\n", bValid, sFileName);
   getWordEntry(sFileName, &bValid, "DeleteClue");
   printf("Valid: %d\n%s\n", bValid, sFileName);
   getWordEntry(sFileName, &bValid, "ViewClue");
   printf("Valid: %d\n%s\n", bValid, sFileName);
   getWordEntry(sFileName, &bValid, "File");
   printf("Valid: %d\n%s\n", bValid, sFileName);
   */
   
   // isUniqueLetter()
   /*
   strcpy(aEntries[0].answer, "Ant");
   strcpy(aEntries[1].answer, "Beetle");
   strcpy(aEntries[2].answer, "Cat");
   strcpy(aEntries[3].answer, "Dog");
   aEntries[0].use = 0;
   aEntries[1].use = 0;
   aEntries[2].use = 2;
   aEntries[3].use = 0;
   gameboard[0][0] = 'D';
   gameboard[0][1] = 'C';
   nElem = isUniqueLetter(gameboard[0], 2, 0, aEntries);
   printf("%d\n\n", nElem);
   nElem = isUniqueLetter(gameboard[0], 2, 2, aEntries);
   printf("%d\n\n", nElem);
   gameboard[0][0] = 'A';
   aEntries[0].use = 1;
   nElem = isUniqueLetter(gameboard[0], 2, 0, aEntries);
   printf("%d\n\n", nElem);
   */
   
   // checkLettersLeft()
   /*
   initializeEntries(aEntries);
   // import("INIT1.txt", aEntries, &nElem);
   // import("INIT2.txt", aEntries, &nElem);
   // strcpy(aEntries[7].answer, "Wheel");
   // nElem += 1;
   aEntries[0].use = 1;
   aEntries[1].use = 1;
   aEntries[2].use = 2;
   aEntries[3].use = 1;
   aEntries[4].use = 1;
   aEntries[5].use = 2;
   aEntries[6].use = 2;
   // aEntries[7].use = 2;
   
   printf("Letters: %d", checkLettersLeft(aEntries, nElem));
   */
   
   // checkGameboard()
   /*
   int nInd, nCol;
   strcpy(aEntries[0].answer, "Ant");
   strcpy(aEntries[1].answer, "Wheel");
   strcpy(aEntries[2].answer, "Snake");
   strcpy(aEntries[3].answer, "Dog");
   strcpy(aEntries[1].answer, "Apex");
   gameboard[0][0] = 'A';
   gameboard[0][1] = 'W';
   gameboard[0][2] = 'S';
   gameboard[0][3] = 'D';
   strcpy(aEntries[0].answer, "Ant");
   aEntries[0].use = 1; // 1 or 2
   // aEntries[4].use = 1;
   
   do{
      printf("Enter letter: ");
      getSingleChar(&cChoice);
      checkGameboard(gameboard[0], aEntries, 5, &nInd, &nCol, cChoice, 1, 4);
      printf("Index: %d\nCol: %d\n", nInd, nCol);
   }while(cChoice != '~');
   printf("End\n\n");
   */
   
   // initializeEntries()
   /*
   strcpy(aEntries[0].answer, "Ant");
   strcpy(aEntries[0].clueList[0].relation, "Kind of");
   strcpy(aEntries[0].clueList[0].relValue, "Insect");
   aEntries[0].numClues = 1;
   aEntries[0].use = 3;
   strcpy(aEntries[1].answer, "Apple Pie");
   strcpy(aEntries[1].clueList[0].relation, "Taste");
   strcpy(aEntries[1].clueList[0].relValue, "Sweet");
   aEntries[1].numClues = 1;
   aEntries[1].use = 2;
   strcpy(aEntries[2].answer, "Ax");
   strcpy(aEntries[2].clueList[0].relation, "Usage");
   strcpy(aEntries[2].clueList[0].relValue, "Wood cutting");
   aEntries[2].numClues = 1;
   aEntries[2].use = 1;
   
   for(nElem = 0; nElem < 3; nElem++)
   {
      printf("Object: %s\n", aEntries[nElem].answer);
      printf("%s: %s\n", aEntries[nElem].clueList[0].relation, aEntries[nElem].clueList[0].relValue);
      printf("Clues: %d\nUse: %d\n\n", aEntries[nElem].numClues, aEntries[nElem].use);
   }
   initializeEntries(aEntries);
   
   for(nElem = 0; nElem < 3; nElem++)
   {
      printf("Object: %s\n", aEntries[nElem].answer);
      printf("%s: %s\n", aEntries[nElem].clueList[0].relation, aEntries[nElem].clueList[0].relValue);
      printf("Clues: %d\nUse: %d\n\n", aEntries[nElem].numClues, aEntries[nElem].use);
   }
   initializeEntries(aEntries);
   
   for(nElem = 0; nElem < 3; nElem++)
   {
      printf("Object: %s\n", aEntries[nElem].answer);
      printf("%s: %s\n", aEntries[nElem].clueList[0].relation, aEntries[nElem].clueList[0].relValue);
      printf("Clues: %d\nUse: %d\n\n", aEntries[nElem].numClues, aEntries[nElem].use);
   }
   */ 
   
   // addTriviaMenu()
   /*
   strcpy(aEntries[0].answer, "Ant");
   strcpy(aEntries[0].clueList[0].relation, "Kind of");
   strcpy(aEntries[0].clueList[0].relValue, "Insect");
   aEntries[0].numClues = 1;
   strcpy(aEntries[1].answer, "Apple Pie");
   strcpy(aEntries[1].clueList[0].relation, "Taste");
   strcpy(aEntries[1].clueList[0].relValue, "Sweet");
   aEntries[1].numClues = 1;
   strcpy(aEntries[2].answer, "Ax");
   strcpy(aEntries[2].clueList[0].relation, "Usage");
   strcpy(aEntries[2].clueList[0].relValue, "Wood cutting");
   aEntries[2].numClues = 1;
   
   addTriviaMenu(aEntries, 3);
   */ // P
   
   // modifyWord()
   /*
   strcpy(aEntries[0].answer, "Ant");
   strcpy(aEntries[0].clueList[0].relation, "Kind of");
   strcpy(aEntries[0].clueList[0].relValue, "Insect");
   aEntries[0].numClues = 1;
   strcpy(aEntries[1].answer, "Apple Pie");
   strcpy(aEntries[1].clueList[0].relation, "Taste");
   strcpy(aEntries[1].clueList[0].relValue, "Sweet");
   aEntries[1].numClues = 1;
   strcpy(aEntries[2].answer, "Ax");
   strcpy(aEntries[2].clueList[0].relation, "Usage");
   strcpy(aEntries[2].clueList[0].relValue, "Wood cutting");
   aEntries[2].numClues = 1;
   
   modifyWord(&aEntries[2], aEntries, 3);
   
   // printf("%s", aEntries[2].answer);
   */
   
   // getClueModification()
   /*
   strcpy(aEntries[1].answer, "Apple Pie");
   strcpy(aEntries[1].clueList[0].relation, "Color");
   strcpy(aEntries[1].clueList[0].relValue, "Golden Brown");
   
   getClueModification(&aEntries[1].clueList[0], aEntries[1].clueList[0].relation, "Relation");
   printf("Object: %s\n%s: %s", aEntries[1].answer, aEntries[1].clueList[0].relation, aEntries[1].clueList[0].relValue);
   */
   
   // modifyClue()
   /*
   strcpy(aEntries[1].answer, "Apple Pie");
   strcpy(aEntries[1].clueList[0].relation, "Color");
   strcpy(aEntries[1].clueList[0].relValue, "Golden Brown");
   
   modifyClue(&aEntries[1].clueList[0]);
   printf("\nObject: %s\n%s: %s", aEntries[1].answer, aEntries[1].clueList[0].relation, aEntries[1].clueList[0].relValue);
   */
   
   // executeModification()
   /*
   strcpy(aEntries[0].answer, "Ant");
   strcpy(aEntries[0].clueList[0].relation, "Kind of");
   strcpy(aEntries[0].clueList[0].relValue, "Insect");
   aEntries[0].numClues = 1;
   strcpy(aEntries[1].answer, "Apple Pie");
   strcpy(aEntries[1].clueList[0].relation, "Taste");
   strcpy(aEntries[1].clueList[0].relValue, "Sweet");
   aEntries[1].numClues = 1;
   strcpy(aEntries[2].answer, "Ax");
   strcpy(aEntries[2].clueList[0].relation, "Usage");
   strcpy(aEntries[2].clueList[0].relValue, "Wood cutting");
   aEntries[2].numClues = 1;
   
   executeModification(aEntries, 3);
	for(nElem = 0; nElem < 3; nElem++)
   {
      printf("Object: %s\n", aEntries[nElem].answer);
      printf("%s: %s\n", aEntries[nElem].clueList[0].relation, aEntries[nElem].clueList[0].relValue);
      printf("Clues: %d\n\n", aEntries[nElem].numClues);
   }
   */
   
   // clearLastEntry()
   /*
   strcpy(aEntries[0].answer, "Ant");
   strcpy(aEntries[0].clueList[0].relation, "Kind of");
   strcpy(aEntries[0].clueList[0].relValue, "Insect");
   aEntries[0].numClues = 1;
   aEntries[0].use = 1;
   strcpy(aEntries[1].answer, "Apple Pie");
   strcpy(aEntries[1].clueList[0].relation, "Taste");
   strcpy(aEntries[1].clueList[0].relValue, "Sweet");
   aEntries[1].numClues = 1;
   aEntries[1].use = 0;
   strcpy(aEntries[2].answer, "Ax");
   strcpy(aEntries[2].clueList[0].relation, "Usage");
   strcpy(aEntries[2].clueList[0].relValue, "Wood cutting");
   aEntries[2].numClues = 1;
   aEntries[2].use = 3;
   
   printf("Not Cleared\n");
   for(nElem = 0; nElem < 3; nElem++)
   {
      printf("Object: %s\n", aEntries[nElem].answer);
      printf("%s: %s\n", aEntries[nElem].clueList[0].relation, aEntries[nElem].clueList[0].relValue);
      printf("Clues: %d\nUse: %d\n\n", aEntries[nElem].numClues, aEntries[nElem].use);
   }
   
   clearLastEntry(&aEntries[2]);
   printf("Cleared\n");
   for(nElem = 0; nElem < 3; nElem++)
   {
      printf("Object: %s\n", aEntries[nElem].answer);
      printf("%s: %s\n", aEntries[nElem].clueList[0].relation, aEntries[nElem].clueList[0].relValue);
      printf("Clues: %d\nUse: %d\n\n", aEntries[nElem].numClues, aEntries[nElem].use);
   }
   
   printf("Next set w/ garbage values\n");
   aEntries[2].numClues = 3;
   
   for(nElem = 0; nElem < 3; nElem++)
   {
      printf("Object: %s\n", aEntries[nElem].answer);
      printf("%s: %s\n", aEntries[nElem].clueList[0].relation, aEntries[nElem].clueList[0].relValue);
      printf("Clues: %d\nUse: %d\n\n", aEntries[nElem].numClues, aEntries[nElem].use);
   }
   
   clearLastEntry(&aEntries[2]);
   printf("Cleared\n");
   for(nElem = 0; nElem < 3; nElem++)
   {
      printf("Object: %s\n", aEntries[nElem].answer);
      printf("%s: %s\n", aEntries[nElem].clueList[0].relation, aEntries[nElem].clueList[0].relValue);
      printf("Clues: %d\nUse: %d\n\n", aEntries[nElem].numClues, aEntries[nElem].use);
   }
   */
   
   
   // executeClueDeletion()
   /*
   strcpy(aEntries[3].answer, "Cleaning Bot");
   strcpy(aEntries[3].clueList[0].relation, "Usage");
   strcpy(aEntries[3].clueList[0].relValue, "Cleaning");
   strcpy(aEntries[3].clueList[1].relation, "Shape");
   strcpy(aEntries[3].clueList[1].relValue, "Cylinder");
   strcpy(aEntries[3].clueList[2].relation, "Made of");
   strcpy(aEntries[3].clueList[2].relValue, "Metal");
   strcpy(aEntries[3].clueList[3].relation, "Powered by");
   strcpy(aEntries[3].clueList[3].relValue, "Electricity");
   aEntries[3].numClues = 4;
   
   do{
      printf("Object: %s\n", aEntries[3].answer);
      for(nElem = 0; nElem < aEntries[3].numClues; nElem++)
         printf("%s: %s\n", aEntries[3].clueList[nElem].relation, aEntries[3].clueList[nElem].relValue);
      printf("Clues: %d", aEntries[3].numClues);
      
      printf("\nEnter index: ");
      scanf("%d%c", &bValid, &cChoice);
      
      executeClueDeletion(aEntries[3].clueList, &aEntries[3].numClues, bValid);
   }while(bValid != -1);
   */
   
   // deleteClue()
   /*
   strcpy(aEntries[0].answer, "Ant");
   strcpy(aEntries[0].clueList[0].relation, "Kind of");
   strcpy(aEntries[0].clueList[0].relValue, "Insect");
   aEntries[0].numClues = 1;
   strcpy(aEntries[1].answer, "Apple Pie");
   strcpy(aEntries[1].clueList[0].relation, "Kind of");
   strcpy(aEntries[1].clueList[0].relValue, "Dessert");
   aEntries[1].numClues = 1;
   strcpy(aEntries[2].answer, "Ax");
   strcpy(aEntries[2].clueList[0].relation, "Used for");
   strcpy(aEntries[2].clueList[0].relValue, "Chopping Wood");
   strcpy(aEntries[2].clueList[1].relation, "Sharpness");
   strcpy(aEntries[2].clueList[1].relValue, "High");
   strcpy(aEntries[2].clueList[2].relation, "Made of");
   strcpy(aEntries[2].clueList[2].relValue, "Wood and metal");
   aEntries[2].numClues = 3;
   
   deleteClue(aEntries, 3);
   deleteClue(aEntries, 3);
   */
	
	// viewClues
	/*
	// import("INIT1.txt", aEntries, &nElem);
	// import("EXPORT3.txt", aEntries, &nElem);
	
	viewClues(aEntries, 0); // nElem or 0
	*/
   
   // swapEntries()
   /*
   strcpy(aEntries[0].answer, "Ant");
   strcpy(aEntries[0].clueList[0].relation, "Kind of");
   strcpy(aEntries[0].clueList[0].relValue, "Insect");
   aEntries[0].numClues = 1;
   strcpy(aEntries[1].answer, "Apple Pie");
   strcpy(aEntries[1].clueList[0].relation, "Kind of");
   strcpy(aEntries[1].clueList[0].relValue, "Dessert");
   
   for(nElem = 0; nElem < 2; nElem++)
   {
      printf("Object: %s\n", aEntries[nElem].answer);
      printf("%s: %s\n", aEntries[nElem].clueList[0].relation, aEntries[nElem].clueList[0].relValue);
      printf("Clues: %d\nUse: %d\n\n", aEntries[nElem].numClues, aEntries[nElem].use);
   }
   
   swapEntries(&aEntries[0], &aEntries[0]);
   printf("Swapped to itself\n");
   for(nElem = 0; nElem < 2; nElem++)
   {
      printf("Object: %s\n", aEntries[nElem].answer);
      printf("%s: %s\n", aEntries[nElem].clueList[0].relation, aEntries[nElem].clueList[0].relValue);
      printf("Clues: %d\nUse: %d\n\n", aEntries[nElem].numClues, aEntries[nElem].use);
   }
   
   swapEntries(&aEntries[0], &aEntries[1]);
   printf("Swapped\n");
   for(nElem = 0; nElem < 2; nElem++)
   {
      printf("Object: %s\n", aEntries[nElem].answer);
      printf("%s: %s\n", aEntries[nElem].clueList[0].relation, aEntries[nElem].clueList[0].relValue);
      printf("Clues: %d\nUse: %d\n\n", aEntries[nElem].numClues, aEntries[nElem].use);
   }
   
   swapEntries(&aEntries[0], &aEntries[1]);
   printf("Swapped back\n");
   for(nElem = 0; nElem < 2; nElem++)
   {
      printf("Object: %s\n", aEntries[nElem].answer);
      printf("%s: %s\n", aEntries[nElem].clueList[0].relation, aEntries[nElem].clueList[0].relValue);
      printf("Clues: %d\nUse: %d\n\n", aEntries[nElem].numClues, aEntries[nElem].use);
   }
   */
   
   // import()
   /*
   int k;
   strcpy(aEntries[0].answer, "Ant");
   strcpy(aEntries[0].clueList[0].relation, "Kind of");
   strcpy(aEntries[0].clueList[0].relValue, "Insect");
   aEntries[0].numClues = 1;
   strcpy(aEntries[1].answer, "Apple Pie");
   strcpy(aEntries[1].clueList[0].relation, "Taste");
   strcpy(aEntries[1].clueList[0].relValue, "Sweet");
   aEntries[1].numClues = 1;
   strcpy(aEntries[2].answer, "Ax");
   strcpy(aEntries[2].clueList[0].relation, "Usage");
   strcpy(aEntries[2].clueList[0].relValue, "Wood cutting");
   aEntries[2].numClues = 1;
   nElem = 3;
   
   import("SAMPLE-TRIVIA.txt", aEntries, &nElem);
   for(bValid = 0; bValid < nElem; bValid++)
   {
      printf("Object: %s\n", aEntries[bValid].answer);
      
      for(k = 0; k < aEntries[bValid].numClues; k++)
         printf("%s: %s\n", aEntries[bValid].clueList[k].relation, aEntries[bValid].clueList[k].relValue);
      
      printf("Clues: %d\n\n", aEntries[bValid].numClues);
   }
   */
   /*
   int k;
   nElem = 0;
   initializeEntries(aEntries);
   
   // import("SAMPLE-TRIVIA2.txt", aEntries, &nElem);
   // import("EXPORT3.txt", aEntries, &nElem);
   for(bValid = 0; bValid < nElem; bValid++)
   {
      printf("Object: %s\n", aEntries[bValid].answer);
      
      for(k = 0; k < aEntries[bValid].numClues; k++)
         printf("%s: %s\n", aEntries[bValid].clueList[k].relation, aEntries[bValid].clueList[k].relValue);
      
      printf("Clues: %d\n\n", aEntries[bValid].numClues);
   }
   */
   /*
   int k;
   nElem = 149;
   strcpy(aEntries[148].answer, "Yoyo");
   strcpy(aEntries[148].clueList[0].relation, "Used for");
   strcpy(aEntries[148].clueList[0].relValue, "Playing");
   aEntries[148].numClues = 1;
   
   import("LAST.txt", aEntries, &nElem);
   for(bValid = 148; bValid < nElem; bValid++)
   {
      printf("Object: %s\n", aEntries[bValid].answer);
      
      for(k = 0; k < aEntries[bValid].numClues; k++)
         printf("%s: %s\n", aEntries[bValid].clueList[k].relation, aEntries[bValid].clueList[k].relValue);
      
      printf("Clues: %d\n\n", aEntries[bValid].numClues);
   }
   */
   
   // initBoard()
   /*
   int k, j;
   initializeEntries(aEntries);
   // import("INIT1.txt", aEntries, &nElem);
   // import("INIT2.txt", aEntries, &nElem);
   
   bValid = initBoard(gameboard, 3, 2, aEntries, nElem);
   printf("%d\n", bValid);
   for(k = 0; k < 3 && bValid; k++)
      for(j = 0; j < 2; j++)
         printf("%c ", gameboard[k][j]);
      printf("\n");
   for(k = 0; k < nElem && bValid; k++)
      printf("%s\nUse: %d\n", aEntries[k].answer, aEntries[k].use);
   */
   
   // play()
   /*
   gameboard[0][0] = 'Q';
   gameboard[0][1] = 'W';
   gameboard[0][2] = 'E';
   gameboard[0][3] = 'R';
   gameboard[1][0] = 'A';
   gameboard[1][1] = 'S';
   gameboard[1][2] = 'D';
   gameboard[1][3] = 'F';
   strcpy(aEntries[0].answer, "Wheel");
   strcpy(aEntries[0].clueList[0].relation, "Shape");
   strcpy(aEntries[0].clueList[0].relValue, "Circle");
   aEntries[0].numClues = 1;
   aEntries[0].use = 1;
   
   nElem = 1;
   
   play(gameboard, 2, 4, aEntries, nElem);
   */
   
   // maintenaince();
   // /*
   maintenance();
   // */
   
   // searchWord
   /*
   int ind;
   strcpy(aEntries[0].answer, "Ant");
   strcpy(aEntries[1].answer, "Apple Pie");
   strcpy(aEntries[2].answer, "Ax");
   
   do{
      printf("Enter word: ");
      bValid = getString(sFileName, 20, "~");
      if(bValid == 1)
         ind = searchWord(sFileName, aEntries, 3);
      printf("%d", ind);
   }while(bValid != -1);
   */
   
   // addWord()
   /*
   int k, j;
   // strcpy(aEntries[0].answer, "Ant");
   // strcpy(aEntries[1].answer, "Apple Pie");
   // strcpy(aEntries[2].answer, "Ax");
   // nElem = 1; // 1 or 3
   
   addWord(aEntries, &nElem);
   
   for(j = 0; j < nElem; j++)
   {
      printf("Object: %s\n", aEntries[j].answer);
      for(k = 0; k < aEntries[j].numClues; k++)
         printf("%s: %s\n", aEntries[j].clueList[k].relation, aEntries[j].clueList[k].relValue);
      printf("\n");
   }
   */
   
   // addTrivia()
   /*
   int k;
   
   strcpy(aEntries[0].answer, "Ant");
   strcpy(aEntries[0].clueList[0].relation, "Kind of");
   strcpy(aEntries[0].clueList[0].relValue, "Insect");
   aEntries[0].numClues = 1;
   strcpy(aEntries[1].answer, "Apple Pie");
   strcpy(aEntries[1].clueList[8].relation, "Taste");
   strcpy(aEntries[1].clueList[8].relValue, "Sweet");
   aEntries[1].numClues = 9;
   strcpy(aEntries[2].answer, "Ax");
   strcpy(aEntries[2].clueList[0].relation, "Usage");
   strcpy(aEntries[2].clueList[0].relValue, "Wood cutting");
   aEntries[2].numClues = 1;
   
   addTrivia(&aEntries[1]);
   
   for(k = 8; k < aEntries[1].numClues; k++)
      printf("%s: %s\n", aEntries[1].clueList[k].relation, aEntries[1].clueList[k].relValue);
   */
   
   // modifyEntry()
   /*
	int k;
   strcpy(aEntries[3].answer, "Cleaning Bot");
   strcpy(aEntries[3].clueList[0].relation, "Usage");
   strcpy(aEntries[3].clueList[0].relValue, "Cleaning");
   strcpy(aEntries[3].clueList[1].relation, "Shape");
   strcpy(aEntries[3].clueList[1].relValue, "Cylinder");
   strcpy(aEntries[3].clueList[2].relation, "Made of");
   strcpy(aEntries[3].clueList[2].relValue, "Metal");
   strcpy(aEntries[3].clueList[3].relation, "Powered by");
   strcpy(aEntries[3].clueList[3].relValue, "Electricity");
   aEntries[3].numClues = 4;
   
   modifyEntry(&aEntries[3]);
	
	for(k = 0; k < aEntries[3].numClues; k++)
      printf("%s: %s\n", aEntries[3].clueList[k].relation, aEntries[3].clueList[k].relValue);
   */
   
   // deleteWord()
   /*
   int k;
   strcpy(aEntries[0].answer, "Ant");
   strcpy(aEntries[0].clueList[0].relation, "Kind of");
   strcpy(aEntries[0].clueList[0].relValue, "Insect");
   aEntries[0].numClues = 1;
   strcpy(aEntries[1].answer, "Apple Pie");
   strcpy(aEntries[1].clueList[0].relation, "Taste");
   strcpy(aEntries[1].clueList[0].relValue, "Sweet");
   aEntries[1].numClues = 1;
   strcpy(aEntries[2].answer, "Ax");
   strcpy(aEntries[2].clueList[0].relation, "Usage");
   strcpy(aEntries[2].clueList[0].relValue, "Wood cutting");
   aEntries[2].numClues = 1;
   nElem = 0; // 3 or 0
   
   deleteWord(aEntries, &nElem);
   
   for(k = 0; k < nElem; k++)
   {
      printf("Object: %s\n", aEntries[k].answer);
      printf("%s: %s\n", aEntries[k].clueList[0].relation, aEntries[k].clueList[0].relValue);
      printf("Clues: %d\nUse: %d\n\n", aEntries[k].numClues, aEntries[k].use);
   }
	printf("Object: %s\n", aEntries[k].answer);
   printf("%s: %s\n", aEntries[k].clueList[0].relation, aEntries[k].clueList[0].relValue);
   printf("Clues: %d\nUse: %d\n\n", aEntries[k].numClues, aEntries[k].use);
	printf("Elem: %d\n", nElem);
   */
   
   // viewWords()
   /*
   // import("INIT1.txt", aEntries, &nElem);
   
   viewWords(aEntries, 0); // nElem or 0
   */
   
   // export()
   /*
   strcpy(aEntries[0].answer, "Ant");
   strcpy(aEntries[0].clueList[0].relation, "Kind of");
   strcpy(aEntries[0].clueList[0].relValue, "Insect");
   aEntries[0].numClues = 1;
   strcpy(aEntries[1].answer, "Apple Pie");
   strcpy(aEntries[1].clueList[0].relation, "Taste");
   strcpy(aEntries[1].clueList[0].relValue, "Sweet");
	// strcpy(aEntries[1].clueList[1].relation, "Made of");
   // strcpy(aEntries[1].clueList[1].relValue, "Apple and dough");
	// strcpy(aEntries[1].clueList[2].relation, "Kind of");
   // strcpy(aEntries[1].clueList[2].relValue, "Dessert");
	// strcpy(aEntries[1].clueList[3].relation, "Type");
   // strcpy(aEntries[1].clueList[3].relValue, "Pie");
	// strcpy(aEntries[1].clueList[4].relation, "Shape");
   // strcpy(aEntries[1].clueList[4].relValue, "Circular");
	// strcpy(aEntries[1].clueList[5].relation, "Type");
   // strcpy(aEntries[1].clueList[5].relValue, "Food");
	// strcpy(aEntries[1].clueList[6].relation, "Texture");
   // strcpy(aEntries[1].clueList[6].relValue, "Crusty");
	// strcpy(aEntries[1].clueList[7].relation, "Main ingredient");
   // strcpy(aEntries[1].clueList[7].relValue, "Apple");
	// strcpy(aEntries[1].clueList[8].relation, "Made out of");
   // strcpy(aEntries[1].clueList[8].relValue, "Love");
	// strcpy(aEntries[1].clueList[9].relation, "Contains");
   // strcpy(aEntries[1].clueList[9].relValue, "Sugar");
   aEntries[1].numClues = 1; // 1 or 2 or 10
   strcpy(aEntries[2].answer, "Ax");
   strcpy(aEntries[2].clueList[0].relation, "Usage");
   strcpy(aEntries[2].clueList[0].relValue, "Wood cutting");
   aEntries[2].numClues = 1;
   nElem = 3;
   
   export("EXPORT3.txt", aEntries, nElem);
   */
   
   // sortEntries()
   /*
   int k, j;
   import("INIT1.txt", aEntries, &nElem);
   bValid = 1;
	do{
   sortEntries(aEntries, nElem);
   // sortEntries(aEntries, 0); nElem = 0;
   
   for(j = 0; j < nElem; j++)
   {
      printf("Object: %s\n", aEntries[j].answer);
      
      for(k = 0; k < aEntries[j].numClues; k++)
         printf("%s: %s\n", aEntries[j].clueList[k].relation, aEntries[j].clueList[k].relValue);
      
      printf("Clues: %d\n\n", aEntries[j].numClues);
   }
	printf("Elem: %d\n\n", nElem);
	bValid += 1;
	}while(bValid <= 2);
   */
   
   
   
   return 0;
}