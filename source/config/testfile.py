#!/usr/bin/env python
import sys, string

class test:
	errors=0
	line=''
	lastline=''
	linenr= -1
	f = open(sys.argv[1])
	
	def write(self):
		self.errors = self.errors + 1
		self.linenr = self.linenr + 1
		self.lastline = self.line
		self.line = self.f.readline()
		print	
		print '------------ ' + `self.linenr` + ' ---------------'
		self.lastline = string.strip(self.lastline)
		self.line = string.strip(self.line)
		if self.line_has('*/'):
			print self.lastline + '    ' + self.line
			self.lastline = self.line
			self.getLine()
			self.line = string.strip(self.line[0:])
			print self.line
			return 1
		else:
			print self.lastline
			print self.line

	def ende(self):
		self.f.close()
		print
		sys.exit(1)	 	
	
	def line_has(self, x):
		return string.find(self.line, x) != -1
 	
	def getLine(self):
		self.linenr = self.linenr + 1
		self.lastline = self.line
		self.line = self.f.readline()
		if not self.line: sys.exit(1)
		self.line = string.strip(self.line[0:-1])
		if self.line_has('BAUSTELLE')\
			or self.line_has('throw Exception::NotImplemented(__FILE__, __line__)') \
			or self.line_has('cout <<'):
			self.write()
		return 1

	def BALL_TEST_TEST(self):
		while self.getLine() == 1:
			if self.line[:5] =='CHECK':
				self.getLine()
				if self.line_has('RESULT') or self.line == '' or self.line_has('BAUSTELLE'):
					self.write()
		ende()
 	
	def BALL_HEADER_TEST(self):
		WOERTER=[]
		while self.getLine() == 1:
			if (self.line_has('///') and not self.line_has('////')) or self.line_has('/**'):
				WOERTER = string.split(self.line)
				if len(WOERTER) < 2:
					self.write()
		ende()
 	
	def BALL_ALL_TEST(self):
		while self.getLine() == 1:
			pass
		ende()	


	def __init__(self, filename):
		global f
		#f = open(filename)	
	
		# file is a TEST-FILE
		if string.find(filename, '_test.C') != -1:
			self.BALL_TEST_TEST()
			
		# file is a HEADER-FILE	
		if string.find(filename, '.h') != -1:
			self.BALL_HEADER_TEST()
			
		# general testing		
		self.BALL_ALL_TEST()


if len(sys.argv) <2:
	print 'zu wenige argumente'
	sys.exit(-1)
t = test(sys.argv[1])
sys.exit(t.errors)