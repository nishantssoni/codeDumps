
/*
input:
6
5 6
0 1
1 4
5 4
1 2
4 0


output:
1
4

*/


#include<bits/stdc++.h>
using namespace std;

int main(){
    freopen("input.txt","r",stdin);
    freopen("output.txt", "w", stdout);

    int arr[1001]={0};
    int n,big=0;
    cin>>n;
    
    for(int i =0;i<n;i++){
        int a, b;
        cin>>a>>b;
        ++arr[a];
        ++arr[b];

        if (arr[a] > big) big = arr[a];
        if (arr[b] > big) big = arr[b];
    }

    for(int i=0; i<1001; i++){
        if(arr[i] == big) cout<<i<<endl;
    }

    return 0;
}