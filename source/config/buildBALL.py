#!/opt/gnu/bin/python

import sys, os, string

# global variables

# a map, containing command lines for the commands we need
commands = {}


# helper function, searching in a file for the first occurence 
# of the string "target"
def seek(inputfile, target):
	line = inputfile.readline()
	while (line and (line.find(target)==-1)):
		line = inputfile.readline()

# this function is responsible for transferring file "file"
# as user "user" to the host "host", path "path"
def put(file, user, host, path):
	
	# prepare the scp - command
	if (commands.has_key("scp")):
		commandline = commands["scp"]
	else:
		commandline = "scp "
		
	commandline += " "+file+" "+user.strip()+"@"+host.strip()
	commandline += ":"+os.path.expanduser(path)
	
	retval = os.system(commandline)

	if (retval != 0):
		sys.exit(retval)

# begin by reading our general config file
# this file contains a list of hostnames, and
# for each of these we assume to have a subdirectory
# it also contains paths to programs we can use
config = open(sys.argv[1], 'r')

# search the paths for commands we can use
seek(config, "[commands]")

commandline = config.readline()
comlist = []
while (commandline and (commandline.find("[") == -1)):
	if (commandline.strip() != ""):
		comlist = string.split(commandline, '=')
		commands[comlist[0]] = comlist[1][:-1]
	commandline = config.readline()
config.close()

config = open(sys.argv[1], 'r')

# search for the subdirs, each containing a seperate
# configuration, in the config file
seek(config, "[subdirs]")

# create a list of all the hosts
subdirlist = []

subdir = config.readline()
while (subdir and (subdir.find("[") == -1)):
	if (subdir.strip() != ""):
		subdirlist.append(subdir[:-1])
	subdir = config.readline()
config.close()

subdirconfig = {}

# now we will walk all the subdirectories for all the hosts
for subdir in subdirlist:
	
	# each subdirectory contains the configuration for one
	# test build we are supposed to do
	# right now, we only need the following infos from there:
	#    - which tarfile should we use
	#		 - which host do we use
	#		 - what path on the host will we use
	subconfigfile = open(subdir+"/config", 'r')
	line = subconfigfile.readline()

	while (line):
		if (line.strip != ""):
			comlist = string.split(line, '=')
			subdirconfig[comlist[0].strip()] = comlist[1].strip()
		line = subconfigfile.readline()
	subconfigfile.close()

	# now, put the tar file on the remote host into remotepath
	put(subdir+"/"+subdirconfig["tarfile"], subdirconfig["user"], subdirconfig["host"], subdirconfig["remotepath"])

	# also, we need the compile script and the corresponding config
	# on the remote host
	put("testBuild.py", subdirconfig["user"], subdirconfig["host"], subdirconfig["remotepath"])
	put(subdir+"/config", subdirconfig["user"], subdirconfig["host"], subdirconfig["remotepath"])

	# finally, execute the testBuild - script on the remote host
	if (commands.has_key("ssh")):
		commandline = commands["ssh"]
	else:
		commandline = "ssh"

	commandline += " "+subdirconfig["user"].strip()+"@"+subdirconfig["host"].strip()+" \""

	commandline += "cd "+os.path.expanduser(subdirconfig["remotepath"])+"; "

	if (subdirconfig.has_key("python")):
		commandline += subdirconfig["python"]
	else:
		commandline += "python"

	commandline += " "+os.path.expanduser(subdirconfig["remotepath"])+"/testBuild.py\""

	os.system(commandline)

	# now we have got the tar-file back... we just have to untar it
	commandline = "cd "+subdir+" && "

	if (commands.has_key("tar")):
		if (commands["tar"].strip() != "echo"):
			commandline += commands["tar"]
		else:
			commandline += "tar"
	else:
		commandline += "tar"

	commandline += " xf ./results.tar"

	os.system(commandline)
