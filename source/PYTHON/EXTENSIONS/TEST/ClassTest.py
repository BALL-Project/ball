#!env python -f
false = 0
true = 1

import sys
sys.path.append("/home/oliver/BALL/lib/Linux-i386-g++_3.0.2")

from BALL import *

class Status:
	def __init__(self):
		# whether all tests 
		self.all_passed = false
		# whether the current test passed
		self.current_passed = false

		# the name of the current test (usually the signature of the method being tested)
		self.current_name = ''
		
		# whether we are inside a CHECK/RESULT block or outside
		self.in_test = false
		
		# whether the results are printed or not
		self.verbose = false

		# the precision for floating point comparisons
		self.precision = 1e-6

		# check for command line arguments: -v
		if (len(sys.argv) == 2):
			self.verbose = (sys.argv[1] == "-v")
		else:
			self.verbose = false

	def check(self, args):
		self.current_passed = true
		self.in_test = true
		self.current_name = args
		if self.verbose:
			print "running test for '" + self.current_name + "'"

	def passTest(self, msg):
		# verify that we are inside a test block
		if not self.in_test:
			print "ERROR: calling passTest outside a CHECK/RESULT block!"
			sys.exit(2)
		
		if self.verbose:
			print "  passed:",msg

	def failTest(self, msg):
		# verify that we are inside a test block
		if not self.in_test:
			print "ERROR: calling failTest outside a CHECK/RESULT block!"
			sys.exit(2)
		self.current_passed = false
		
		if self.verbose:
			print "  failed:",msg

	def result(self):	
		self.all_passed = self.all_passed & self.current_passed
		self.in_test = false;
		if self.verbose:
			if self.current_passed == true:
				print " - passed"
			else:
				print " - FAILED"

	def startTest(self, args):
		self.all_passed = true
		if self.verbose:
			print "running class tests for",args

	def endTest(self):
		if self.all_passed:
			print "OK"
			sys.exit(0)
		else:
			print "FAILED"
			sys.exit(1)

	def getPrecision(self):
		return self.precision

	def setPrecision(self, x):
		self.precision = x

status = Status()

def START_TEST(args):
	status.startTest(args)

def END_TEST():
	status.endTest()

def CHECK(args):
	status.check(args)
	
def RESULT():
	status.result()

def TEST_EQUAL(a, b):
	msg = 'TEST_EQUAL: expected ' + str(b) + ', got ' + str(a)
	if (a == b):
		status.passTest(msg)
	else:
		status.failTest(msg)

def TEST_NOT_EQUAL(a, b):
	msg = 'TEST_NOT_EQUAL: forbidden is ' + str(b) + ', got ' + str(a)
	if (a != b):
		status.passTest(msg)
	else:
		status.failTest(msg)

def PRECISION(x):
	status.setPrecision(x)

def TEST_REAL_EQUAL(a, b):
	difference = abs(a - b)
	msg = 'TEST_REAL_EQUAL: expected ' + str(b) + ', got ' + str(a) + ' (difference was ' + str(difference) + ', precision was ' + str(status.getPrecision()) + ')'
	if (abs(a - b) < status.getPrecision()):
		status.passTest(msg)
	else:
		status.failTest(msg)

def FAIL():
	status.failTest('caught unexpected exception')
