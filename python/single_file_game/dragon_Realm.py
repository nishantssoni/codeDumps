# dragon Realm by nishantssoni
import random
import time

def displayIntro():

    name = input('Enter your name to play game ::  ')
    print('loading....')
    time.sleep(2)

    print(f'\nHi, {name}')
    time.sleep(2)

    print('You are in a land full of dragons...')
    time.sleep(2)

    print('In front of you, you see two caves...')
    time.sleep(2)

    print('''In one cave, 
the dragon is friendly and will share his treasure with you.''')
    time.sleep(2)

    print('''In Second cave,
The dragon is greedy and hungry, and will eat you on sight.''')
    time.sleep(2)



def chooseCave():
    cave = ''
    while cave != '1' and cave != '2':
        print('Which cave will you go into? (1 or 2)')
        cave = input()

    return cave

def checkCave(choosenCave):
    print('You approach the cave...')
    time.sleep(2)
    print('It is dark and spooky...')
    time.sleep(2)
    print('A large dragon jumps out in front of you! He opens his jaws and ...')
    time.sleep(4)

    friendlyCave = random.randint(1,2)

    if choosenCave == str(friendlyCave):
        print('Gives you his treasure!')
    else:
        print('Gobbles you down in one bite!')
        

if __name__ == '__main__':
    playAgain = 'yes'
    while playAgain == 'y' or playAgain == 'yes':
        displayIntro()
        caveNumber = chooseCave()
        checkCave(caveNumber)
        print('\nDo you want to play again? (yes or no)')
        playAgain = input()