//NISHANT SHEKHAR SONI
//Arrar stack
#include<iostream>
using namespace std;

class Arraystack{
    int top;
    int maxcount;
    int *val;
    public:
    Arraystack(int length){
        top = -1;
        maxcount = length;
        val = new int[length];
    }
   
    int getTop(){        
        return val[top];
    }
    
    int isEmpity(){
        if (top == -1){ 
            return 1;
        }
        return 0;      
    }

    int isFull(){
        if (top == maxcount -1)
        {
           return 1;
        }
        return 0;      
    }

    
    void push(int value){
        if ( isFull() )
        {   
            maxcount *= 2;
            cout<<"stack overflow but new stack is created with size  :: "<<maxcount<<endl;          
            
            int *temp = val;
            val =  new int[maxcount];

            for(int i= 0;i<=top;i++){
                val[i] = temp[i];
            }
            delete temp;
        }
        val[++top] = value;
    }
    int pop(){
        if( ! isEmpity() ){
            int temp = val[top];
            --top;
            return temp;
        }
        cout<<"underflow"<<endl;
        return 0;
    }

    void print(){
        if(isEmpity()){ 
            return;
        }
        for (int i = 0; i <= top; i++)
        {
            cout<<val[i]<<"   ";
        }
        cout<<endl;    
    }
};

int main()
{
    Arraystack a(4);
    cout<<" is empity "<<a.isEmpity()<<endl;
    cout<<" is full "<<a.isFull()<<endl;
    a.push(1);
    a.push(2);
    a.push(3);
    a.push(4);
    a.push(5);
    a.push(6);
    a.push(7);
    a.pop();
    a.print();
    cout<<"the top element is  ::  "<<a.getTop()<<endl;
}