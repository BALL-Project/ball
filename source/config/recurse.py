#!/opt/gnu/bin/python
import sys
import glob
import os
import stat
from time import localtime,asctime


if (len(sys.argv) != 3):
	print 'Two arguments required!'
	sys.exit(1)

path_A = sys.argv[1]
path_B = sys.argv[2]

def collect_files(path):
	all_list = []
	# recurse all directories
	for f in os.listdir(path):
		mode = os.stat('%s/%s' % (path, f))[stat.ST_MODE]
		new_path = '%s/%s' % (path, f)
		if stat.S_ISDIR(mode):
      # recurse into directory
			all_list = all_list + collect_files(new_path)
		else:
			all_list = all_list + [new_path]
		
	return all_list

print 'Collecting files in ',path_A,'...',
path = os.getcwd()
os.chdir(path_A)
listA =  collect_files('.')
listA.sort()
print '(',len(listA),'files)'


print 'Collecting files in ',path_B,'...',
os.chdir(path)
os.chdir(path_B)
listB =  collect_files('.')
os.chdir(path)
listB.sort()
print '(',len(listB),'files)'


missing_in_B = []
new_in_B = []
common = []
print 'comparing files'
for i in listA:
	if (i not in listB):
		missing_in_B = missing_in_B + [i]
	else:
		listB.remove(i)
		common = common + [i]

new_in_B = listB	

print 'Files in common: ',len(common)
print 'Files missing in ',path_B,': ', len(missing_in_B)
print 'Files missing in ',path_A,': ', len(new_in_B)




newer_in_A = []
newer_in_B = []
identical = []
same_time_only = []
same_size = []

for i in common:
	stat_A = os.stat('%s/%s' % (path_A, i))
	stat_B = os.stat('%s/%s' % (path_B, i))
	if (stat_A[stat.ST_MTIME] < stat_B[stat.ST_MTIME]):
		newer_in_B = newer_in_B + [i]
	elif (stat_A[stat.ST_MTIME] != stat_B[stat.ST_MTIME]):
		newer_in_A = newer_in_A + [i]
		if (stat_A[stat.ST_SIZE] == stat_B[stat.ST_SIZE]):
			same_size = same_size + [i]
	else:
		if (stat_A[stat.ST_SIZE] == stat_B[stat.ST_SIZE]):
			identical = identical + [i]
		else:
			same_time_only = same_time_only + [i]


print 'Identical files: ', len(identical)
print 'Same time only:  ', len(same_time_only)
print 'Same size:       ', len(same_size)
print 'Newer in A     : ' , len(newer_in_A)
print 'Newer in B     : ' , len(newer_in_B)


same_size_only = []
for i in same_size:
	ret_val = os.system('cmp -s %s/%s %s/%s ' % (path_A, i, path_B, i))  
	if (ret_val == 0):
		identical = identical + [i]
		if (i in newer_in_A):
			newer_in_A.remove(i)
		if (i in newer_in_B):
			newer_in_B.remove(i)
	else:
		same_size_only = same_size_only + [i]

print 'Identical files: ', len(identical)
print 'Same time only:  ', len(same_time_only)
print 'Same size only:  ', len(same_size_only)
print 'Newer in A     : ' , len(newer_in_A)
print 'Newer in B     : ' , len(newer_in_B)
			

print 'Files, that are newer in A than in B:'
print '=---------------------------------------'
for i in newer_in_A + same_size_only:
	print 'cp %s/%s %s/%s' % (path_A, i, path_B, i)


print 'Files, that do not exist in B (',len(missing_in_B),'):'
print '=---------------------------------------'
for i in missing_in_B:
	print 'cp %s/%s %s/%s' % (path_A, i, path_B, i)
