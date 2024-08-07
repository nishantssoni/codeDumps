#include<bits/stdc++.h>
using namespace std;

int main(){
	int n,dsum;
	cin>>n;

	int nov[n];
	for(int i=0; i<n; ++i) cin>>nov[i];

	for(int i=0; i<(n/2); ++i){
		dsum = nov[i] + nov[(n-1)-i];
		cout<< (dsum / 10)<<" "<<(dsum % 10)<<endl;
	}

	return 0;
}
