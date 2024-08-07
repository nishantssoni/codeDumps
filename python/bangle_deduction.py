import random
MAX_DIGIT = 3
MAX_CHANCE = 10

def getSecretNum():
    a = list(range(10))
    random.shuffle(a)
    guess = ''
    for i in range(MAX_DIGIT):
        guess += str(a[i])
    return guess

def getClues(guess, secretNum):
    
    if guess == secretNum:
        return "you got it huree!"
    
    isBangel = True
    txt = ''
    
    for i in range(MAX_DIGIT):
        if guess[i] == secretNum[i]:
            txt += ' Fermi '
            isBangel = False
    
    
    for i in range(MAX_DIGIT):
        for j in range(MAX_DIGIT):
            if secretNum[i] == guess[j] and (i != j):
                txt += ' Pico '
                isBangel = False
    if isBangel:
        return "bangle"
    return txt

def getInput():
    
    while True:
        ch = input()
        if not (ch.isdigit() and 2<len(ch)<4):
            print("Wrong Input!! enter number of digit 3")
        else:
            return ch


if __name__ == '__main__':
    
    print(""" 
I am thinking of a 3-digit number. Try to guess what it is.
The clues I give are...
When I say:
That means:
Bagels
None of the digits is correct.
Pico
One digit is correct but in the wrong position.
Fermi
One digit is correct and in the right position.
I have thought up a number. You have 10 guesses to get it. \n\n""")
    
    secretNum = getSecretNum()
    print(secretNum)
    winStatus = False
    
    for i in range(MAX_CHANCE):
        
        print(f"Guess #{i+1}:")
        
        ch = getInput()
        
        print(getClues(ch, secretNum))
        
        if ch == secretNum:
            winStatus = True
            break
    if not winStatus:
        print(f'Sorry! you ran out of chance secret Number is {secretNum}')
