// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"

// Represents a node in a trie
typedef struct node
{
    bool is_word;
    char letter;
    struct node *next_letter;
    struct node *children;
}
node;
// Represents a trie
node *root;
int con = 0;


node* searchLetter (node* node, char letter);
bool erase_dictionary (node* temp);
void insert_word (char word []);
void add_child (node* father, char letter);
void add_next_letter (node* father, char letter);


// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize trie
    root = malloc(sizeof(node));
    if (root == NULL)
    {
        return false;
    }
    root->is_word = false;
    root->next_letter = NULL;
    root->children = NULL;
    root->letter = '\0';

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        unload();
        return false;
    }

    // Buffer for a word
    char word[LENGTH + 1];

    // Insert words into trie
    while (fscanf(file, "%s", word) != EOF)
    {
        // TODO
        insert_word(word);
        con ++;
    }

    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}

void insert_word (char word [])
{
    node *temp = root;
    int word_size = strlen(word);
    char letter;
    for ( int i = 0; i < word_size; i++ )
    {
        letter = word[i];

        if (temp->children != NULL)
        {
            temp = temp->children;
            temp = searchLetter (temp, letter);
            if (temp->letter != letter)
            {
                add_next_letter(temp, letter);
                temp = temp->next_letter;
            }
            
        }
        else
        {
            add_child (temp,letter);
            temp = temp->children;
        }

        if(i == word_size - 1) 
            temp->is_word = true;
            
        
    }
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return con;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    node *temp = root;
    int word_size = strlen(word);
    char letter;
    for ( int i = 0; i < word_size; i++ )
    {
        letter = word[i];

        if (temp->children == NULL)
        {
            return false;
        }
        else
        {
            letter = tolower(letter);
            temp = temp->children;
            temp = searchLetter (temp, letter);
            if (temp->letter != letter)
            {
                return false; 
            }
            else
            {   
                if(i == (word_size - 1) && temp->is_word) 
                    return true;
            }
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

    if (temp->children != NULL)
        erase_dictionary(temp->children);
    if (temp->next_letter != NULL)
        erase_dictionary(temp->next_letter);
    
    free(temp);
    return true;
}

node* searchLetter (node* temp, char letter)
{
    do
    {
        char node_letter =  temp->letter;
        if (node_letter == letter)
        {
            return temp;
        }
        else
        {
            if (temp->next_letter != NULL)
            {
                temp = temp->next_letter; 
            }
            else
            {
                return temp;
            }
        }
    }while (true);
}

void add_child (node* father, char letter)
{
    node *new_node;
    new_node = malloc(sizeof(node));
    new_node->letter = letter;
    new_node->next_letter = NULL;
    new_node->is_word = false;

    father->children = new_node;
}
void add_next_letter (node* father, char letter)
{
    node *new_node;
    new_node = malloc(sizeof(node));
    new_node->letter = letter;
    new_node->children = NULL;
    new_node->is_word = false;

    father->next_letter = new_node;
}