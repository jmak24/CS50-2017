/**
 * Declares a dictionary's functionality.
 */

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdbool.h>

// maximum length for a word
// (e.g., pneumonoultramicroscopicsilicovolcanoconiosis)
#define LENGTH 45

// preprocesser that converts the char (a-z) into an index (0-25)
// if ASCII value is 39, then it is an aspotrophe so assign it an index value of 26
#define CHAR_TO_INDEX(c) (c == 39) ? 26 : ((int)c - (int)'a');

/**
 * Structure of each node containing an array of 27 pointers (alphabets + apostrophe)
 * and a boolean to indicate if a valid word exists
 */
typedef struct node
{
    bool is_word;
    struct node *children[27];
}
node;

// create root node of trie structure 
node *root;

// create the cursor which will point from node-to-node in the trie 
// (used in load and check)
node *crsr;

// store number of dictionaries words during load function
unsigned int dictSize; 

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word);

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary);

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void);

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void);

/**
 * Initializes all elements within a children array of a newly created node
 */
void initChild(node *ptr);

/**
 * Recursive function which travels to lowest node and frees up each children element as it traverses back up
 */
bool freeTrie(node *ptr);


#endif // DICTIONARY_H
