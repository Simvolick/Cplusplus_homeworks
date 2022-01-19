/**
* MFE C++ 2022 - Assignment 3
* 
* In this assignment, we will explore estimating the value of European Call options using simulation and Black Scholes.
* We are also taking our first foray into using classes
* 
* The code is divided into three parts.
*   1) There are two helper functions (simStandardNormal & normalCDF) that 
*      simulate a standard normal random variable and calculate N() respectively.
*   2) The class EuroCallOption provides functionality for valuing European call options
*   3) Test code at the bottom that uses the EuroCallOption objects to value the call options 
* 
* Your job is to fill in the functionality of the following methods:
*    getIntrinsicValue() - this returns the exercise value of option for a given spot value
*    simulatePath() - returns a simulated ending spot value given starting spot value and number of steps
*    estimateValue() - runs N simulated paths, calculates the _discounted_ intrinsic call value for each path and then returns the average
*	 calculateBlackScholes() - returns the Black Scholes estimate for the call value for a given spot
* 
* 
* Please refer to the assignment writeup for more details and some hints.
*  

   Dec 13 - The comments in main() have been updated to reflect proper Black Scholes values.

 
*/


#include <iostream>
#include <random>
#include <cmath>
#include <iomanip>

#include <algorithm>
using namespace std;


/*
simStandardNormal() - a helper function that calculates a pseudo-random standard normal gaussian
*/

double simStandardNormal() {
	static std::mt19937 mt_rand(12345);
	static std::normal_distribution<double> dis_normal(0.0, 1.0);

	return dis_normal(mt_rand);
};

double normalCDF(double x) // Phi(-infinity, x) aka N(x)
{
	return std::erfc(-x / std::sqrt(2)) / 2;
};


class EuroCallOption {
public:
	double K;
	double Sigma;
	double r;
	double T;


public:
	// This is a constructor.  We will cover constructors in a later lecture
	EuroCallOption(double k, double sigma, double r, double T) : K(k), Sigma(sigma), r(r), T(T) {};


	double getIntrinsicValue(double spot) {
		double intrinsicValue = max((spot - (this->K)), 0.0);
		return intrinsicValue;
	};

	double simulatePath(double spot, int steps) {
		double return_final;
		for (int i = 0; i < steps; i++) {
			double drift = ((this -> r) - 0.5 * ((this -> Sigma) * (this -> Sigma))) * ((this -> T) / i);
			double vol = ((this -> Sigma) * std::sqrt(((this -> T) / steps)));
			return_final += (drift + (vol * simStandardNormal()));
		};
		return return_final;
	}

	// double estimateValue(double spot, int steps, int N) {
	// 	double sum=0.0, average;
	// 	double simulatedPaths[N];
	// 	double intrinsticValues[N];
	// 	double discountValue;
	// 	for (int i = 0; i < N; i++) {
	// 		simulatedPaths[i] += simulatePath(spot, steps);
	// 		intrinsticValues[i] += getIntrinsicValue(simulatedPaths[i]);
	// 		for(int z = 0; z < N; z++)
	// 			{
	// 				sum += intrinsticValues[z];
	// 			};
	// 		average = sum / N;
	// 		double discountValue = exp(-average * (this -> T));
	// };
	// 	return discountValue;
	// }

	double estimateValue(double spot, int steps, int N) {
		double endingVal = 0,intrinsicVal = 0;
		for(int i = 0; i < N; ++i){
			endingVal = simulatePath(spot,steps);
			intrinsicVal += getIntrinsicValue(endingVal);
			}
		return (intrinsicVal / N) * exp(-r * T);
}

	double calculateBlackScholes(double spot) {
		double d1 =  ((log(spot/(this ->K)) + (((this -> r) + (0.5*(pow(this->Sigma,2.0))))*(this->T)))/((this-> Sigma)*(pow((this -> T),0.5))));
		double d2 = (d1 - (this-> Sigma*(pow(this -> T,0.5))));
		double blackScholesResult = ((normalCDF(d1) * spot) - (normalCDF(d2) * ((this->K) * exp(-(this->r)*(this->T)))));
		return blackScholesResult;
	};
};


void runTest(double S, double K, double sigma, double r, double T, int steps) {
	
	EuroCallOption call(K, sigma, r, T);
	double BS = call.calculateBlackScholes(S);
	double simV10 = call.estimateValue(S, steps, 10);
	double simV100 = call.estimateValue(S, steps, 100);
	double simV1000 = call.estimateValue(S, steps, 1000);
	double simV10000 = call.estimateValue(S, steps, 10000);
	// double simV100000 = call.estimateValue(S, steps, 1000000);

	std::cout << "S: " << S << " steps: " << steps << " BS: " << BS << " SIM VALUES  " << simV10 << " " << simV100 << " " << simV1000 << " " << simV10000 << " " << std::endl;

}


int main() {
	std::cout << "TEST RUNS" << std::endl;
	// SHOULD PRINT SOMETHING LIKE THIS (some numbers may be different but not the BS number)
	// S: 100 steps: 1 BS: 8.26002 SIM VALUES  6.29249 9.1837 8.15842 8.32233 8.26708
	runTest(100, 100, 0.25, 0.05, 0.5, 1);

	// SHOULD PRINT SOMETHING LIKE THIS (some numbers may be different but not the BS number)
	// S: 90 steps: 2 BS: 4.37454 SIM VALUES  3.51654 4.37033 4.18685 4.28715 4.3817
	runTest(90, 100, 0.15, 0.05, 1.25, 2);

	std::cout << std::endl <<  "STARTING ASSIGNMENT RUNS" << std::endl;
	runTest(100, 90, 0.15, 0.1, 0.5, 1);
	runTest(90, 100, 0.25, 0.05, 1.25, 10);
	runTest(10, 10, 0.25, 0.05, 1.0, 1);
	runTest(15, 50, 0.5, 0.01, 2.0, 10);
	runTest(30, 65, 0.6, 0.005, 1.1, 1);
	
	return 0;
}