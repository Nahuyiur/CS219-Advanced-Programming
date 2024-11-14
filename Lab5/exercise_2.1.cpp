#include<iostream>
using namespace std;
int main(){
    char ch;
    int counta=0,counte=0,counto=0,countu=0,counti=0;
    cout<<"Enter text(print Enter to finish):"<<endl;
    while(cin.get(ch)){
        if(ch=='\n')break;
        if(ch=='A'||ch=='a')counta++;
        if(ch=='E'||ch=='e')counte++;
        if(ch=='O'||ch=='o')counto++;
        if(ch=='U'||ch=='u')countu++;
        if(ch=='I'||ch=='i')counti++;
    }

    int total=counta+counte+counti+counto+countu;
    printf("Number of vowels: %d\n",total);
    return 0;
}