#include<iostream>
#include "swap.h"
using namespace std;
int main(){
    cout<<"Please input two integers:";
    int a,b;
    cin>>a>>b;

    cout<<"Before swap: "<<a<<","<<b<<endl;
    swap(a,b);
    cout<<"After  swap: "<<a<<","<<b<<endl;
    return 0;
}