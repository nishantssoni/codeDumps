#include<iostream>
#include<cuchar>

using namespace std;

//conio.h functions
#include <sys/ioctl.h>
#include <termios.h>
bool kbhit()
{
    termios term;
    tcgetattr(0, &term);

    termios term2 = term;
    term2.c_lflag &= ~ICANON;
    tcsetattr(0, TCSANOW, &term2);

    int byteswaiting;
    ioctl(0, FIONREAD, &byteswaiting);

    tcsetattr(0, TCSANOW, &term);

    return byteswaiting > 0;
}


#include <unistd.h>

char getch(void)
{
    char buf = 0;
    struct termios old = {0};
    fflush(stdout);
    if(tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if(tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if(read(0, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if(tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    return buf;
 }


//variables
bool gameOver;
const int width = 50;
const int height = 30;
int x, y, fruitX, fruitY, score=0;
enum eDirction {STOP = 0, LEFT, RIGHT, UP, DOWN} dir;
int tailx[100], taily[100], tailCount=0;
void setup(){
    gameOver = false;
    dir = STOP;
    x = width / 2;
    y = height /2;
    tailx[0] = x;
    taily[0] = y;
    fruitX = rand() % width;
    fruitY = rand() % height;
}

void Draw()
{
    //clear the window
   system("clear");
   sleep(.8);

    //top wall
    for(int i = 0; i<width; i++){
        printf("#");
    }
    cout<<endl;

    //side walls
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            if(j == 0)
                cout<<"#";

            //snake
            if(tailCount)
            for(int count = 0; count<tailCount;count++){
                if(i == taily[count] && j == tailx[count]){
                    cout<<"o";
                }
            }
            if(i == y && j == x)cout<<"0";
                
            //fruit
            else if(i == fruitY && j == fruitX)
                cout<<"F";

            else
                cout<<" ";
            
            if(j == width-1)
                cout<<"#";
        }
        cout<<endl;
    }
    
    
    //bottom wall
    for(int i = 0; i<width; i++){
        printf("#");
    }
    cout<<endl;
    cout<<"your score is :: "<<score<<endl;
    cout<<"your tailcount is :: "<<tailCount<<endl;
    cout<<x<<","<<y<<endl;
    for(int i = 0; i <tailCount; i++){
        cout<<tailx[i]<<","<<taily[i]<<"  ";
    }
    cout<<endl; 
}

void Input(){

    if(kbhit())
    {
        switch(getchar()){
            
            case 'a':
                dir = LEFT;
                break;

            case 'd':
                dir = RIGHT;
                break;

            case 'w':
                dir = UP;
                break;

            
            case 's':
                dir = DOWN;
                break;

            case 'x':
                gameOver = true;
                break;
        }

    }
}

void Logic()

{   

        //tail
        
        for(int count = tailCount; count>0; --count){

            tailx[count] = tailx[count-1];
            taily[count] = taily[count-1];
            }
        tailx[0] = x;
        taily[0] = y;

    // sleep(1);
    switch (dir)
    {
    case LEFT:
        x--;
        break;
    
    case RIGHT:
        x++;
        break;
    
    case UP:
        y--;
        break;
    
    case DOWN:
        y++;
        break;
    
    }

    if(x > width || x < 0 || y > height || y < 0)
        gameOver = true;

    if(x == fruitX && y == fruitY){
        score += 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
        tailCount++;

    }
   
    if (dir == LEFT || dir == RIGHT)
    {
        usleep(100000);
    }

    if (dir == UP || dir == DOWN)
    {
        usleep(150000);
    }
    
    
    

}

int main()
{
    setup();


    while(!gameOver){
        
        Draw();
        Input();
        Logic();
    
    }
    return 0;
}