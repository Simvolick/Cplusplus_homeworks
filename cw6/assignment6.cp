/**
* MFE C++ 2022 - Assignment 6
*
*
* Please refer to the assignment writeup for more details.  
* You must alter this file to answer Q1, Q2 and Q3.
*
*/


#include <iostream>
#include "options.h"

using namespace std;


void print_values(double spot, Option& opt, string desc) {
	cout << desc << opt.getValue(spot) << " binomial_tree(depth=100): " << opt.calcBinomialTreeValue(spot, 100) << " Black Scholes: " << opt.calcBlackScholesValue(spot) << endl;

}



int main() {
	double test_spot = 50;
	double test_strike = 50;

	double test_r = 0.1;
	double test_t = 0.5;
	double test_sigma = 0.2;

	EuropeanCall test_ec1(test_strike, test_sigma, test_t, test_r);
	AmericanCall test_ac1(test_strike, test_sigma, test_t, test_r);
	EuropeanCallKnockout test_ecko1(test_strike, 60, test_sigma, test_t, test_r);
	EuropeanPut test_ep1(test_strike, test_sigma, test_t, test_r);
	AmericanPut test_ap1(test_strike, test_sigma, test_t, test_r);

	cout << "TEST VALUES" << endl;
	print_values(test_spot, test_ec1,   "TEST:     EURO CALL value: ");
	print_values(test_spot, test_ac1,   "TEST: AMERICAN CALL value: ");
	print_values(test_spot, test_ecko1, "TEST: KNOCKOUT CALL value: ");
	print_values(test_spot, test_ep1,   "TEST:      EURO PUT value: ");
	print_values(test_spot, test_ap1,   "TEST:  AMERICAN PUT value: ");


	double spot = 100;
	double strike = 100;

	double r = 0.05;
	double t = 1;
	double sigma = 0.25;

	EuropeanCall ec1(spot, sigma, t, r);
	AmericanCall ac1(spot, sigma, t, r);
	EuropeanCallKnockout ecko1(spot, 125, sigma, t, r);
	EuropeanPut ep1(spot, sigma, t, r);
	AmericanPut ap1(spot, sigma, t, r);


	cout << endl<< "FINAL VALUES" << endl;
	print_values(spot, ec1, "FINAL:     EURO CALL value: ");
	print_values(spot, ac1, "FINAL: AMERICAN CALL value: ");
	print_values(spot, ecko1, "FINAL: KNOCKOUT CALL value: ");
	print_values(spot, ep1, "FINAL:      EURO PUT value: ");
	print_values(spot, ap1, "FINAL:  AMERICAN PUT value: ");


	Portfolio portfolio_test;
	portfolio_test.addPosition(Position(10, &ec1));
	portfolio_test.addPosition(Position(10, &ac1));
	portfolio_test.addPosition(Position(10, &ecko1));
	portfolio_test.addPosition(Position(10, &ep1));
	portfolio_test.addPosition(Position(10, &ap1));

	cout << endl << endl;
	cout << "Final Portfolio Value: " << portfolio_test.getValue(100) << endl;
	cout << "Final Portfolio Delta: " << portfolio_test.getDelta(100) << endl;



	/**
	 PLEASE PUT YOUR Q1 CODE HERE
	*/



	/**
	 PLEASE PUT YOUR Q2 CODE HERE
	*/



	/**
	 PLEASE PUT YOUR Q3 CODE HERE
	*/



	return 0;

}