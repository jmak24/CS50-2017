#include <stdio.h>
#include <cs50.h>

int main(void)
{
    void print_multiple (int n, char c);
    int height;
 
    do {
         
        printf("Height: ");
        height = get_int();
         
    } while (height < 0 && height > 23);
     
     
    for (int i = 1; i <= height; i++)
    {
        int spaces = height - i;
        int hashes = i;
        int gap = 2;
         
        // print spaces for left pyramid
        print_multiple(spaces, ' ');
        // print hashes for left pyramid
        print_multiple(hashes, '#');
        // print gap
        print_multiple(gap, ' ');
        // print hashes for right pyramid
        print_multiple(hashes, '#');
        // print new line
        printf("\n");
    }
 
}

void print_multiple (int n, char c) 
{
    for (int i = 0; i < n; i++)
    {
        printf("%c", c);
    }
}