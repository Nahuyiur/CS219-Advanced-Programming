#ifndef SALES_H
#define SALES_H

namespace SALES {
    const int QUARTERS = 4;

    struct Sales {
        double sales[QUARTERS];
        double average;
        double max;
        double min;
    };

    void setSales(Sales& s, const double ar[], int n = QUARTERS);
    void showSales(const Sales& s, int n = QUARTERS);
}

#endif