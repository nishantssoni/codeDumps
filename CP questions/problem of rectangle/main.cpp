#include<bits/stdc++.h>
using namespace std;

int main(){

	// 1 1 3 4
	// 2 3 6 7
	// total area is :: 21
	int fx1,fy1,fx2,fy2;
	int sx1,sy1,sx2,sy2;
	int tin, bin, lin, rin;
	int total_area, in_area;

	cin>>fx1>>fy1>>fx2>>fy2;
	int area1 = (fy2-fy1) * (fx2-fx1);

	cin>>sx1>>sy1>>sx2>>sy2;
	int area2 = (sy2-sy1) * (sx2-sx1);
	
	tin = min(fy2, sy2);
	bin = max(fy1, sy1);
	lin = max(fx1, sx1);
	rin = min(fx2, sx2);
	
	if(tin < bin && lin > rin){
		in_area = 0;
	}else{
		in_area = (tin - bin) * (rin - lin);		
	}
	
	total_area = (area1 + area2) - in_area;
	cout<<total_area<<endl;

	return 0;
}
