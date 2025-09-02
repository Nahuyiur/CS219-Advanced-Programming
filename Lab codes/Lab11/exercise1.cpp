#include<iostream>
#include "rectangle.h"
using namespace std;
int main(){
    Rectangle r1;

    r1.display();
    Rectangle r2(4,35.2);
    r2.display();

    cout<<"The number of rectangles is: "<<Rectangle::countOfObject<<endl;
    return 0;
}