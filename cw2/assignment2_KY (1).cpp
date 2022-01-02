// MFE C++ - Assignment 2
// Due 4pm Dec 16, 2021 Pacific Standard Time

// 1. Create a program that has 5 functions:

#include <iostream>
#include <cmath>
#include <algorithm>

using namespace std;


double calculateCallIntrinsicValue(double spot, double strike) {
    return std::max(0.0, (spot - strike));
}


double calculatePutIntrinsicValue(double spot, double strike) {
    return std::max(0.0, (strike - spot));
}

double calculateCallTimeValue(double price, double spot, double strike) {
    return (price - calculateCallIntrinsicValue(spot, strike));
}

double calculatePutTimeValue(double price, double spot, double strike) {
    return (price - calculatePutIntrinsicValue(spot, strike));
}


int main(int argc, char **argv)
{
    for (double spot = 85; spot < 116; spot++) {
        double strike = 95;
        cout << "for strike = 95 " << endl;
        cout << calculateCallIntrinsicValue(spot, strike) << endl;
        cout << calculatePutIntrinsicValue(spot, strike) << endl;
    };
    
    for (double strike = 90; strike < 111; strike++) {
        double spot = 105;
        cout << "for spot = 105 " << endl;
        cout << calculateCallIntrinsicValue(spot, strike) << endl;
        cout << calculatePutIntrinsicValue(spot, strike) << endl;
    };


    {
    int a;
    cout << "Please give me a 0 or a 1: ";
    cin >> a;

    if (a == 0) {
        double price;
        cout << "Please give me an option price: ";
        cin >> price;
        double strike = 100;
        double spot = 95;
        double timeValue = calculatePutTimeValue(price, spot, strike);
            if (timeValue < 0.0) {
                cout << " ERROR " << endl;
            } else {
                cout << timeValue << endl;
            }
        } else if (a==1) {
            double strike;
            cout << "Please give me a strike: ";
            cin >> strike;
            double price = 10;
            double spot = 63.50;
            double timeValue = calculateCallTimeValue(price, spot, strike);
            if (timeValue < 0.0) {
                cout << " ERROR " << endl;
            } else {
                cout << timeValue << endl;
            }
                
        } else {
            cout << " ERROR " << endl;
        }
        return 0;
    };
};
