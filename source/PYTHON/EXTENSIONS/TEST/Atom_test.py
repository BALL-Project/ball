#!/usr/bin/env /opt/bin/python2.1
from ClassTest import *

START_TEST('Atom::Atom()')

CHECK('Atom.Atom()')
a = Atom()
TEST_EQUAL(a.isValid(), true)
RESULT()


atom = Atom()
CHECK('Atom.getCharge()')
try:
	TEST_REAL_EQUAL(atom.getCharge(), 0.0)
except:
	FAIL()
RESULT()

CHECK('Atom.setCharge(charge)')
try:
	atom.setCharge(1.23456)
	TEST_REAL_EQUAL(atom.getCharge(), 1.23456)
except:
	FAIL()
RESULT()

CHECK('Atom.getName()')
try:
	TEST_EQUAL(atom.getName(), '')
except:
	FAIL()
RESULT()

CHECK('Atom.setName(name)')
try:
	atom.setName('ATOMNAME')
	TEST_EQUAL(atom.getName(), 'ATOMNAME')
	atom.setName('')
	TEST_EQUAL(atom.getName(), '')
except:
	FAIL()
RESULT()

CHECK('Atom.getElement()')
try:
	TEST_EQUAL(atom.getElement(), Element.UNKNOWN)
except:
	FAIL()
RESULT()

CHECK('Atom.setElement(element)')
try:
	atom.setElement(PTE_.getElement(1))
	TEST_EQUAL(atom.getElement(), PTE_.getElement(1))
except:
	FAIL()
RESULT()

END_TEST()
