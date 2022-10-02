#include <stdio.h>

// gcc <x>.c -o <x>
// ./<x>

void printInstructions()
{
    printf("\nFollowing commands are available:\n");
    printf("\n1) 'f <row> <column>' to flag a box e.g. f 1 2\n");
    printf("\n2) 'u <row> <column>' to unflag a box e.g. u 1 2\n");
    printf("\n3) 'c <row> <column>' to click a box e.g. c 1 2\n");
    printf("\nNote: Rows and columns must not be outside the range of [0, size - 1]\n");
    // using the format f<row><col> without spaces resulted in waiting for more input apparently.
}

int main()
{
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
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            grid[i][j] = '*'; // initially star only

    // print 'mines left' and grid
    printf("\n\nMines left: %d", mines);
    // printing column numbers
    printf("\n\n   "); // two lines gap and then some space for row numbers
    for (int i = 0; i < size; i++)
    {
        printf("%d  ", i);
    }
    printf("\n\n");

    // printing row numbers and then the rows
    for (int i = 0; i < size; i++)
    {
        printf("%d  ", i);
        for (int j = 0; j < size; j++)
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

        // user enters command in the form of e.g. f 2 4
        printf("\nEnter command: ");
        scanf("%c %d %d", &cmd, &thisRow, &thisCol);

        if (cmd == 'f' || cmd == 'c' || cmd == 'u') // if cmd is other than these then err
        {
            if (thisRow >= 0 && thisCol >= 0 && thisRow < size && thisCol < size) // row and col no. must be in range
            {
                // think more about what to do on each cmd and what if game hasn't started yet
                if (!started)
                {
                    // set up mines
                    grid[0][0] = 'm';
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
                printf("Invalid row or column");
        }
        else
            printf("Invalid command");

    } while (!win && !blast); // enter conditions

    if (win == 1)
        printf("You won");
    else if (blast == 1)
        printf("You failed");
}
