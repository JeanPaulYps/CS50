#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

bool verify_size(int argc);
bool verify_input(string in);
string vigenere (string text, string key);
char rotate_letter (char letter, char keyRotation);
int get_positions (char letter);

int main (int argc, string argv[])
{
    if(verify_size(argc))
    {
        string keyS = argv[1];
        if( verify_input(keyS))
        {
            string text = get_string("plaintext: ");
            string result = vigenere(text, keyS);
            printf("ciphertext: %s\n", result);
        }
        else
            return 1;
    }
    else
    {
       printf("Usage: ./vigenere keyword\n");
       return 1;
    }
}

bool verify_size(int argc)
{
    if (argc == 2) 
        return true;
    return false;
}


bool verify_input(string in)
{
    int length = strlen(in),con = 0;

    for (int i = 0; i < length; i++)
    {
        if (isalpha(in[i])) 
            con ++;
    }
    if (con == length) 
        return true;
    return false;

    return false;
}

string vigenere (string text, string key)
{
    int textLen = strlen(text);
    int keyLen = strlen(key);
    int keyPosition;
    int con = 0;
    char keyLetter,textLetter;


    for (int i = 0; i < textLen; i ++)
    {
        textLetter = text[i];
        if ( isalpha(textLetter) )
        {
            keyPosition = con%keyLen;
            keyLetter =  key[keyPosition];
            text[i] = rotate_letter(textLetter, keyLetter);
            con ++;
        }
        
    }

    return text;

}

char rotate_letter (char letter, char keyRotation)
{
    char temp;
    int positions = get_positions(keyRotation);

    if (letter >= 'a' && letter <= 'z')
    {
        temp = letter - 'a' ;
        temp = (temp + positions)%26;
        temp += 'a';
        return temp;
    }

    else if (letter >= 'A' && letter <= 'Z')
    {
        temp = letter - 'A' ;
        temp = (temp + positions)%26;
        temp += 'A';
        return temp;
    }

    return letter;
}

int get_positions (char letter)
{
    int position;

    if (letter >= 'a' && letter <= 'z')
    {
        position = letter - 'a';
        return position;
    }

    else if (letter >= 'A' && letter <= 'Z')
    {
        position = letter - 'A';
        return position;
    }

    position = letter;
    return position;
}

