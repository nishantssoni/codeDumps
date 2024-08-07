#include<bits/stdc++.h>
using namespace std;

int main(){
	int size;
	cin>>size;
	//list frndlist variable and connection
	int f_lst[size][2];
	int c_lst[size*2][2];
	int c_coutn = 0;

	//filling 0 in loop
	for(int i = 0; i<size*2; ++i){ 
		c_lst[i][1] = 0;
		c_lst[i][0] = -1;
	}

	//main loop
	for(int i = 0; i < size; ++i){
		
		//input
		cin>>f_lst[i][0]>>f_lst[i][1];
		
		//values
		int first = f_lst[i][0];
		int second = f_lst[i][1];
		
		//logic
		for(int j = 0; j<size*2; ++j){
			
			if(first == c_lst[j][0]){
				cout<<endl<<"first == c_lst[j][0]"<<first<<"  "<<c_lst[j][0];
				c_lst[j][1]++;		
			}
			else if(second == c_lst[i][0]){
				cout<<endl<<"secon== c_lst[j][0]"<<second<<"  "<<c_lst[j][0];
				c_lst[j][1]++; }
			
			else{
				c_lst[j][0] = first;
				cout<<endl<<"c_lst[j][0] = first;"<<c_lst[j][0]<<"  "<<first<<endl;
				c_lst[j][1]++;

				c_lst[++j][0] = second;
				c_lst[j][1]++;
				cout<<endl<<"c_lst[++j][0] = second;"<<c_lst[j][0]<<"  "<<second<<endl;
			}		
		}

	}
		cout<<endl;
		for(int i = 0; i<size*2; ++i){
			if(c_lst[i][0]== -1) break;
			cout<<c_lst[i][0]<<" "<<c_lst[i][1]<<endl;
		
		}



return 0;
}
