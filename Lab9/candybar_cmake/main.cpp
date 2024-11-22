#include "candybar.h"
#include <iostream>
using namespace std;
int main() {
    CandyBar cb1, cb2;
    //指针
    setCandyBar(cb1);
    //引用
    setCandyBar(&cb2);
    cout<<endl;
    
    showCandyBar(cb1);
    cout<<endl;
    showCandyBar(&cb2);

    return 0;
}