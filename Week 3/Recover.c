#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define BLOCK_SIZE 512

typedef uint8_t BYTE;
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
    FILE *jpeg_file = NULL;
    while( fread(jpeg_image,1,sizeof(jpeg_image), memory_file)  != 0x00)
    {
        //It's a jpeg?
        if (jpeg_image[0] == 0xff &&
            jpeg_image[1] == 0xd8 &&
            jpeg_image[2] == 0xff &&
            (jpeg_image[3] & 0xf0) == 0xe0)
            {
                if (jpeg_file != NULL)
                {
                    fclose(jpeg_file);
                }
 
                //Set jpeg name
                sprintf(jpeg_name, "%03i.jpg",con);
                //Create a jpeg file
                jpeg_file = fopen(jpeg_name, "w");
                //write jpeg image
                fwrite(jpeg_image, 1,sizeof(jpeg_image),jpeg_file);
                con ++;
                
            }
        else
        {
            if (jpeg_file != NULL)
            {
                fwrite(jpeg_image, 1,sizeof(jpeg_image),jpeg_file);
            }
        }
    }
    
    fclose(jpeg_file);
    fclose(memory_file);
    return 0;
}
