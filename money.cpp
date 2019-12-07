/*
   Money.cpp
   By:Jared Maltos
   Created: 10/24/2019
   CS 202: Homework 04
   Purpose: .cpp file for Money
   Details: Put money.hpp, money.cpp, and moneytest.cpp into a project to work
*/
//note works in Visual Studio but not in Dev C++
#include "Money.hpp"

#include <iostream>
#include <ostream>
using std::ostream;


//overload for money / double
Money operator/(const Money& money, const double& num) {
	double unroundedBalance = money.getBalance() / num;
	double newBalance = floor(unroundedBalance * 100 + 0.5) / 100;
	Money newMoney(newBalance);
	return newMoney;
}

//overload for money * double
Money operator*(const Money& money, const double& num) {
	double unroundedBalance = money.getBalance() * num;
	double newBalance = floor(unroundedBalance * 100 + 0.5) / 100;
	Money newMoney(newBalance);
	return newMoney;
}

//overload for double * money
Money operator*(const double& num, const Money& money) {
	double unroundedBalance = money.getBalance() * num;
	double newBalance = floor(unroundedBalance * 100 + 0.5) / 100;
	Money newMoney(newBalance);
	return newMoney;
}

//overload to print money to ostream
ostream& operator<<(ostream& os, const Money& money) {

	if (money.getBalance() == 0) {
		os << "$0.00";
	}
	else if (money.getBalance() < 0) {
		double newBalance = money.getBalance() * -1.00;
		os << "-$" << newBalance;
	}
	else {
		os << "$" << money.getBalance();
	}

	return os;
}
//end of file