# code by nishantssoni in 26th july, 2021 at 2:53 pm
import random

def draw_board(board):
    print()
    print(board[1] + '|' + board[2] + '|' + board[3])
    print('-+-+-')
    print(board[4] + '|' + board[5] + '|' + board[6])
    print('-+-+-')
    print(board[7] + '|' + board[8] + '|' + board[9])


def player_letter():
    while True:
        print('Do you want to be X or O?')
        ch = input()
        if ch == 'O' or ch == 'o':
            return ['X', 'O']
        elif ch == 'X' or ch == 'x':
            return ['O', 'X']
        else:
            print('Wrong letter entered, plz enter X or O')


def whose_goes_first():
    return random.randint(1, 2)


# player move
def player_move(board, ltr):
    while True:
        print('\n P L A Y E R - T U R N')
        print('\nwhats your move(1-9) ?')
        ch = input()

        if (not ch.isdigit) or (len(ch) > 1):
            print('enter digit value between 1 - 9')

        elif board[int(ch)] == ' ':
            board[int(ch)] = ltr
            return
        else:
            print('something went wrong!! try again')


def check_win(board):
    lst = ['789', '456', '123', '741', '852', '963', '753', '951']

    for i in lst:
        f = int(i[0])
        s = int(i[1])
        t = int(i[2])
        ch = False

        if (board[f] == board[s] == board[t]) and (board[f] == board[s] == board[t] != ' '):
            ch = True
            break
    return ch


def check_tie(board):
    for i in range(1, 10):
        if board[i] == ' ':
            return False

    if check_win(board):
        return False
    return True


def check_moves_lef(board):
    lst = []
    for i in range(1, 10):
        if board[i] == ' ':
            lst.append(i)
    return lst



def player_turn(board, ltr):
    player_move(board, ltr)
    tie = check_tie(board)
    win = check_win(board)

    return True, True, tie, win


def critical_check(board, ltrs):
    lst = ['789', '456', '123', '741', '852', '963', '753', '951']

    status = False

    for ltr in ltrs:

        for i in lst:
            f = int(i[0])
            s = int(i[1])
            t = int(i[2])
            ch = -1

            if board[f] == board[s] == ltr and board[t] == ' ':
                ch = t
                status = True
                break

            elif board[f] == board[t] == ltr and board[s] == ' ':
                ch = s
                status = True
                break

            elif board[s] == board[t] == ltr and board[f] == ' ':
                ch = f
                status = True
                break
        if status:
            break
    return ch


def place_random(board):
    case_lst = []
    lst = check_moves_lef(board)

    if board[5] == ' ':
        return 5

    for i in lst:
        for j in [1, 3, 7, 9]:
            if i == j:
                case_lst.append(i)

    if len(case_lst) > 0:
        ch = random.randint(0, len(case_lst) - 1)
        return case_lst[ch]

    rn = random.randint(0, len(lst) - 1)
    return lst[rn]


# computer Turn
def computer_turn(board, ltrs):
    print('\n C O M P U T E R - T U R N')

    cc = critical_check(board, ltrs)

    if cc != -1:
        board[cc] = ltrs[0]

    else:
        rn = place_random(board)
        board[rn] = ltrs[0]

    tie = check_tie(board)
    win = check_win(board)

    return True, True, tie, win

def logic():
    # odd for player and even for computer
    board = [' ' for i in range(10)]

    p_ltr = player_letter()

    first = whose_goes_first()

    if first == 1:
        print('\nyou go first\n')
    else:
        print('\ncomputer go first\n')

    draw_board(board)
    # # game_running, moves_left, tie, won
    game_status = (True, True, False, False)

    while game_status[0]:

        if first % 2 == 0:
            game_status = computer_turn(board, p_ltr)
        else:
            game_status = player_turn(board, p_ltr[1])

        if game_status[2]:
            draw_board(board)
            print('\nmatch is tie')
            break

        elif game_status[3]:
            if first % 2 == 0:
                draw_board(board)
                print('\ncomputer win')
                break
            else:
                draw_board(board)
                print('\nyou win')
                break

        elif game_status[0] and game_status[1]:
            first += 1
        draw_board(board)


if __name__ == '__main__':
    logic()

