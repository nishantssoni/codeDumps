//NISHANT SHEKHAR SONI
//STACK
#include<iostream>
using namespace std;

//stack
struct stackArray{
    int top;
    int maxCount;
    int *array;
};

stackArray *creatStack(int size){
    
    stackArray *temp =(stackArray*)malloc(sizeof(stackArray));
    //stackArray *temp = new stackArray;
    
    temp ->array =(int*)malloc(sizeof(int)*size);
    // temp ->array = new int[5];
    
    temp -> maxCount = size;
    temp ->top = -1;
    return temp;

}

int isFul(stackArray *s){
    if(s->top == s->maxCount-1) return 1;
    else return 0;
}

int isEmpty(stackArray *s){
    if(s->top<0) return 1;
    else return 0;
}

void push(stackArray *s , int info ){
   
    if(isFul(s)){
        
        cout<<"overflow"<<endl;

    }else
    {   
        s->top++;
        s->array[s->top] = info;
         
    }
}

int pop(stackArray *s){
    int popVal;
    if(isEmpty(s)){
        cout<<"underflow"<<endl;
    }else
    {   popVal = s->array[s->top];
        s->top--;
        return popVal;
    }
    return -1;
}

void printStack(stackArray *s){
    
    for(int i = 0; i <= s->top; i++){
        cout<<s->array[i]<<endl;
    }
}

int main()
{
    stackArray *st = creatStack(5);
    push(st,1);
    push(st,2);
    push(st,3);
    push(st,4);
    push(st,5);
    push(st,6);

    printStack(st);
    
    cout<<endl<<endl;

    int popVal =  pop(st);

    printStack(st);
    cout<<"pop value :: "<<popVal<<endl;
}