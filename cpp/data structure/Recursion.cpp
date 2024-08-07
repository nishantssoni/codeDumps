//NISHANT SHEKHAR SONI
//Recursion
#include<iostream>
using namespace std;

int sumByRecc(int i){
    if(i ==1){      //base case
        return i;  //terminator
    }
      return i+sumByRecc(i-1);
}


int main(){
 cout<<"sum is  "<<sumByRecc(10)<<endl;
}
