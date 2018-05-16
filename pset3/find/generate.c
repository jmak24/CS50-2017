/**
 * generate.c
 *
 * Generates pseudorandom numbers in [0,MAX), one per line.
 *
 * Usage: generate n [s]
 *
 * where n is number of pseudorandom numbers to print
 * and s is an optional seed
 */
 
#define _XOPEN_SOURCE

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// upper limit on range of integers that can be generated *65536
#define LIMIT 65536

int main(int argc, string argv[])
{
    // check that user passed through either 2 or 3 arguments in cmd-line
    if (argc != 2 && argc != 3)
    {
        printf("Usage: ./generate n [s]\n");
        return 1;
    }

    // convert 'n' argument passed through as a string into an integer
    int n = atoi(argv[1]);

    // if a 3rd argument is passed through (seed option)
    if (argc == 3)
    {
        // set the seed as that value
        srand48((long) atoi(argv[2]));
    }
    else
    {
        // else set it to NULL
        srand48((long) time(NULL));
    }

    // generate and print out 'n' amount of numbers indidcated by user
    for (int i = 0; i < n; i++)
    {
        printf("%i\n", (int) (drand48() * LIMIT));
    }

    // success
    return 0;
}
