/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
 
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "helpers.h"

// declare pointer for count array used in counting sort function
int *count;

// maximum range of hay 0 - 65536 (same value as MAX in find.c)
const int RANGE = 65536;

// declare binary function prototype
bool binary(int value, int values[], int start, int end); 

/**
 * Returns true if value is in array of n values, else false.
 * Binary Search Algorithm implemented 
 */
bool search(int value, int values[], int n)
{
    // initialize start & end boundaries of search 
    int start = 0;
    int end = n;
    
    // return false if 'n' is non-positive integer    
    if (n < 0) {
        return false;
    }
    
    // execute recursive binary search function 
    bool found = binary(value, values, start, end);
    return found; 
}

/**
 * Recursive search function for Binary algorithm
 */
bool binary(int value, int values[], int start, int end) 
{
    // initialize middle value based on start & end boundaries
    int mid = (start + end) / 2;
    
    // immediately return false if start exceeds end which indicates entire array has been searched
    if (start > end) {
        return false;
    // else if target value is greater than mid, search the right half
    } else if (values[mid] < value) {
        return binary(value, values, mid + 1, end);
    // else if target value is less than mid, search the left half
    } else if (values[mid] > value) {
        return binary(value, values, start, mid - 1);
    // else return true because target value is found at middle
    } else {
        return true;
    }
}

/**
 * Sorts array of n values.
 * Counting Sort Algorithm implemented 
 */
void sort(int values[], int n)
{
    // declare sorted array
    int sorted[n];
    
    // declare count array with max range of integers (+1 to account for element 0)
    // dynamically allocate space for array using malloc (allows enough memory to create array of size 65536)
    count = (int *) malloc(sizeof(int)*(RANGE + 1)); 
    
    // iterate through input array
    // increment count array for values that exist
    for (int i = 0; i < n; i++) {
        ++count[values[i]];
        printf("%i count: %i\n", values[i], count[values[i]]);
    }
    // change the count values into actual positions for the inputted values
    for (int i = 1; i <= RANGE; i++) {
        count[i] += count[i - 1];
    }
    
    // build the sorted array by assigning values from input array and using the count positions
    // decrement the count value after element is sorted
    for (int i = 0; i < n; i++) {
        sorted[count[values[i]]-1] = values[i];
        --count[values[i]];
    }
    // copy sorted array to original input array pointer 
    for (int i = 0; i < n; i++) {
        values[i] = sorted[i];
    }
    
    return;
}
