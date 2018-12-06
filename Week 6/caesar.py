import sys
from sys import argv
import sys

def caesar (text, key):
    cypher = ""
    LOWERMINLIMIT = ord("a")
    LOWERMAXLIMIT = ord("z")
    UPPERMINLIMIT = ord("A")
    UPPERMAXLIMIT = ord("Z") 
    for letter in text:
        letterCode = ord(letter)

        if (letterCode >= LOWERMINLIMIT and letterCode <= LOWERMAXLIMIT):
            new_letter = ( ( letterCode - LOWERMINLIMIT ) + key) % 26
            new_letter = new_letter + LOWERMINLIMIT
            cypher = cypher + chr(new_letter)

        elif (letterCode >=  UPPERMINLIMIT and letterCode <= UPPERMAXLIMIT):
            new_letter = ( ( letterCode -  UPPERMINLIMIT ) + key) % 26
            new_letter = new_letter +  UPPERMINLIMIT
            cypher = cypher + chr(new_letter)
        else:
            cypher = cypher + letter
    return cypher


if __name__ == "__main__":
    if (len(argv) == 2):
        text = input("plaintext: ")
        key = int(argv[1])
        print("ciphertext:  " + caesar(text,key))
        sys.exit(0)
    else:
        print("Usage: python caesar.py k")
        sys.exit(1)