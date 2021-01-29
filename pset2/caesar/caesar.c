#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


int main(int argc, string argv[])
{
    if (argc == 2 && isdigit(*argv[1])) //succeeds with entering key
    {
        for (int i = 0, n = strlen(argv[1]); i < n; i++) //checks if key is digit
        {
            if (!(isdigit(argv[1][i]))) //fails
            {
                printf("Usage: ./ceasar key\n");
                return 1;
            }
        }
        string text = get_string("plaintext: "); //ask for text
        
        int key = atoi(argv[1]); //store key as integer
        
        printf("ciphertext: ");
        
        for (int i = 0, n = strlen(text); i < n; i++)
        {
            char c = text[i];
            
            if (text[i] >= 'a' && text[i] <= 'z')
            {
                c = ((c - 97 + key) % 26) + 97;
                printf("%c", c);
            }
            
            else if (text [i] >= 'A' && text[i] <= 'Z')
            {
                c = ((c - 65 + key) % 26) + 65;
                printf("%c",  c);
            }
            else
            {
                printf("%c", c);               
            }

        }
        printf("\n");
        return 0;
    }
    else //fails
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

}