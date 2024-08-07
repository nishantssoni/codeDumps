#include<iostream>
using namespace std;

void print_ptr(int* ptr,int n){
	for(int i=0; i<n; i++) cout<<*(ptr+i)<<endl;
}


int pop(int* ptr,int* size, int index){
	int pop_item,count=0;

	for(int i=0; i<*size; i++){
		
		if(index == i) pop_item = *(ptr + i++);
		
		*(ptr + count++) = *(ptr + i);
	}
	*(size) = *(size) - 1;
	return pop_item;
}

int main(){
	int inpt;
	cin>>inpt;

	int* n = &inpt;

	int arr[*(n)];
	int* ptr = arr;

	for(int i=0; i<*n; i++) arr[i] = (1+i);

	print_ptr(ptr,*(n));

	cout<<"Your pop item is :: "<<pop(ptr,n,3)<<endl;
	cout<<"Your count item is :: "<<*(n)<<endl;
	print_ptr(ptr,*(n));

	
	
	
	return 0;
}
