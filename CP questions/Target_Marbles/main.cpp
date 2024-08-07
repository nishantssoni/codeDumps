#include<bits/stdc++.h>
using namespace std;

int main(){
	
	//nom : number of marbels
	//sum : sum of the values
	int nom,target,sum=0;
	cin>>nom>>target;	
	
	//mv: marbel values
	//cs: continuous subset
	//cs_count: continuous subset count
	int mv[nom],cs[nom];
	int cs_count;
	
	//marbels input
	for(int i=0; i<nom; ++i) cin>>mv[i];

	for(int i=0; i<nom; ++i){
		
		cs_count = 0;
		sum = 0;
		
		for(int j=i; j<nom; ++j){
			if(sum < target){

				sum += mv[j];
				cs[cs_count] = mv[j]; 
				cs_count++;
			}
		}

		if(sum == target) break;
	}
	
	if(sum == target){
		cout<<"true"<<endl;
		for(int i=0; i<cs_count; ++i){
			cout<<cs[i]<<" ";
		}
	}else{
	
		cout<<"false";
	}

	return 0;
}
