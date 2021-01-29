// Implements a dictionary's functionality
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdint.h>
#include <strings.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 100000^(LENGTH + 1);

// Hash table
node *table[N];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    //hash word to obtain value
    int idx = hash(word);

    //acess linked list
    node *cursor = table[idx];

    //move to next, if cursor == NULL stop
    while (cursor != NULL)
    {
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }

        //move to next
        cursor = cursor->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{

    //source of hash function: https://www.strchr.com/hash_functions
    int hash = 0;
    int M = 31;
    for (int i = 0; i < strlen(word); i++)
    {
        hash = M * hash + tolower(word[i]);
    }
    return (hash % N);
}

//keep track of number of words added
int counter = 0;

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    //pointer called file that opens and reads dictionary
    FILE *file = fopen(dictionary, "r");

    //return false if unable to open the file
    if (file == NULL)
    {
        printf("Unable to open file: %s\n", dictionary);
        return false;
    }

    //define word so we can read ("fscanf") into file
    char word[LENGTH + 1];

    //read one word at a time
    while (fscanf(file, "%s", word) != EOF)
    {
        //store adress of node and copy to new adress
        node *n = malloc(sizeof(node));

        //check memory
        if (n == NULL)
        {
            return false;
        }

        //store word in n
        strcpy(n->word, word);

        //set next pointer to point at nothing
        n->next = NULL;

        //hash word to obatin an integer
        int idx = hash(word);

        //check if start of linked list already pointing to something and assign accordingly
        if (table[idx] == NULL)
        {
            table[idx] = n;
            counter++;
        }
        else
        {
            n->next = table[idx];
            table[idx] = n;
            counter++;
        }
    }
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    //return value counter that keeps track of added words
    return counter;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    //set cursor so it points to a linked list
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];

        //if cursor is pointing at something do this
        while (cursor != NULL)
        {
            //set a tmp pointer, before removing and then free()
            node *tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }
    }
    return true;
}
