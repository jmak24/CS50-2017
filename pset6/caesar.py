import cs50
import sys

UPPERCASE_ASCII = 65
LOWERCASE_ASCII = 97


# ensure the user inputted exactly 2 arguments in the cmd-line
# ./viginere + key 
if len(sys.argv) != 2:
    # if not, return exit code 1
    print("Usage: ./caesar key (2 arguments)")
    exit (1)

# prompt user for the plaintext to be enciphered
print("plaintext: ", end="")
pt = cs50.get_string()

# convert the key input from a string to an integer 
key = int(sys.argv[1])

print("ciphertext: ", end="")
# iterate through the plaintext characters
for c in pt: 
    # check that the plaintext character is a letter, then encipher it
    if c.isalpha():
        # check if plaintext letter is upper or lowercase and flag it
        # change the letter to a Unicode integer using ASCII constants 
        if c.islower():
            pt_int = ord(c) - LOWERCASE_ASCII
            is_lowercase = True 
        else: 
            pt_int = ord(c) - UPPERCASE_ASCII
            is_lowercase = False 
        
        # apply the key shift to the plaintext's current character
        # and revert it back to a letter 
        if is_lowercase:
            print(chr((pt_int + key) % 26 + LOWERCASE_ASCII))
        else: 
            print(chr((pt_int + key) % 26 + UPPERCASE_ASCII))
        
    # else print the character as is
    else:
        print(c)
    