#include<bits/stdc++.h>
using namespace std;

int main(){
	int cum1=0,cum2=0,c_lead,lead=0,max=0;
	int n;
	cin>>n;

	while(n--){
		int s1,s2,t_lead;
		cin>> s1 >> s2;
		
		cum1+=s1;
		cum2+=s2;

		if(cum1  > cum2){
			c_lead = s1 - s2;
			t_lead = 1;
		}
		else {
			c_lead = s2 -s1;
			t_lead = 2;
		}

		if(c_lead > max){
			max = c_lead;
			lead = t_lead; 
		}
	}
	cout<<lead<<" "<<max;
	return 0;
}
