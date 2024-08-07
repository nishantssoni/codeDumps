import fileoperation as fl

status = True
while status:
    try:
        print('\n\n0.exit')
        print('1.making driectory')
        print('2.renaming directory or file')
        print('3.view your current directory')
        print('4.change your current directory')
        print('5.make file in your current directory')
        print('6.write in your directory')
        print('7.list all your file and directory in current directory')
        ch = int(input('\ninput your choice :: '))

        if ch == 0:
            status = False

        elif ch == 1:
            fl.makeDir()
        elif ch == 2:
            fl.RenameDirOrFile()
        elif ch == 3:
            print('your current directory is {0}'.format(fl.viweYourCurrentDirectory()))
        else:
            print('wrong input')
            continue
    except ValueError:
        print("\nchose avalable options")
