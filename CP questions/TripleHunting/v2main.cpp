#include<bits/stdc++.h>
using namespace std;

int main(){
int size;
cin>>size;

int lst[size],t_lst[size];
int count = 0;

for(int i =0; i<size; ++i){
	cin>>lst[i];
	if(lst[i] % 3 == 0){
		t_lst[count] = (i+1);
		++count;
	}
   }
if(count != 0){
	cout<<count<<endl;
	for(int i = 0; i<count; ++i){
		cout<<t_lst[i]<<" ";
	}

}
else{
	cout<<"Nothing here!"<<count;

}


}
