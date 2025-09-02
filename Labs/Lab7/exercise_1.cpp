#include <iostream>
#include <cstring> // for strlen(), strcpy()
using namespace std;
struct stringy
{
    char *str; // points to a string
    int ct;    // length of string (not counting '\0')
};

// prototypes for set() and two overloading functions show() with default arguments
void set(stringy &beany, char *test)
{
    beany.ct = strlen(test);
    beany.str = new char[beany.ct + 1];
    strcpy(beany.str, test);
}
void show(stringy beany)
{
    for (size_t i = 0; i < beany.ct; i++)
    {
        cout << beany.str[i];
    }
    cout << endl;
}
void show(stringy beany, int n)
{
    for (size_t k = 0; k < n; k++)
    {
        for (size_t i = 0; i < beany.ct; i++)
        {
            cout << beany.str[i];
        }
        cout << endl;
    }
}
void show(char *testing)
{
    for (size_t i = 0; i < strlen(testing); i++)
    {
        cout << testing[i];
    }
    cout << endl;
}
void show(char *testing, int n)
{
    for (int k = 0; k < n; k++)
    {
        for (size_t i = 0; i < strlen(testing); i++)
        {
            cout << testing[i];
        }
        cout << endl;
    }
}
void show(const char* str){
    cout<<str<<endl;
}
int main()
{
    stringy beany;
    char testing[] = "Reality isn't what it used to be.";

    set(beany, testing); // first argument is a reference,
                         // allocates space to hold copy of testing,
                         // sets str member of beany to point to the
                         // new block, copies testing to the new block,
                         // and sets ct member of beany

    show(beany);    // prints member string once
    show(beany, 2); // prints member string twice

    testing[0] = 'D';
    testing[1] = 'u';
    show(testing);    // prints testing string once
    show(testing, 3); // prints test string thrice
    show("Done!");    // prints "Done" on the screen

    // free the memory
    delete[] beany.str;
    return 0;
}
