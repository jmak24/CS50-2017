#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t  BYTE;

int main(int argc, char *argv[]) 
{
    // ensure proper usage 
    if (argc != 2) {
        fprintf(stderr, "Usage: ./recover file");
        return 1;
    }
    
    // open input file 
    FILE *inptr = fopen(argv[1], "r"); 
    if (inptr == NULL) {
        fprintf(stderr, "Could not open %s.\n", argv[1]);
        return 2;
    }
    
    // create buffer to store card data
    BYTE buffer[512];
    
    // save the number of blocks/bytes being stored into the buffer from fread
    int size = 0;
    
    // store filename of jpeg
    char filename[8]; 
    
    // keep count of JPEG file being created
    int jpegCounter = 0; 
    
    // declare output File pointer
    FILE *outptr;

    do {
        // keep reading 512 blocks of bytes into the buffer until the end of card file 
        // fread returns the number of blocks of bytes it has read into the buffer
        size = fread(&buffer, sizeof(BYTE), 512, inptr); 
        
        // checks if buffer is a of type JPEG (indicating start of a new JPEG)
        // first 3 bytes: 0xff 0xd8 0xff + 4th byte: 0xe0 - 0xef
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0) {
            // check if jpeg file has already been opened
            if (jpegCounter != 0) {
                // if a jpeg file is open, close it
                fclose(outptr);
            }
        
            // assign file name of format jpeg.### to variable filename 
            sprintf(filename, "%03i.jpg", jpegCounter);
            
            // open new jpeg file
            outptr = fopen(filename, "w");
            
            // increment jpegCounter
            jpegCounter++;
        }
        
        // check again if jpeg file has been opened and that buffer size is valid
        if (jpegCounter != 0 && size == 512) {
            // if a jpeg file is open, write the buffer into it
            fwrite(&buffer, sizeof(BYTE), 512, outptr);
        }
    
    // end of file has been reached when buffer size does not equal 512
    } while (size == 512);
}