#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define UPPERCASE_ASCII 65
#define LOWERCASE_ASCII 97


int main(int argc, string argv[])
{
    string pt; 
    string key;
    int k_n;
    int key_pos;
    int key_shift;
    int pt_int; 
    bool is_lowercase;
    
  
    // ensure the user inputted exactly 2 arguments in the cmd-line
    // ./viginere + key 
    if (argc != 2) {
        // if not, return exit code 1
        printf("Usage: ./vigenere key (2 arguments)\n");
        return 1;
    } 

    // ensure the cmd-line argument contains letters only
    for (int i = 0, n = strlen(argv[1]); i < n; i++) {
        if (!isalpha(argv[1][i])) {
            // if not, return exit code 1
            printf("Usage: ./vigenere key (only letters)\n");
            return 1;
        }
    }
    
    // prompt user for the plaintext to be enciphered
    printf("plaintext: ");
    pt = get_string();
    
    // store some values into variables
    key = argv[1];
    k_n = strlen(argv[1]);
    
    printf("ciphertext: ");
    // iterate through the plaintext characters
    for (int p_i = 0, k_i = 0, p_n = strlen(pt); p_i < p_n; p_i++) {
        // if the plaintext character is a letter, we must encipher it
        if (isalpha(pt[p_i])) {
            // check if plaintext letter is upper or lowercase and flag it
            // change the letter to an integer using ASCII constants 
            if (islower(pt[p_i])) {
                pt_int = (int)pt[p_i] - LOWERCASE_ASCII;
                is_lowercase = true; 
            } else {
                pt_int = (int)pt[p_i] - UPPERCASE_ASCII;
                is_lowercase = false; 
            }
            
            // determine the key's letter position 
            key_pos = k_i % k_n; 
            
            // convert key to lowercase and get the key_shift value using ASCII constant
            key_shift = tolower((int)key[key_pos]) - LOWERCASE_ASCII;  
            
            // apply the key shift to the plaintext's current character
            // and revert it back to a letter 
            if (is_lowercase) {
                printf("%c", (pt_int + key_shift) % 26 + LOWERCASE_ASCII);
            } else {
                printf("%c", (pt_int + key_shift) % 26 + UPPERCASE_ASCII);
            }
        
        // move to the next key letter 
        k_i++;
         
        // else print the character as is
        } else {
            printf("%c", pt[p_i]);
        }
    }
    printf("\n");
}