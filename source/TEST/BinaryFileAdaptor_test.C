// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: BinaryFileAdaptor_test.C,v 1.5.2.1 2003/01/07 13:22:16 anker Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/SYSTEM/file.h>

///////////////////////////

START_TEST(BinaryFileAdaptor, "$Id: BinaryFileAdaptor_test.C,v 1.5.2.1 2003/01/07 13:22:16 anker Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

BinaryFileAdaptor<double>* double_bfa_ptr;

CHECK(default constructor)
	double_bfa_ptr = new BinaryFileAdaptor<double>;
	TEST_NOT_EQUAL(double_bfa_ptr, 0)
RESULT

CHECK(destructor)
	delete double_bfa_ptr;
RESULT

CHECK(detailed constructor / accessors)
	BinaryFileAdaptor<double> double_bfa(0.87);
	TEST_EQUAL(double_bfa.getData(), 0.87)
	double_bfa.setData(9.87);
	TEST_EQUAL(double_bfa.getData(), 9.87)
RESULT

CHECK(streams)
	String outfile_name;
	NEW_TMP_FILE(outfile_name)
	File outfile(outfile_name, ::std::ios::out);
	double test = 95.92;
	BinaryFileAdaptor<double> double_bfa;
	outfile << double_bfa << test;
	outfile.close();

	File infile(outfile_name);
	BinaryFileAdaptor<double> double_bfa2;
	double test2 = 0.0;
	infile >> double_bfa2 >> test2;
	TEST_EQUAL(double_bfa2.getData(), double_bfa.getData())
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
