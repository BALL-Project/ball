import os, tempfile, sys, re, shutil

if (len(sys.argv) != 3):
	print """Usage: %s BALL-Visualstudio-solution tests-file"""

# prepare the list of tests
testnames = open(sys.argv[2], 'r').readlines()

# make a directory in which we can work without overwriting anything
pwd = os.path.abspath(".")
tempdir = pwd+'\\temp\\'

if (os.path.exists(tempdir) and len(os.listdir(tempdir)) > 0):
  print "Directory "+tempdir+" not empty! Aborting!"
  exit(1)

os.mkdir(tempdir)
tmpslnfile = open(tempdir+os.path.basename(sys.argv[1]), 'w')

# read in the visual studio file
sln_orig = open(sys.argv[1], 'r')

GUIDS = {}

project_line = 'Project("{%s}") = "%s", "TESTS\%s\%s.vcproj", "{%s}"\nEndProject\n'
test_projects_re = re.compile("^Project\(.*\) = \"TESTS\", \"TESTS\"")
sln_lines = sln_orig.readlines()

tests_guid = ""
 
for sln_index in range(len(sln_lines)):
  sln_line = sln_lines[sln_index]
  #find the TEST-projects
  if (test_projects_re.match(sln_line)):
    tests_guid = sln_line.split(",")[2]
    tests_guid = tests_guid.replace('"', '')
    tests_guid = tests_guid.strip()
    tmpslnfile.write(sln_line)
    # don't forget to read the "EndProject" - stuff
    sln_index+=1
    tmpslnfile.write(sln_lines[sln_index])
    # create the project lines
    for test in testnames:
      test = test[:-1]
      # we need two guids, one for the project and one for the .vcproj
      guid_project = os.popen("computeNewGuid").readline().upper()[:-1]
      guid_vcproj = os.popen("computeNewGuid").readline().upper()[:-1]
      GUIDS[test] = guid_vcproj
      tmpslnfile.write(project_line % (guid_project, test, test, test, guid_vcproj))
  elif ("GlobalSection(NestedProjects)" in sln_line):
    tmpslnfile.write(sln_line)
    for test in testnames:
      test = test[:-1]
      tmpslnfile.write("\t\t{%s} = %s\n" % (GUIDS[test], tests_guid))
  else:
    tmpslnfile.write(sln_line)

sln_orig.close()

# now build the directories needed for the individual tests
for test in testnames:
  test = test[:-1]
  os.mkdir(tempdir+"\\"+test)
  inprojfile = open(pwd+"\\Test_Template\\Test_Template.vcproj", 'r')
  outprojfile = open(tempdir+"\\%s\\%s.vcproj" % (test, test), 'w')
  for line in inprojfile.readlines():
    line = re.sub("Test_Template", test, line)
    if ("ProjectGUID" in line):
      outprojfile.write('\t\tProjectGUID="{%s}"\n' % GUIDS[test])
    else:
      outprojfile.write(line)
  outprojfile.close() 
  inprojfile.close()


# clean up
tmpslnfile.close()
