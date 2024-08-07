//NISHANT SHEKHAR SONI
//string reverse using stack
#include<iostream>
#include<cstring>
#include<stack> //stack from standerd library STL
using namespace std;

void reverseString(char *ch,int len){
    stack<char> stk;

    for (int i = 0; i < len; i++)
    {
        stk.push(ch[i]);
    }
    
    for (int i = 0; i < len; i++)
    {
        ch[i] = stk.top();
        stk.pop();
    }

    
}

int main()
{
    char c[51];
    cout<<"enter a name  :: ";
    cin>>c;
    reverseString(c,strlen(c));
    cout<<endl<<"your reverse string is :: "<<c<<endl;

}