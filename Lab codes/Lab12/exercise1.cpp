#include <iostream>
using namespace std;
class Complex
{
private:
    double real;
    double imag;

public:
    Complex() : real(1), imag(1) {}
    Complex(double re, double im) : real(re), imag(im) {}

    friend Complex operator*(double n, Complex &rhs);
    friend Complex operator*(Complex &rhs, double n);
    friend ostream &operator<<(ostream &os, const Complex&rhs);
    friend istream &operator>>(istream &is, Complex &rhs);
    friend Complex operator+(const Complex &lhs, const Complex &rhs);
    friend Complex operator-(const Complex &lhs, const Complex &rhs);
    friend Complex operator*(Complex &rhs1, Complex &rhs2);
    Complex operator~()
    {
        return Complex(this->real, -this->imag);
    }
    bool operator==(const Complex &rhs) const
    {
        return this->real == rhs.real && this->imag == rhs.imag;
    }
    bool operator!=(const Complex &rhs) const
    {
        return !(*this == rhs);
    }
};

Complex operator+(const Complex &lhs, const Complex &rhs)
{
    return Complex(lhs.real + rhs.real, lhs.imag + rhs.imag);
}

Complex operator-(const Complex &lhs, const Complex &rhs)
{
    return Complex(lhs.real - rhs.real, lhs.imag - rhs.imag);
}

Complex operator*(double n, Complex &rhs)
{
    return Complex(rhs.real * n, rhs.imag * n);
}
Complex operator*(Complex &rhs, double n)
{
    return Complex(rhs.real * n, rhs.imag * n);
}
Complex operator*(Complex &rhs1, Complex &rhs2)
{   
    double i=rhs1.imag*rhs2.real+rhs1.real*rhs2.imag;
    double r=rhs1.real*rhs2.real-rhs1.imag*rhs2.imag;
    return Complex(r,i);
}
ostream &operator<<(ostream &os, const Complex &rhs)
{
    os << rhs.real << ((rhs.imag >= 0) ? "+" : "") << rhs.imag << "i";
    return os;
}
istream &operator>>(istream &is, Complex &rhs)
{
    is >> rhs.real >> rhs.imag;
    return is;
}
int main()
{
    Complex a(3, 4);
    Complex b(2, 6);

    cout << "a= " << a << endl;
    cout << "b= " << b << endl;
    cout << "~b= " << ~b << endl;
    cout << "a+b " << a + b << endl;
    cout << "a-b " << a - b << endl;
    cout << "a*b " << a * b << endl;
    cout << "2*a " << 2 * a << endl;
    cout << "a*2 " << a * 2 << endl;

    Complex c = b;
    cout << "b==c ? " << boolalpha << (b == c) << endl;
    cout << "b!=c ? " << (b != c) << endl;
    cout << "a==b ? " << (a == b) << endl;

    Complex d;
    cout << "Enter a complex number(real part and imaginary part):";
    cin >> d;
    cout << d << endl;
    return 0;
}