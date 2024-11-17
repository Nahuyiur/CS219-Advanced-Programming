#include <iostream>
#include "sales.h"
using namespace std;
int main()
{
    SALES::Sales s1;
    cout << "Input n: ";
    int n;
    cin >> n;

    if (n > SALES::QUARTERS)
    {
        cout << "n is not correct." << endl;
        cout << "Aborted" << endl;
        return 0;
    }

    cout << "Please input " << n << " double values: ";
    double salesData[n];
    for(int i=0;i<n;i++){
        cin>>salesData[i];
    }

    SALES::setSales(s1, salesData, n);

    SALES::showSales(s1);

    return 0;
}