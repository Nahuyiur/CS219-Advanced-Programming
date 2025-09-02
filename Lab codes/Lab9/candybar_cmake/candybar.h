#ifndef EXC_CANDYBAR_H
#define EXC_CANDYBAR_H
#include <iostream>

const int LEN = 30;
struct CandyBar {
    char brand[LEN];
    double weight;
    int calorie;
};

// Prompt the user to enter the preceding items of
// information and store them in the CandyBar structure
void setCandyBar(CandyBar & cb);
void setCandyBar(CandyBar * cb);
void showCandyBar(const CandyBar & cb);
void showCandyBar(const CandyBar * cb);

#endif // EXC_CANDYBAR_H