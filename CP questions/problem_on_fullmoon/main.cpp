//input : 17
//output: 2 34
#include<iostream>
using namespace std;

int main(){
	int number_of_fruits;
	cin>>number_of_fruits;
	
	bool dis = false;
	int fullmoon = 0;
	while(!dis){
		if(number_of_fruits % 11 == 1){
			dis = true;
			cout<<fullmoon<<" "<<number_of_fruits<<endl;
		}else{
			++fullmoon;
			number_of_fruits *= 2;
		}
	}

return 0;
}
