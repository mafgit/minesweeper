#include <stdio.h>
// following are to reduce warnings in compiler
#include <stdlib.h>
#include <time.h>

void showInstructions() {
    printf("\n  Following commands are available:\n");
    printf("\n  - 'f' to flag a box");
    printf("\n  - 'c' to click a box");
    printf("\n  - 'u' to unflag a box");
    printf("\n  - 'e' to end the game\n");
    printf("\n  Note: For the first three commands, after pressing enter you will be asked to give coordinates.\n  Rows and columns must not be outside the range of [0, size - 1].\n  '*' represents unvisited areas.");
}

void showGrid(char grid[10][10], int size, int mines, int difficulty, int hideMines) {
	printf("\n\n  ===============================");
	printf("\n\n  Difficulty level: ");
	printf(difficulty == 1 ? "Easy" : difficulty == 2 ? "Medium" : "Hard");
	
    // print 'mines left' and grid
    printf("\n  Mines left: %d", mines);
    
	// printing column numbers
    printf("\n\n     "); // two lines gap and then some space for row numbers
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
            if (hideMines == 1 && (grid[i][j] == 'm')) // if its an unflagged mine then print *, i.e. don't show it
                printf("%c  ", '*');
            else if (grid[i][j] == 'g') // if it is a flagged mine, the show f, not g
            	printf("%c  ", 'f');
            else
                printf("%c  ", grid[i][j]); // if it is * then show *
        printf("\n\n");
    }
    
    
	printf("  ===============================\n");
}

int getNumOfMines(int difficulty, int size) {
	// just a general formula that gives 7/25 mines for easy, 21/49 for medium and 50/100 for hard.
	return (size * (size - (3 - difficulty))) / 2;
}

int getSurroundingMines(char grid[10][10], int size, int thisRow, int thisCol) {
	int bottom, top, left, right, topLeft, topRight, bottomLeft, bottomRight;
	
	if (thisRow < size)
		bottom = grid[thisRow - 1][thisCol] == 'm' || grid[thisRow - 1][thisCol] == 'g';
	else bottom = 0;
	
	if (thisRow > 0)
		top = grid[thisRow + 1][thisCol] == 'm' || grid[thisRow + 1][thisCol] == 'g';
	else top = 0;
	
	if (thisCol > 0)
		left = grid[thisRow][thisCol - 1] == 'm' || grid[thisRow][thisCol - 1] == 'g';
	else left = 0;
	
	if (thisCol < size)
		right = grid[thisRow][thisCol + 1] == 'm' || grid[thisRow][thisCol + 1] == 'g';
	else right = 0;
	
	if (thisCol > 0 && thisRow > 0)
		topLeft = grid[thisRow - 1][thisCol - 1] == 'm' || grid[thisRow - 1][thisCol - 1] == 'g';
	else topLeft = 0;
	
	if (thisCol < size && thisRow > 0)
		topRight = grid[thisRow - 1][thisCol + 1] == 'm' || grid[thisRow - 1][thisCol + 1] == 'g';
	else topRight = 0;
	
	if (thisCol > 0 && thisRow < size)
		bottomLeft = grid[thisRow + 1][thisCol - 1] == 'm' || grid[thisRow + 1][thisCol - 1] == 'g';
	else bottomLeft = 0;
	
	if (thisCol < size && thisRow < size)
		bottomRight = grid[thisRow + 1][thisCol + 1] == 'm' || grid[thisRow + 1][thisCol + 1] == 'g';
	else bottomRight = 0;
	
	int surroundingMines = bottom + top + left + right + topLeft + topRight + bottomLeft + bottomRight;
	
	return surroundingMines;
}

int main() {
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
    	
    	showGrid(grid, size, mines, difficulty, 0);
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
                    } else {
                    	// check surrounding mines
                    	
                    	
                    	grid[thisRow][thisCol] = 48 + getSurroundingMines(grid, size, thisRow, thisCol); // 48 is the ascii for 0, and we need to convert 0 to a character as grid array is of characters
                	}
                } else if (cmd == 'f') {
                	if (grid[thisRow][thisCol] == 'm') {
						grid[thisRow][thisCol] = 'g';
						mines--;
					} else if (grid[thisRow][thisCol] == '*') {
						grid[thisRow][thisCol] = 'f';
						mines--;
					}
						
				} else if (cmd == 'u') {
					if (grid[thisRow][thisCol] == 'g') {
						grid[thisRow][thisCol] = 'm';	
						mines++;
					}	else if (grid[thisRow][thisCol] == 'f') {
						grid[thisRow][thisCol] = '*';
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
        printf("\n  You lost!");
    started = 0;
}
