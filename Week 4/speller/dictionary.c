// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "dictionary.h"

// Defines new methods
int getposition(char letter);


// Represents number of children for each node in a trie
#define N 27

// Represents a node in a trie
typedef struct node
{
    bool is_word;
    struct node *children[N];
}
node;

bool erase_dictionary (node* temp);
// Represents a trie
node *root;

unsigned int WORDS_IN_DICTIONARY;

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    WORDS_IN_DICTIONARY = 0;
    // Initialize trie
    root = malloc(sizeof(node));
    if (root == NULL)
    {
        return false;
    }
    root->is_word = false;
    for (int i = 0; i < N; i++)
    {
        root->children[i] = NULL;
    }

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        unload();
        return false;
    }

    // Buffer for a word
    char word[LENGTH + 1];
    int con = 0;
    // Insert words into trie
    while (fscanf(file, "%s", word) != EOF)
    {
        // TODO
        char letter;
        node *temp = root;
        int word_size = strlen(word);
        for ( int i = 0; i < word_size; i++ )
        {
            letter = word[i];
            int position = getposition(letter);

            if (temp->children[position] == NULL && position != -1)
            {
                node *new_node;
                new_node = malloc(sizeof(node));

                if(i == word_size - 1) (*new_node).is_word = true;
                else (*new_node).is_word = false;

                for (int j = 0; j < N; j++) (*new_node).children[j] = NULL;

                temp->children[position] = new_node;
                temp = new_node;
            }
            else
            {
                if (i == word_size - 1) temp -> children[position] -> is_word = true;
                else
                {
                    temp = temp->children[position];
                }
            }

        }

        WORDS_IN_DICTIONARY ++;
    }

    // Close dictionary
    fclose(file);
    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return WORDS_IN_DICTIONARY;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // TODO
    node *temp = root;
    int position = -1;
    int word_size = strlen(word);
    for(int i = 0; i < word_size; i++)
    {
        char letter = word[i];
        letter = tolower(letter);
        position = getposition(letter);

        if (position != -1)
        {
            if(temp->children[position] != NULL)
            {
                temp = temp->children[position];
                if (i == word_size -1)
                {
                    bool is_a_word = (*temp).is_word;
                    if(is_a_word)
                        return true;
                    else
                        return false;
                }
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }

    }
    return false;

}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    return erase_dictionary(root);
}

bool erase_dictionary (node* temp)
{
    for (int i = 0; i <N; i++)
    {
        if (temp->children[i] != NULL)
        {
            erase_dictionary(temp->children[i]);
        }
    }
    free(temp);
    return true;
}
int getposition(char letter)
{
    char alphabet [] = "abcdefghijklmnopqrstuvwxyz'";
    for (int i = 0; i < N; i++)
    {
        if (letter == alphabet[i])
        {
            return i;
        }
    }
    return -1;
}
