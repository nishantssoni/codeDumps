#include<bits/stdc++.h>
using namespace std;

int moon(int a, int b){
    if( b % 11 == 1){
        cout<<a<<" "<<b;
        return 0;
    }
    moon(++a,(2*b));
    return 0;
}

int main(){
    freopen("input.txt","r",stdin);
    freopen("output.txt", "w", stdout);

    int a;
    cin>>a;
    moon(0, a);

    return 0;
}