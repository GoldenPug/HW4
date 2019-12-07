// money_test.cpp
// Chris Hartman, 9/23/2010
//
// For CS 202 Fall 2010
// Test program for class Money
// Used in Assignment 2,

// Includes for code to be tested
// Minor modifications 2011, 2015, 2016, 2017
// CMH

#include "Money.hpp"   // For class Money
#include "Money.hpp"   // Double inclusion test

// Includes for testing package & code common to all test programs
#include <iostream>     // for std::cout, std::endl, std::cin
#include <string>       // for std::string
#include <stdexcept>    // for std::runtime_error

// Additional includes for this test program
#include <sstream>      // for std::ostringstream


// ************************************************************************
// Testing Package:
//     Class Tester - For Tracking Tests
// ************************************************************************


// class Tester
// For extremely simple unit testing.
// Keeps track of number of tests and number of passes.
// Use test (with success/failure parameter) to do a test.
// Get results with numTests, numPassed, numFailed, allPassed.
// Restart testing with reset.
// Invariants:
//     countTests_ == number of tests (calls to test) since last reset.
//     countPasses_ == number of times function test called with true param
//      since last reset.
//     0 <= countPasses_ <= countTests_.
//     tolerance_ >= 0.
class Tester {

	// ***** Tester: ctors, dctor, op= *****
public:

	// Default ctor
	// Sets countTests_, countPasses_ to zero, tolerance_ to given value
	// Pre: None.
	// Post:
	//     numTests == 0, countPasses == 0, tolerance_ == abs(theTolerance)
	// Does not throw (No-Throw Guarantee)
	Tester(double theTolerance = 0.0000001)
		:countTests_(0),
		countPasses_(0),
		tolerance_(theTolerance >= 0 ? theTolerance : -theTolerance)
	{}

	// Compiler-generated copy ctor, copy op=, dctor are used

// ***** Tester: general public functions *****
public:

	// test
	// Handles single test, param indicates pass/fail
	// Pre: None.
	// Post:
	//     countTests_ incremented
	//     countPasses_ incremented if (success)
	//     Message indicating test name (if given)
	//      and pass/fail printed to cout
	// Does not throw (No-Throw Guarantee)
	//  - Assuming exceptions have not been turned on for cout.
	void test(bool success,
		const std::string & testName = "")
	{
		++countTests_;
		if (success) ++countPasses_;

		std::cout << "    ";
		if (testName != "")
		{
			std::cout << "Test: "
				<< testName
				<< " - ";
		}
		std::cout << (success ? "passed" : "********** FAILED **********")
			<< std::endl;
	}

	// ftest
	// Does single floating-point test.
	// Tests passes iff difference of first two values is <= tolerance.
	// Pre: None.
	// Post:
	//     countTests_ incremented
	//     countPasses_ incremented if (abs(val1-val2) <= tolerance_)
	//     Message indicating test name (if given)
	//      and pass/fail printed to cout
	// Does not throw (No-Throw Guarantee)
	void ftest(double val1,
		double val2,
		const std::string & testName = "")
	{
		test(val1 - val2 <= tolerance_ && val2 - val1 <= tolerance_, testName);
	}

	// reset
	// Resets *this to default constructed state
	// Pre: None.
	// Post:
	//     countTests_ == 0, countPasses_ == 0
	// Does not throw (No-Throw Guarantee)
	void reset()
	{
		countTests_ = 0;
		countPasses_ = 0;
	}

	// numTests
	// Returns the number of tests that have been done since last reset
	// Pre: None.
	// Post:
	//     return == countTests_
	// Does not throw (No-Throw Guarantee)
	int numTests() const
	{
		return countTests_;
	}

	// numPassed
	// Returns the number of tests that have passed since last reset
	// Pre: None.
	// Post:
	//     return == countPasses_
	// Does not throw (No-Throw Guarantee)
	int numPassed() const
	{
		return countPasses_;
	}

