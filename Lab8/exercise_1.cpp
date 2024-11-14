#include<iostream>
#include<stdlib.h>
#include<cstring>
#include<fstream>
using namespace std;
int main(){
    string s="cs219 test";
    char* p=&s[0];
    cout<<*(p+2)<<endl;

    int* p6=new int[10];
    for(int i=0;i<10;i++){
        p6[i]=i-1;
    }
    cout<<p6[4]<<endl;
    
    const char* str="jack";

    char* ss=new char[10];
    strncpy(ss,str,2);
    ss[2]='\0';
    cout<<ss<<endl;
    delete []ss;

    ofstream outfile;
    outfile.open("test.txt",ios::out);
    if(outfile.is_open())outfile<<"hello!";
    outfile.close();

    return 0;
}