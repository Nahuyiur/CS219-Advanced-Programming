#include<iostream>
using namespace std;
int main(){
    int k=1;
    int p=4762%13;
    for (size_t i = 0; i < 5367; i++)
    {
        k*=p;
        k%=13;
    }
    cout<<p<<endl;
    cout<<k<<endl;
    return 0;
}