	// numFailed
	// Returns the number of tests that have not passed since last reset
	// Pre: None.
	// Post:
	//     return + countPasses_ == numTests_
	// Does not throw (No-Throw Guarantee)
	int numFailed() const
	{
		return countTests_ - countPasses_;
	}

	// allPassed
	// Returns true if all tests since last reset have passed
	// Pre: None.
	// Post:
	//     return == (countPasses_ == countTests_)
	// Does not throw (No-Throw Guarantee)
	bool allPassed() const
	{
		return countPasses_ == countTests_;
	}

	// setTolerance
	// Sets tolerance_ to given value
	// Pre: None.
	// Post:
	//     tolerance_ = abs(theTolerance)
	// Does not throw (No-Throw Guarantee)
	void setTolerance(double theTolerance)
	{
		tolerance_ = (theTolerance >= 0 ? theTolerance : -theTolerance);
	}

	// ***** Tester: data members *****
private:

	int countTests_;    // Number of tests done since last reset
	int countPasses_;   // Number of tests passed since last reset
	double tolerance_;  // Tolerance for floating-point near-equality tests

};  // end class Tester


// ************************************************************************
// Testing Package:
//     Class TypeCheck - Helper Class for Type Checking
// ************************************************************************


// class TypeCheck
// This class exists in order to have static member function check, which
// takes a parameter of a given type, by reference. Objects of type
// TypeCheck<T> cannot be created.
// Usage:
//     TypeCheck<MyType>::check(x)
//     returns true if the type of x is (MyType) or (const MyType),
//     otherwise false.
// Invariants: None.
// Requirements on Types: None.
template<typename T>
class TypeCheck {

private:

	// Uncopyable class. Do not define copy ctor, copy assn.
	TypeCheck(const TypeCheck &);
	TypeCheck<T> & operator=(const TypeCheck &);

	// Compiler-generated dctor is used (but irrelevant).

public:

	// check
	// The function and function template below simulate a single function
	// that takes a single parameter, and returns true iff the parameter has
	// type T or (const T).

	// check (reference-to-const T)
	// Pre: None.
	// Post:
	//     Return is true.
	// Does not throw (No-Throw Guarantee)
	static bool check(const T & param)
	{
		return true;
	}

	// check (reference-to-const non-T)
	// Pre: None.
	// Post:
	//     Return is false.
	// Requirements on types: None.
	// Does not throw (No-Throw Guarantee)
	template <typename OtherType>
	static bool check(const OtherType & param)
	{
		return false;
	}

};  // End class TypeCheck


// ************************************************************************
// Testing Package:
//     Class Counter - Helper Class for Counting Calls & Objects, Throwing
// ************************************************************************


// class Counter
// Item type for counting ctor, dctor, op= calls, counting existing
//  objects, and possibly throwing on copy. Has operator< (which always
//  returns false), allowing it to be the value type of a sorted container.
// If static member copyThrow_ is set, then copy ctor and copy assn throw
//  std::runtime_error. Exception object's "what" member is set to "C" by
//  the copy ctor and "A" by copy assn.
// Increments static data member ctorCount_ on default construction and
//  successful copy construction. Increments static data member assnCount_
//  on successful copy assignment. Increments static data member
//  dctorCount_ on destruction.
// Increments static data member existing_ on construction, and decrements
//  it on destruction.
// Static data member maxExisting_ is highest value of existing_ since last
//  reset, or start of program if reset has never been called.
// Invariants:
//     Counter::existing_ is number of existing objects of this class.
//     Counter::ctorCount_ is number of successful ctor calls since
//      most recent call to reset, or start of program if reset has never
//      been called.
//     Counter::dctorCount_ is (similarly) number of dctor calls.
//     Counter::assnCount_ is (similarly) number of copy assn calls.
//     Counter::maxExisting_ is (similarly) highest value existing_ has
//      assumed.
class Counter {

	// ***** Counter: Ctors, dctor, op= *****
public:

