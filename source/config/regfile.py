#!/usr/bin/env python
# script to create regular files for BALL-Tests
import sys, string 

filename='NMRSpectrum_test.dat'

if len(sys.argv) > 1:
	filename= sys.argv[1]
	
precision = 2	
if len(sys.argv) > 2:
	precision = int(sys.argv[2])

def _express_negative(nr, precision):
	erg = ''
	pos = 0
	for pos in range(len(nr)):
		if nr[pos] == '.':
			break
		
	if nr[pos] != '.':
		print 'PROBLEM'
		sys.exit(-1)

	pos = min(pos + precision + 1, len(nr))	
	nr = nr[0:pos]
	erg = '[0-9]*'
	uebertrag = '+-'

	while len(nr) > 0:
		x = nr[len(nr) - 1]
		
		if x == '.' or x == '-':
			erg = x + erg
			nr = nr[0:-1]
			continue
		
		if uebertrag == '0':
			erg = x + erg
			nr = nr[0:-1]
			continue
			
		if uebertrag == '+-':
			nr = nr[0:-1]
			if x == '9':
				erg = '[890]' + erg
				uebertrag = '-'
				continue
			if x == '0':
				erg = '[901]' + erg
				uebertrag = '+'
				continue
			erg = '[' + `int(x) - 1` + '-' + `int(x) + 1` +']' + erg
			uebertrag = '0'
			continue
	
		if x == '9' and uebertrag == '-':
			erg = '[90]' + erg
			nr = nr[0:-1]
			continue

		if x == '0' and uebertrag == '+':
			erg = '[90]' + erg
			nr = nr[0:-1]
			continue
		
		if uebertrag == '-':
			erg = '[' + x + '-' + `int(x) + 1` +']' + erg
			uebertrag = '0'
			
		if uebertrag == '+':
			erg = '[' + `int(x) - 1` + '-' + x +']' + erg
			uebertrag = '0'
			
		nr = nr[0:-1]
	return erg
	
	
def express(nr, precision):
	try:
		float(nr)
	except:
		return nr
		
	integer = 1
	try:
		int(nr)
	except:
		integer=0
	if integer == 1:
		return nr

	if nr[0] == '-':
		return _express_negative(nr, precision)
		
	erg = ''
	pos = 0
	for pos in range(len(nr)):
		if nr[pos] == '.':
			break
		
	if nr[pos] != '.':
		print 'PROBLEM'
		sys.exit(-1)

	pos = min(pos + precision + 1, len(nr))	
	nr = nr[0:pos]
	erg = '[0-9]*'
	uebertrag = '+-'

	while len(nr) > 0:
		x = nr[len(nr) - 1]
		
		if x == '.' or x == '-':
			erg = x + erg
			nr = nr[0:-1]
			continue
		
		if uebertrag == '0':
			erg = x + erg
			nr = nr[0:-1]
			continue
			
		if uebertrag == '+-':
			nr = nr[0:-1]
			if x == '9':
				erg = '[890]' + erg
				uebertrag = '+'
				continue
			if x == '0':
				erg = '[901]' + erg
				uebertrag = '-'
				continue
			erg = '[' + `int(x) - 1` + '-' + `int(x) + 1` +']' + erg
			uebertrag = '0'
			continue
	
		if x == '0' and uebertrag == '-':
			erg = '[90]' + erg
			nr = nr[0:-1]
			continue

		if x == '9' and uebertrag == '+':
			erg = '[90]' + erg
			nr = nr[0:-1]
			continue
		
		if uebertrag == '+':
			erg = '[' + x + '-' + `int(x) + 1` +']' + erg
			uebertrag = '0'
			
		if uebertrag == '-':
			erg = '[' + `int(x) - 1` + '-' + x +']' + erg
			uebertrag = '0'
			
		nr = nr[0:-1]
	return erg
	

f = open(filename)
while 1:
	LINE= f.readline()
	LINE = LINE[0:-1]
	if not LINE: break
	FIELD=string.split(LINE)

	for i in range(0,len(FIELD)):
		if i == 0:
			print '/' + express(FIELD[0], precision),
		else:
			print express(FIELD[i], precision),
	print
