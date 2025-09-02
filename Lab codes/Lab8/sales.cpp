#include <iostream>
#include "sales.h"
#include <algorithm> 
#include <numeric>  
using namespace std;
namespace SALES {
    void setSales(Sales& s, const double ar[], int n) {
        double total = 0.0;

        for (int i = 0; i < n; ++i) {
            s.sales[i] = ar[i];
            total += ar[i];
        }
        s.average = total / n;
        s.max = *max_element(s.sales, s.sales + n);
        s.min = *min_element(s.sales, s.sales + n);
    }

    void showSales(const Sales& s, int n) {
        cout << "Sales:";
        for (int i = 0; i < n; ++i) {
            cout << s.sales[i] <<" ";
        }
        cout<<endl;

        cout << "Average: " << s.average << endl;
        cout << "Max: " << s.max << endl;
        cout << "Min: " << s.min << endl;
    }
}