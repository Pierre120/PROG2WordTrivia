#include "trivia_HERNANDEZ.c"  //modify this to reflect the filename that you used for your solution.

int
main()
{
   arrMatrix gameboard;
   int nRows, nCols;
   int opt;
   char cDump;
   wordList aEntries;
   int nElem = 0;
   int test;
   strWord fname;
   srand(time(0));
	
   do
   {
		// printf("Main Menu\n");
		// printf("1 - Game Phase\n");
		// printf("2 - Maintenance Phase\n");
		// printf("3 - Exit\n");
		system("color 0b");
		printf("\n\n%78s\n", "MAIN MENU");
		displayHeaderUnderline();
		printf("\n\n");
		printf("%60s%18s\n", "[1]", "GAME PHASE");
		printf("%60s%25s\n", "[2]", "MAINTENANCE PHASE");
		printf("%60s%12s\n\n", "[3]", "EXIT");
		displayScanDivider();
    
		scanf("%d%c", &opt, &cDump);
   
		switch (opt)
		{
			case 1: 
				do
				{
					do
		         {
						// initializeEntries(aEntries);
						printf("%86s", "How many rows for the board? ");
						scanf("%d", &nRows);
						printf("%89s", "How many columns for the board? ");
						scanf("%d%c", &nCols, &cDump);
						
						printf("%68s", "Enter filename for source of trivia: ");
						getString(fname, 20, "~"); //scanf("%s", fname);
						import(fname, aEntries, &nElem);
						
						test = initBoard(gameboard, nRows, nCols, aEntries, nElem);
					} while (test == 0 || !(nRows > 0 && nRows <= 10) || 
				         !(nCols > 0 && nCols <= 10));
						
					play(gameboard, nRows, nCols, aEntries, nElem);
					printf("\n%61s", "Wanna play another round? [Y/y/N/n]: ");
					getSingleChar(&cDump);
				}while(cDump != 'n' && cDump != 'N');
					break;
			case 2: maintenance();
         case 3: break;
         default: printf("%81s\n", "Invalid option");
     }
   } while (opt != 3);	

   return 0;                 				 
}