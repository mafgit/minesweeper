#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

void showInstructions ();
void showGrid (int rowSize, int colSize, char[rowSize][colSize],int, int, int);
int getNumOfMines (int);
void initializeGrid (int, int colSize, char (*)[colSize]);
int getSurroundingMines (int rowSize, int colSize, char grid[rowSize][colSize], int, int);
void flagBox (int rowSize, int colSize, char grid[rowSize][colSize], int, int, int*);
void unflagBox (int rowSize, int colSize, char grid[rowSize][colSize], int, int, int*);
void expand (int rowSize, int colSize, char grid[rowSize][colSize], int, int, int*);
void game (int);

int main() {
	int more;
	
	do {
		system("cls"); // to clear the screen
		
//		game(1); // dev mode (for testing)
		game(0); // normal mode
		
		printf("\n  Do you want to play again? Enter 1 for yes and 0 for no: ");
		scanf("%d", &more);
	} while (more != 0);
}

void game (int devMode) { // devMode means don't hide mines & don't clear screen
	/*
		'*': unvisited
		' ': visited and has no surrounding mines
		'f': flagged, but not a mine
		'g': flagged, but it is a mine (but user sees only 'f')
		'm': unflagged mine (but user sees '*') 
	*/
	srand(time(0));
    int rowSize, colSize, difficulty, mines;
    
    // three things that user inputs
    char cmd; // command
    int thisRow, thisCol;
    
	// used as boolean flags
    int win = 0, blast = 0, started = 0;
	
	if (!devMode) {
		showInstructions();
		
		// === difficulty ===
		printf("  Difficulty levels: \n\n  1) Easy  2) Medium  3) Hard");
		printf("\n\n  Enter number of difficulty level: ");
		scanf("%d", &difficulty);
	
		if (difficulty == 1) // 9x9
			rowSize = 9, colSize = 9;
		else if (difficulty == 2) // 16x16
			rowSize = 16, colSize = 16;
		else { // 16x30
			difficulty = 3;
			rowSize = 16;
			colSize = 30;
		}
		
		mines = getNumOfMines(difficulty);
		// x---x difficulty x---x
	} else {
		// set values manually for testing
		rowSize = 3;
		colSize = 3;
		mines = 2;
	}
	
    char grid[rowSize][colSize];
	
	int visited = 0;
	int visitedToWin = (rowSize * colSize) - mines; // total boxes - mines
	
    initializeGrid(rowSize, colSize, grid);
//	 could've done what's below but made this function to use pointers
//    for (int i = 0; i < rowSize; i++)
//        for (int j = 0; j < colSize; j++)
//            grid[i][j] = '*'; // initially star only

    do {
    	if (!devMode) {
			system("cls"); // clear the output in terminal
    		showInstructions();
    	}
    	
    	showGrid(rowSize, colSize, grid, mines, difficulty, devMode);

        cmd = ' ';
        while (1) {
			// for cmd validation
			printf("\n  Enter command: ");
	        scanf(" %c", &cmd);
        	
        	if (cmd == 'e' || cmd == 'u' || cmd == 'f' || cmd == 'c')
        		break;
        	
            printf("\n  Invalid command\n\n");
		}
        
        if (cmd == 'f' || cmd == 'c' || cmd == 'u') {
        	printf("\n  Enter row number of the box: ");
        	scanf("%d", &thisRow);
        	
        	printf("\n  Enter column number of the box: ");
        	scanf("%d", &thisCol);
        	
            if (thisRow >= 0 && thisCol >= 0 && thisRow < rowSize && thisCol < colSize) // row and col no. must be in range
            {
                if (!started)
                {
                    // set up mines
                    for (int i = 0; i < mines; i++) {
                    	int randomRow, randomCol;
						do {
							// to prevent from the first box touched being the mine
							randomRow = rand() % rowSize;
                    		randomCol = rand() % colSize;
						} while ((randomCol == thisCol && randomRow == thisRow) || (grid[thisRow][thisCol] == 'm'));
                    	grid[randomRow][randomCol] = 'm';
					}
					started = 1;
                }

                if (cmd == 'c') {
                    if (grid[thisRow][thisCol] == 'm' || grid[thisRow][thisCol] == 'g') // box was a mine
                    {
                        // end game after printing grid with all mines
                        blast = 1;
                        break;
                    } else {
                    	visited++;
                    	// check surrounding mines
                    	int surroundingMines = getSurroundingMines(rowSize, colSize, grid, thisRow, thisCol);
                    	grid[thisRow][thisCol] = 48 + surroundingMines;
						// 48 is the ascii for 0, and we need to convert 0 to a character as grid array is of characters
						
						// TODO: delimiter, visited
						
						if (surroundingMines == 0)
							expand(rowSize, colSize, grid, thisRow, thisCol, &visited);
						if (devMode) printf("\n  Visited: %d\n\n", visited);
	
						if (visited == visitedToWin) {
							win = 1;
							break;
						}
						
                	}
                } else if (cmd == 'f')
                	flagBox(rowSize, colSize, grid, thisRow, thisCol, &mines);
				else if (cmd == 'u')
					unflagBox(rowSize, colSize, grid, thisRow, thisCol, &mines);
					
			} else
                printf("\n  Invalid row or column"); 
			
		} else if (cmd == 'e') {
        	printf("\n  Game ended\n\n");
        	break;
		}
    } while (!win && !blast); // enter conditions

    if (win == 1)
        printf("\n  YOU WON!\n\n");
    else if (blast == 1)
        printf("\n  YOU LOST!\n\n");
    
    if (win || blast) // only show blast again when win or lose, not on entering 'e'
    	showGrid(rowSize, colSize, grid, mines, difficulty, 1);
}

