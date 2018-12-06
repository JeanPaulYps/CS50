def get_input (message):
    while(True):
        userInput = input(message)
        if (userInput.isdigit()):
            userInput = int(userInput)
            if (userInput >= 1 and userInput <= 8):
                return userInput

def print_pyramid(stairs):
    for i in range(stairs):
        for j in range(stairs):
            if (j < (stairs - i -1) ):
                print(" ", end = "")
            else:
                print("#", end = "")
        print()


def print_full_pyramid (stairs):
    for i in range(stairs):
        for j in range(stairs):
            if (j < (stairs - i -1) ):
                print(" ", end = "")
            else:
                print("#", end = "")
        print("  ", end = "")
        for j in range(stairs):
            if (j < i + 1):
                print("#", end = "")
        print()


if __name__ == "__main__":
    stairs = get_input("Height: ")
    print_full_pyramid(stairs)