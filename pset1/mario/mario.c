#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int n;
    do
    {
        //ask height or number of rows
        n = get_int("height?: ");
    }
    while (n < 1 || n > 8);
    for (int i = 0; i < n; i++)
    {
        //left create reverse number of dots first
        for (int j = n; j > i + 1; j--)
        {
            printf(" ");
        }

        //left create number of #
        for (int j = 0; j < i + 1; j++)
        {
            printf("#");
        }
        printf("  ");
        //right create number of #
        for (int j = 0; j < i + 1; j++)
        {
            printf("#");
        }

        printf("\n");
    }
}