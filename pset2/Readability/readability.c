#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <math.h>

int letters = 0;
int words = 0;
int sentence = 0;
float index;
string text;

int count_letters();
int count_words();
int count_sentence();
int grade();

int main(void)
{
    text = get_string("Text: ");

    count_letters();
    // printf("%i letter(s)\n", letters);

    count_words();
    // printf("%i word(s)\n", words);

    count_sentence();
    // printf("%i sentence(s)\n", sentence);

    grade();

    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    if (index > 16)
    {
        printf("Grade 16+\n");
    }
    if (index <= 16 && index >= 1)
    {
        //round float so we get even grade instead of int
        printf("Grade %.0f\n", round(index));
    }


}

int grade() //calculated grade in float
{
    float average(int length, int array[]);
    float L;
    float S;

    L = ((float)letters / (float)words) * 100;
    S = ((float)sentence / (float)words) * 100;

    index = 0.0588 * L - 0.296 * S - 15.8;
    return index;
}

int count_sentence() //count sentences
{
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (text[i] == '!')
        {
            sentence++;
        }
        if (text[i] == '?')
        {
            sentence++;
        }
        if (text[i] == '.')
        {
            sentence++;
        }
    }
    return sentence;
}


int count_words() //count words
{
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (text[i] == ' ')
        {
            words++;
        }
    }
    if (letters > 0)
    {
        words++;
    }
    return words;
}

int count_letters() //count letters
{
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (text[i] >= 'a' && text[i] <= 'z')
        {
            letters++;
        }
        if (text[i] >= 'A' && text[i] <= 'Z')
        {
            letters++;
        }
    }
    return letters;
}