#include<iostream>
#include<string.h>
using namespace std;
const char* match(const char* s,char ch){
    for(int i=0;i<strlen(s);i++){
        if(s[0]==ch)return s;
        s++;
    }
    return NULL;
}