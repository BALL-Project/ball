#
# Lara Schneider 2010-04-15
# A very simple utility for converting HyperChem files into MOL2 files
#

import sys
from BALL import *

#### for use in BALLView
#system = getSystems()[0]
infile_name ="" 

#### for command line use
#issue a usage hint if called without parameters
if (len(sys.argv) != 2):
	print "Usage: ", sys.argv[0], " <HIN infile>"
	exit()

#create outfile name from infile name
infile_name = sys.argv[1]
pos = infile_name.rfind('.',0,len(infile_name))
if (pos == -1):
  #if file does not exist: complain and abort
	print "error opening ", infile_name, " for input."
	exit()

outfile_name = infile_name[:pos]
outfile_name = outfile_name + ".mol2"

#create a system and read the content of the HIN file
system = System()
infile = HINFile(infile_name)
infile.read(system)
infile.close()
print "System contains " , system.countAtoms() , " atoms."

#create and write MOL2 file 
outfile = MOL2File(outfile_name, File.MODE_OUT)
outfile.write(system)
outfile.close()
print "Wrote MOL2 file " , outfile_name

#done