	// Default ctor
	// Pre: None.
	// Post:
	//     (ctorCount_ has been incremented.)
	//     (existing_ has been incremented.)
	// Does not throw (No-Throw Guarantee)
	Counter()
	{
		++existing_;
		if (existing_ > maxExisting_)
			maxExisting_ = existing_;
		++ctorCount_;
	}

	// Copy ctor
	// Throws std::runtime_error if copyThrow_.
	// Pre: None.
	// Post:
	//     (ctorCount_ has been incremented.)
	//     (existing_ has been incremented.)
	// May throw std::runtime_error
	// Strong Guarantee
	Counter(const Counter & other)
	{
		if (copyThrow_)
			throw std::runtime_error("C");
		++existing_;
		if (existing_ > maxExisting_)
			maxExisting_ = existing_;
		++ctorCount_;
	}

	// Copy assignment
	// Throws std::runtime_error if copyThrow_.
	// Pre: None.
	// Post:
	//     Return value is *this.
	//     (assnCount_ has been incremented.)
	// May throw std::runtime_error
	// Strong Guarantee
	Counter & operator=(const Counter & rhs)
	{
		if (copyThrow_)
			throw std::runtime_error("A");
		++assnCount_;
		return *this;
	}

	// Dctor
	// Pre: None.
	// Post:
	//     (dctorCount_ has been incremented.)
	//     (existing_ has been decremented.)
	// Does not throw (No-Throw Guarantee)
	~Counter()
	{
		--existing_;
		++dctorCount_;
	}

	// ***** Counter: Functions dealing with count *****
public:

	// reset
	// Pre: None.
	// Post:
	//     maxExisting_ == existing_.
	//     ctorCount_ == 0.
	//     dctorCount_ == 0.
	//     assnCount_ == 0.
	//     copyThrow_ == shouldThrow.
	// Does not throw (No-Throw Guarantee)
	static void reset(bool shouldThrow = false)
	{
		maxExisting_ = existing_;
		ctorCount_ = 0;
		dctorCount_ = 0;
		assnCount_ = 0;
		copyThrow_ = shouldThrow;
	}

	// getExisting
	// Pre: None.
	// Post:
	//     return == existing_.
	// Does not throw (No-Throw Guarantee)
	static int getExisting()
	{
		return existing_;
	}

	// getMaxExisting
	// Pre: None.
	// Post:
	//     return == maxExisting_.
	// Does not throw (No-Throw Guarantee)
	static int getMaxExisting()
	{
		return maxExisting_;
	}

	// getCtorCount
	// Pre: None.
	// Post:
	//     return == ctorCount_.
	// Does not throw (No-Throw Guarantee)
	static int getCtorCount()
	{
		return ctorCount_;
	}

	// getDctorCount
	// Pre: None.
	// Post:
	//     return == dctorCount_.
	// Does not throw (No-Throw Guarantee)
	static int getDctorCount()
	{
		return dctorCount_;
	}

	// getAssnCount
	// Pre: None.
	// Post:
	//     return == assnCount_.
	// Does not throw (No-Throw Guarantee)
	static int getAssnCount()
	{
		return assnCount_;
	}

	// setCopyThrow
	// Pre: None.
	// Post:
	//     copyThrow_ == shouldThrow
	// Does not throw (No-Throw Guarantee)
	static void setCopyThrow(bool shouldThrow)
	{
		copyThrow_ = shouldThrow;
	}

	// ***** Counter: Data Members *****
private:

	static int existing_;     // # of existing objects
	static int maxExisting_;  // Max # of existing objects
	static int ctorCount_;    // # of successful (non-throwing) ctor calls
	static int dctorCount_;   // # of dctor calls
	static int assnCount_;    // # of successful (non-throwing) copy = calls
	static bool copyThrow_;   // true if copy operations (ctor, =) throw

};  // End class Counter

// Definition of static data member of class Counter
int Counter::existing_ = 0;
int Counter::maxExisting_ = 0;
int Counter::ctorCount_ = 0;
int Counter::dctorCount_ = 0;
int Counter::assnCount_ = 0;
bool Counter::copyThrow_ = false;


