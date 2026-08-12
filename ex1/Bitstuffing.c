#include <stdio.h>
#include <string.h>

char binary[1000];
const char FLAG[] = "01111110";

void stringToBinary(char str[])
{
    int i, j, k = 0;

    printf("\nASCII Values:\n");
    for(i = 0; str[i] != '\0'; i++)
        printf("%c = %d\n", str[i], str[i]);

    printf("\nBinary Representation:\n");

    for(i = 0; str[i] != '\0'; i++)
    {
        for(j = 7; j >= 0; j--)
        {
            binary[k++] = ((str[i] >> j) & 1) + '0';
            printf("%c", binary[k - 1]);
        }
    }
    binary[k] = '\0';
    printf("\n");
}

void bitStuffing()
{
    char input[100];
    char stuffed[2000];
    char framed[2100];
    int i, j = 0, count = 0;

    printf("Enter any string: ");
    scanf("%s", input);

    stringToBinary(input);
    for(i = 0; binary[i] != '\0'; i++)
    {
        stuffed[j++] = binary[i];
        if(binary[i] == '1')
        {
            count++;
            if(count == 5)
            {
                stuffed[j++] = '0';
                count = 0;
            }
        }
        else
        {
            count = 0;
        }
    }
    stuffed[j] = '\0';

    sprintf(framed, "%s%s%s", FLAG, stuffed, FLAG);

    printf("\nBit Stuffed Data:\n%s\n", stuffed);
    printf("\nFramed Data (Flag + Stuffed + Flag):\n%s\n", framed);
}

void bitDestuffing()
{
    char inputFrame[2100], stuffed[2000], destuffed[2000];
    int i, j = 0, count = 0;

    printf("Enter Framed Binary Data:\n");
    scanf("%s", inputFrame);

    int frameLen = strlen(inputFrame);
    int flagLen = strlen(FLAG);

    if (frameLen >= (flagLen * 2) &&
        strncmp(inputFrame, FLAG, flagLen) == 0 &&
        strncmp(&inputFrame[frameLen - flagLen], FLAG, flagLen) == 0)
    {
        strncpy(stuffed, &inputFrame[flagLen], frameLen - (flagLen * 2));
        stuffed[frameLen - (flagLen * 2)] = '\0';
        printf("\nExtracted Stuffed Payload:\n%s\n", stuffed);
    }
    else
    {
       strcpy(stuffed, inputFrame);
    }

    for(i = 0; stuffed[i] != '\0'; i++)
    {
        if(stuffed[i] == '1')
        {
            count++;
            destuffed[j++] = stuffed[i];
            if(count == 5)
            {
                i++;
                count = 0;
            }
        }
        else
        {
            count = 0;
            destuffed[j++] = stuffed[i];
        }
    }

    destuffed[j] = '\0';
    printf("\nDestuffed Binary:\n%s\n", destuffed);
}
int main()
{
    int choice;

    do
    {
        printf("\n ||FRAMING PROTOCOL IMPLEMENTATION||\n1. Bit Stuffing\n2. Bit Destuffing\n3. Exit\n");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                bitStuffing();
                break;

            case 2:
                bitDestuffing();
                break;

            case 3:
                printf("\nExiting Program...\n");
                break;

            default:
                printf("\nInvalid Choice!\n");
        }

    } while(choice != 3);

    return 0;
}
