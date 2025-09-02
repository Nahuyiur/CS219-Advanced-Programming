#include<iostream>
#include <cstring>
using namespace std;

struct point
{
    int p1;
    int p2;
};

template <typename T>
T maxn(T* arr,int n){
    T maxx=arr[0];
    for (size_t i = 1; i < n; i++)
    {
        if(arr[i]>maxx)maxx=arr[i];
    }
    return maxx;
}
template<>
const char* maxn<const char*>(const char* arr[], int n) {
    const char* lstr=arr[0];
    int lmax=strlen(arr[0]);

    for (size_t i = 1; i < n; i++)
    {
        int len=strlen(arr[i]);
        if(len>lmax){
            lmax=len;
            lstr=arr[i];
        }
    }
    return lstr;
}
const point* maxn(const point* pp1,const point* pp2){
    int t1=pp1->p1+pp1->p2;
    int t2=pp2->p1+pp2->p2;
    if(t1>t2)return pp1;
    return pp2;
}
int main() {
    int a1[] = {1, 2, 3, 4, 5};
    cout << "Max int is: " << maxn(a1, 5) <<endl;

    double a2[] = {1.1, 2.7, -3.5, -2.0};
    cout << "Max double is:" << maxn(a2, 4) <<endl;

    const char* a3[] = {"this", "no body", "morning", "birds", "sky"};
    cout << "Longest string is:" << maxn(a3, 5) <<endl;

    point point1{100,100};
    point point2{40,10};
    cout<<maxn(&point1,&point2)->p1<<" "<<maxn(&point1,&point2)->p2<<endl;
    return 0;
}