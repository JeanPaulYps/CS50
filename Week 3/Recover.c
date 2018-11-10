#include <stdio.h>
#include "bmp.h"
#include <stdbool.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    //open memory card file
    char *memory_name = argv[1];
    FILE *memory_file = fopen(memory_name,"r");

    if (!memory_file)
    {
        printf("Could not open the file: %s\n", memory_name);
        return 2;
    }
    BYTE jpeg_image[512];
    int con = 0;
    char jpeg_name [9];
    bool first_jpeg = false;
    FILE *jpeg_file = NULL;
    while( fread(jpeg_image,1,sizeof(jpeg_image), memory_file)  != 0x00)
    {
        //It's a jpeg?
        if (jpeg_image[0] == 0xff &&
            jpeg_image[1] == 0xd8 &&
            jpeg_image[2] == 0xff &&
            (jpeg_image[3] & 0xf0) == 0xe0)
            {
                if (!first_jpeg)
                {
                    first_jpeg = true;
                    //Set jpeg name
                    sprintf(jpeg_name, "%03i.jpeg",con);
                    //Create a jpeg file
                    jpeg_file = fopen(jpeg_name, "w");
                    //write jpeg image
                    fwrite(jpeg_image, 1,sizeof(jpeg_image),jpeg_file);
                    con ++;
                }
                else
                {
                    fclose(jpeg_file);

                    sprintf(jpeg_name, "%03i.jpeg",con);
                    //Create a jpeg file
                    jpeg_file = fopen(jpeg_name, "w");
                    //write jpeg image
                    fwrite(jpeg_image, 1,sizeof(jpeg_image),jpeg_file);
                    con ++;
                }

            }
        
        
        else
        {
            if (first_jpeg)
            {
                fwrite(jpeg_image, 1,sizeof(jpeg_image),jpeg_file);
            }
        }
    }
    
    fclose(jpeg_file);
    fclose(memory_file);

    

    return 0;
}

