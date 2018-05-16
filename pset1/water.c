#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int mins;
    
    do {
        
        printf("What is the length of your shower? ");
        mins = get_int();
    
    } while (mins <= 0); 
    
    printf("Minutes: %i\n", mins);
    printf("Bottles: %i\n", mins * 12);
}