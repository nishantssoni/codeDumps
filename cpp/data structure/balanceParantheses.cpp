//NISHANT SHEKHAR SONI
//parantheses
#include<iostream>
#include<stack>
#include<cstring>
using namespace std;
#define MAXSIZE 51

char getReverse(char ch){
    if(ch == '}')
        return '{';
    else if(ch == ']')
        return '[';
    else
        return '(';
}

bool isBalanced(char *ch,int len){
    stack<char> s;

        for (int i = 0; i < len; i++)
        {   
            if( ch[i]=='{'  || ch[i]=='[' ||  ch[i]=='('  ){
                s.push(ch[i]);
            }
            
            else if(ch[i]=='}'  || ch[i]==']' ||  ch[i]==')' ){

                if(s.empty()){
                    return false;
                }else{
                           
                    if(s.top() == getReverse(ch[i])){
                        s.pop();
                    }
                    else{
                        return false;
                    }
                }
            }    
        
        }
    
    
    if(s.empty()){
        return true;
    }else{   
        return false;
    }

}

int main()
{   
    char strs[MAXSIZE];
    cout<<" enter a strpression :: ";
    cin>>strs;
    cout<<endl<<isBalanced(strs,strlen(strs));

}