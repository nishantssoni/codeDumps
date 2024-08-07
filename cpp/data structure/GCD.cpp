//NISHANT SHEKHAR SONI 11:44 MAY 21 2020
//GCD
#include<iostream>
using namespace std;

int GCD(int a, int b){

    if(a == b)
        return a;
    if(a == 0)
        return b;
    if(b == 0)
        return a;

    if(a > b)
        return GCD(a%b , b);
    else
        return GCD(a , b%a);

    return 0;
}

void GCDdemons(int a ,int b){
    int store;

if (a==0 || b==0)
    cout<<endl<<" write counting numbers "<<endl;

while(a!=0 || b!=0){
    if(a>b){
        cout<<endl<<a<<" = "<<b<<" X "<<a/b<<" + "<<a%b<<endl;
        store = a;
        a = b;
        b = store%b;}
    if(b>a){
        cout<<endl<<b<<" = "<<a<<" X "<<b/a<<" + "<<b%a<<endl;
        store = b;
        b = a;
        a = store%b;}
}


}

int main(){
    int num1 = 12;
    int num2 = 10;
    int gcd  = GCD(num1, num2);

    GCDdemons(num1,num2);
    
    cout<<endl<<"your GCD of number "<<num1<<" and "<<num2<<" is "<<gcd<<endl;
   
}