void showInstructions() {
    printf("\n A box in the grid may be one of the following:\n");
    printf("\n  '*': an unvisited box\n");
    printf("\n  ' ': a visited box that has no surrounding mines\n");
    printf("\n  '5': a visited box that has 5 surrounding mines\n");
    printf("\n  'f': a box that is flagged by the user\n");
    printf("\n  'm': a box that has a mine, and the user hasn't flagged it yet (but the user sees '*' instead)\n");
    printf("\n  'g': a box that has a mine, and is flagged by the user (but the user sees 'f' instead)\n");
    printf("\n Following commands are available:\n");
    printf("\n  'c' to click a box to visit it\n");
    printf("\n  'f' to flag a box\n");
    printf("\n  'u' to unflag a box\n");
    printf("\n  'e' to end the game\n");
    printf("\n  Note: After the first three commands, you are asked to give coordinates.\n\n");
}

void showGrid (int rowSize, int colSize, char grid[rowSize][colSize],int mines, int difficulty, int devMode) {
	printf("  ==============================================");
	
	if (!devMode) {
		printf("\n\n  Difficulty level: ");
		printf(difficulty == 1 ? "Easy" : difficulty == 2 ? "Medium" : "Hard");
	} else {
		printf("\n\n  Dev Mode");
	}
	
	printf("\n  Mines left: %d", mines);
    
	// printing column numbers
    printf("\n\n        "); // two lines gap and then some space for row numbers
    for (int i = 0; i < colSize; i++)
    {
        printf("%d  ", i);
    	if (i < 10) printf(" ");
    }
    printf("\n\n");

    // printing row numbers and then the rows
	for (int i = 0; i < rowSize; i++)
    {
    	if (i < 10) printf(" ");
        printf("  %d  ", i);
        for (int j = 0; j < colSize; j++)
        	if (!devMode) {
	            if (grid[i][j] == 'm') // if its an unflagged mine then print *, i.e. don't show it
	                printf("| %c ", '*');
	            else if (grid[i][j] == 'g') // if it is a flagged mine, then show f, not g
	            	printf("| %c ", 'f');
	            else if (grid[i][j] == '0') {
	            	printf("| %c ", ' ');
				} else
	                printf("| %c ", grid[i][j]); // if it is * then show *, ' ', or number then show 
	        } else {
	        	if (grid[i][j] == '0')
	            	printf("| %c ", ' ');
	        	else
					printf("| %c ", grid[i][j]);
			}

	        printf("|\n\n");
	    }
    
	printf("  ==============================================\n");
}

