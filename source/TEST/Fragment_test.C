// $Id: Fragment_test.C,v 1.1 1999/08/26 08:02:36 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

#include <BALL/KERNEL/fragment.h>
#include <BALL/KERNEL/molecule.h>

using namespace BALL;

START_TEST(Fragment, "$Id: Fragment_test.C,v 1.1 1999/08/26 08:02:36 oliver Exp $")



/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

Fragment*	frag;
CHECK(default constructor)
frag = new Fragment;
TEST_NOT_EQUAL(frag, 0);
RESULT

CHECK(isValid)
TEST_EQUAL(frag->isValid(), true)
RESULT

CHECK(destructor)
delete frag;
frag = new Fragment;
delete frag;
RESULT

/*
	Fragment
		(const Fragment& fragment,
		 bool deep = true);

	Fragment
		(const String &name)

	virtual void *clone
		(bool deep = true) const;

	virtual void clear
		(void);

	virtual void destroy
		(void);

	Fragment &operator =
		(const Fragment& fragment);

	static const Fragment *getDefault
		(void);
	 virtual bool isValid
		(void) const;

	virtual void dump
		(ostream &s = cout,
		 unsigned long depth = 0) const;

	virtual void read
		(istream &s);

	virtual void write
		(ostream &s) const;
*/

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

END_TEST
