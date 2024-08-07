#include<iostream>
using namespace std;

int power(int x, int y){
	int result = 1;
	
	for(int i=0; i<y; ++i){
		result *= x ;
	}
	return result;
}

int main(){
	int num1 = 5, powr = 5;
	cout<<"your "<<num1<<" ^ "<<powr<<" is "<<power(num1,powr); 
	return 0;
}
