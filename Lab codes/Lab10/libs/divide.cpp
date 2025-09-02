#include "function.h"
#include<iostream>
using namespace std;
double divide(int a, int b) {
    if (b == 0) {
        cout<<"b can't be 0!"<<endl;
        return -1;
    }
    return static_cast<double>(a) / b;
}