
/*
input:

6 5
1151
723
1321
815
780
931
4
3
4
5
1


output:

815
1321
815
780
1151

*/


#include<bits/stdc++.h>
using namespace std;

int main(){
    freopen("input.txt","r",stdin);
    freopen("output.txt", "w", stdout);

    int n,q;
    cin>>n>>q;
    int arr[n+q];
    for(int i =0;i<n+q;i++) cin>>arr[i];

    for(int i=0; i<q; i++) cout<<arr[arr[n+i]-1]<<endl;

    return 0;
}