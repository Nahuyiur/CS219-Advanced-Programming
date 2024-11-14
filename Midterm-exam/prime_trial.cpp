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
    for (size_t i = 0; i < 100; i++)
    {
        int ans=i*i-79*i+1601;
        if(!isPrime(ans)){
            cout<<"i="<<i<<"   "<<ans<<endl;
        }
    }
    
    return 0;
}