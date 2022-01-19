



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
    }

double normalCDF(double x) // Phi(-infinity, x) aka N(x)
    {
    return std::erfc(-x / std::sqrt(2)) / 2;
    }

    
    double getValue(double spot) {
        for (auto it = Positions.begin(); it != Positions.end(); ++it) {
        // for (std::vector<Position>::iterator& it = Positions.begin(); it != Positions.end(); ++it) {
            double elems_sum = (((*it).weight()) ((*it).option()));
            };
            return 0;
            // double sum_of_elems += it.weight * it.option->getValue(spot);
         };
        // double sum_of_elems = std::accumulate(Positions.begin(), Positions.end(), 0.0);
        // return sum_of_elems.weight * sum_of_elems.option->getValue(spot);
    };

