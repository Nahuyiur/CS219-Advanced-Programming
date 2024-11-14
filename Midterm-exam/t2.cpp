#include<iostream>
#include<iomanip>
#include<cstring>
#include<stdlib.h>
#include<sstream>
using namespace std;
bool judge(char* s,int n){
    for (size_t i = 0; i <=(n+1)/2; i++)
    {
        char* l=&s[i];
        char* r=&s[n-1-i];
        if(*l!=*r)return false;
    }
    return true;
}
int main(){
    string str;
    getline(cin,str);
    char* sss=&str[0];
    int len=strlen(sss);
    
    int max=1;
    for(int l=0;l<len;l++){
        int t=1;
        char* s=&str[l];
        for(int i=len-l;i>0;i--){
            if(judge(s,i)){
                t=i;
                break;
            }
        }
        if(t>max)max=t;
    }
    
    cout<<max<<endl;
    return 0;
}