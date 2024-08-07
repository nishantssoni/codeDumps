#include<iostream>
using namespace std;

int isnegative(int x){
	if(x >= 0)
		return 0;
	return 1;
}
int place_value(int x){
	int counter = 0;
	while(x){
		x /= 10;
		++counter;
	}
	return counter;
}


void reverse_int(int x){
	int rem,abs_value,rev_int = 0,counter = 0;
	
	counter = place_value(x);
	
	if(isnegative(x)){
 
		abs_value = 1;
		x = -1 * x; 
	}	
	while(x != 0){
		rem = x % 10;
		x /= 10;
		rev_int += (counter * 10) * rem;
		--counter; 		
	}
	
	cout<<"your reverse string is "<<rev_int<<endl;
	}

int main(){
	int num;
	
	cout<<"enter a number :: ";
	cin>>num;
	

	reverse_int(num);

	return 0;
}
