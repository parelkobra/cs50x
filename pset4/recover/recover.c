#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define BLOCK 512

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s image\n", argv[0]);
        return 1;
    }

    FILE *inptr = fopen(argv[1], "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open '%s'\n", argv[1]);
        return 2;
    }

    char fname[8];
    int count = 0;
    uint8_t buffer[BLOCK];
    FILE *outptr = NULL;

    while (fread(&buffer, BLOCK, 1, inptr) != 0)
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            sprintf(fname, "%03d.jpg", count++);
            outptr = fopen(fname, "w");
            fwrite(&buffer, sizeof(uint8_t), BLOCK, outptr);
            
            continue;
        }
        if (outptr != NULL)
        {
            fwrite(&buffer, sizeof(uint8_t), BLOCK, outptr);
        }
    }

    fclose(outptr);
    fclose(inptr);
    
    return 0;
}
