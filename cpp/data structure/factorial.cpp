//NISHANT SHEKHAR SONI 12:40
//Factorial
#include<iostream>
using namespace std;

int Fact(int a){
    if(a==1){
        return a;
    }
    return a*Fact(a-1);
}


int main(){

    cout<<"you factorial is  :: "<<Fact(5)<<endl;

}
