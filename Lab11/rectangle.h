#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <iostream>
using namespace std;
class Rectangle{
    private:
        double width;
        double height;
    public:
        static int countOfObject;
        Rectangle();
        Rectangle(double w,double h);
        double getWidth();
        double getHeight();
        void setWidth(double w);
        void setHeight(double h);
        double getArea();
        double getPerimeter();
        int getCount();
        void display();
};

#endif