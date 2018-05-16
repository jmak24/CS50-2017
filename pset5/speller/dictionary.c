/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "dictionary.h"

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    // point cursor back to root node
    crsr = root;
    
    // for each character of the text word
    for (int i = 0, sl = strlen(word); i < sl; i++)
    {
        // convert char to lowercase and then to integer index value 
        int index = CHAR_TO_INDEX(tolower(word[i]));
        
        // if corresponding index exists within children array
        if (crsr->children[index] != NULL)
        {
            // point cursor to the next node 
            crsr = crsr->children[index];
        }
        // else if it does not exist
        else
        {
            // return false because the word is not in the dictionary
            return false;
        }
        
        // if last letter has been reached, check if is_word boolean is true
        if ((i == sl - 1) && (crsr->is_word == true))
        {
            // return true because it is a vaild word in the dictionary 
            return true;
        }
    }
    return false;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    // open dictionary text file and create a file pointer
    FILE *fp = fopen(dictionary, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", dictionary);
        return 1;
    }
    
    // allocate dynamic memory for the root node 
    root = malloc(sizeof(node));
    
    // initialize the elements within root children array
    initChild(root);
    
    // create buffer array to store dictionary word (45 + 2)
    char word[LENGTH + 2];
    
    // scan each dictionary word and store into word buffer
    while (fgets(word, LENGTH + 2, fp) != NULL) 
    {
        // keep count of words in dictionary
        dictSize++;
        
        // point cursor back to root node
        crsr = root;

        // iterate through each character of word (exclude '\0')
        for (int i = 0, sl = strlen(word); i < sl - 1; i++)
        {   
            // convert the current character into integer index value 
            int index = CHAR_TO_INDEX(word[i]);
            
            // if value at children[index] is null
            if (crsr->children[index] == NULL)
            {
                // malloc a new node 
                node *next = malloc(sizeof(node));
                
                // initialize the elements within next children array
                initChild(next);
                            
                // connect children[index] to next node 
                crsr->children[index] = next;
                
                // point cursor to next node
                crsr = crsr->children[index];
            } 
            // else if node already exists at children[index]
            else 
            {
               // point cursor to next node
               crsr = crsr->children[index];
            } 
            // if last letter has been reached (exclude '\0')
            if (i == sl - 2)
            {
                // set is_word boolean to true
                crsr->is_word = true;
            }
        } 
    }
    // close the dictionary text file
    fclose(fp);
    
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    // return dictionary size variable used in load 
    return dictSize;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    // free the entire trie structure
    return freeTrie(root);
    
}

/**
 * Recursive function which travels to lowest node and frees up each children element as it traverses back up
 */
bool freeTrie(node *ptr)
{
    // iterate over each element within children array
    for (int i = 0; i <= 26; i++) 
    {
        // if a node exists within children[i] 
        if (ptr->children[i] != NULL)
        {
            // recursively move down into that child node
            freeTrie(ptr->children[i]);
        }
    }
    // when bottom is reached, free each node while moving up in the trie
    free(ptr);
    
    return true;
}

/**
 * Initializes all elements of a children array and is_word for a newly created node
 */
void initChild(node *ptr)
{
    // for each element within the children array
    for (int i = 0; i <= 26; i++)
    {   
        // set it to NULL
        ptr->children[i] = NULL;
    }
    // set is_word to default false
    ptr->is_word = false;
}