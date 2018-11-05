#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

bool verify_size(int argc);
bool verify_input(string in);
string caesar (string text, int key);
int to_int(string in);
char rotate_letter (char letter, int positions);

int main(int argc, string argv[])
{
   string keyS = argv[1];
   if (verify_size(argc) && verify_input(keyS))
   {
       string text = get_string("plaintext: ");
       int key = to_int(keyS);
       string result = caesar(text,key);
       printf("ciphertext: %s\n", result);
   }
   else
   {
       printf("Usage: ./caesar key");
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
    if (length <= 2)
    {
        for (int i = 0; i < length; i++)
        {
            if (isdigit(in[i])) 
                con ++;
        }
        if (con == length) 
            return true;
        return false;
    }
    return false;
}


string caesar (string text, int key)
{
    
    int length = strlen(text);
    char c;   
    for (int i = 0; i < length; i++)
    {
        c = text[i];
        text[i] = rotate_letter(c,key);
    }
    return text;
}


int to_int(string in)
{
    int length = strlen(in), result;
    if (length == 2)
    {
        result = in[1] - '0';
        result += (in[0]-'0')*10;
    }
    else
        result = in[0] - '0';    
    return result;
}


char rotate_letter (char letter, int positions)
{
    char temp;
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

