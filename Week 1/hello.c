#include <stdio.h>
#include <cs50.h>
//This is the solution of hello, in the course cs50, introduction to computer science

int main(void)
{
    string name = get_string("What is your name? ");
    printf("hello, %s\n", name);
}
