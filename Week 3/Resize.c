// Copies a BMP file

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int get_padding (BITMAPINFOHEADER bi);
void copy_bits_in_new_file (FILE *out, BITMAPINFOHEADER bi_output, int scale);

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize f infile outfile\n");
        return 1;
    }

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    int scale = atoi(argv[1]);


    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
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
        return 4;
    }



    BITMAPINFOHEADER bi_output = bi;
    BITMAPFILEHEADER bf_output = bf;

    bi_output.biHeight = bi.biHeight * scale;
    bi_output.biWidth = bi.biWidth * scale;


    int padding = get_padding(bi);
    int padding_output = get_padding(bi_output);

    bi_output.biSizeImage = (bi_output.biWidth *sizeof(RGBTRIPLE) + padding_output) * abs(bi_output.biHeight);
    bf_output.bfSize = bi_output.biSizeImage + 54;


    // write outfile's BITMAPFILEHEADER
    fwrite(&bf_output, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi_output, sizeof(BITMAPINFOHEADER), 1, outptr);





    RGBTRIPLE arrayRGB[bi_output.biWidth];
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // iterate over pixels in scanline
        for (int j = 0; j < bi.biWidth; j++)
        {
            // temporary storage
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
            for (int l = 0; l <scale; l ++)
            {
                arrayRGB[(j*scale) + l] = triple;
            }
        }

        // skip over padding, if any
        fseek(inptr, padding, SEEK_CUR);


        for(int p = 0,height = scale; p < height; p++)
        {
            for (int x = 0; x < bi_output.biWidth ; x++)
            {
                fwrite(&arrayRGB[x], sizeof(RGBTRIPLE), 1, outptr);
            }
            for (int k = 0; k < padding_output; k++)
            {
                fputc(0x00, outptr);
            }
        }
    }


    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}

int get_padding (BITMAPINFOHEADER bi)
{
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    return padding;
}

