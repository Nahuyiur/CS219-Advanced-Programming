#include<iostream>
#include<iomanip>
#include<cstring>
#include<stdlib.h>
#include<sstream>
using namespace std;
bool judge(int m,int n,int a,int b){
    if(a<0||a>=m)return false;
    if(b<0||b>=n)return false;
    return true;
}
int finds(int m,int n,int a,int b,int k){
    
    while(true){
        int ii=a-k,jj=b-k;
        if(judge(m,n,ii,jj))return k;
        k--;
    }
}
int findf(int m,int n,int a,int b,int k){
    
    while(true){
        int ii=a+k,jj=b+k;
        if(judge(m,n,ii,jj))return k;
        k--;
    }
}
int main(){
    int m,n;
    cin>>m>>n;

    int a[m][n];
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            cin>>a[i][j];
        }
    }
    
    int k;
    cin>>k;

    int ans[m][n];
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            ans[i][j]=0;
            int start=finds(m,n,i,j,k),final=findf(m,n,i,j,k);
            //cout<<i<<","<<j<<":"<<"s="<<start<<","<<"f="<<final<<endl;
            for(int t=(0-start);t<=final;t++){  
                    ans[i][j]+=a[i+t][j+t];
            }
            cout<<ans[i][j]<<" ";
        }
        cout<<endl;
    }
    
    // for(int i=0;i<m;i++){
    //     for(int j=0;j<n;j++){
    //         cout<<ans[i][j]<<" ";
    //     }
    //     cout<<endl;
    // }
    return 0;
}