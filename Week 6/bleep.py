from sys import argv
import sys

wordsBanned = set()

def add_wordsBanned(file_name):
    file = open(file_name,"r")
    for line in file:
        wordsBanned.add(line.rstrip("\n"))
    file.close()

def get_star(lenght):
    result = ""
    for i in range (lenght):
        result = result + "*"
    return result

def check_input(text):
    text = text.split()
    result = []
    for word in text:
        if (word.lower() in wordsBanned):
            word = get_star(len(word))
        result.append(word)
    return " ".join(result)

if __name__ == "__main__":
    if (len(argv) == 2):
        add_wordsBanned(argv[1])
        text = input("What message would you like to censor?")
        print(check_input(text))
        sys.exit(0)
    else:
        print("Usage: python bleep.py dictionary")
        sys.exit(1)