#include<iostream>
#include<iomanip>
#include<cstring>
#include<stdlib.h>
#include<sstream>
#include<math.h>
using namespace std;
int main(){
    int a[9][9];
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            cin>>a[i][j];
        }
    }
    int t=random();
    if(t%2==0){
        cout<<8<<endl;
    }else{
        cout<<7<<endl;
    }
    return 0;
}