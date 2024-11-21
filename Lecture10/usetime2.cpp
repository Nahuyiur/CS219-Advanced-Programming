// usetime2.cpp -- using the third draft of the Time class
// compile usetime2.cpp and mytime2.cpp together
#include <iostream>
#include "mytime2.h"

//friend Time operator*( double mult, Time in);

int main()
{
    using std::cout;
    using std::endl;

    Time weeding(4, 35);
    Time waxing(2, 47);
    Time total;
    Time diff;
    Time adjusted, adjusted2;

    cout << "weeding time = ";
    weeding.Show();
    cout << endl;

    cout << "waxing time = ";
    waxing.Show();
    cout << endl;

    cout << "total work time = ";
    total = weeding + waxing; // use operator+()
    total.Show();
    cout << endl;

    diff = weeding - waxing; // use operator-()
    cout << "weeding time - waxing time = ";
    diff.Show();
    cout << endl;

    adjusted = total * 1.5; // use operator+()
    cout << "adjusted work time = ";
    adjusted.Show();
    cout << endl;
    
    /*
    adjusted2 = 2.5 * total; // use friend operator+()
    cout << "secondly adjusted work time = ";
    adjusted2.Show();
    cout << endl;
    */

    return 0;
}
