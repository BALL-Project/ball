import os
import re

# Get list of files in the current directory.
files = os.listdir(".")

filepat = re.compile(".*\.C")
filepat2 = re.compile("\.C")

t = "OBJS = "

# Iterate through all the files.
for filename in files:
  if filepat.search(filename) != None:
    print t,'\\'
    t = filepat2.sub(".obj", filename)

print t