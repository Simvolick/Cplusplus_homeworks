



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

        virtual double getValue(double spot) = 0;

        virtual double getIntrinsicValue(double spot) = 0;

        virtual double calcBlackScholesValue(double spot)=0;

        double getDelta(double spot){
            return (getValue(spot*1.01) - getValue(spot));
        };

};

class EuropeanOption : public Option {

     public:

        EuropeanOption(double strike, double sigma, double t, double riskFreeRate) : Option(strike, sigma, t, riskFreeRate){};

        virtual double getValue(double spot) {
            return calcBlackScholesValue(spot);
        };
};


class EuropeanCall : public EuropeanOption {

    public:

    EuropeanCall(double strike, double sigma, double t, double riskFreeRate) : EuropeanOption(strike, sigma, t, riskFreeRate){};

    virtual double getIntrinsicValue(double spot) {
        return max(spot - Strike, 0.0);
    };

    virtual double calculateBlackScholes(double spot) {
        double d1 = (1/(sqrt(T)*Sigma))*(((RiskFreeRate+Sigma*Sigma/2)*T)+(log(spot/Strike)));
        double d2 = d1 - Sigma*sqrt(T);
        return normalCDF(d1) * spot - normalCDF(d2) * Strike * exp(-RiskFreeRate * T);
    }
     

};

class EuropeanPut : public EuropeanOption {
    public:

    EuropeanPut(double strike, double sigma, double t, double riskFreeRate) : EuropeanOption(strike, sigma, t, riskFreeRate){};

    virtual double getIntrinsicValue(double spot) {
        return max(Strike - spot, 0.0);
    };

    virtual double calculateBlackScholes(double spot) {
        double d1 = (1/(sqrt(T)*Sigma))*(((RiskFreeRate+Sigma*Sigma/2)*T)+(log(spot/Strike)));
        double d2 = d1 - Sigma*sqrt(T);
        return - normalCDF(-d1) * spot + normalCDF(-d2) * Strike * exp(-RiskFreeRate * T);
    };

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

    // Position() :
    // vecotor<>

    void addPosition(const Position& pos){
         Positions.push_back(pos);
    };

    virtual double getValue(double spot) {
        double sum_of_elems = 0.0;
        for (auto it = Positions.begin(); it != Positions.end(); ++it) {
        // for (std::vector<Position>::iterator& it = Positions.begin(); it != Positions.end(); ++it) {
            double sum_of_elems = sum_of_elems + ((*it).weight * (*it).option->getValue(spot));
        };
        return sum_of_elems;
   
    };

    double getDelta(double spot) {
        double sum_of_elems = 0.0;
        for (auto it = Positions.begin(); it != Positions.end(); ++it) {
        // for (std::vector<Position>::iterator& it = Positions.begin(); it != Positions.end(); ++it) {
            double sum_of_elems = sum_of_elems + ((*it).weight * (*it).option->getDelta(spot));
        };
        return sum_of_elems;
    };

    double calcDailyVaR(double spot, double sigma, double riskFreeRate, int N) {
        double dt = 1.0/252.0;
        double vol = sqrt(dt) * sigma;
        double drift = dt*(riskFreeRate - 0.5*vol*vol);
        vector<double> vals;

        for (int i = 0; i < N; i++) {
            double spot_new = spot * exp(drift + vol*simStandardNormal());
            vals[i] = getValue(spot_new);
        };

        sort(vals.begin(), vals.end());

        return getValue(spot) - (vals[0.05 * N -1]);

        /*
        for the specified number of iterations N:
            calculate a new spot value as:
            spot_new = spot * exp(drift + vol*standardNormalRandomVariable)
            calculate the portfolio value for spot_new
            store the portfolio value in a vector called vals

        */

    };

    double calcDailyExpectedShortfall(double spot, double sigma, double riskFreeRate, int N) {
        double dt = 1.0/252.0;
        double vol = sqrt(dt) * sigma;
        double drift = dt*(riskFreeRate - 0.5*vol*vol);
        vector<double> vals;

        for (int i = 0; i < N; i++) {
            double spot_new = spot * exp(drift + vol*simStandardNormal());
            vals[i] = getValue(spot_new);
        };

        sort(vals.begin(), vals.end());

        double sum_vals = std::accumulate(vals.begin(), vals.end(), 0);

        return getValue(spot) - ((1 / (0.05 * N))*(sum_vals));
    };
};


#endif
