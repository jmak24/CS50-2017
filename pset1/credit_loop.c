#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void) 
{
    long long digit_shifter;
    int even_digit_sum;
    int odd_digit_sum;
    int total_sum;
    int last_digit;
    bool first_digit_is_even = false; 
    int even_final_digit; 
    int odd_final_digit;
    int card_length;
    int first_2_digits;
    bool valid_card = false;

    do
    {   
        // Reset variable values
        int i = 0;
        even_digit_sum = 0;
        odd_digit_sum = 0;
        total_sum = 0;
        digit_shifter = 10;
        
        // Prompt user for Credit card Number
        printf("Number:\n");
        long long credit = get_long_long(); 
        
        while (credit/digit_shifter > 9) {
            // A multipler of 10 that shifts the position of the digits 
            digit_shifter = pow(10, i);
            
            // Get the last digit (which is the remainder of credit/10 after shifting the value)m
            last_digit = (credit/digit_shifter) % 10;
            
            // Check the placement of the digit and perform task accordingly
            // If placement of digit is Even
            if (i % 2 != 0) {
                // We must now check if (last_digit * 2) yeilds double digits
                // If it is double digits, retrieve the sum of those 2 digits (The first digit is always 1)  
                if (last_digit * 2 > 9) {
                    int sum_dd = ((last_digit * 2) % 10) + 1; 
                    
                    // Add the sum of the double digits to the total even_digit_sum
                    even_digit_sum += sum_dd;
                } else {
                    // Add the last_digit to the total even_digit_sum
                    even_digit_sum += last_digit * 2;
                }
                
                // Set first digit to be even as true
                first_digit_is_even = true;
                // Keep storing most recent even last digit
                even_final_digit = last_digit;
                
            // Else if placement of digit is Odd
            } else {
                // Add the last_digit to the total even_digit_sum
                odd_digit_sum += last_digit; 
                
                // Set final digit to be odd as false 
                first_digit_is_even = false;
                // Keep storing most recent odd last digit
                odd_final_digit = last_digit;
            }
            // Iterate the digit_shifter
            i += 1;
        }
        
        // Get the first 2 digits by combining the final odd and even digits 
        if (first_digit_is_even) {
            first_2_digits = (even_final_digit * 10) + odd_final_digit; 
        } else {
            first_2_digits = (odd_final_digit * 10) + even_final_digit;
        }
        
        // Store the card legnth into a variable
        card_length = i;
        
        // Get the total sum
        total_sum = even_digit_sum + odd_digit_sum;
        
        // Validate Check Sum 
        if ((total_sum != 0) && (total_sum % 10 == 0)) {
            // Validate Credit card vendor criteria (First 2 digits and Card Length)
            if ((first_2_digits == 34 || first_2_digits == 37) && (card_length == 15)) {
                printf("AMEX\n");
                valid_card = true;
            } else if ((first_2_digits >= 51 && first_2_digits <= 55) && (card_length == 16)) {
                printf("MASTERCARD\n");
                valid_card = true;
            } else if ((first_2_digits >= 40 && first_2_digits <= 49) && (card_length == 13 || card_length == 16)) {
                printf("VISA\n");
                valid_card = true;
            }
        } else {
            printf("INVALID\n");
        }
    
    // Keep prompting user until they provide valid Credit card     
    } while (!valid_card);
} 
