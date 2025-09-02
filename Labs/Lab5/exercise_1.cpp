#include<iostream>
using namespace std;
int main(){
    cout<<"Enter words (type 'done' to finish)"<<endl;
    string word;
    int count=0;

    while(true){
        cin>>word;
        if(word=="done")break;
        count++;
        
    }

    printf("Number of words entered: %d\n",count);
    return 0;
}