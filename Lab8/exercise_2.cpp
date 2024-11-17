#include "match.h"
#include<iostream>
using namespace std;
int main(){
    string str;
    char ch;
    cout<<"Please input a string: ";
    getline(cin,str);
    cout<<"Please input a character: ";
    cin>>ch;

    char* s=&str[0];
    
    if(match(s,ch)!=NULL){
        cout<<match(s,ch)<<endl;
    }else{
        cout<<"Not Found"<<endl;
    }
    
    return 0;
}