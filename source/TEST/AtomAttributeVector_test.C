// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: AtomAttributeVector_test.C,v 1.1.32.1 2007/03/25 21:46:45 oliver Exp $
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/KERNEL/atom.h>
///////////////////////////

START_TEST(class_name, "$Id: AtomAttributeVector_test.C,v 1.1.32.1 2007/03/25 21:46:45 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

CHECK(~AttributeVector() throw())
	Atom::AttributeVector a;
	Atom::StaticAtomAttributes as;
	a.push_back(as);
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
