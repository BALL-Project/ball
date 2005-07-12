# call this with "python -u cartoon_test.py | tee output.txt"
import os
import shutil

# adapt the paths below
DIR = "/local/amoll/all-pdbs/"
TEMPDIR = "/var/tmp/"
BALLVIEW = "/home/student/amoll/bin/BALLView"
TESTDIR = "/local/amoll/cartoon_test_dir/"

result_file = file = open(TESTDIR + "results.txt", 'w')
#shutil.copyfile(DIR + f, TEMPDIR + "test.pdb")

for f in os.listdir(DIR):
	state = os.spawnl(os.P_WAIT, TESTDIR + "myzcat", "", DIR + f)

	print
	print "--------------------------------"
	print f
	print

	if state != 0:
		continue

	state = os.spawnl(os.P_WAIT, BALLVIEW, "", TESTDIR + "cartoon_test2.py")
	if state == 0:
		result_file.write(f + "   OK\n")
		shutil.move(TEMPDIR + "test1.png", TESTDIR + f + "_1.png")
		shutil.move(TEMPDIR + "test2.png", TESTDIR + f + "_2.png")
	else:
		result_file.write(f + "   ERROR!\n")
		print "ERROR:   " + f

	result_file.flush()
