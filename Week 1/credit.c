#include <stdio.h>
#include <cs50.h>

//This is the solution of credit, in the course cs50, introduction to computer science
int count_digits(long num);
void get_digits(int numA [], int size,long num);
bool luhn (int numA[], int size);
bool valid_card (int size);
void get_card (int numA[], int size);

int main(void)
{
    long creditNum = get_long("Number: ");
    int size = count_digits(creditNum);
    int numA[size];
    get_digits(numA,size,creditNum);
    if (luhn(numA,size) && valid_card (size))
    {
        get_card (numA, size);
    } 
    else printf("INVALID\n");
}


int count_digits(long num)
{
    int count=0;
    long digit;
    while(num)
    {
        digit = num % 10;
        num/= 10;
        count ++;
    }
    return count;
}


void get_digits(int numA [], int size,long num)
{
    int count=0;
    int i;
    long digit;
    while(num)
    {
        digit = num % 10;
        num/= 10; 
        numA[count] = digit;
        count ++;
    }
}


bool luhn (int numA[], int size)
{
    int count = 0;
    for (int i = 0; i < size; i += 2)
        count += numA[i];

    if (count % 10) return true;
    return false;
}


bool valid_card (int size)
{
    if(size == 16 || size == 15 || size == 13)
        return true;
    return false;
}

void get_card (int numA[], int size)
{
    int firstDigits = 10 * numA[size-1] + numA[size - 2];
    //printf("%i",firstDigits);
    if (firstDigits == 34 || firstDigits == 37)
        printf("AMEX\n");
    else if (firstDigits>=51 && firstDigits <= 55)
        printf("MASTERCARD\n");
    else if (numA[size - 1] == 4)
        printf("VISA\n");
    else
        printf("INVALID\n");
}
