#!/usr/bin/env python

# script to search for common problems in BALL-files
# you can specify a specific problem to look for.
# to do so add a number as second argument.
# 0 is the first general problem
# 100 is the first problem in header-files

import sys, string, re

if len(sys.argv) <2:
	print 'zu wenige argumente'
	sys.exit(-1)

class test:
	errors=0
	line=''
	lastline=''
	linenr= -1
	carriage=0
	testnr= 0
	f = open(sys.argv[1])

	# expressions for use in all files.
	exp = [\
		re.compile(''),																		#00 DOS carriage
		re.compile('\?\?\?'),																	#01 code problems
		re.compile('cout'),																	#02 no cout in BALL!
		re.compile('cerr'),																	#03 no cerr in BALL!
		re.compile('[\s(]+int[\s\(\)\&\*]+'),								#04 integer values are bad!
		re.compile('[\s(]+long[\s\(\)\&\*]+'),              #05 long values are
		re.compile('[\s(]+short[\s\(\)\&\*]+'),							#06 short values are bad!
		re.compile(';{2}'),																	#07 ;;
		re.compile('}[\s]*else[\s]*{'),											#08 } else {
		re.compile('throw[\s]*Exception::NotImplemented'),  #09 Exception Not Implemented
		re.compile('\([\s]*Exception::NotImplemented'),  		#10 Exception Not Implemented in throw specifier
		re.compile('[^:]std::endl'),                        #11 missing :: before std::endl
		re.compile('\([\s]*bool[\s]*\)'),										#12 superflous bool cast
		re.compile('const[\s]*float[\s]*&'),								#13 no const float references
		re.compile('const[\s]*double[\s]*&')								#14 no const double references
	]
	
	# expressions for use with header-files
	exp_header = [
		re.compile('///[\s]*\Z'),														#100 empty comment
		re.compile('/\*\*[\s]*\Z'),													#101 empty comment
		re.compile('@exception[\s]*NotImplemented'),				#102 no usefull information
		re.compile('@param[\s]*{'),													#103 standard problem => tex error
		re.compile('@return[\s]*{') 												#104 standard problem => tex error
	]
	
	#test if check is empty																		
	exp_check = [	
		re.compile('RESULT'),
		re.compile('\A[\s]*\Z'),
		re.compile('BAUSTELLE')
	]

	# expressions for use in test-files
	#exp_test = []																				#not yet an idea

			
	def write(self, error_code):
		print '\n------------ ' + `self.linenr` + ' --------------- ' + `error_code`
		print string.strip(self.line[0:-1]),


	def ende(self):
		self.f.close()
		if self.errors > 0:
			print
		sys.exit(self.errors)


	def test_line(self, x):
		#test for DOS-carriage, print it just once
		if self.exp[0].search(x, 0) and self.carriage==0:
			self.carriage=1
			self.write(0)
		for i in range(0, len(self.exp)):
			if self.exp[i].search(x, 0):
				self.errors = self.errors + 1	
				self.write(i)
	
	
	def getLine(self):
		self.linenr = self.linenr + 1
		self.lastline = self.line
		self.line = self.f.readline()
		if not self.line: return 0
		self.test_line(self.line)
		return 1


	def BALL_TEST_TEST(self):
		while self.getLine() == 1:
			if self.line[:5] =='CHECK':
				self.getLine()
				for i in range(len(self.exp_check)):
					if self.exp_check[i].search(self.line, 0):
						self.errors = self.errors + 1	
						self.write('test ' + `i`)		
		self.ende()


	def BALL_HEADER_TEST(self):
		while self.getLine() == 1:
			for i in range(len(self.exp_header)):
				if self.exp_header[i].search(self.line, 0):
					self.errors = self.errors + 1	
					self.write('header')
		self.ende()


	def BALL_ALL_TEST(self):
		while self.getLine() == 1:
			pass
		self.ende()

	def ONE_TEST(self):
		if self.testnr < 100:
			if self.testnr > len(self.exp):
				print 'no valid number of test given, aborting...'
				sys.exit(-1)
			myexp=self.exp[self.testnr]
		if self.testnr > 99 and self.testnr < 200:
			if self.testnr - 100 > len(self.exp_header):
				print 'no valid number of test given, aborting...'
				sys.exit(-1)
			myexp=self.exp_header[self.testnr - 100]
		
		self.line = self.f.readline()
		while self.line:
			self.linenr = self.linenr + 1
			if myexp.search(self.line, 0):
				self.errors = self.errors + 1
				self.write(sys.argv[2])
			self.lastline = self.line
			self.line = self.f.readline()
		self.ende()

	def debug(self):
		while self.getLine() == 1:
			# Header tests
			for i in range(len(self.exp_header)):
				if self.exp_header[i].search(self.line, 0):
					self.errors = self.errors + 1
					print '  H ', i
					
			# Testfile tests	
			for i in range(len(self.exp_test)):
				if self.exp_test[i].search(self.line, 0):
					self.errors = self.errors + 1
					print '  T ', i


	def __init__(self):
		try:
			self.testnr=int(sys.argv[2])
			if self.testnr>299 or self.testnr<0:
				print 'no valid number of test given, aborting...'
				self.testnr=0
		except:
			self.testnr=0
		# test for a given problem
		if self.testnr != 0:
			self.ONE_TEST()
			sys.exit(self.errors)
		
		# file for debuging this script
		if string.find(sys.argv[1], 'test.file') != -1:
			self.debug()
			sys.exit(self.errors)
			
		# general testing
		self.BALL_ALL_TEST()
		
		# file is a TEST-FILE
		if string.find(sys.argv[1], '_test.C') != -1:
			self.BALL_TEST_TEST()
			sys.exit(self.errors)
			
		# file is a HEADER-FILE
		if string.find(sys.argv[1], '.h') != -1:
			self.BALL_HEADER_TEST()
			sys.exit(self.errors)

		sys.exit(self.errors)



t = test()
sys.exit(t.errors)
