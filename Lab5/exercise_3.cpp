#include<iostream>
#include<fstream>
using namespace std;
char* toUpper(char* str){
    for(int i=0;str[i]!='\0';i++){
        if(str[i]>='a'&&str[i]<='z'){
            str[i]=str[i]+'A'-'a';
        }
    }
    return str;
}
int main(){
    string str;
    getline(cin,str);

    char* cstr = &str[0];
    toUpper(cstr);

    fstream file1,file2;
    file1.open("f1.txt",ios::out);
    if(file1.is_open()){
        file1<<str<<endl;
        file1.close();
    }else{
        cerr<<"can't open file for writing!"<<endl;
        return 1;
    }

    file2.open("f2.txt",ios::out);
    if(file2.is_open()){
        file2<<cstr<<endl;
        file2.close();
    }else{
        cerr<<"can't open file for writing!"<<endl;
        return 1;
    }

    return 0;
}