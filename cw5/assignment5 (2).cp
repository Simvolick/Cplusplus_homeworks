/**
* MFE C++ 2022 - Assignment 5
*
*
* Please refer to the assignment writeup for more details.  There is no need to alter this file but you must include it in your project.
*
*/


#include <iostream>
#include "option.h"

using namespace std;


int main() {
	EuropeanCall ec1(100, 0.25, 1.0, 0.05);
	EuropeanCall ec2(105, 0.225, 1.0, 0.05);
	EuropeanCall ec3(110, 0.2, 1.0, 0.05);
	EuropeanCall ec4(100, 0.35, 0.5, 0.025);
	EuropeanCall ec5(105, 0.325, 0.5, 0.025);
	EuropeanCall ec6(110, 0.3, 0.5, 0.025);
	EuropeanCall ec7(100, 0.45, 0.25, 0.01);
	EuropeanCall ec8(105, 0.425, 0.25, 0.01);
	EuropeanCall ec9(110, 0.4, 0.25, 0.01);



	EuropeanPut ep1(100, 0.25, 1.0, 0.05);
	EuropeanPut ep2(95, 0.275, 1.0, 0.05);
	EuropeanPut ep3(90, 0.3, 1.0, 0.05);
	EuropeanPut ep4(100, 0.35, 0.5, 0.025);
	EuropeanPut ep5(95, 0.375, 0.5, 0.025);
	EuropeanPut ep6(90, 0.4, 0.5, 0.025);
	EuropeanPut ep7(100, 0.45, 0.25, 0.01);
	EuropeanPut ep8(95, 0.475, 0.25, 0.01);
	EuropeanPut ep9(90, 0.5, 0.25, 0.01);


	Portfolio portfolio_test;

	portfolio_test.addPosition(Position(10, &ec1));
	portfolio_test.addPosition(Position(10, &ec2));
	portfolio_test.addPosition(Position(10, &ep1));
	portfolio_test.addPosition(Position(10, &ep2));

	std::cout << "TEST RUNS" << std::endl;


	cout << "ec1 value,delta " << ec1.getValue(100) << " " << ec1.getDelta(100) << endl;
	cout << "ec2 value,delta " << ec2.getValue(100) << " " << ec2.getDelta(100) << endl;
	cout << "ep1 value,delta " << ep1.getValue(100) << " " << ep1.getDelta(100) << endl;
	cout << "ep2 value,delta " << ep2.getValue(100) << " " << ep2.getDelta(100) << endl;

	cout << "Test Portfolio Value: " << portfolio_test.getValue(100) << endl;
	cout << "Test Portfolio Delta: " << portfolio_test.getDelta(100) << endl;
	cout << "  Test Portfolio VaR: " << portfolio_test.calcDailyVaR(100, 0.45, 0.01, 10000) << endl;
	cout << "   Test Portfolio ES: " << portfolio_test.calcDailyExpectedShortfall(100, 0.45, 0.01, 10000) << endl;

	// add in the rest of the positions
	portfolio_test.addPosition(Position(-20, &ec3));
	portfolio_test.addPosition(Position(15, &ec4));
	portfolio_test.addPosition(Position(10, &ec5));
	portfolio_test.addPosition(Position(-25, &ec6));
	portfolio_test.addPosition(Position(25, &ec7));
	portfolio_test.addPosition(Position(30, &ec8));
	portfolio_test.addPosition(Position(-55, &ec9));

	portfolio_test.addPosition(Position(15, &ep3));
	portfolio_test.addPosition(Position(25, &ep4));
	portfolio_test.addPosition(Position(-40, &ep5));
	portfolio_test.addPosition(Position(30, &ep6));
	portfolio_test.addPosition(Position(15, &ep7));
	portfolio_test.addPosition(Position(20, &ep8));
	portfolio_test.addPosition(Position(-35, &ep9));


	cout << endl << endl;
	cout << "Final Portfolio Value: " << portfolio_test.getValue(100) << endl;
	cout << "Final Portfolio Delta: " << portfolio_test.getDelta(100) << endl;
	cout << "  Final Portfolio VaR: " << portfolio_test.calcDailyVaR(100, 0.45, 0.01, 10000) << endl;
	cout << "   Final Portfolio ES: " << portfolio_test.calcDailyExpectedShortfall(100, 0.45, 0.01, 10000) << endl;
	
	return 0;

}