// operator< (Counter)
// Dummy-ish operator<, forming a strict weak order for Counter class
// Returns false (which is legal for a strict weak order; all objects of
//  type Counter are equivalent).
// Pre: None.
// Post:
//     Return value == false.
// Does not throw (No-Throw Guarantee)
bool operator<(const Counter & a,
	const Counter & b)
{
	return false;
}


// ************************************************************************
// Test Suite Functions
// ************************************************************************


// test_class_Money_default_ctor
// Test suite for class Money, default ctor
// Pre: None.
// Post:
//     Pass/fail status of tests have been registered with t.
//     Appropriate messages have been printed to cout.
// Does not throw (No-Throw Guarantee)
void test_class_Money_operatorReturnTypes(Tester & t)
{
	std::cout << "Test Suite: class Money, operator return types" << std::endl;
	t.test(TypeCheck<bool>::check(Money() == Money()), "operator== return type");
	t.test(TypeCheck<bool>::check(Money() != Money()), "operator!= return type");
	t.test(TypeCheck<bool>::check(Money() <= Money()), "operator<= return type");
	t.test(TypeCheck<bool>::check(Money() >= Money()), "operator>= return type");
	t.test(TypeCheck<bool>::check(Money() < Money()), "operator< return type");
	t.test(TypeCheck<bool>::check(Money() > Money()), "operator> return type");

	t.test(TypeCheck<Money>::check(Money()*2.0), "operator*(Money, double) return type");
	t.test(TypeCheck<Money>::check(2.0*Money()), "operator*(double, Money) return type");
	t.test(TypeCheck<Money>::check(Money() / 1.0), "operator/ return type");
	t.test(TypeCheck<Money>::check(Money() + Money()), "operator+ return type");
	t.test(TypeCheck<Money>::check(Money() - Money()), "operator- return type");
}


// test_class_Money_data_ctor
// Test suite for class Money, ctor from data
// Pre: None.
// Post:
//     Pass/fail status of tests have been registered with t.
//     Appropriate messages have been printed to cout.
// Does not throw (No-Throw Guarantee)
void test_class_Money_ctors(Tester & t)
{
	std::cout << "Test Suite: class Money, ctors and operator<<" << std::endl;

	// data ctor param type
	const Money con0;
	const Money con1(2, 2);
	const Money con2(2.02);
	const Money con3(-3.03);
	std::ostringstream os;
	os << con0;
	t.test(os.str() == "$0.00", "print, default Money gives '$0.00'");

	os.str(""); //reset output holder
	os << con1;
	t.test(os.str() == "$2.02", "Ctor from data, Money(2,2) operator<< gives '$2.02'");

	os.str(""); //reset output holder
	os << con2;
	t.test(os.str() == "$2.02", "Ctor from data, Money(2.02) operator<< gives '$2.02'");

	os.str("");
	os << con3;
	t.test(os.str() == "-$3.03", "Ctor from data, Money(-3.03) operator<< gives '-$3.03'");

	const Money con4(7.072);
	os.str("");
	os << con4;
	t.test(os.str() == "$7.07", "Ctor from data, Money(7.072) operator<< gives '$7.07'");

	const Money con5(7.077);
	os.str("");
	os << con5;
	t.test(os.str() == "$7.08", "Ctor from data, Money(7.077) operator<< gives '$7.08'");

	const Money con6(-7.072);
	os.str("");
	os << con6;
	t.test(os.str() == "-$7.07", "Ctor from data, Money(-7.072) operator<< gives '-$7.07'");

	const Money con7(-7.077);
	os.str("");
	os << con7;
	t.test(os.str() == "-$7.08", "Ctor from data, Money(-7.077) operator<< gives '-$7.08'");
}

