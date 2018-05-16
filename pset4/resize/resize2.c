/**
 * Copies a BMP piece by piece, just because.
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./copy resize infile outfile\n");
        return 1;
    }
    
    /* check if resize is an integer
    if (!isnumber(argv[1])) 
    {
        fprintf(stderr, "Resize value must be positive integer value\n");
    }*/
    
    // check if resize integer is between 0 and 100
    if ((int)(argv[1]) > 0 && (int)(argv[1]) <= 100) 
    {
        fprintf(stderr, "Resize value must be of integer value 0-100\n");
        return 2;
    }
    
    // remember resize integer
    int n = atoi(argv[1]);
    
    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];
    
    // open input file 
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 3;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 4;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 5;
    }
    
    // ---- TO DO ----
    
    // HEADER FILE values that need to be changed
    // bi.biWidth and bi.biHeight (in pixels) 
    // bi.biSizeImage = ((sizeof(RGBTRIPLE) * bi.biWidth) + padding) * abs(bi.biHeight);
    // bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    
    // RESIZE HORIZONTALLY
    // fread infile normally while skipping padding
    // fwrite to outfile n times (multiplying the pixels) and create new padding
    
    // RESIZE VERTICALLY
    // rewrite method: make copy of each row and write it n times
    // recopy method: for n-1 times, write pixels and add padding to outfile, send infile cursor back

    // keep original biWidth and biHeight
    int inWidth = bi.biWidth;
    int inHeight = abs(bi.biHeight);
        
    // change BITMAPINFOHEADER biWidth and biHeight according to resize multiple
    bi.biWidth *= n;
    bi.biHeight *= n;
    
    // determine infile's padding for scanlines
    int inPadding = (4 - (inWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    // determine outfile's padding for scanlines (after adjusted biWidth)
    int outPadding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    printf("biWidth: %d\n", bi.biWidth);
    printf("inPadding: %d\n outPadding: %d\n", inPadding, outPadding);
    
    // calculate new file's biSizeImage and bfSize (after adjusted biWidth and biHeight)
    bi.biSizeImage = ((sizeof(RGBTRIPLE) * bi.biWidth) + outPadding) * abs(bi.biHeight);
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);
    
    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // iterate over infile's scanlines
    for (int i = 0; i < inHeight; i++)
    {
        // for each scanline row, resize vertically (n times)
        for (int v = 0; v < n; v++) 
        {
            printf("v: %d\n", v);
            // iterate over pixels in scanline
            for (int j = 0; j < inWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;
    
                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                
                printf("%d - R: %d  G: %d  B: %d\n", j, triple.rgbtRed, triple.rgbtGreen, triple.rgbtBlue);
                
                // resize pixels horizontally (n times)
                for (int h = 0; h < n; h++) 
                {
                    // write RGB triple to outfile
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }
            
            // add padding to outfile
            for (int k = 0; k < outPadding; k++)
            {
                fputc(0x00, outptr);
            }
            
            // continually send infile cursor back to start of scanline until last resize row
            if (v < (n - 1)) 
            {
                fseek(inptr, -(long int)(inWidth * sizeof(RGBTRIPLE)), SEEK_CUR);
            }
        }
        
        // skip over padding for infile cursor
        fseek(inptr, inPadding, SEEK_CUR);

    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
