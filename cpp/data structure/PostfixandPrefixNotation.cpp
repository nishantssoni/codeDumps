#include <bits/stdc++.h>
using namespace std;

int doOperation(char ch, int a, int b);

bool isNumeric(char c);

bool isOperator(char c);

int Postfix(string str);

int Prefix(string str);

int main()
{

    string exp_1 = "2 3 * 5 4 * + 9 - ";
    string exp_2 = "- + * 2 3 * 5 4 9 ";

    cout << Postfix(exp_1) << endl;

    cout << Prefix(exp_2) << endl;

    return 0;
}

int Postfix(string str)
{

    stack<int> st;

    for (int i = 0; i < str.length(); i++)
    {
        if (str[i] == ' ' || str[i] == ',')
            continue;

        if (isOperator(str[i]))
        {
            int a = st.top();
            st.pop();
            int b = st.top();
            st.pop();
            int result = doOperation(str[i], b, a);
            st.push(result);
        }
        else if (isNumeric(str[i]))
        {
            int operand = 0;
            while (i < str.length() && isNumeric(str[i]))
            {
                operand = (operand * 10) + (str[i] - '0');
                ++i;
                cout << " i is :: " << i << endl;
            }
            --i;
            st.push(operand);
        }
    }

    return st.top();
}

int Prefix(string str)
{
    stack<int> st;

    for (int i = str.length() - 1; i >= 0; --i)
    {
        if (str[i] == ' ' || str[i] == ',')
            continue;

        if (isOperator(str[i]))
        {

            int a = st.top();
            st.pop();
            int b = st.top();
            st.pop();
            int result = doOperation(str[i], a, b);
            st.push(result);
        }
        else if (isNumeric(str[i]))
        {
            int operand = (str[i] - '0'), placeValue = 0;

            while (i >= 0 && isNumeric(str[i]))
            {
                operand += (10 * placeValue * (str[i] - '0'));
                ++placeValue;
                --i;
            }
            ++i;
            st.push(operand);
        }
    }

    return st.top();
}

int doOperation(char ch, int a, int b)
{
    if (ch == '+')
    {
        return a + b;
    }
    else if (ch == '-')
    {
        return a - b;
    }
    else if (ch == '*')
    {
        return a * b;
    }
    else if (ch == '/')
    {
        return a / b;
    }
    return 0;
}

bool isNumeric(char c)
{
    if (c >= '0' && c <= '9')
        return true;
    return false;
}

bool isOperator(char c)
{
    if (c == '+' || c == '-' || c == '*' || c == '/')
        return true;
    return false;
}
