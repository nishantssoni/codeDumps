#include<bits/stdc++.h>
using namespace std;

int main(){
	int n,sum=0;
	cin>>n;
	
	int** arr = new int*[n];
	
	for(int i=0; i<n; ++i){
		arr[i] = new int[n];
		for(int j=0; j<n; ++j){
			cin>>arr[i][j];
		}}
	for(int i=0; i < n; ++i){
		for(int j=0; j < n; ++j){
                 	if(i == 0 || i == n-1){
				//cout<<arr[i][j]<<" ";
				sum +=arr[i][j];
			}
			else if(j==0 || j==n-1){
				//cout<<arr[i][j]<<" ";
				sum +=arr[i][j];
			}
			else if(i != 0 || i != n-1){
				if(i == j || (j==(n-1-i))){
					//cout<<arr[i][j]<<" ";
					sum +=arr[i][j];
				}
			}
			
		}
	}
	cout<<"your sum is :: "<<sum<<endl;
	return 0;
}