int getNumOfMines (int difficulty) {
	return (difficulty == 1) ? 10 : (difficulty == 2) ? 40 : 99;
}

void initializeGrid (int rowSize, int colSize, char (*gridRowPtr)[colSize]) {
	// gridRowPtr is declared as a pointer to an array of size colSize.
	for (int i = 0; i < rowSize; i++) {
		for (int j = 0; j < colSize; j++) {
			*(*(gridRowPtr + i) + j) = '*'; // all stars initially
		}
	}
}

int getSurroundingMines (int rowSize, int colSize, char grid[rowSize][colSize], int thisRow, int thisCol) {
	int bottom, top, left, right, topLeft, topRight, bottomLeft, bottomRight;

	if (thisRow < rowSize - 1)
		bottom = grid[thisRow + 1][thisCol] == 'm' || grid[thisRow + 1][thisCol] == 'g';
	else bottom = 0;
	
	if (thisRow > 0)
		top = grid[thisRow - 1][thisCol] == 'm' || grid[thisRow - 1][thisCol] == 'g';
	else top = 0;
	
	if (thisCol > 0)
		left = grid[thisRow][thisCol - 1] == 'm' || grid[thisRow][thisCol - 1] == 'g';
	else left = 0;
	
	if (thisCol < colSize - 1)
		right = grid[thisRow][thisCol + 1] == 'm' || grid[thisRow][thisCol + 1] == 'g';
	else right = 0;
	
	if (thisCol > 0 && thisRow > 0)
		topLeft = grid[thisRow - 1][thisCol - 1] == 'm' || grid[thisRow - 1][thisCol - 1] == 'g';
	else topLeft = 0;
	
	if (thisCol < colSize - 1 && thisRow > 0)
		topRight = grid[thisRow - 1][thisCol + 1] == 'm' || grid[thisRow - 1][thisCol + 1] == 'g';
	else topRight = 0;
	
	if (thisCol > 0 && thisRow < rowSize - 1)
		bottomLeft = grid[thisRow + 1][thisCol - 1] == 'm' || grid[thisRow + 1][thisCol - 1] == 'g';
	else bottomLeft = 0;
	
	if (thisCol < colSize - 1 && thisRow < rowSize - 1)
		bottomRight = grid[thisRow + 1][thisCol + 1] == 'm' || grid[thisRow + 1][thisCol + 1] == 'g';
	else bottomRight = 0;
	
	int surroundingMines = bottom + top + left + right + topLeft + topRight + bottomLeft + bottomRight;
	
	return surroundingMines;
}

void flagBox (int rowSize, int colSize, char grid[rowSize][colSize], int thisRow, int thisCol, int *mines) {
	// arrays are passed by reference
	if (grid[thisRow][thisCol] == 'm') {
		grid[thisRow][thisCol] = 'g';
		(*mines)--;
	} else if (grid[thisRow][thisCol] == '*') {
		grid[thisRow][thisCol] = 'f';
		(*mines)--;
	}
}

void unflagBox (int rowSize, int colSize, char grid[rowSize][colSize], int thisRow, int thisCol, int *mines) {
	if (grid[thisRow][thisCol] == 'g') {
		grid[thisRow][thisCol] = 'm';	
		(*mines)++;
	}	else if (grid[thisRow][thisCol] == 'f') {
		grid[thisRow][thisCol] = '*';
		(*mines)++;
	}
}

