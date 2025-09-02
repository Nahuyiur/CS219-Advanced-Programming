#include<iostream>
#include <cctype>
#include <cstring> 
using namespace std;
bool judgeLength(string str){
    if(str.length()>=8)return true;
    return false;
}
bool judgeLower(string str){
    for(char c:str){
        if(islower(c))return true;
    }
    return false;
}
bool judgeUpper(string str){
    for(char c:str){
        if(isupper(c))return true;
    }
    return false;
}
bool judgeDig(string str){
    for(char c:str){
        if(isdigit(c))return true;
    }
    return false;
}
bool judgeSpe(string str){
    for(char c:str){
        if(c=='!'||c=='@'||c=='#'||c=='$'||c=='%'||c=='^'||c=='&'||c=='*'||c=='('||c==')'||c=='-'||c=='+')return true;
    }
    return false;
}
bool judgeCon(string str){
    char arr[str.length()];
    strcpy(arr,str.c_str());
    for(int i=0;i<str.length()-1;i++){
        if(arr[i]==arr[i+1])return false;
    }
    return true;
}
int jud(string str){
    int count=0;
    if(judgeCon(str))count++;
    if(judgeDig(str))count++;
    if(judgeLength(str))count++;
    if(judgeLower(str))count++;
    if(judgeUpper(str))count++;
    if(judgeSpe(str))count++;
    return count;
}
int main(){
    string str;
    getline(cin,str);

    int s=jud(str);
    if(s==6){
        cout<<"Jett"<<endl;
    }else if(s==5){
        cout<<"Raze"<<endl;
    }else if(s==4){
        cout<<"Neon"<<endl;
    }else{
        cout<<"No"<<endl;
    }
    return 0;
}