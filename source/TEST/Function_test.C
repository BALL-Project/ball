// $Id: Function_test.C,v 1.4 2001/03/09 14:35:01 anker Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

// insert includes here
#include <BALL/MATHS/function.h>

///////////////////////////

START_TEST(class_name, "$Id: Function_test.C,v 1.4 2001/03/09 14:35:01 anker Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

///  insert tests for each member function here         
///

MutableConstant<>* constant_ptr;

CHECK(MutableConstant::MutableConstant())
	constant_ptr = new MutableConstant<>;
	TEST_NOT_EQUAL(constant_ptr, 0)
RESULT

CHECK(MutableConstant::~MutableConstant())
	delete constant_ptr;
RESULT

CHECK(MutableConstant::MutableConstant(DataType constant))
	MutableConstant<> muco1(47.11);
	TEST_REAL_EQUAL(muco1.getConstant(), 47.11)
RESULT

CHECK(MutableConstant::MutableConstant(const MutableConstant& constant))
	MutableConstant<> muco1;
	MutableConstant<> muco2(muco1);
	TEST_NOT_EQUAL(muco2.getConstant(), 47.11)
	muco1.setConstant(47.11);
	MutableConstant<> muco3(muco1);
	TEST_REAL_EQUAL(muco3.getConstant(), 47.11)
RESULT

CHECK(MutableConstant::operator = (const MutableConstant& constant))
	MutableConstant<> muco1;
	MutableConstant<> muco2 = muco1;
	TEST_NOT_EQUAL(muco2.getConstant(), 47.11)
	muco1.setConstant(47.11);
	MutableConstant<> muco3 = muco1;
	TEST_REAL_EQUAL(muco3.getConstant(), 47.11)
RESULT

CHECK(MutableConstant::operator == (const MutableConstant& constant))
	MutableConstant<> muco(8.15);
	MutableConstant<> muco2;
	bool test = (muco == muco2);
	TEST_EQUAL(test, false)
RESULT

CHECK(MutableConstant::operator () ())
	MutableConstant<> muco(8.15);
	TEST_REAL_EQUAL(muco(6789.535481), 8.15)
RESULT

CHECK(MutableConstant::setConstant())
	MutableConstant<> muco;
	muco.setConstant(3.1415);
	TEST_REAL_EQUAL(muco(29847524), 3.1415)
RESULT

CHECK(MutableConstant::getConstant())
	MutableConstant<> muco(1e2);
	TEST_REAL_EQUAL(muco.getConstant(), 1e2)
RESULT


Addition<float, float>* add_ptr;

CHECK(Addition::Addition())
	add_ptr = new Addition<float, float>;
	TEST_NOT_EQUAL(add_ptr, 0)
RESULT

CHECK(Addition::~Addition())
	delete add_ptr;
RESULT

CHECK(Addition::Addition(const Addition& add))
	Addition<float, float> add1;
	add1.setFirst(3.0);
	add1.setSecond(0.1415);
	Addition<float, float> add2(add1);
	TEST_REAL_EQUAL(add1.getFirst(), add2.getFirst())
	TEST_REAL_EQUAL(add1.getSecond(), add2.getSecond())
RESULT

CHECK(Addition::operator = (const Addition& add))
	Addition<float, float> add1;
	add1.setFirst(3.0);
	add1.setSecond(0.1415);
	Addition<float, float> add2 = add1;
	TEST_REAL_EQUAL(add1.getFirst(), add2.getFirst())
	TEST_REAL_EQUAL(add1.getSecond(), add2.getSecond())
RESULT

CHECK(Addition::operator == (const Addition& add))
	Addition<MutableConstant<>, MutableConstant<> > add1;
	add1.getFirst().setConstant(3.0);
	add1.getSecond().setConstant(0.1415);
	Addition<MutableConstant<>, MutableConstant<> > add2;
	bool test = (add1 == add2);
	TEST_EQUAL(test, false)
	add2.setFirst(3.0);
	add2.setSecond(0.1415);
	test = (add1 == add2);
	TEST_EQUAL(test, true)
RESULT

CHECK(Addition::operator () ())
	Addition<MutableConstant<>, MutableConstant<> > add1;
	add1.getFirst().setConstant(3.0);
	add1.getSecond().setConstant(0.1415);
	TEST_REAL_EQUAL(add1(6789.535481), 3.1415)
RESULT

CHECK(Addition::getFirst())
	MutableConstant<> c1(3.0);
	Addition<MutableConstant<>, MutableConstant<> > add1;
	add1.setFirst(c1);
	Addition<MutableConstant<>, MutableConstant<> > add2 = add1;
	bool test(add2.getFirst() == c1);
	TEST_EQUAL(test, true);
RESULT

CHECK(Addition::setFirst())
	MutableConstant<> c1(3.0);
	Addition<MutableConstant<>, MutableConstant<> > add1;
	add1.setFirst(c1);
	Addition<MutableConstant<>, MutableConstant<> > add2 = add1;
	bool test(add2.getFirst() == c1);
	TEST_EQUAL(test, true);
RESULT

CHECK(Addition::getSecond())
	MutableConstant<> c2(0.1415);
	Addition<MutableConstant<>, MutableConstant<> > add1;
	add1.setSecond(c2);
	Addition<MutableConstant<>, MutableConstant<> > add2 = add1;
	bool test(add2.getSecond() == c2);
	TEST_EQUAL(test, true);
RESULT

CHECK(Addition::setSecond())
	MutableConstant<> c2(3.0);
	Addition<MutableConstant<>, MutableConstant<> > add1;
	add1.setSecond(c2);
	Addition<MutableConstant<>, MutableConstant<> > add2 = add1;
	bool test(add2.getSecond() == c2);
	TEST_EQUAL(test, true);
RESULT


Subtraction<float, float>* sub_ptr;

CHECK(Subtraction::Subtraction())
	sub_ptr = new Subtraction<float, float>;
	TEST_NOT_EQUAL(sub_ptr, 0)
RESULT

CHECK(Subtraction::~Subtraction())
	delete sub_ptr;
RESULT

CHECK(Subtraction::Subtraction(const Subtraction& sub))
	Subtraction<float, float> sub1;
	sub1.setFirst(3.0);
	sub1.setSecond(0.1415);
	Subtraction<float, float> sub2(sub1);
	TEST_REAL_EQUAL(sub1.getFirst(), sub2.getFirst())
	TEST_REAL_EQUAL(sub1.getSecond(), sub2.getSecond())
RESULT

CHECK(Subtraction::operator = (const Subtraction& sub))
	Subtraction<float, float> sub1;
	sub1.setFirst(3.0);
	sub1.setSecond(0.1415);
	Subtraction<float, float> sub2 = sub1;
	TEST_REAL_EQUAL(sub1.getFirst(), sub2.getFirst())
	TEST_REAL_EQUAL(sub1.getSecond(), sub2.getSecond())
RESULT

CHECK(Subtraction::operator == (const Subtraction& sub))
	Subtraction<MutableConstant<>, MutableConstant<> > sub1;
	sub1.getFirst().setConstant(3.0);
	sub1.getSecond().setConstant(0.1415);
	Subtraction<MutableConstant<>, MutableConstant<> > sub2;
	bool test = (sub1 == sub2);
	TEST_EQUAL(test, false)
	sub2.setFirst(3.0);
	sub2.setSecond(0.1415);
	test = (sub1 == sub2);
	TEST_EQUAL(test, true)
RESULT

CHECK(Subtraction::operator () ())
	Subtraction<MutableConstant<>, MutableConstant<> > sub1;
	sub1.getFirst().setConstant(3.0);
	sub1.getSecond().setConstant(0.1415);
	TEST_REAL_EQUAL(sub1(6789.535481), 2.8585)
RESULT

CHECK(Subtraction::getFirst())
	MutableConstant<> c1(3.0);
	Subtraction<MutableConstant<>, MutableConstant<> > sub1;
	sub1.setFirst(c1);
	Subtraction<MutableConstant<>, MutableConstant<> > sub2 = sub1;
	bool test(sub2.getFirst() == c1);
	TEST_EQUAL(test, true);
RESULT

CHECK(Subtraction::setFirst())
	MutableConstant<> c1(3.0);
	Subtraction<MutableConstant<>, MutableConstant<> > sub1;
	sub1.setFirst(c1);
	Subtraction<MutableConstant<>, MutableConstant<> > sub2 = sub1;
	bool test(sub2.getFirst() == c1);
	TEST_EQUAL(test, true);
RESULT

CHECK(Subtraction::getSecond())
	MutableConstant<> c2(0.1415);
	Subtraction<MutableConstant<>, MutableConstant<> > sub1;
	sub1.setSecond(c2);
	Subtraction<MutableConstant<>, MutableConstant<> > sub2 = sub1;
	bool test(sub2.getSecond() == c2);
	TEST_EQUAL(test, true);
RESULT

CHECK(Subtraction::setSecond())
	MutableConstant<> c2(3.0);
	Subtraction<MutableConstant<>, MutableConstant<> > sub1;
	sub1.setSecond(c2);
	Subtraction<MutableConstant<>, MutableConstant<> > sub2 = sub1;
	bool test(sub2.getSecond() == c2);
	TEST_EQUAL(test, true);
RESULT


Product<float, float>* prod_ptr;

CHECK(Product::Product())
	prod_ptr = new Product<float, float>;
	TEST_NOT_EQUAL(prod_ptr, 0)
RESULT

CHECK(Product::~Product())
	delete prod_ptr;
RESULT

CHECK(Product::Product(const Product& prod))
	Product<float, float> prod1;
	prod1.setFirst(3.0);
	prod1.setSecond(0.1415);
	Product<float, float> prod2(prod1);
	TEST_REAL_EQUAL(prod1.getFirst(), prod2.getFirst())
	TEST_REAL_EQUAL(prod1.getSecond(), prod2.getSecond())
RESULT

CHECK(Product::operator = (const Product& prod))
	Product<float, float> prod1;
	prod1.setFirst(3.0);
	prod1.setSecond(0.1415);
	Product<float, float> prod2 = prod1;
	TEST_REAL_EQUAL(prod1.getFirst(), prod2.getFirst())
	TEST_REAL_EQUAL(prod1.getSecond(), prod2.getSecond())
RESULT

CHECK(Product::operator == (const Product& prod))
	Product<MutableConstant<>, MutableConstant<> > prod1;
	prod1.getFirst().setConstant(3.0);
	prod1.getSecond().setConstant(0.1415);
	Product<MutableConstant<>, MutableConstant<> > prod2;
	bool test = (prod1 == prod2);
	TEST_EQUAL(test, false)
	prod2.setFirst(3.0);
	prod2.setSecond(0.1415);
	test = (prod1 == prod2);
	TEST_EQUAL(test, true)
RESULT

CHECK(Product::operator () ())
	Product<MutableConstant<>, MutableConstant<> > prod1;
	prod1.getFirst().setConstant(3.0);
	prod1.getSecond().setConstant(0.1415);
	TEST_REAL_EQUAL(prod1(6789.535481), 0.4245)
RESULT

CHECK(Product::getFirst())
	MutableConstant<> c1(3.0);
	Product<MutableConstant<>, MutableConstant<> > prod1;
	prod1.setFirst(c1);
	Product<MutableConstant<>, MutableConstant<> > prod2 = prod1;
	bool test(prod2.getFirst() == c1);
	TEST_EQUAL(test, true);
RESULT

CHECK(Product::setFirst())
	MutableConstant<> c1(3.0);
	Product<MutableConstant<>, MutableConstant<> > prod1;
	prod1.setFirst(c1);
	Product<MutableConstant<>, MutableConstant<> > prod2 = prod1;
	bool test(prod2.getFirst() == c1);
	TEST_EQUAL(test, true);
RESULT

CHECK(Product::getSecond())
	MutableConstant<> c2(0.1415);
	Product<MutableConstant<>, MutableConstant<> > prod1;
	prod1.setSecond(c2);
	Product<MutableConstant<>, MutableConstant<> > prod2 = prod1;
	bool test(prod2.getSecond() == c2);
	TEST_EQUAL(test, true);
RESULT

CHECK(Product::setSecond())
	MutableConstant<> c2(3.0);
	Product<MutableConstant<>, MutableConstant<> > prod1;
	prod1.setSecond(c2);
	Product<MutableConstant<>, MutableConstant<> > prod2 = prod1;
	bool test(prod2.getSecond() == c2);
	TEST_EQUAL(test, true);
RESULT


Division<float, float>* div_ptr;

CHECK(Division::Division())
	div_ptr = new Division<float, float>;
	TEST_NOT_EQUAL(div_ptr, 0)
RESULT

CHECK(Division::~Division())
	delete div_ptr;
RESULT

CHECK(Division::Division(const Division& div))
	Division<float, float> div1;
	div1.setFirst(3.0);
	div1.setSecond(0.1415);
	Division<float, float> div2(div1);
	TEST_REAL_EQUAL(div1.getFirst(), div2.getFirst())
	TEST_REAL_EQUAL(div1.getSecond(), div2.getSecond())
RESULT

CHECK(Division::operator = (const Division& div))
	Division<float, float> div1;
	div1.setFirst(3.0);
	div1.setSecond(0.1415);
	Division<float, float> div2 = div1;
	TEST_REAL_EQUAL(div1.getFirst(), div2.getFirst())
	TEST_REAL_EQUAL(div1.getSecond(), div2.getSecond())
RESULT

CHECK(Division::operator == (const Division& div))
	Division<MutableConstant<>, MutableConstant<> > div1;
	div1.getFirst().setConstant(3.0);
	div1.getSecond().setConstant(0.1415);
	Division<MutableConstant<>, MutableConstant<> > div2;
	bool test = (div1 == div2);
	TEST_EQUAL(test, false)
	div2.setFirst(3.0);
	div2.setSecond(0.1415);
	test = (div1 == div2);
	TEST_EQUAL(test, true)
RESULT

CHECK(Division::operator () ())
	Division<MutableConstant<>, MutableConstant<> > div1;
	div1.getFirst().setConstant(3.0);
	div1.getSecond().setConstant(0.1415);
	TEST_REAL_EQUAL(div1(6789.535481), 21.201413428)
RESULT

CHECK(Division::getFirst())
	MutableConstant<> c1(3.0);
	Division<MutableConstant<>, MutableConstant<> > div1;
	div1.setFirst(c1);
	Division<MutableConstant<>, MutableConstant<> > div2 = div1;
	bool test(div2.getFirst() == c1);
	TEST_EQUAL(test, true);
RESULT

CHECK(Division::setFirst())
	MutableConstant<> c1(3.0);
	Division<MutableConstant<>, MutableConstant<> > div1;
	div1.setFirst(c1);
	Division<MutableConstant<>, MutableConstant<> > div2 = div1;
	bool test(div2.getFirst() == c1);
	TEST_EQUAL(test, true);
RESULT

CHECK(Division::getSecond())
	MutableConstant<> c2(0.1415);
	Division<MutableConstant<>, MutableConstant<> > div1;
	div1.setSecond(c2);
	Division<MutableConstant<>, MutableConstant<> > div2 = div1;
	bool test(div2.getSecond() == c2);
	TEST_EQUAL(test, true);
RESULT

CHECK(Division::setSecond())
	MutableConstant<> c2(3.0);
	Division<MutableConstant<>, MutableConstant<> > div1;
	div1.setSecond(c2);
	Division<MutableConstant<>, MutableConstant<> > div2 = div1;
	bool test(div2.getSecond() == c2);
	TEST_EQUAL(test, true);
RESULT


Reciprocal<MutableConstant<> >* reciprocal_pointer;

CHECK(Reciprocal::Reciprocal())
	reciprocal_pointer = new Reciprocal<MutableConstant<> >;
	TEST_NOT_EQUAL(reciprocal_pointer, 0)
RESULT

CHECK(Reciprocal::~Reciprocal())
	delete reciprocal_pointer;
RESULT

CHECK(Reciprocal::Reciprocal(const rec))
	MutableConstant<> c1(3.0);
	Reciprocal<MutableConstant<> > rec1;
	rec1.setFunction(c1);
	Reciprocal<MutableConstant<> > rec2(rec1);
	bool test = (rec1 == rec2);
	TEST_EQUAL(test, true)
RESULT

CHECK(Reciprocal::operator = (const Reciprocal& rec))
	MutableConstant<> c1(4.0);
	Reciprocal<MutableConstant<> > rec1;
	rec1.setFunction(c1);
	Reciprocal<MutableConstant<> > rec2;
	bool test = (rec1.getFunction() == rec2.getFunction());
	TEST_EQUAL(test, false);
	rec2 = rec1;
	test = (rec1.getFunction() == rec2.getFunction());
	TEST_EQUAL(test, true);
RESULT

CHECK(Reciprocal::operator == (const Reciprocal& rec))
	MutableConstant<> c1(4.0);
	Reciprocal<MutableConstant<> > rec1;
	rec1.setFunction(c1);
	Reciprocal<MutableConstant<> > rec2;
	bool test = (rec1 == rec2);
	TEST_EQUAL(test, false);
	rec2 = rec1;
	test = (rec1 == rec2);
	TEST_EQUAL(test, true);
RESULT

CHECK(Reciprocal::operator () (DataType x))
	MutableConstant<> c3(4711.0);
	Reciprocal<MutableConstant<> > rec1;
	rec1.setFunction(c3);
	TEST_REAL_EQUAL(rec1.operator() (13), 0.000212269)
	// BAUSTELLE
	// DivisionByZero fangen
RESULT

CHECK(Reciprocal::setFunction(const Function& function))
	MutableConstant<> c1(4.0);
	Reciprocal<MutableConstant<> > rec;
	rec.setFunction(c1);
	bool test = (rec.getFunction() == c1);
	TEST_EQUAL(test, true)
RESULT

CHECK(Reciprocal::getFunction())
	MutableConstant<> c1(4.0);
	Reciprocal<MutableConstant<> > rec;
	rec.setFunction(c1);
	bool test = (rec.getFunction() == c1);
	TEST_EQUAL(test,true);
RESULT


SquareFunction<MutableConstant<> >* sqiprocal_pointer;

CHECK(SquareFunction::SquareFunction())
	sqiprocal_pointer = new SquareFunction<MutableConstant<> >;
	TEST_NOT_EQUAL(sqiprocal_pointer, 0)
RESULT

CHECK(SquareFunction::~SquareFunction())
	delete sqiprocal_pointer;
RESULT

CHECK(SquareFunction::SquareFunction(const sq))
	MutableConstant<> c1(3.0);
	SquareFunction<MutableConstant<> > sq1;
	sq1.setFunction(c1);
	SquareFunction<MutableConstant<> > sq2(sq1);
	bool test = (sq1 == sq2);
	TEST_EQUAL(test, true)
RESULT

CHECK(SquareFunction::operator = (const SquareFunction& sq))
	MutableConstant<> c1(4.0);
	SquareFunction<MutableConstant<> > sq1;
	sq1.setFunction(c1);
	SquareFunction<MutableConstant<> > sq2;
	bool test = (sq1.getFunction() == sq2.getFunction());
	TEST_EQUAL(test, false);
	sq2 = sq1;
	test = (sq1.getFunction() == sq2.getFunction());
	TEST_EQUAL(test, true);
RESULT

CHECK(SquareFunction::operator == (const SquareFunction& sq))
	MutableConstant<> c1(4.0);
	SquareFunction<MutableConstant<> > sq1;
	sq1.setFunction(c1);
	SquareFunction<MutableConstant<> > sq2;
	bool test = (sq1 == sq2);
	TEST_EQUAL(test, false);
	sq2 = sq1;
	test = (sq1 == sq2);
	TEST_EQUAL(test, true);
RESULT

PRECISION(0.001)

CHECK(SquareFunction::operator () (DataType x))
	MutableConstant<> c3(4711.0);
	SquareFunction<MutableConstant<> > sq1;
	sq1.setFunction(c3);
	TEST_REAL_EQUAL(sq1.operator() (13), 22193521)
RESULT

CHECK(SquareFunction::setFunction(const Function& function))
	MutableConstant<> c1(4.0);
	SquareFunction<MutableConstant<> > sq;
	sq.setFunction(c1);
	bool test = (sq.getFunction() == c1);
	TEST_EQUAL(test, true)
RESULT

CHECK(SquareFunction::getFunction())
	MutableConstant<> c1(4.0);
	SquareFunction<MutableConstant<> > sq;
	sq.setFunction(c1);
	bool test = (sq.getFunction() == c1);
	TEST_EQUAL(test,true);
RESULT


CubicFunction<MutableConstant<> >* cuiprocal_pointer;

CHECK(CubicFunction::CubicFunction())
	cuiprocal_pointer = new CubicFunction<MutableConstant<> >;
	TEST_NOT_EQUAL(cuiprocal_pointer, 0)
RESULT

CHECK(CubicFunction::~CubicFunction())
	delete cuiprocal_pointer;
RESULT

CHECK(CubicFunction::CubicFunction(const cu))
	MutableConstant<> c1(3.0);
	CubicFunction<MutableConstant<> > cu1;
	cu1.setFunction(c1);
	CubicFunction<MutableConstant<> > cu2(cu1);
	bool test = (cu1 == cu2);
	TEST_EQUAL(test, true)
RESULT

CHECK(CubicFunction::operator = (const CubicFunction& cu))
	MutableConstant<> c1(4.0);
	CubicFunction<MutableConstant<> > cu1;
	cu1.setFunction(c1);
	CubicFunction<MutableConstant<> > cu2;
	bool test = (cu1.getFunction() == cu2.getFunction());
	TEST_EQUAL(test, false);
	cu2 = cu1;
	test = (cu1.getFunction() == cu2.getFunction());
	TEST_EQUAL(test, true);
RESULT

CHECK(CubicFunction::operator == (const CubicFunction& cu))
	MutableConstant<> c1(4.0);
	CubicFunction<MutableConstant<> > cu1;
	cu1.setFunction(c1);
	CubicFunction<MutableConstant<> > cu2;
	bool test = (cu1 == cu2);
	TEST_EQUAL(test, false);
	cu2 = cu1;
	test = (cu1 == cu2);
	TEST_EQUAL(test, true);
RESULT

CHECK(CubicFunction::operator () (DataType x))
	MutableConstant<> c3(11.5);
	CubicFunction<MutableConstant<> > cu1;
	cu1.setFunction(c3);
	TEST_REAL_EQUAL(cu1.operator() (13), 1520.875)
RESULT

CHECK(CubicFunction::setFunction(const Function& function))
	MutableConstant<> c1(4.0);
	CubicFunction<MutableConstant<> > cu;
	cu.setFunction(c1);
	bool test = (cu.getFunction() == c1);
	TEST_EQUAL(test, true)
RESULT

CHECK(CubicFunction::getFunction())
	MutableConstant<> c1(4.0);
	CubicFunction<MutableConstant<> > cu;
	cu.setFunction(c1);
	bool test = (cu.getFunction() == c1);
	TEST_EQUAL(test,true);
RESULT


MutablePower<MutableConstant<> >* mpiprocal_pointer;

CHECK(MutablePower::MutablePower())
	mpiprocal_pointer = new MutablePower<MutableConstant<> >;
	TEST_NOT_EQUAL(mpiprocal_pointer, 0)
RESULT

CHECK(MutablePower::~MutablePower())
	delete mpiprocal_pointer;
RESULT

CHECK(MutablePower::MutablePower(const mp))
	MutableConstant<> c1(3.0);
	MutablePower<MutableConstant<> > mp1;
	mp1.setFunction(c1);
	MutablePower<MutableConstant<> > mp2(mp1);
	bool test = (mp1 == mp2);
	TEST_EQUAL(test, true)
RESULT

CHECK(MutablePower::operator = (const MutablePower& mp))
	MutableConstant<> c1(4.0);
	MutablePower<MutableConstant<> > mp1;
	mp1.setFunction(c1);
	MutablePower<MutableConstant<> > mp2;
	bool test = (mp1.getFunction() == mp2.getFunction());
	TEST_EQUAL(test, false);
	mp2 = mp1;
	test = (mp1.getFunction() == mp2.getFunction());
	TEST_EQUAL(test, true);
RESULT

CHECK(MutablePower::operator == (const MutablePower& mp))
	MutableConstant<> c1(4.0);
	MutablePower<MutableConstant<> > mp1;
	mp1.setFunction(c1);
	MutablePower<MutableConstant<> > mp2;
	bool test = (mp1 == mp2);
	TEST_EQUAL(test, false);
	mp2 = mp1;
	test = (mp1 == mp2);
	TEST_EQUAL(test, true);
RESULT

CHECK(MutablePower::operator () (DataType x))
	MutableConstant<> c3(11.5);
	MutablePower<MutableConstant<> > mp1;
	mp1.setFunction(c3);
	mp1.setExponent(2.4);
	TEST_REAL_EQUAL(mp1.operator() (13), 351.297304)
RESULT

CHECK(MutablePower::setFunction(const Function& function))
	MutableConstant<> c1(4.0);
	MutablePower<MutableConstant<> > mp;
	mp.setFunction(c1);
	bool test = (mp.getFunction() == c1);
	TEST_EQUAL(test, true)
RESULT

CHECK(MutablePower::getFunction())
	MutableConstant<> c1(4.0);
	MutablePower<MutableConstant<> > mp;
	mp.setFunction(c1);
	bool test = (mp.getFunction() == c1);
	TEST_EQUAL(test,true);
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
