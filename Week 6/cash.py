def is_float(input):
    try:
        num = float(input)
    except ValueError:
        return False
    return True
    
def min_coins (value):
    coins = [25,10,5,1]
    con = 0
    for i in coins:
        con = con + (value//i)
        value = value % i
    return con

def get_input(message):
    while(True):
        userInput = input(message)
        if (is_float(userInput)):
            userInput = float(userInput)
            if (userInput >= 0):
                return userInput

if __name__ == "__main__":
    value = get_input("Change owed: ")
    value *= 100
    value = int(value)
    print(min_coins(value))