#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

bool verify_size(int argc);
bool verify_input(string in);
string caesar (string text, int key);
int to_int(string in);

int main(int argc, string argv[])
{
   string key = argv[1];
   if (verify_size(argc) && verify_input(key))
   {
       string text = get_string("plaintext: ");
       int keyt = to_int(key);
       //printf("%i\n", to_int(key));
       string result = caesar(text,keyt);
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
            if (isdigit(in[i])) con ++;
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
    //key -= '0';
    //printf("%i\n",key);
    char temp;   
    for (int i = 0; i < length; i++)
    {
        char c = text[i];
        if (c >= 'a' && c <= 'z')
        {
            temp = c - 'a' ;
            //
            //printf("%i\n", temp);
            temp = (temp + key)%26;
            temp += 'a';
            text[i] = temp;
        }
        else if (c >= 'A' && c <= 'Z')
        {
            temp = c - 'A' ;
            //
            //printf("%i\n", temp);
            temp = (temp + key)%26;
            temp += 'A';
            text[i] = temp;
        }
    }
    //printf("%s", text);
    return text;
}
int to_int(string in)
{
    int length = strlen(in);
    int result,digit;
    if (length == 2)
    {
        result = in[1] - '0';
        result += (in[0]-'0')*10;
    }
    else
        result = in[0] - '0';
    //printf("%i",result);
    
    return result;
}

