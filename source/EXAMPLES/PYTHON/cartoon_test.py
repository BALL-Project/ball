# call this with "python -u cartoon_test.py | tee output.txt"
# not to be used in BALLView!
import os
import shutil

# adapt the paths below
DIR = "/big/incoming/all-pdbs/"
TEMPDIR = "/var/tmp/"
BALLVIEW = "/usr/src/BALL/source/APPLICATIONS/BALLVIEW/BALLView"
TESTDIR = "/big/incoming/cartoon_test_dir/"
ZCAT = TESTDIR + "myzcat"
print ZCAT

result_file = file = open(TESTDIR + "results.txt", 'w')

for f in os.listdir(DIR):
	os.popen("zcat " + DIR + f  + " > " + TEMPDIR + "test.pdb");
	print
	print "--------------------------------"
	print DIR + f
	print
	state = os.spawnl(os.P_WAIT, BALLVIEW, "", "cartoon_test2.py")
	if state == 0:
		result_file.write(f + "   OK\n")
		shutil.move(TEMPDIR + "test1.png", TESTDIR + f + "_1.png")
		#shutil.move(TEMPDIR + "test2.png", TESTDIR + f + "_2.png")
	else:
		result_file.write(f + "   ERROR!\n")
		print "ERROR:   " + f
	result_file.flush()