void test_class_Money_relational_operators(Tester & t)
{
	std::cout << "Test Suite: class Money, relational operators" << std::endl;

	const Money m0;
	const Money m1(1);

	t.test((m0 == m0), "operator== on default Money objects returns true");
	t.test(!(m0 == m1), "operator== on default vs $1.00 returns false");

	t.test(!(m0 != m0), "operator!= on default Money objects returns false");
	t.test((m0 != m1), "operator!= on default vs $1.00 returns true");

	t.test(!(m0 < m0), "operator< on default Money objects returns false");
	t.test((m0 < m1), "operator< on default vs $1.00 returns true");
	t.test(!(m1 < m0), "operator< on $1.00 vs default returns true");

	t.test((m0 <= m0), "operator<= on default Money objects returns true");
	t.test((m0 <= m1), "operator<= on default vs $1.00 returns true");
	t.test(!(m1 <= m0), "operator<= on $1.00 vs default returns false");

	t.test(!(m0 > m0), "operator> on default Money objects returns false");
	t.test(!(m0 > m1), "operator> on default vs $1.00 returns false");
	t.test((m1 > m0), "operator> on $1.00 vs default returns true");

	t.test((m0 >= m0), "operator>= on default Money objects returns true");
	t.test(!(m0 >= m1), "operator>= on default vs $1.00 returns false");
	t.test((m1 >= m0), "operator>= on $1.00 vs default returns true");
}

void test_class_Money_arithmetic_operators(Tester & t)
{
	std::cout << "Test Suite: class Money, arithmetic operators" << std::endl;

	const Money m0;
	Money m1(1);
	Money m2(2);
	const Money mc1(m1);
	const Money mc2(2);

	m1 -= mc1;
	t.test(m1 == m0, "operator-=(Money)");

	m1 += mc1;
	t.test(m1 == mc1, "operator+=(Money)");

	m1 *= 2.0;
	t.test(m1 == mc2, "operator*=(double)");

	m1 /= 2.0;
	t.test(m1 == mc1, "operator/=(double");

	t.test(mc1 + mc1 == mc2, "operator+()");

	t.test(mc1 - mc1 == m0, "operator-()");

	t.test(2 * mc1 == mc2, "operator*(double,Money");

	t.test(mc1 * 2 == mc2, "operator*(Money,double");

	t.test(mc2 / 2 == mc1, "operator/(Money,double");

	Money m12;
	m12 = mc1;
	t.test(m12 == mc1, "operator=()");

	(m2 -= mc1) = m0;
	t.test(m2 == m0, "operator= applied to return value of operator-=");

	(m2 += mc1) = m0;
	t.test(m2 == m0, "operator= applied to return value of operator+=");

	m2 = Money(4.2);
	(m2 /= 2.0) = m0;
	t.test(m2 == m0, "operator= applied to return value of operator/=");

	m2 = Money(4.2);
	(m2 *= 2.0) = m0;
	t.test(m2 == m0, "operator= applied to return value of operator*=");
}




// test_class_Money
// Test suite for class Money
// Uses other test-suite functions
// Pre: None.
// Post:
//     Pass/fail status of tests have been registered with t.
//     Appropriate have been messages printed to cout.
// Does not throw (No-Throw Guarantee)
void test_class_Money(Tester & t)
{
	// Do all the test suites
	std::cout << "TEST SUITES FOR CLASS Money" << std::endl;
	test_class_Money_operatorReturnTypes(t);
	test_class_Money_ctors(t);
	test_class_Money_relational_operators(t);
	test_class_Money_arithmetic_operators(t);
}


// ************************************************************************
// Main program
// ************************************************************************


// main
// Runs class Money test suite, prints results to cout.
int main()
{
	Tester t;
	test_class_Money(t);
	std::cout << std::endl;
	if (t.allPassed())
	{
		std::cout << "All tests successful"
			<< std::endl;
	}
	else
	{
		std::cout << "Tests ********** UNSUCCESSFUL **********"
			<< std::endl;
	}
	std::cout << std::endl;

	std::cout << "Press ENTER to quit ";
	while (std::cin.get() != '\n');

	return 0;
}