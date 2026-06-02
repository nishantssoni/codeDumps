# guess the number by nishantssoni
import random
import sys


def game(name, guess):
    game_coutn = 0
    while True:
        print(f"\nyou have {6 - game_coutn} try left")
        number = int(input('take a guess :: '))
        if game_coutn + 1 >= 6:
            print(f"\nsorry {name} you exceed the limit of guessing, thanks and bye!")
            sys.exit()
        elif number > guess:
            print('Your guess is too high.')
        elif number < guess:
            print('Your guess is too low.')
        elif number == guess:
            print(f"Good job, {name}! You guessed my number in {game_coutn + 1} guesses!")
            sys.exit()
        game_coutn += 1


def intro():
    name = input("Hello What's your Name? :: ")
    my_number = random.randint(1, 20)
    print(f'Well, {name} i am thinking a number between 1 and 20.\n')
    return name, my_number


if __name__ == '__main__':
    name, guess = intro()
    game(name, guess)
