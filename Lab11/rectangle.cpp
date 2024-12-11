#include <iostream>
#include "rectangle.h"
using namespace std;
int Rectangle::countOfObject=0;
Rectangle::Rectangle(){
    width=1;
    height=1;
    countOfObject++;
}
Rectangle::Rectangle(double w,double h){
    width=w;
    height=h;
    countOfObject++;
}
double Rectangle::getWidth(){
    return width;
}
double Rectangle::getHeight(){
    return height;
}
void Rectangle::setWidth(double w){
    width=w;
}
void Rectangle::setHeight(double h){
    height=h;
}
double Rectangle::getArea(){
    return width*height;
}
double Rectangle::getPerimeter(){
    return 2*(width+height);
}
int Rectangle::getCount(){
    return countOfObject;
}
void Rectangle::display(){
    cout<<"Rectangle "<<countOfObject<<endl;
    cout<<"----------------------------"<<endl;
    cout<<"Width:"<<"   "<<getWidth()<<endl;
    cout<<"Height:"<<"   "<<getHeight()<<endl;
    cout<<"Perimeter:"<<"   "<<getPerimeter()<<endl;
    cout<<"Area:"<<"   "<<getArea()<<endl;
}
