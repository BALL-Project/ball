// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/SYSTEM/file.h>
#include <BALL/SYSTEM/binaryFileAdaptor.h>

//We need this to get some of the defines used in the implementation part
#ifdef BALL_NO_INLINE_FUNCTIONS
	#include <BALL/SYSTEM/binaryFileAdaptor.iC>
#endif

///////////////////////////

START_TEST(BinaryFileAdaptor)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

BinaryFileAdaptor<double>* double_bfa_ptr = 0;

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

CHECK(T& getData())
	BinaryFileAdaptor<float> bfa;
	bfa.getData() = 2.345;
	TEST_REAL_EQUAL(bfa.getData(), 2.345)
RESULT

CHECK(const T& getData() const)
	BinaryFileAdaptor<float> bfa;
	TEST_REAL_EQUAL(bfa.getData(), 0)
RESULT

CHECK(void setData(const T& data) throw())
	BinaryFileAdaptor<float> bfa;
	bfa.setData(2.345);
	TEST_REAL_EQUAL(bfa.getData(), 2.345)
RESULT

CHECK(streams)
	String outfile_name;
	NEW_TMP_FILE(outfile_name)
	File outfile(outfile_name, ::std::ios::out);
	double test = 95.92;
	BinaryFileAdaptor<double> double_bfa;
	double_bfa.setData(test);
	outfile << double_bfa;
	outfile.close();

	File infile(outfile_name);
	BinaryFileAdaptor<double> double_bfa2;
	infile >> double_bfa2;
	TEST_EQUAL(double_bfa2.getData(), double_bfa.getData())
RESULT

CHECK(Byte Swapping (short))
#if BALL_SHORT_SIZE == 2
	short x = 0x1122;
	swapBytes(x);
	TEST_EQUAL(x, 0x2211);
	swapBytes(x);
	TEST_EQUAL(x, 0x1122);
#elif BALL_SHORT_SIZE == 4
	short x = 0x11223344;
	swapBytes(x);
	TEST_EQUAL(x, 0x44332211);
	swapBytes(x);
	TEST_EQUAL(x, 0x44331122);
#endif
RESULT

CHECK(Byte Swapping (unsigned short))
#if BALL_USHORT_SIZE == 2
	unsigned short x = 0xAABBu;
	swapBytes(x);
	TEST_EQUAL(x, 0xBBAAu);
	swapBytes(x);
	TEST_EQUAL(x, 0xAABBu);
#elif BALL_USHORT_SIZE == 4
	unsigned short x = 0xAABBCCDDu;
	swapBytes(x);
	TEST_EQUAL(x, 0xDDCCBBAAu);
	swapBytes(x);
	TEST_EQUAL(x, 0xAABBCCDDu);
#endif
RESULT

CHECK(Byte Swapping (uint32_t))
	BALL_UINT32 x = 0xAABBCCDDu;
	swapBytes(x);
	TEST_EQUAL(x, 0xDDCCBBAAu);
	swapBytes(x);
	TEST_EQUAL(x, 0xAABBCCDDu);
RESULT

CHECK(Byte Swapping (int32_t))
	BALL_INT32 x = 0x11223344;
	swapBytes(x);
	TEST_EQUAL(x, 0x44332211);
	swapBytes(x);
	TEST_EQUAL(x, 0x11223344);
RESULT

CHECK(Byte Swapping (uint64_t))
	BALL_UINT64 x = 0xAAABACADBABBBCBDull;
	swapBytes(x);
	TEST_EQUAL(x, 0xBDBCBBBAADACABAAull);
	swapBytes(x);
	TEST_EQUAL(x, 0xAAABACADBABBBCBDull);
RESULT

CHECK(Byte Swapping (int64_t))
	BALL_INT64 x = 0x0011223344556677ll;
	swapBytes(x);
	TEST_EQUAL(x, 0x7766554433221100ll);
	swapBytes(x);
	TEST_EQUAL(x, 0x0011223344556677ll);
RESULT

CHECK(Byte Swapping (float))
	__private::UFloat tmp(0.0f);
	tmp.u = 0xAABBCCDD;
	swapBytes(tmp.f);
	TEST_EQUAL(tmp.u, 0xDDCCBBAA);
	swapBytes(tmp.f);
	TEST_EQUAL(tmp.u, 0xAABBCCDD);
RESULT

CHECK(Byte Swapping (double))
	__private::UDouble tmp(0.0);
	tmp.u = 0xAAABACADBABBBCBDull;
	swapBytes(tmp.f);
	TEST_EQUAL(tmp.u, 0xBDBCBBBAADACABAAull);
	swapBytes(tmp.f);
	TEST_EQUAL(tmp.u, 0xAAABACADBABBBCBDull);
RESULT

CHECK(Byte Swapping (arbitrary))
	char bytes[10] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j'};
	swapBytes(bytes);
	TEST_EQUAL(bytes[0], 'j');
	TEST_EQUAL(bytes[1], 'i');
	TEST_EQUAL(bytes[2], 'h');
	TEST_EQUAL(bytes[3], 'g');
	TEST_EQUAL(bytes[4], 'f');
	TEST_EQUAL(bytes[5], 'e');
	TEST_EQUAL(bytes[6], 'd');
	TEST_EQUAL(bytes[7], 'c');
	TEST_EQUAL(bytes[8], 'b');
	TEST_EQUAL(bytes[9], 'a');
	swapBytes(bytes);
	TEST_EQUAL(bytes[0], 'a');
	TEST_EQUAL(bytes[1], 'b');
	TEST_EQUAL(bytes[2], 'c');
	TEST_EQUAL(bytes[3], 'd');
	TEST_EQUAL(bytes[4], 'e');
	TEST_EQUAL(bytes[5], 'f');
	TEST_EQUAL(bytes[6], 'g');
	TEST_EQUAL(bytes[7], 'h');
	TEST_EQUAL(bytes[8], 'i');
	TEST_EQUAL(bytes[9], 'j');
RESULT

CHECK(swapEndianess)
	BinaryFileAdaptor<BALL_UINT32> a;
	a.setSwapEndian(false);

	String outfile_name;
	NEW_TMP_FILE(outfile_name)
	File outfile(outfile_name, ::std::ios::out | ::std::ios::binary);

	a.setData(0xAABBCCDD);
	outfile << a;

	outfile.close();

	File infile(outfile_name, ::std::ios::in | ::std::ios::binary);


	infile >> a;
	TEST_EQUAL(a.getData(), 0xAABBCCDD)

	infile.seekg(0, ::std::ios::beg);

	a.setSwapEndian(true);
	infile >> a;
	TEST_EQUAL(a.getData(), 0xDDCCBBAA)

	infile.close();
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
