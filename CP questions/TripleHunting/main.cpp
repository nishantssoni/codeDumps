#include<bits/stdc++.h>
using namespace std;

int lst_count(int arr[],int size){
	int count = 0;
	for(int i = 0; i < size; ++i){
		if(arr[i] % 3 == 0) ++count;
	}
	return count;
}

int main(){
	int size;
	cin>>size;

	int lst[size];
	
	for(int i = 0; i < size; ++i){
	cin>>lst[i];
	}	
	
	int tri_size = lst_count(lst, size);
	int tri_lst[tri_size];
	int tri_count = 0;

	for(int i = 0; i < size; ++i){
		if(lst[i] % 3 == 0){
			tri_lst[tri_count] = (i+1);
			++tri_count;
		}
	}

	cout<<endl<<tri_size<<endl;

	for(int i = 0; i<tri_size; ++i){
	cout<<tri_lst[i];

	}


return 0;
}
