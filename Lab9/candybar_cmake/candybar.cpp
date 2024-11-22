#include "candybar.h"
#include <iostream>
#include <cstring>
using namespace std;
void setCandyBar(CandyBar &cb) {
    cout << "Call the set function of Passing by reference:\n";
    cout << "Enter brand name of a Candy bar: ";
    cin.ignore();
    cin.getline(cb.brand, LEN);
    cout << "Enter weight of the Candy bar: ";
    cin >> cb.weight;
    cout << "Enter calories (an integer value) in the Candy bar: ";
    cin >> cb.calorie;
}

void setCandyBar(CandyBar *cb) {
    cout << "Call the set function of Passing by pointer:\n";
    cout << "Enter brand name of a Candy bar: ";
    cin.ignore();
    cin.getline(cb->brand, LEN);
    cout << "Enter weight of the Candy bar: ";
    cin >> cb->weight;
    cout << "Enter calories (an integer value) in the Candy bar: ";
    cin >> cb->calorie;
}

void showCandyBar(const CandyBar &cb) {
    cout << "Call the show function of Passing by reference:\n";
    cout << "Brand: " << cb.brand << "\n";
    cout << "Weight: " << cb.weight << "\n";
    cout << "Calories: " << cb.calorie << "\n";
}

void showCandyBar(const CandyBar *cb) {
    cout << "Call the show function of Passing by pointer:\n";
    cout << "Brand: " << cb->brand << "\n";
    cout << "Weight: " << cb->weight << "\n";
    cout << "Calories: " << cb->calorie << "\n";
}