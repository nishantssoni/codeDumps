# code by nishantssoni in 25th july, 2021 1:58 PM
import random

def printHungman(choice):
    ch = ''
    if choice == 1 or choice == 0:
        ch = '''
    +---+
        |
        |
        |
       ====
        '''
    elif choice == 2:
        ch = '''
    +---+
    0   |
        |
        |
       ====
        '''
    elif choice == 3:
        ch = '''
    +---+
    0   |
    |   |
        |
       ====
        '''
    elif choice == 4:
        ch = '''
    +---+
    0   |
   /|   |
        |
       ====
        '''

    elif choice == 5:
        ch = '''
    +---+
    0   |
   /|\  |
        |
       ====
        '''
    elif choice == 6:
        ch = '''
    +---+
    0   |
   /|\  |
   /    |
       ====
        '''
    elif choice == 7:
        ch = '''
    +---+
    0   |
   /|\  |
   / \  |
       ====
        '''

    print(ch)


def getWord():
    words = {'Colors': 'red orange yellow green blue indigo violet\
     white black brown'.split(),

            'Shapes':'square triangle rectangle circle ellipse rhombus\
             trapezoid chevron pentagon hexagon septagon octagon'.split(),

             'Fruits': 'apple orange lemon lime pear watermelon grape grapefruit\
              cherry banana cantaloupe mango strawberry tomato'.split(),

             'Animals': 'bat bear beaver cat cougar crab deer dog donkey duck eagle\
             fish frog goat leech lion lizard monkey moose mouse otter owl panda\
             python rabbit rat shark sheep skunk squid tiger turkey turtle weasel\
            whale wolf wombat zebra'.split()}


    a = random.randint(0, len(words)-1)
    keys = list(words.keys())
    key = keys[a]
    b = random.randint(0, len(words[key])-1)
    return key, words[key][b]


def guessLtr():
    print('\nguess a letter:')
    while True:
        guess_ltr = input()
        guess_ltr = guess_ltr.lower()
        if len(guess_ltr) != 1:
            print('Please enter a single letter.')
        elif guess_ltr not in 'abcdefghijklmnopqrstuvwxyz':
            print('Please enter a LETTER.')
        else:
            return guess_ltr


def logic():
    status = True
    count = 0
    category, guess = getWord()
    missed = ['_' for i in range(len(guess))]

    print('H A N G M A N')
    life = 0
    while life < 7:
        print()
        printHungman(life+1)
        # print('guess letter is :: ',guess)
        print('life left:: ', 7 - life)
        # print('count is :: ',count)
        print('Missed letter:')
        for letter in missed:
            print(letter, end=' ')
        print('\nhint:: it is from a category',category)
        guess_ltr = guessLtr()

        for i in range(len(guess)):
            if guess_ltr == guess[i] and missed[i] == '_':
                missed[i] = guess[i]
                status = True
                count += 1
                break
            else:
                status = False

        if not status:
            life += 1
            print('worng guess try another')
        else:
            print('guess write')
        if count == len(guess):
            break

    if life < len(guess):
        print('hurre you guessd right')

    else:
        print('sorry you faild')
        print('your guess word is', guess)


if __name__ == '__main__':
    while True:
        logic()
        print('do you want to play again (yes or no)?')
        ch = input()
        if ch == 'yes' or ch == 'y':
            print('\nnew game started \n')
        else:
            break
