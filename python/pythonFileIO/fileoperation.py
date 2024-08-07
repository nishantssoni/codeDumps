import os
import shutil


def makeDir():

    print('\ndo you want to make dir in {0}'.format(os.getcwd()))
    c = input('press y for yes and n for no :: ')
    if c == 'y':

        name = input('\ninput your folder name :: ')
        try:
            os.mkdir(name)
            print('\nfolder created successfully')
        except FileExistsError:
            print('\nfile already exist')
        finally:
            return

    elif c == 'n':
        print('\ncancling successfully')
        return
    else:
        print('something went wrong')
        return


def RenameDirOrFile():
    """Rename directory or file"""

    print('\ndo you want to rename file or directory')
    c = input('\npress y for yes and n for no :: ')

    if c == 'y':
        file = input(
            '\nenter a file with extension or folder name which you want to rename :: ')

        # check file is present or not
        if file in os.listdir(os.getcwd()):
            print('\nfile found successfully')
            fileName = input(
                '\ninput your file with extension or folder name ')
            os.rename(file, fileName)
            print('\nsuccessfully renamed')
    elif c == 'n':
        print('\ncancling successfully')
    else:
        print('someting went wrong')


def viweYourCurrentDirectory():
    return os.getcwd()

def changeDirectory():
    return
