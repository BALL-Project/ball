// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: TranslationProcessor_test.C,v 1.1 2003/05/23 15:39:23 oliver Exp $

#include <BALL/CONCEPT/classTest.h>

#include <BALL/STRUCTURE/geometricTransformations.h>
#include <BALL/KERNEL/fragment.h>

START_TEST(TranslationApplictor, "$Id: TranslationProcessor_test.C,v 1.1 2003/05/23 15:39:23 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

TranslationProcessor* app;
CHECK(default constructor)
	app = new TranslationProcessor;
	TEST_NOT_EQUAL(app, 0)
RESULT

Vector3	t(0.0, 0.0, 0.0);
CHECK(getTranslation/setTranslation)
	TEST_EQUAL(app->getTranslation(), t)
	t.set(1,2,3);
	app->setTranslation(t);
	TEST_EQUAL(app->getTranslation(), t)
RESULT

CHECK(destructor/constructor)
	delete app;
	app = new TranslationProcessor(t);
	TEST_NOT_EQUAL(app, 0)
	TEST_EQUAL(app->getTranslation(), t)
RESULT

CHECK(operator ())
	using BALL::Fragment;
	using BALL::Atom;

	Fragment* frag = new Fragment;
	Atom* atom1 = new Atom;
	Atom* atom2 = new Atom;
	atom1->setPosition(t);
	t.set(0,0,0);
	atom2->setPosition(t);
	frag->insert(*atom1);
	frag->insert(*atom2);
	frag->apply(*app);
	t.set(1,2,3);
	TEST_EQUAL(atom2->getPosition(), t)
	t *= 2;
	TEST_EQUAL(atom1->getPosition(), t)
RESULT
delete app;


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
