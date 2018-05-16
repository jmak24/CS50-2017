/**
 * fifteen.c
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE *file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }
    
    // greet user with instructions
    greet();
    
    // initialize the game board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = get_int();
        
        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
    }
    
    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered d*d - 1 through 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */
void init(void)
{
    // calculate max tile value based on board dimensions
    int max_tile = (d * d) - 1;
    int tile = max_tile;
    
    // iterate through every row of board array
    for (int r = 0; r < d; r++) {
        // iterate through every column of every row
        for (int c = 0; c < d; c++) {
            // assign current tile value to tile
            board[r][c] = tile;
            // decrement the current tile value
            tile--;
        }
    }
    // if # of tiles on board are odd, then swap tiles 1 and 2
    if ((max_tile) % 2 == 1) {
        // assign 2 to the 2nd last tile (where 1 is located)
        board[d-1][d-2] = 2;
        // assign 1 to the 3rd last tile (where 2 is located)
        board[d-1][d-3] = 1;
    }
}

/**
 * Prints the board in its current state.
 * note: "%2i" will print a blank space before single-digit numbers
 */
void draw(void)
{
    // print every tile from top-bottom, left-right
    printf("\n");
    for (int r = 0; r < d; r++) {
        for (int c = 0; c < d; c++) {
            // print an underscore if tile is 0 (blank space)
            if (board[r][c] == 0) {
                printf("  _ ");
            } else {
                printf(" %2i ", board[r][c]);
            }
        }
        printf("\n\n");
    }
    
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(tile)
{
    // initialize array variables to save tiles' x y locations
    int tile_input[2];
    int tile_empty[2];
    
    // intialize found boolean
    bool tile_found = false;
    
    // scan board for the tile input value and empty tile location
    for (int r = 0; r < d; r++) {
        for (int c = 0; c < d; c++) {
            // if tile is found, save x y location and flag as found 
            if (board[r][c] == tile && board[r][c] != 0) {
                tile_input[0] = r;
                tile_input[1] = c;
                tile_found = true;
            }
            // if empty tile is found, save x y location
            if (board[r][c] == 0) {
                tile_empty[0] = r;
                tile_empty[1] = c;
            }
        }
    }
    
    // if inputted tile is not found, return false
    if (!tile_found) {
        return false;
    }
    // if x axis matches, check if tile inputted is above/below empty tile
    if (tile_empty[0] == tile_input[0]) {
        if ((tile_empty[1]+1 == tile_input[1]) || (tile_empty[1]-1 == tile_input[1])) {
            // move is legal, make the swap
            board[tile_empty[0]][tile_empty[1]] = tile;
            board[tile_input[0]][tile_input[1]] = 0; 
            return true;
        }
    }    
    // if y axis matches, check if tile inputted is to left/right of empty tile
    if (tile_empty[1] == tile_input[1]) {
        if ((tile_empty[0]+1 == tile_input[0]) || (tile_empty[0]-1 == tile_input[0])) {
            // move is legal, make the swap
            board[tile_empty[0]][tile_empty[1]] = tile;
            board[tile_input[0]][tile_input[1]] = 0; 
            return true;
        }
    } 
    // return false if move is illegal 
    return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */ 
bool won(void)
{
    // initialize winning tile value starting at 1
    int winning_tile = 1;
    
    // iterate through game board and check for winning values
    for (int r = 0; r < d; r++) {
       for (int c = 0; c < d; c++) {
            // return false if current tile doesn't match winning tile value
            if (board[r][c] != winning_tile) {
                // end check if bottom right value is reached and it equals 0
                if ((r == d-1 && c == d-1) && (board[r][c] == 0))  {
                    break;
                } 
                return false;
            }
            // increment winning tile value 
            winning_tile++; 
       } 
    }
    // return true if all tiles have been verified
    return true;
}
