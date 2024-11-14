#include <iostream>
#include <cstring>
#include <algorithm>
using namespace std;
int main() {
    char str[30000];
    cin.getline(str,30000);

    char words[1000][30];
    int counts[1000]={0};
    int wordnum=0;

    char *token=strtok(str,",");
    while(token!=nullptr){
        bool flg=false;
        for (int i = 0; i < wordnum; i++){
            if(strcmp(words[i],token)==0){
                counts[i]++;
                flg=true;
                break;
            }
        }
        if(!flg){
            strcpy(words[wordnum],token);
            counts[wordnum]++;
            wordnum++;
        }
        token=strtok(nullptr,",");
    }

    for(int i=0;i<wordnum;i++){
        for(int j=i+1;j<wordnum;j++){
            if(strcmp(words[i],words[j])>0){
                char temp[30];
                strcpy(temp,words[i]);
                strcpy(words[i],words[j]);
                strcpy(words[j],temp);

                int t=counts[i];
                counts[i]=counts[j];
                counts[j]=t;
            }
        }
    }

    for(int i=0;i<wordnum;i++){
        cout<<words[i]<<": "<<counts[i]<<endl;
    }
    return 0;
}
