

game_board = {'1' : ' ', '2' : ' ', '3' : ' ',
            '4' : ' ', '5' : ' ', '6' : ' ',
            '7' : ' ', '8' : ' ', '9': ' '}

board_keys = []

for keys in game_board:
    board_keys.append(keys)


def printBoard(board):

    print(board['1'] + '|' + board['2'] + '|' + board['3'])
    print('-+-+-')
    print(board['4'] + '|' + board['5'] + '|' + board['6'])
    print('-+-+-')
    print(board['7'] + '|' + board['8'] + '|' + board['9'])


def userTurn(count, move, board):
    if count % 2 != 0:
        board[move] = 'X'
    elif count % 2 == 0:
        board[move] = 'O'
    return board


def gameChecker(board):
    if (board['1'] == board['2'] == board['3'] != ' '):
        return (True, board['1'])
    elif (board['4'] == board['5'] == board['6'] != ' '):
        return (True, board['4'])
    elif (board['7'] == board['8'] == board['9'] != ' '):
        return (True, board['7'])
    elif (board['1'] == board['4'] == board['7'] != ' '):
        return (True, board['1'])
    elif (board['2'] == board['5'] == board['8'] != ' '):
        return (True, board['2'])
    elif (board['3'] == board['6'] == board['9'] != ' '):
        return (True, board['3'])
    elif (board['1'] == board['5'] == board['9'] != ' '):
        return (True, board['1'])
    elif (board['3'] == board['5'] == board['7'] != ' '):
        return (True, board['3'])
    else:
        return (False, 0)

def main():

    count = 1
    local_board = game_board.copy()

    for i in range(10):
        printBoard(local_board)
        move = input()
        local_board = userTurn(count, move, local_board)
        count = count + 1
        if count > 5:
            check = gameChecker(local_board)
            if check != 0:
                print('The winner of the game is', check)



if __name__ == "__main__":
    main()
