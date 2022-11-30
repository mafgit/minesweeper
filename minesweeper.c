#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void showInstructions ();
void showGrid (int rowSize, int colSize, char[rowSize][colSize],int, int, int);
int getNumOfMines (int);
void initializeGrid (int, int colSize, char (*)[colSize]);
int getSurroundingMines (int rowSize, int colSize, char grid[rowSize][colSize], int, int);
int traverseOnFirstClick (int, int colSize, char (*gridRowPtr)[colSize], int, int);
void game (int, int);

int main() {
	int more;
	
	do {
		system("cls"); // to clear the screen
		
		game(1, 1);
//		for testing:
//		game(0, 0);
		
		printf("\n  Do you want to play again? Enter 1 for yes and 0 for no: ");
		scanf("%d", &more);
	} while (more != 0);
}

void game (int hideMines, int clearScreen) {
	/*
		'*': unvisited
		' ': visited and empty
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
    int win = 0, blast = 0, started = 0, firstClick = 1;
	
	showInstructions();
	
	// === difficulty ===
	printf(" Difficulty levels: \n\n  1) Easy, 2) Medium, 3) Hard");
	printf("\n\n  Enter number of difficulty level: ");
	scanf("%d", &difficulty);
	
	if (difficulty == 1) // 9x9
		rowSize = 9, colSize = 10; // +1 for delimiter
	else if (difficulty == 2) // 16x16
		rowSize = 16, colSize = 17;
	else {
		// in case user entered other than 1, 2 and 3
		difficulty = 3;
		// 16x30
		rowSize = 16;
		colSize = 31;
	}
	// x---x difficulty x---x
	
    char grid[rowSize][colSize];
	mines = getNumOfMines(difficulty);
	
    initializeGrid(rowSize, colSize, grid);
//	 could've done what's below but made this function to use pointers
//    for (int i = 0; i < rowSize; i++)
//        for (int j = 0; j < colSize; j++)
//            grid[i][j] = '*'; // initially star only

    do {
    	if (clearScreen) system("cls"); // clear the output in terminal
    	showInstructions();
    	
    	showGrid(rowSize, colSize, grid, mines, difficulty, hideMines);
        printf("\n  Enter command: ");
        scanf(" %c", &cmd);

        if (cmd == 'f' || cmd == 'c' || cmd == 'u')
        {
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
						} while (randomCol == thisCol && randomRow == thisRow);
                    	grid[randomRow][randomCol] = 'm';
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
                    	int surroundingMines = getSurroundingMines(rowSize, colSize, grid, thisRow, thisCol);
						if (surroundingMines == 0)
							grid[thisRow][thisCol] = ' ';
						else
                    		grid[thisRow][thisCol] = 48 + surroundingMines;
						// 48 is the ascii for 0, and we need to convert 0 to a character as grid array is of characters
						
						// if 1st click, traverse all four sides
						if (firstClick) {
							// right
							for (int c = thisCol + 1; c <= colSize; c++) {
								int mineFound = traverseOnFirstClick(rowSize, colSize, grid, thisRow, c);
								if (mineFound==1) break;
							}
							
							// left
							for (int c = thisCol - 1; c >= 0; c--) {
								int mineFound = traverseOnFirstClick(rowSize, colSize, grid, thisRow, c);
								if (mineFound==1) break;
							}
							
							// top
							for (int r = thisRow - 1; r >= 0; r--) {
								int mineFound = traverseOnFirstClick(rowSize, colSize, grid, r, thisCol);
								if (mineFound==1) break;
							}
							
							// bottom
							for (int r = thisRow + 1; r <= rowSize; r++) {
								int mineFound = traverseOnFirstClick(rowSize, colSize, grid, r, thisCol);
								if (mineFound==1) break;
							}

							firstClick = 0;
						}
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
        	printf("\n  Game ended\n\n");
        	break;
		}
        else
            printf("\n  Invalid command\n\n");

    } while (!win && !blast); // enter conditions

    if (win == 1)
        printf("\n  YOU WON!\n\n");
    else if (blast == 1)
        printf("\n  YOU LOST!\n\n");
        
    showGrid(rowSize, colSize, grid, mines, difficulty, hideMines);
}

void showInstructions() {
    printf("\n A box in the grid may be one of the following:\n");
    printf("\n  '*': an unvisited box\n");
    printf("\n  ' ': a visited and empty box\n");
    printf("\n  'f': a box that is flagged by the user\n");
    printf("\n  'm': a box that has a mine, and the user hasn't flagged it yet (but the user sees '*' instead)\n");
    printf("\n  'g': a box that has a mine, and is flagged by the user (but the user sees 'f' instead)\n");
    printf("\n Following commands are available:\n");
    printf("\n  'c' to click a box to visit it\n");
    printf("\n  'f' to flag a box\n");
    printf("\n  'u' to unflag a box\n");
    printf("\n  'e' to end the game\n");
    printf("\n  Note: After the first three commands, you are asked to give coordinates.");
    printf(" Rows and columns must not be outside the range of [0, size - 1].\n\n");
}

void showGrid (int size1, int size2, char grid[size1][size2],int mines, int difficulty, int hideMines) {
	printf("  ===============================");
	
	if (hideMines == 1) {
		printf("\n\n  Difficulty level: ");
		printf(difficulty == 1 ? "Easy" : difficulty == 2 ? "Medium" : "Hard");
	    printf("\n  Mines left: %d", mines);
	}
    
	// printing column numbers
    printf("\n\n      "); // two lines gap and then some space for row numbers
    for (int i = 0; i < size2; i++)
    {
        printf("%d ", i);
    	if (i < 10) printf(" ");
    }
    printf("\n\n");

    // printing row numbers and then the rows
	for (int i = 0; i < size1; i++)
    {
    	if (i < 10) printf(" ");
        printf("  %d  ", i);
        for (int j = 0; j < size2; j++)
        	if (hideMines) {
	            if (grid[i][j] == 'm') // if its an unflagged mine then print *, i.e. don't show it
	                printf("%c  ", '*');
	            else if (grid[i][j] == 'g') // if it is a flagged mine, then show f, not g
	            	printf("%c  ", 'f');
	            else
	                printf("%c  ", grid[i][j]); // if it is * then show *, ' ', or number then show 
	        } else {
	            if (grid[i][j] == 'm')
	                printf("%c  ", 'm');
	            else if (grid[i][j] == 'g')
	            	printf("%c  ", 'g');
	            else
	                printf("%c  ", grid[i][j]);
			}
//			if ()
	        printf("\n\n");
	    }
    
	printf("  ===============================\n");
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

int getSurroundingMines (int size1, int size2, char grid[size1][size2], int thisRow, int thisCol) {
	int bottom, top, left, right, topLeft, topRight, bottomLeft, bottomRight;
	
	if (thisRow < size1)
		bottom = grid[thisRow - 1][thisCol] == 'm' || grid[thisRow - 1][thisCol] == 'g';
	else bottom = 0;
	
	if (thisRow > 0)
		top = grid[thisRow + 1][thisCol] == 'm' || grid[thisRow + 1][thisCol] == 'g';
	else top = 0;
	
	if (thisCol > 0)
		left = grid[thisRow][thisCol - 1] == 'm' || grid[thisRow][thisCol - 1] == 'g';
	else left = 0;
	
	if (thisCol < size2)
		right = grid[thisRow][thisCol + 1] == 'm' || grid[thisRow][thisCol + 1] == 'g';
	else right = 0;
	
	if (thisCol > 0 && thisRow > 0)
		topLeft = grid[thisRow - 1][thisCol - 1] == 'm' || grid[thisRow - 1][thisCol - 1] == 'g';
	else topLeft = 0;
	
	if (thisCol < size2 && thisRow > 0)
		topRight = grid[thisRow - 1][thisCol + 1] == 'm' || grid[thisRow - 1][thisCol + 1] == 'g';
	else topRight = 0;
	
	if (thisCol > 0 && thisRow < size1)
		bottomLeft = grid[thisRow + 1][thisCol - 1] == 'm' || grid[thisRow + 1][thisCol - 1] == 'g';
	else bottomLeft = 0;
	
	if (thisCol < size2 && thisRow < size1)
		bottomRight = grid[thisRow + 1][thisCol + 1] == 'm' || grid[thisRow + 1][thisCol + 1] == 'g';
	else bottomRight = 0;
	
	int surroundingMines = bottom + top + left + right + topLeft + topRight + bottomLeft + bottomRight;
	
	return surroundingMines;
}

int traverseOnFirstClick (int rowSize, int colSize, char (*gridRowPtr)[colSize], int r, int c) {
	// if mine found or surrounding mines found, then there should be a break in for loop of game function
	// returning 1 represents mine found
	int surroundingMines;
	if (gridRowPtr[r][c] == 'm' || gridRowPtr[r][c] == 'g')
		return 1;
	else {
		surroundingMines = getSurroundingMines(rowSize, colSize, gridRowPtr, r, c);
		if (surroundingMines == 0) {
			gridRowPtr[r][c] = ' ';
			return 0;
		}
		else {
    		gridRowPtr[r][c] = 48 + surroundingMines;
			return 1;	
		}
	}
}
