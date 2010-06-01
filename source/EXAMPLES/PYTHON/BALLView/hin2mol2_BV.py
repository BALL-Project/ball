#
# Lara Schneider 2010-04-15
# A very simple utility for converting HyperChem files into MOL2 files
#

import sys
from BALL import *

#### for use in BALLView
system = getSystems()[0]
infile_name = str(system.getName()) + ".hin"

#create outfile name from infile name
pos = infile_name.rfind('.',0,len(infile_name))
if (pos == -1):
  #if file does not exist: complain and abort
	print "error opening ", infile_name, " for input."
	exit()

outfile_name = infile_name[:pos]
outfile_name = outfile_name + ".mol2"

#create and write MOL2 file 
outfile = MOL2File(outfile_name, File.MODE_OUT)
outfile.write(system)
outfile.close()
print "Wrote MOL2 file " , outfile_name, " in BALL/build"

#done


