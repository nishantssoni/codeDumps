#include<bits/stdc++.h>
using namespace std;

int main()
{
	int size;
	cin>>size;

	int arr[1001];

	for(int i=0; i<1001; ++i) arr[i] = 0;

	for(int i=0; i<size; ++i){
		
		int f1,f2;
		cin>>f1>>f2;

		++arr[f1];
		++arr[f2];
	}
	int max = 0;
	for(int i=0; i<1001; ++i){
		if(max < arr[i]) max = arr[i];
	}
	for(int i=0; i<1001; ++i){
		if(arr[i] == max) cout<<i<<endl;
	}
    
return 0;
}
