#!/usr/bin/env /opt/bin/python2.1
from ClassTest import *

START_TEST('ClassTest')

CHECK('TEST_EQUAL')
try:
	TEST_EQUAL(1, 1)
except:
	FAIL()
RESULT()

CHECK('TEST_NOT_EQUAL')
try:
	TEST_NOT_EQUAL(1, 0)
except:
	FAIL()
RESULT()

CHECK('TEST_REAL_EQUAL')
try:
	TEST_REAL_EQUAL(1.0000001, 1.0000002)
except:
	FAIL()
RESULT()

END_TEST()
