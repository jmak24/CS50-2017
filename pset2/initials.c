#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(void)
{
    // prompt user for name input
    string s = get_string();
    
    // iterate through the given string
    for (int i = 0, n = strlen(s); i < n; i++) {
        
        // while there are spaces in the string, skip through them
        // or until the end of the string is reached
        while (s[i] == ' ' && s[i] != '\0') {
            i++;
        }
        
        // if a character exists 
        if (s[i] != ' ' && s[i] != '\0') {
            // print out the first character (the initials) in Uppercase 
            printf("%c", toupper(s[i]));
        }
        
        // skip the rest of the characters until there are spaces again
        // or until the end of the string is reached
        while (s[i] != ' ' && s[i] != '\0') {
            i++;
            
        }
    }
    printf("\n");
}