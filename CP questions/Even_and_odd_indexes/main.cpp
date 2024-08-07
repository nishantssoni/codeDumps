#include<bits/stdc++.h>
using namespace std;

int main(){
	int size,odd_sum=0, even_sum=0;
	cin>>size;

	int arr[size];
	
	for(int i=0; i<size; ++i){
		cin>>arr[i];
		
		if((i%2) == 0 && (arr[i]%2 == 0)){
			even_sum += arr[i];
		}
		if((i%2) != 0 && (arr[i]%2 != 0)){
			odd_sum += arr[i];
		}
	}
	cout<<even_sum<<" "<<odd_sum;

	return 0;
}
