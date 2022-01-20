



#include <iostream>
#include <numeric>
#include <vector>
using namespace std;

#ifndef option_header
#define option_header

/*
class EuroCallOption {
    protected:
        double K;
        double Sigma;
        double r;
        double T;
}
*/

class Option {
    protected:
    /*

    Add the following Properties:
    double Strike – the strike for the option
    double Sigma – the annual volatility of the option
    double T – the time to expiry of the option
    double RiskFreeRate – the risk free rate associated with the option

    */

        double Strike;
        double Sigma;
        double T;
        double RiskFreeRate;

        

    public:

        Option(double strike, double sigma, double t, double riskFreeRate);

        // Option() = default;

        virtual double getValue(double spot) = 0;

        virtual double getIntrinsicValue(double spot) = 0;

        virtual double calcBlackScholesValue(double spot) = 0;

        double getDelta(double spot);

};

class EuropeanOption : public Option {
        

     public:

        EuropeanOption(double strike, double sigma, double t, double riskFreeRate) : Option(strike, sigma, t, riskFreeRate) {};

        double getValue(double spot);
};


class EuropeanCall : public EuropeanOption {


    public:

    EuropeanCall(double strike, double sigma, double t, double riskFreeRate): EuropeanOption(strike, sigma, t, riskFreeRate) {};

    double getIntrinsicValue(double spot);

    double calcBlackScholesValue(double spot);
     

};

class EuropeanPut : public EuropeanOption {


    
    public:

    EuropeanPut(double strike, double sigma, double t, double riskFreeRate):EuropeanOption(strike, sigma, t, riskFreeRate) {};

    double getIntrinsicValue(double spot);

    double calcBlackScholesValue(double spot);

};

class Position {
public:

    double weight;
    Option* option;

    Position() : weight(0), option(NULL) {}
    Position(double w, Option* o) : weight(w), option(o) {}
};

class Portfolio {

    protected:

    vector<Position> Positions;
    
    
    public:

    Portfolio(vector<Position> positions);

    Portfolio() = default;

    // Position() :
    // vecotor<>

    void addPosition(const Position& pos);

    double getValue(double spot);

    double getDelta(double spot);

    double calcDailyVaR(double spot, double sigma, double riskFreeRate, int N);

    double calcDailyExpectedShortfall(double spot, double sigma, double riskFreeRate, int N);
};


#endif
