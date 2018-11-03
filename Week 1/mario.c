#include <cs50.h>
#include <stdio.h>
//This is the solution of mario, in the course cs50, introduction to computer science

void print_pyramid (int height);
int get_input(void);
int LIMIT_INF = 1;
int LIMIT_SUP = 8;

int main(void)
{
    int height = get_input();
    print_pyramid (height);
    
}


void print_pyramid (int height)
{
    int con = height;

    for(int i = height; i > 0; i--)
    {
        for(int j = 1; j <= height; j++)
        {
            if (j >= con) printf("#");
            else printf(" ");    
        }
        con --;
        printf("\n");
    }
    
}


int get_input(void)
{
    int in;

    do
    {
        in = get_int("Height: ");
    } while (in < LIMIT_INF || in > LIMIT_SUP);
    
    return in;    
}
