#include <iostream> 
using namespace std; 
int main()
{
    int *pArr;
    pArr=new int[5];

    for(int i=0;i<5;i++){
        *pArr=i;
        pArr++;
    }


    for(int i=0;i<5;i++){
        pArr--;
        cout<<*pArr<<" ";
    }
    delete [] pArr;
    return 0;
}