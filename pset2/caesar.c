#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#define UPPERCASE_ASCII 65
#define LOWERCASE_ASCII 97


int main(int argc, string argv[])
{
    string pt; 
    int pt_int;
    int key;
    bool is_lowercase;
    
  
    // ensure the user inputted exactly 2 arguments in the cmd-line
    // ./viginere + key 
    if (argc != 2) {
        // if not, return exit code 1
        printf("Usage: ./caesar key (2 arguments)\n");
        return 1;
    } 

    // prompt user for the plaintext to be enciphered
    printf("plaintext: ");
    pt = get_string();
    
    // convert the key input from a string to an integer 
    key = atoi(argv[1]);
    
    printf("ciphertext: ");
    // iterate through the plaintext characters
    for (int i = 0, n = strlen(pt); i < n; i++) {
        // check that the plaintext character is a letter, then encipher it
        if (isalpha(pt[i])) {
            // check if plaintext letter is upper or lowercase and flag it
            // change the letter to an integer using ASCII constants 
            if (islower(pt[i])) {
                pt_int = (int)pt[i] - LOWERCASE_ASCII;
                is_lowercase = true; 
            } else {
                pt_int = (int)pt[i] - UPPERCASE_ASCII;
                is_lowercase = false; 
            }
            
            // apply the key shift to the plaintext's current character
            // and revert it back to a letter 
            if (is_lowercase) {
                printf("%c", (pt_int + key) % 26 + LOWERCASE_ASCII);
            } else {
                printf("%c", (pt_int + key) % 26 + UPPERCASE_ASCII);
            }
         
        // else print the character as is
        } else {
            printf("%c", pt[i]);
        }
    }
    printf("\n");
}