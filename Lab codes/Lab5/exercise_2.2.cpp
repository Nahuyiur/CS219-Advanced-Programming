#include<iostream>
using namespace std;
int main(){
    char ch;
    int counta=0,counte=0,counto=0,countu=0,counti=0;
    cout<<"Enter text(print Enter to finish):"<<endl;
    while(cin.get(ch)){
        if(ch=='\n')break;
        switch (ch)
        {
        case 'A':
            counta++;break;
        case 'a':
            counta++;break;
        case 'O':
            counto++;break;
        case 'o':
            counto++;break;
        case 'E':
            counte++;break;
        case 'e':
            counte++;break;
        case 'I':
            counti++;break;
        case 'i':
            counti++;break;
        case 'U':
            countu++;break;
        case 'u':
            countu++;break;
        }
        
    }

    int total=counta+counte+counti+counto+countu;
    printf("Number of vowels: %d\n",total);
    cout<<"a: "<<counta<<endl;
    cout<<"e: "<<counte<<endl;
    cout<<"i: "<<counti<<endl;
    cout<<"o: "<<counto<<endl;
    cout<<"u: "<<countu<<endl;
    return 0;
}