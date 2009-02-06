#!/usr/bin/python

import sys

from amberparser import *

def printUsage():
	print "\nUsage: ./parmConvert.py -i filename [-m file1 .. fileN] [-o output_file]\n"
	print "paramConvert accepts the following parameters:"
	print "\t--help                      Show this help and exit."
	print "\t-p,--parm filename          The pram*.dat file which should be processed"
	print "\t-i,--in file1, .. , fileN   The .in files that should be processed"
	print "\t-m,--mod	file1 .. fileN   A list of frcmod files"


inputFile=""
inFiles=[]
modFiles=[]
outputFile=""

i = 1
while i < len(sys.argv):
	p = sys.argv[i]
	if p == "--help":
		printUsage()
		exit(0)
	elif p == "-p" or p == "--parm":
		i += 1
		inputFile = sys.argv[i]
	elif p == "-o" or p == "--output":
		i += 1
		outputFile = sys.argv[i]
	elif p == "-i" or p == "--in":
		i += 1
		start = i
		while i < len(sys.argv) and not sys.argv[i].startswith("-"):
			i += 1
		inFiles=sys.argv[start:i]
		i -= 1
	elif p == "-m" or p == "--mod":
		i += 1
		start = i
		while i < len(sys.argv) and not sys.argv[i].startswith("-"):
			i += 1
		modFiles=sys.argv[start:i]
	else:
		print "Unknown parameter " + sys.argv[i]
		printUsage()
		exit(-1)
	i += 1

if inputFile == "":
	print "You must specify an input file!"
	printUsage()
	exit(-1)

if outputFile == "":
	outputFile = inputFile.split(".")[0]

#try:
#	input = open(inputFile)
#except:
#	print "You must specify a valid input file!"
#	exit(-1)

try:
	output = open(outputFile, "w")
except:
	print "Could not open " + outputFile + " for writing"
	exit(-1)

#line = input.readline()
#output.write(";" + line)

parser = AmberParser()
param = parser.parse(inputFile)

inparser = InParser()
param2 = inparser.parse(inFiles)

param.write(output)
param2.write(output)

