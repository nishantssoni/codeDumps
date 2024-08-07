//NISHANT SHEKHAR SONI 11:44 MAY 21 2020
//PNC
#include <iostream>
#include <string>
using namespace std;

string swap(string k, int a, int b)
{
    char store = k[a];
    k[a] = k[b];
    k[b] = store;
    return k;
}
void PNC(string name, int i, int n)
{
    int j;
    static int count;

    if (i == n)
    {
        ++count;
        cout << count << " . " << name << endl;
    }

    else
    {
        for (j = i; j <= n; j++)
        {
            name = swap(name, i, j);
            PNC(name, i + 1, n);
            name = swap(name, i, j); //backtracking
        }
    }
}

int main()
{
    string name = "ABCD";

    PNC(name, 0, name.length() - 1);
}
