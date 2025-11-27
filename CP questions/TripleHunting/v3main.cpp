
/*
input:
7
10
12
8
9
3
29
30


output:
4
2 4 5 7 

*/


#include<bits/stdc++.h>
using namespace std;

int main(){
    freopen("input.txt","r",stdin);
    freopen("output.txt", "w", stdout);

    int n, k=0;
    cin>>n;
    int arr[n];
    int tri[n];
    for(int i =0;i<n;i++) cin>>arr[i];

    for(int i=0; i<n; i++){
        if(!(arr[i] % 3)) tri[k++] = i+1;
    }

    if(!k) cout<<"Nothing here!";
    else{
        cout<<k<<endl;
        for(int i = 0; i<k; i++) cout<<tri[i]<<" ";
    }

    return 0;
}