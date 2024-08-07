#include<bits/stdc++.h>
using namespace std;

int main(){
	
	int n,max_profit=0;
	cin>>n;

	int* arr = new int[n];

	for(int i=0; i<n; ++i) cin>>arr[i];

	for(int i=0; i<n; ++i){
		for(int j=i; j<n; ++j){
			int c_profit = arr[j] - arr[i];
			
			if(c_profit > max_profit) max_profit = c_profit;
		}
	}

	cout<<max_profit;


	return 0;
}
