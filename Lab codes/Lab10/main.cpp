#include <iostream>
#include "function.h"
using namespace std;
int main()
{
    cout << "input two integers: ";
    int a, b;
    cin >> a >> b;

    cout << "Add: " << add(a, b) << endl;
    cout << "Sub: " << sub(a, b) << endl;
    cout << "Mul: " << mul(a, b) << endl;
    printf("Div: %f \n",divide(a,b));

    return 0;
}