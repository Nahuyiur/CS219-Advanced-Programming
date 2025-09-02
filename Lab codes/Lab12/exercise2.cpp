#include <iostream>
#include <memory>

using namespace std;

int main()
{
    double *p_reg = new double(5);
    shared_ptr<double> pd;
    // 直接构造 shared_ptr
    pd = shared_ptr<double>(p_reg);
    cout << "*pd = " << *pd << endl;

    // 用`make_shared` 创建并初始化一个新的对象
    shared_ptr<double> pshared(p_reg); 
    cout << "*pshared = " << *pshared << endl;

    // 不能将栈上的对象交给 shared_ptr 管理
    string *str = new string("Hello World!");
    shared_ptr<string> pstr(str); // 将指针 `str` 动态分配后传递给 shared_ptr
    cout << "*pstr = " << *pstr << endl;

    return 0;
}