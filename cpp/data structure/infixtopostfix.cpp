//Nishant shekhar soni
//infinix to postfix
#include <iostream>
#include <stack>
using namespace std;

//declaration of function
string infixToPostfix(string s);
bool isCharacter(char c);
bool isOperator(char c);
int checkPrecedence(char c);

int main()
{
    string s = "A+B*C-D*E";
    cout << "Your answer is ::  " << infixToPostfix(s) << endl;
    return 0;
}

string infixToPostfix(string s)
{
    string post = "";
    stack<char> st;

    for (int i = 0; i < s.length(); i++)
    {
        if (isCharacter(s[i]))
        {
            post += s[i];
        }
        else if (isOperator(s[i]))
        {
            if (st.empty())
            {
                st.push(s[i]); //if stack is empty then push first value for not having error
            }
            else
            {
                if (checkPrecedence(st.top()) > checkPrecedence(s[i]))
                {
                    while (!st.empty()) // if precendence is higher then append into postfix notation
                    {
                        post += st.top();
                        st.pop();
                    }
                    st.push(s[i]);
                }
                else
                {
                    st.push(s[i]);
                }
            }
        }
    }

    while (!st.empty())
    {
        post += st.top();
        st.pop();
    }

    return post;
}

bool isCharacter(char c)
{
    if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122))
        return true;
    return false;
}

bool isOperator(char c)
{
    if (c == '+' || c == '*' || c == '-' || c == '/')
        return true;
    return false;
}

int checkPrecedence(char c)
{
    if (c == '+' || c == '-')
        return 1;

    else if (c == '*' || c == '/')
        return 2;
    return -1;
}
