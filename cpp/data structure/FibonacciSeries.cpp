//NISHANT SHEKHAR SONI 12:40
//Fibonacci Series
#include<iostream>
using namespace std;

int Fbs(int num){

    if(num >2){
        return Fbs(num-1)+Fbs(num-2);
    }
    return 1;
}

void FbsDemos(int num){
    int a = 1;
    cout<<"your series is :: ";
    while(num>=a){
        cout<<Fbs(a)<<" , ";
        ++a;
    }
    cout<<endl;

}

int main(){
    int a = 10;
    FbsDemos(a);
    cout<<" your "<<a<<"th term of Fibonacci Series is  :: "<<Fbs(a)<<endl;

}
