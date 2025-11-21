#include<bits/stdc++.h>
using namespace std;

int max(int a, int b){
    return a>b?a:b;
}
int min(int a, int b){
    return a>b?b:a;
}

int main(){
    freopen("input.txt","r",stdin);
    freopen("output.txt", "w", stdout);

    int ax1,ay1,ax2,ay2;
    int bx1,by1,bx2,by2;
    int cx1, cy1, cx2, cy2; 
    cin>>ax1>>ay1>>ax2>>ay2;
    cin>>bx1>>by1>>bx2>>by2;
    cx1 = max(ax1, bx1);
    cx2 = min(ax2, bx2);
    cy1 = max(ay1, by1);
    cy2 = min(ay2, by2);
    int aarea = (ax2 - ax1) * (ay2 - ay1);
    int barea = (bx2 - bx1) * (by2 - by1);
    int carea = (cx2 - cx1) * (cy2 - cy1);
    
    cout<<(aarea + barea - carea);

    return 0;
}
