#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    int nfiles = 0;
    char name[50];

    if (argc != 2)
    {
        printf("Use: copy SOURCE DESTINATION\n");
        return 1;
    }
    FILE *card = fopen(argv[1], "r");
    if (card == NULL)
    {
        printf("Não foi possível abrir: %s.\n", argv[1]);
        return 2;
    }
    FILE *img = NULL;
    BYTE buffer[512];
    while(fread(&buffer, sizeof(BYTE), 512, card))
    {
        if(buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            if(nfiles == 0)
            {
                sprintf(name, "%03i.jpg", nfiles);
                img = fopen(name, "w");
                fwrite(&buffer, sizeof(BYTE), 512, img);
                nfiles++;
            }
            else
            {
                fclose(img);
                sprintf(name, "%03i.jpg", nfiles);
                img = fopen(name, "w");
                fwrite(&buffer, sizeof(BYTE), 512, img);
                nfiles++;
            }
        } else if(nfiles != 0)
        {
            fwrite(&buffer, sizeof(BYTE), 512, img);
        }
    }
    fclose(card);
    fclose(img);
    return 0;
}
