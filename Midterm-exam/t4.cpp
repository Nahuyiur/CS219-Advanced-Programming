#include<iostream>
#include<iomanip>
#include<cstring>
#include<stdlib.h>
#include<sstream>
using namespace std;

int main(){
    char pp;
    cin>>pp;

    char chess[8][8];
    int x=-1,y=-1;
    for (int i = 0; i < 8; i++)
    {
        for(int j=0;j<8;j++){
            cin>>chess[i][j];
            if(chess[i][j]=='E'){
                x=i;y=j;
            }
        }
    }


    //在这里修改好棋盘
    int i,j;
    chess[x][y]=pp;
    //right
    bool flag1=false;
    
    i=x,j=y+1;
    while(i>=0&&j>=0&&i<8&&j<8){
        if(chess[i][j]==pp){
            flag1=true;
            break;
        }
        j++;
    }

    if(flag1){
        while(j>y){
            chess[i][j]=pp;
            j--;
        }
    }
    //left
    bool flag2=false;
    
    i=x,j=y-1;
    while(i>=0&&j>=0&&i<8&&j<8){
        if(chess[i][j]==pp){
            flag2=true;
            break;
        }
        j--;
    }

    if(flag2){
        while(j<y){
            chess[i][j]=pp;
            j++;
        }
    }

    //up
    bool flag3=false;
    
    i=x-1,j=y;
    while(i>=0&&j>=0&&i<8&&j<8){
        if(chess[i][j]==pp){
            flag3=true;
            break;
        }
        i--;
    }

    if(flag3){
        while(i<x){
            chess[i][j]=pp;
            i++;
        }
    }
    //down
    bool flag4=false;
    
    i=x+1,j=y;
    while(i>=0&&j>=0&&i<8&&j<8){
        if(chess[i][j]==pp){
            flag4=true;
            break;
        }
        i++;
    }

    if(flag4){
        while(i>x){
            chess[i][j]=pp;
            i--;
        }
    }

    //up-left
    //下面输出
    bool flag5=false;
    
    i=x+1,j=y+1;
    while(i>=0&&j>=0&&i<8&&j<8){
        if(chess[i][j]==pp){
            flag5=true;
            break;
        }
        i++;j++;
    }

    if(flag5){
        while(i>x){
            chess[i][j]=pp;
            i--;j--;
        }
    }
    //6
    bool flag6=false;
    
    i=x+1,j=y-1;
    while(i>=0&&j>=0&&i<8&&j<8){
        if(chess[i][j]==pp){
            flag6=true;
            break;
        }
        i++;j--;
    }

    if(flag6){
        while(i>x){
            chess[i][j]=pp;
            i--;j++;
        }
    }

    //7
    bool flag7=false;
    
    i=x-1,j=y-1;
    while(i>=0&&j>=0&&i<8&&j<8){
        if(chess[i][j]==pp){
            flag7=true;
            break;
        }
        i--;j--;
    }

    if(flag7){
        while(i<x){
            chess[i][j]=pp;
            i++;j++;
        }
    }
    //8
    //7
    bool flag8=false;
    
    i=x-1,j=y+1;
    while(i>=0&&j>=0&&i<8&&j<8){
        if(chess[i][j]==pp){
            flag8=true;
            break;
        }
        i--;j++;
    }

    if(flag8){
        while(i<x){
            chess[i][j]=pp;
            i++;j--;
        }
    }




    int wcount=0,bcount=0;
    for (int i = 0; i < 8; i++)
    {
        for(int j=0;j<8;j++){
            if(chess[i][j]=='W'){
                wcount++;
            }else{
                bcount++;
            }
        }
    }

    if(wcount>bcount){
        cout<<"W"<<endl;
    }else if(wcount<bcount){
        cout<<"B"<<endl;
    }else{
        cout<<"Tie"<<endl;
    }


    for (int i = 0; i < 8; i++)
    {
        for(int j=0;j<8;j++){
            cout<<chess[i][j]<<" ";
        }
        cout<<endl;
    }
    return 0;
}