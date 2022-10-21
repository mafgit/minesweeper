#include <stdio.h>

// gcc <x>.c -o <x>
// ./<x>

void printInstructions()
{
    printf("\nFollowing commands are available:\n");
    printf("\n1) 'f' to flag a box\n");
    printf("\n2) 'c' to click a box\n");
    printf("\n3) 'u' to unflag a box\n");
    printf("\nNote: Rows and columns must not be outside the range of [0, size - 1]\n");
}

int main()
{
	srand(time(0));
    int size = 5;                        // ask for size e.g. 5 means 5x5
    int mines = (size * (size - 1)) / 2; // make levels of difficulty
    char grid[size][size];

    // three things that user inputs
    char cmd;
    int thisRow, thisCol;

    // used as boolean flags
    int win = 0, blast = 0, started = 0;

    printInstructions();

    // === setting grid ===
    int i, j;
    for (i = 0; i < size; i++)
        for (j = 0; j < size; j++)
            grid[i][j] = '*'; // initially star only

    // print 'mines left' and grid
    printf("\n\nMines Left: %d", mines);
    // printing column numbers
    printf("\n\n   "); // two lines gap and then some space for row numbers
    for (i = 0; i < size; i++)
    {
        printf("%d  ", i);
    }
    printf("\n\n");

    // printing row numbers and then the rows
	for (i = 0; i < size; i++)
    {
        printf("%d  ", i);
        for (j = 0; j < size; j++)
            if (grid[i][j] == 'm') // if its a mine then print *, i.e. don't show it
                printf("%c  ", '*');
            else
                printf("%c  ", grid[i][j]);
        printf("\n\n");
    }
    printf("\n");
    // x---x show grid x---x

    do
    {
        printf("\nEnter Command: ");
        scanf(" %c", &cmd);

        if (cmd == 'f' || cmd == 'c' || cmd == 'u') // if cmd is other than these then err
        {
        	printf("\nEnter Coordinates in the Form <row> <col>: ");
        	scanf("%d %d", &thisRow, &thisCol);
            if (thisRow >= 0 && thisCol >= 0 && thisRow < size && thisCol < size) // row and col no. must be in range
            {
                // think more about what to do on each cmd and what if game hasn't started yet
                if (!started)
                {
                    // set up mines
                    int noOfMines = rand() % (((size * size) / 2) - 4) + 5; // 5 to half of size * size e.g. 5 to 12 in a 5x5 grid
                    for (i = 0; i < noOfMines; i++) {
                    	int randX, randY;
						while (randX == thisRow && randY == thisCol) // to prevent from the first box touched being the mine
                    		randX = rand() % size, randY = rand() % size;
                    	grid[randX][randY] = 'm';
					}
					started = 1;
                }

                if (cmd == 'c') // user clicked on box
                {
                    if (grid[thisRow][thisCol] == 'm') // box was a mine
                    {
                        // end game after printing grid with all mines
                        blast = 1;
                    }
                }
            }
            else
                printf("\nInvalid Row or Column");
        }
        else if (cmd == 'e') {
        	printf("\nGame Ended");
        	break;
		}
        else
            printf("\nInvalid Command");

    } while (!win && !blast); // enter conditions

    if (win == 1)
        printf("You Won");
    else if (blast == 1)
        printf("You Failed");
}
