#!/usr/bin/env python

# script to search for common problems in BALL-files
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
	f = open(sys.argv[1])

	# expressions for use in all files.
	exp = [\
		re.compile(''),																		#DOS carriage
		re.compile('BAUSTELLE'),														#good old BAUSTELLE
		re.compile('cout'),																	#no cout in BALL!
		re.compile('cerr'),																	#no cerr in BALL!
		re.compile('[\s(]+int[\s\(\)\&\*]+'),								#integer values are bad!
		re.compile('[\s(]+long[\s\(\)\&\*]+'),              #long values are
		re.compile('[\s(]+short[\s\(\)\&\*]+'),							#short values are bad!
		re.compile(';{2}'),																	#;;
		re.compile('}[\s]*else[\s]*{'),											#} else {
		re.compile('throw[\s]*Exception::NotImplemented'),  #Exception Not Implemented
		re.compile('\([\s]*Exception::NotImplemented'),  		#Exception Not Implemented in throw specifier
		re.compile('[^:]std::endl'),                        #missing :: before std::endl
		re.compile('\([\s]*bool[\s]*\)'),										#superflous bool cast
		re.compile('const[\s]*float[\s]*&'),								#no const float references
		re.compile('const[\s]*double[\s]*&')								#no const double references
	]
	
	# expressions for use in test-files
	#exp_test = []																				#not yet an idea

	exp_check = [																					#test if check is empty
		re.compile('RESULT'),
		re.compile('\A[\s]*\Z'),
		re.compile('BAUSTELLE')
	]

	# expressions for use with header-files
	exp_header = [
		re.compile('///[\s]*\Z'),														#empty comment
		re.compile('/\*\*[\s]*\Z'),													#empty comment
		re.compile('@exception[\s]*NotImplemented'),				#no usefull information
		re.compile('@param[\s]*{'),													#standard problem => tex error
		re.compile('@return[\s]*{') 												#standard problem => tex error
	]


	def write(self, error_code):
		print
		print '------------ ' + `self.linenr` + ' --------------- ' + `error_code`
		print string.strip(self.line[0:-1])


	def ende(self):
		self.f.close()
		print
		sys.exit(self.errors)


	def test_line(self, x):
		#test for DOS-carriage, print it just once
		if self.exp[0].search(x, 0) and self.carriage==0:
			self.carriage=1
			self.write(0)
		for i in range(1, len(self.exp)):
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
		WOERTER=[]
		while self.getLine() == 1:
			for i in range(len(self.exp_header)):
				if self.exp_header[i].search(self.line, 0):
					self.errors = self.errors + 1	
					self.write('docu')
		self.ende()


	def BALL_ALL_TEST(self):
		while self.getLine() == 1:
			pass
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
