//NISHANT SHEKHAR SONI 12:40
//Tower of hanoi
#include<iostream>
using namespace std;
static int counter = 0;
void TOH(char beg ,char aux,char end,int n){
    if(n>=1){

    TOH(beg,end,aux,n-1);
    cout<<++counter<<". "<<beg<<" ==> "<<end<<endl;
    TOH(aux,beg,end,n-1);

    }
}

int main(){

    int numberOfDisk = 2;
    TOH('A','B','C',numberOfDisk);
}
