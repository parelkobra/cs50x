// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 285000;

// Hash table
node *table[N];

// Number of words in dictionary
int words = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    char lword[LENGTH + 1];
    int i = 0;

    // Lower case word
    for (; word[i]; i++)
        lword[i] = tolower(word[i]);

    lword[i] = '\0';

    unsigned int key = hash(lword);

    for (node *tmp = table[key]; tmp != NULL; tmp = tmp->next)
        if (strcasecmp(word, tmp->word) == 0)
            return true;

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    const int g = 31;
    unsigned int hash = 0;
    int length = strlen(word);

    for (int i = 0; i < length; i++)
        hash = g * hash + word[i];

    hash %= N;

    return hash;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    FILE *inptr = fopen(dictionary, "r");
    if (inptr == NULL)
        return false;

    char word[LENGTH + 1];
    unsigned int key;

    while (fscanf(inptr, "%s", word) != EOF)
    {
        // Get the hash code of the current word
        key = hash(word);

        // Set up the new node
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            fclose(inptr);
            return false;
        }

        strcpy(n->word, word);
        n->next = (table[key] != NULL) ? table[key] : NULL;

        // Insert the new node into the hash table
        table[key] = n;

        // Increase the words counter
        words++;
    }

    fclose(inptr);

    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return words;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];
        node *tmp = cursor;

        while (cursor != NULL)
        {
            cursor = cursor->next;
            free(tmp);
            tmp = cursor;
        }
    }

    return true;
}
