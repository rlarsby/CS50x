#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdint.h>

//size of each element is 512 called BLOCK
#define BLOCK 512

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    //check if there is input
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }
    
    //check if infile is correct
    char *infile = argv[1];
    if (infile == NULL)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }   
    
    //pointer called file that opens and reads infile
    FILE *file = fopen(infile, "r");
    
    //return 1 if unable to open the file
    if (file == NULL)
    {
        printf("Unable to open file: %s\n", infile);
        return 1;
    }
    
    //each filename is 8 bites, "###.jpg + \0"
    char filename[8]; 
    
    //create pointer for new file and make it point to nothing
    FILE *img = NULL;

    //use new type to store unsigned integers in array with size block
    uint8_t buffer[BLOCK];
    
    //tracker for new files: 001.jpg, 002.jpg,.... ###.jpg
    int tracker = 0;
    
    //read the file
    while (fread(buffer, sizeof(uint8_t), BLOCK, file))
    {
        //find beginning of new JPG
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            //if there is already a file open = close it
            if (img != NULL)
            {
                fclose(img);
            }
            
            //open new file and use tracker when giving it a new name
            sprintf(filename, "%03i.jpg", tracker);
            img = fopen(filename, "w");
            tracker++;
        }
        //if there is no start of a new img keep writing
        if (img != NULL)
        {
            fwrite(buffer, sizeof(buffer), 1, img);
        }
    }
    return 0;
}
