import string, sys, os

# global variables
config = {}
logfiles = ["./BALL/source/TEST/*.log", "./BALL/source/BENCHMARKS/bench.log", "./BALL/source/config.log", "./BALL/source/config.status"]

# this function is responsible for transferring file "file"
# as user "user" to the host "host", path "path"
def put(file, user, host, path):
	
	# prepare the scp - command
	if (config.has_key("scp")):
		commandline = config["scp"]
	else:
		commandline = "scp "
		
	commandline += " "+file+" "+user.strip()+"@"+host.strip()
	commandline += ":"+os.path.expanduser(path)
	
	retval = os.system(commandline)

# First, we need to read our config file

configfile = open("./config", 'r')
line = configfile.readline()

comlist = []
	
while (line):
	if (line.strip != ""):
		comlist = string.split(line, '=')
		if (len(comlist) >= 2):
			config[comlist[0]] = comlist[1][:-1]
			line = configfile.readline()
configfile.close()

tarfilename = config["tarfile"]
tarfilesplit = string.split(tarfilename, '/')
tarfilename = tarfilesplit[len(tarfilesplit)-1]
	
# now, unzip the tar.gz - file
if (config.has_key("gunzip")):
	commandline = config["gunzip"]
else:
	commandline = "gunzip"

commandline+=" "+tarfilename

os.system(commandline)

# now the name of the tarfile should have changed
# either we began with a .tar.gz file which is now
# a .tar - file or we began with a .tgz file, which
# is now a .tar - file as well

tarfilesplit = string.split(tarfilename, '.')

if (tarfilesplit[len(tarfilesplit)-1].strip() == "gz"):
	tarfilename = tarfilename.strip()[:-3]
else:
	tarfilename = tarfilename.strip()[:-4]+".tar"

# now we can untar it
if (config.has_key("tar")):
	commandline = config["tar"]
else:
	commandline = "tar"

commandline += " xf " + tarfilename

os.system(commandline)
		
# now we can run configure with the appropriate options
commandline = "cd ./BALL/source/ && touch config.lic"

os.system(commandline)
	
commandline = "cd ./BALL/source && ./configure "

if (config.has_key("configure_options")):
	commandline += config["configure_options"]

retval = os.system(commandline)

if (retval != 0):
	sys.exit(retval)

# finally, we can do the "make"
commandline = "cd ./BALL/source/ && "
	
if (config.has_key("make")):
	commandline += config["make"]
else:
	commandline += "make"

# todo: have an option for make install
commandline += " && make install"

retval = os.system(commandline)

# cool! we are done! if the build finished ok, we can
# start building the tests and BENCHMARKS. but first
# we have to find the libs we just installed
configmak = open("./BALL/source/config.mak")
line = configmak.readline()

while (line and (line.find("BINFMT") == -1)):
	line = configmak.readline()

clist = string.split(line, '=')

ldpath = config["remotepath"]+"/BALL/lib/"+clist[1]
if (ldpath[-1] == '\n'):
	ldpath=ldpath[:-1]

commandline = "cd ./BALL/source/TEST && export TEST_OPTIONS=-continue"
commandline += " && export LD_LIBRARY_PATH="+ldpath+" && "
commandline += " make test >test.log"

retval = os.system(commandline)

commandline = " cd ./BALL/source/BENCHMARKS "
commandline += " && export LD_LIBRARY_PATH="+ldpath+" && "
commandline += " make bench >bench.log"

retval = os.system(commandline)

# fine! now copy the log files back to our parent host. first,
# we will store them in a tar file
# we take care of the special case tar=echo, which means that
# the user wanted to reuse an already compiled version of ball earlier
if (config.has_key("tar")):
	if (config["tar"].strip() != "echo"):
		commandline = config["tar"]
	else:
		commandline = "tar"
else:
	commandline = "tar"

commandline += " cf results.tar "

for i in logfiles:
	commandline += i+" "

os.system(commandline)

put("results.tar", config["parent_user"], config["parent_host"], config["parent_path"])
