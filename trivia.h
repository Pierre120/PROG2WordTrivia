/* You are NOT allowed to modify the contents of this file */

#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX  10
#define LEN1 20
#define LEN2 30
#define CAP  150

typedef char arrMatrix[MAX][MAX];
typedef char strWord[LEN1 + 1];
typedef char phrase[LEN2 + 1]; 

struct clueTag
{
	phrase relation,
	       relValue;
};

typedef struct clueTag arrClues[MAX];

struct triviaTag
{
	strWord   answer;
	arrClues  clueList;
	int       numClues;
	int       use;
};

typedef struct triviaTag wordList[CAP];

/* You are required to implement the following functions
   in your MP.  You are NOT allowed to modify the parameter
   list, the return type or the function name of those that 
   are indicated here. The functions indicated here will be 
   ones called in the main() that you will be provided during the
   MP demo.
   
   You are EXPECTED to add additional functions to support
   the requirements, but these additional functions should be
   called within the functions that you are required to accomplish.
   
   Note that the function names indicated here do not directly
   translate to the tasks that the feature indicated in the 
   MP specs.  Refer to the internal documentation provided to
   know what each function is supposed to accomplish.  This
   also indicates how you are supposed to document your own
   functions.
*/

void import(strWord fname, wordList aEntries, int * pElem);
int initBoard(arrMatrix gameboard, int nRows, int nCols,
              wordList aEntries, int nElem);
void play(arrMatrix gameboard, int nRows, int nCols,
          wordList aEntries, int nElem);
void maintenance();
int searchWord(strWord key, wordList aEntries, int nElem);
void listWords(wordList aEntries, int nElem);
void addWord(wordList aEntries, int * pElem);
void addTrivia(struct triviaTag * oneEntry);
void modifyEntry(struct triviaTag * oneEntry);
void deleteWord(wordList aEntries, int *pElem);
void viewWords(wordList aEntries, int nElem);
void viewEntry(struct triviaTag entry);
void displayAllClues(arrClues aList, int numClues);
void export(strWord fname, wordList aEntries, int nElem);
void sortEntries(wordList aEntries, int nElem);
	


