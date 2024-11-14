#include<iostream>
#include<iomanip>
#include<cstring>
#include<stdlib.h>
#include<sstream>
#include<math.h>
using namespace std;
bool static isPrime(int n){
    if(n==1)return false;
    for(int i=2;i<=sqrt(n);i++){
        if(n%i==0)return false;
    }
    return true;
}
int main(){
    int n;
    cin>>n;

    int a[100];
    a[1]=0;
    a[2]=1;
    for (size_t i = 3; i <=n; i++)
    {
        a[i]=a[i-2]+a[i-1];
        if(isPrime(a[i]))cout<<a[i]<<" ";
    }
    cout<<endl;
    return 0;
}