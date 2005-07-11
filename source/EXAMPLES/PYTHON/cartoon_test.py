import os
import shutil

# adapt the paths below
DIR = "/local/amoll/pdbs/"
TEMPDIR = "/var/tmp/"
BALLVIEW = "/home/student/amoll/bin/BALLView"
TESTDIR = "/home/student/amoll/cartoon_test_dir/"

result_file = file = open(TESTDIR + "results.txt", 'w')

for f in os.listdir(DIR):
	print f
	shutil.copyfile(DIR + f, TEMPDIR + "test.pdb")
	state = os.spawnl(os.P_WAIT, BALLVIEW, "", TESTDIR + "cartoon_test2.py")
	shutil.move(TEMPDIR + "test1.png", TESTDIR + f + "_1.png")
	shutil.move(TEMPDIR + "test2.png", TESTDIR + f + "_2.png")
	if state == 0:
		result_file.write(f + "   OK\n")
	else:
		result_file.write(f + "   ERROR!\n")

