/*
   Money.hpp
   By:Jared Maltos
   Created: 10/24/2019
   CS 202: Homework 04
   Purpose: .hpp file for Money
   Details: Put money.hpp, money.cpp, and moneytest.cpp into a project to work
*/
//note works in Visual Studio but not in Dev C++

#ifndef MONEY_HPP_
#define MONEY_HPP_

#include <iostream>
#include <sstream>
#include <math.h>

#include <ostream>
using std::ostream;

class Money {

public:

	//default constructor
	Money()
		: _balance(0.00) {}

	//dollars & cents constructor (combines to $X.XX)
	Money(const int& dollars, const int& cents)
		: _balance((double)dollars + ((double)cents / 100)) {}

	//double dollars constructor (rounds to $X.XX)
	Money(const double& dollars)
		: _balance(floor(dollars * 100 + 0.5) / 100) {}

	//copy constructor
	Money(const Money& money)
		: _balance(money.getBalance()) {}

	//accessor
	double getBalance() const { return _balance; }

	//equality overloads
	bool operator==(const Money& money) const
	{
		return _balance == money.getBalance();
	}

	bool operator!=(const Money& money) const
	{
		return !(*this == money);
	}

	bool operator<(const Money& money) const
	{
		return _balance < money.getBalance();
	}

	bool operator>(const Money& money) const
	{
		return _balance > money.getBalance();
	}

	bool operator<=(const Money& money) const
	{
		return _balance <= money.getBalance();
	}

	bool operator>=(const Money& money) const
	{
		return _balance >= money.getBalance();
	}

	//arithmetic overloads (and stream insertion overload)
	friend Money operator*(const Money& money, const double& num);
	friend Money operator*(const double& num, const Money& money);
	friend Money operator/(const Money& money, const double& num);
	friend ostream& operator<<(ostream& os, const Money& money);

	Money& operator+=(const Money& money) {
		_balance += money.getBalance();
		return *this;
	}
	Money& operator-=(const Money& money) {
		_balance -= money.getBalance();
		return *this;
	}
	Money& operator*=(const double& num) {
		double unroundedBalance = _balance * num;
		_balance = floor(unroundedBalance * 100 + 0.5) / 100;
		return *this;
	}
	Money& operator/=(const double& num) {
		double unroundedBalance = _balance / num;
		_balance = floor(unroundedBalance * 100 + 0.5) / 100;
		return *this;
	}
	Money operator+(const Money& money) const {
		Money newMoney(_balance + money.getBalance());
		return newMoney;
	}
	Money operator-(const Money& money) const {
		Money newMoney(_balance - money.getBalance());
		return newMoney;
	}

private:
	//money balance (of the form X.XX)
	double _balance;

};
#endif
//end of file