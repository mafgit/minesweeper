#include <stdio.h>
// following are to reduce warnings in compiler
#include <stdlib.h>
#include <time.h>

void showInstructions()
{
    printf("\n  Following commands are available:\n");
    printf("\n  - 'f' to flag a box");
    printf("\n  - 'c' to click a box");
    printf("\n  - 'u' to unflag a box");
    printf("\n  - 'e' to end the game\n");
    printf("\n  Note: For the first three commands, after pressing enter you will be asked to give coordinates.\n  Rows and columns must not be outside the range of [0, size - 1].\n  '*' represents unvisited areas.");
}

void showGrid(char grid[10][10], int size, int mines, int difficulty) {
	printf("\n\n  ===============================");
	printf("\n\n  Difficulty level: ");
	printf(difficulty == 1 ? "Easy" : difficulty == 2 ? "Medium" : "Hard");
	
    // print 'mines left' and grid
    printf("\n  Mines left: %d", mines);
    
	// printing column numbers
    printf("\n\n  -  "); // two lines gap and then some space for row numbers
    for (int i = 0; i < size; i++)
    {
        printf("%d  ", i);
    }
    printf("\n\n");

    // printing row numbers and then the rows
	for (int i = 0; i < size; i++)
    {
        printf("  %d  ", i);
        for (int j = 0; j < size; j++)
            if (grid[i][j] == 'm' || grid[i][j] == 'g') // if its a mine then print *, i.e. don't show it
                printf("%c  ", '*');
            else
                printf("%c  ", grid[i][j]);
        printf("\n\n");
    }
    
    
	printf("  ===============================\n");
}

int getNumOfMines(int difficulty, int size) {
	// just a general formula that gives 7/25 mines for easy, 21/49 for medium and 50/100 for hard.
	return (size * (size - (3 - difficulty))) / 2;
}

int main()
{
	/*
		'*': unvisited
		' ': empty
		'f': flagged, but not a mine
		'g': flagged, but it is a mine
		'm': unflagged mine
		for f, g and m, only f is visible on screen
	*/
	srand(time(0));
    int size, difficulty, mines;
    char grid[10][10];
    
    // three things that user inputs
    char cmd; // command
    int thisRow, thisCol;
    
	// used as boolean flags
    int win = 0, blast = 0, started = 0;
	
	// === difficulty level ===
	printf("  Difficulty levels: \n  1) Easy, 2) Medium, 3) Hard");
	printf("\n  Enter number of difficulty level: ");
	scanf("%d", &difficulty);
	
	if (difficulty < 1 || difficulty > 3) difficulty = 3; 
	
	if (difficulty == 1)
		size = 5;
	else if (difficulty == 2)
		size = 7;
	else {
		difficulty = 3; // in case user entered other than 1,2 and 3 as we need this value later to decide output based on difficulty
		size = 10;
	}
	
	mines = getNumOfMines(difficulty, size);
	// x---x difficulty level x---x
	
    // === setting grid ===
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            grid[i][j] = '*'; // initially star only

    do
    {
    	system("cls"); // clear the output in terminal
    	showInstructions();
    	
    	showGrid(grid, size, mines, difficulty);
        printf("\n  Enter command: ");
        scanf(" %c", &cmd);

        if (cmd == 'f' || cmd == 'c' || cmd == 'u') // if cmd is other than these then err
        {
        	printf("\n  Enter row number of the box: ");
        	scanf("%d", &thisRow);
        	
        	printf("\n  Enter column number of the box: ");
        	scanf("%d", &thisCol);
        	
            if (thisRow >= 0 && thisCol >= 0 && thisRow < size && thisCol < size) // row and col no. must be in range
            {
                // think more about what to do on each cmd and what if game hasn't started yet
                if (!started)
                {
                    // set up mines
                    for (int i = 0; i < mines; i++) {
                    	int randomX, randomY;
						do {
							// to prevent from the first box touched being the mine
                    		randomX = rand() % size;
							randomY = rand() % size;
						} while (randomX == thisRow && randomY == thisCol);
                    	grid[randomX][randomY] = 'm';
					}
					started = 1;
                }

                if (cmd == 'c') // user clicked on box
                {
                    if (grid[thisRow][thisCol] == 'm' || grid[thisRow][thisCol] == 'g') // box was a mine
                    {
                        // end game after printing grid with all mines
                        blast = 1;
                    } else
                    	grid[thisRow][thisCol] = ' ';
                } else if ('f') {
                	if (grid[thisRow][thisCol] == 'm') {
						grid[thisRow][thisCol] = 'g';
						mines--;
					}
					else
						grid[thisRow][thisCol] = 'f';
				} else {
					// u
					if (grid[thisRow][thisCol] == 'g' || grid[thisRow][thisCol] == 'f') {
						grid[thisRow][thisCol] = 'm';	
						mines++;
					}
				}
            }
            else
                printf("\n  Invalid row or column");
        }
        else if (cmd == 'e') {
        	printf("\n  Game ended");
        	break;
		}
        else
            printf("\n  Invalid command");

    } while (!win && !blast); // enter conditions

    if (win == 1)
        printf("\n  You won!");
    else if (blast == 1)
        printf("\n  You failed!");
    started = 0;
}