void expand (int rowSize, int colSize, char grid[rowSize][colSize], int thisRow, int thisCol, int* visited) {
	int surroundingMines;
	
	// bottom
	if (!isdigit(grid[thisRow + 1][thisCol])) // if it has not already been checked for surrounding mines
		// otherwise it will be a huge load on computer
		if (thisRow < rowSize - 1) {
			surroundingMines = getSurroundingMines(rowSize, colSize, grid, thisRow + 1, thisCol);
			grid[thisRow + 1][thisCol] = 48 + surroundingMines;
			(*visited)++;
			if (surroundingMines == 0)
				expand(rowSize, colSize, grid, thisRow + 1, thisCol, visited);
		}
	
	// top
	if (!isdigit(grid[thisRow - 1][thisCol]))
		if (thisRow > 0) {
			surroundingMines = getSurroundingMines(rowSize, colSize, grid, thisRow - 1, thisCol);
			grid[thisRow - 1][thisCol] = 48 + surroundingMines;
			(*visited)++;
			if (surroundingMines == 0)
				expand(rowSize, colSize, grid, thisRow - 1, thisCol, visited);
		}

	// right
	if (!isdigit(grid[thisRow][thisCol + 1]))
		if (thisCol < colSize - 1) {
			surroundingMines = getSurroundingMines(rowSize, colSize, grid, thisRow, thisCol + 1);
			grid[thisRow][thisCol + 1] = 48 + surroundingMines;
			(*visited)++;
			if (surroundingMines == 0)
				expand(rowSize, colSize, grid, thisRow, thisCol + 1, visited);
		}
		
	// left
	if (!isdigit(grid[thisRow][thisCol - 1]))
		if (thisCol > 0) {
			surroundingMines = getSurroundingMines(rowSize, colSize, grid, thisRow, thisCol - 1);
			grid[thisRow][thisCol - 1] = 48 + surroundingMines;
			(*visited)++;
			if (surroundingMines == 0)
				expand(rowSize, colSize, grid, thisRow, thisCol - 1, visited);
		}

	if (!isdigit(grid[thisRow + 1][thisCol + 1]))
		if (thisRow < rowSize - 1 && thisCol < colSize - 1) {
			surroundingMines = getSurroundingMines(rowSize, colSize, grid, thisRow + 1, thisCol + 1);
			grid[thisRow + 1][thisCol + 1] = 48 + surroundingMines;
			(*visited)++;
			if (surroundingMines == 0)
				expand(rowSize, colSize, grid, thisRow + 1, thisCol + 1, visited);
		}
	
	if (!isdigit(grid[thisRow + 1][thisCol - 1]))
		if (thisRow < rowSize - 1 && thisCol > 0) {
			surroundingMines = getSurroundingMines(rowSize, colSize, grid, thisRow + 1, thisCol - 1);
			grid[thisRow + 1][thisCol - 1] = 48 + surroundingMines;
			(*visited)++;
			if (surroundingMines == 0)
				expand(rowSize, colSize, grid, thisRow + 1, thisCol - 1, visited);
		}
	
	if (!isdigit(grid[thisRow - 1][thisCol + 1]))
		if (thisRow > 0 && thisCol < colSize - 1) {
			surroundingMines = getSurroundingMines(rowSize, colSize, grid, thisRow - 1, thisCol + 1);
			grid[thisRow - 1][thisCol + 1] = 48 + surroundingMines;
			(*visited)++;
			if (surroundingMines == 0)
				expand(rowSize, colSize, grid, thisRow - 1, thisCol + 1, visited);
		}
	
	if (!isdigit(grid[thisRow - 1][thisCol - 1]))
		if (thisRow > 0 && thisCol > 0) {
			surroundingMines = getSurroundingMines(rowSize, colSize, grid, thisRow - 1, thisCol - 1);
			grid[thisRow - 1][thisCol - 1] = 48 + surroundingMines;
			(*visited)++;
			if (surroundingMines == 0)
				expand(rowSize, colSize, grid, thisRow - 1, thisCol - 1, visited);
		}
}
		
