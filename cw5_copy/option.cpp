
#include "option.h"
#include <iostream>
#include <random>
#include <cmath>
#include <numeric>

#include <algorithm>
#include <vector>
#include <iomanip>
using namespace std;


double simStandardNormal() {
    static std::mt19937 mt_rand(12345);
    static std::normal_distribution<double> dis_normal(0.0, 1.0);
    return dis_normal(mt_rand);
    };

double normalCDF(double x) // Phi(-infinity, x) aka N(x)
    {
    return std::erfc(-x / std::sqrt(2)) / 2;
    };

    
Option::Option(double strike, double sigma, double t, double riskFreeRate) {
        Strike = strike;
        Sigma = sigma;
        T = t;
        RiskFreeRate = riskFreeRate;
};

double Option::getDelta(double spot){
        return (getValue(spot*1.01) - getValue(spot));
};

// EuropeanOption::EuropeanOption(double strike, double sigma, double t, double riskFree) {};

double EuropeanOption::getValue(double spot) {
            return calcBlackScholesValue(spot);
        };

// EuropeanCall::EuropeanCall(double strike, double sigma, double t, double riskFreeRate) {};

double EuropeanCall::getIntrinsicValue(double spot) {
        return max(spot - Strike, 0.0);
    };



double EuropeanCall::calcBlackScholesValue(double spot) {
        double d1 = (1/(sqrt(T)*Sigma))*(((RiskFreeRate+Sigma*Sigma/2)*T)+(log(spot/Strike)));
        double d2 = d1 - Sigma*sqrt(T);
        return normalCDF(d1) * spot - normalCDF(d2) * Strike * exp(-RiskFreeRate * T);
    };


// EuropeanPut::EuropeanPut(double strike, double sigma, double t, double riskFreeRate) {};

double EuropeanPut::getIntrinsicValue(double spot) {
        return max(Strike - spot, 0.0);
    };

double EuropeanPut::calcBlackScholesValue(double spot) {
    double d1 = (1/(sqrt(T)*Sigma))*(((RiskFreeRate+Sigma*Sigma/2)*T)+(log(spot/Strike)));
    double d2 = d1 - Sigma*sqrt(T);
    return - normalCDF(-d1) * spot + normalCDF(-d2) * Strike * exp(-RiskFreeRate * T);
};

Portfolio::Portfolio(vector<Position> positions) {};

void Portfolio::addPosition(const Position& pos){
        Positions.push_back(pos);
};

double Portfolio::getValue(double spot) {
    double sum_of_elems = 0.0;
    for (auto it = Positions.begin(); it != Positions.end(); ++it) {
    // for (std::vector<Position>::iterator& it = Positions.begin(); it != Positions.end(); ++it) {
        double sum_of_elems = sum_of_elems + ((*it).weight * (*it).option->getValue(spot));
    };
    return sum_of_elems;

};

double Portfolio::getDelta(double spot) {
    double sum_of_elems = 0.0;
    for (auto it = Positions.begin(); it != Positions.end(); ++it) {
    // for (std::vector<Position>::iterator& it = Positions.begin(); it != Positions.end(); ++it) {
        double sum_of_elems = sum_of_elems + ((*it).weight * (*it).option->getDelta(spot));
    };
    return sum_of_elems;
};

double Portfolio::calcDailyVaR(double spot, double sigma, double riskFreeRate, int N) {
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

double Portfolio::calcDailyExpectedShortfall(double spot, double sigma, double riskFreeRate, int N) {
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


