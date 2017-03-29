// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: string_tutorial.C,v 1.2 2002/02/27 12:25:07 sturm Exp $
// string tutorial example
// -----------------------
// play around with the functionality of BALL strings

// standard includes
#include <iostream>

// BALL includes
#include <BALL/common.h>
#include <BALL/DATATYPE/string.h>

using namespace BALL;
using namespace std;

int main()
{
	
	cout << "Concatenating:" << endl;
	String A("Concat");
	String B("enate");
	String C = A + B;
	cout << C << endl;

	string D("Concat");
	char* E = "enate";
	String F = D + E;
	cout << F << endl;

	cout << "Swapping two strings: " << endl;
	String S1("Swap");
	String S2("swaP");
	cout << "S1: " << S1 << ", S2: " << S2 << endl;
	S1.swap(S2);
	cout << "S1: " << S1 << ", S2: " << S2 << endl;

	cout << "Reversing a string:" << endl;
	String R("Reverse me");
	cout << "before: " << R;
	R.reverse();
	cout << " and after reversing: " << R << endl;

	cout << "Substituting substrings:" << endl;
	String SUB1("Please replace REPLACE with something else.");
	cout << SUB1 << endl;
	String SUB2("REPLACE");
	String SUB3("SOMETHING ELSE");
	SUB1.substitute(SUB2, SUB3);
	cout << SUB1 << endl;

	cout << "Conversion to strings by using constructors:" << endl;
	char c_char = 'B';
	int c_int = 1;
	float c_float = 2.99792458;

	String CONV1(c_char);
	String CONV2(c_int);
	String CONV3(c_float);

	cout << "Conversion from strings by using converters:" << endl;
	String i_wanna_be_an_int("4711");
	String i_wanna_be_a_char("A");
	String i_wanna_be_a_double("6.0221e23");

	int i_am_an_int = i_wanna_be_an_int.toInt();
	char i_am_a_char = i_wanna_be_a_char.toChar();
	double i_am_a_double = i_wanna_be_a_double.toDouble();

	cout << "Using predicates:" << endl;
	String predicate_test("This STRING does not start with PREFIX");
	cout << "String is empty? " 
		<< predicate_test.isEmpty() << endl;
	cout << "Has prefix \"PREFIX\"? " 
		<< predicate_test.hasPrefix("PREFIX") << endl;
	cout << "Contains substring \"PREFIX\"? " 
		<< predicate_test.hasSubstring("PREFIX") << endl;
	cout << "Contains only letters? "
		<< predicate_test.isAlpha() << endl;

	cout << "Comparison of strings" << endl;
	String test_string("Compare me.");
	String another_test_string("Blah.");
	char* test_C_string = "No match.";

	cout << test_string.compare(another_test_string) << endl;
	cout << test_string.compare(test_C_string) << endl;
	cout << (test_string == test_string) << endl;
	cout << (test_string != "No, this is not equal.") << endl;

	cout << test_string << another_test_string << endl;

	int start_index = 9;
	int length = 2;
	cout << test_string.compare("me", start_index, length) << endl;


	cout << "Field operations:" << endl;
	String data = "1 2 3 4.567 8 blah";
	cout << "Line contains " << data.countFields() << " values" << endl;
	cout << "The data at index 4 is " << data.getField(4) << endl;

	data = "1 2 3 4.567 \"8 blah\"";
	cout << "Line contains " << data.countFieldsQuoted() << " values" << endl;
	cout << "The data at index 4 is " << data.getFieldQuoted(4) << endl;
}
