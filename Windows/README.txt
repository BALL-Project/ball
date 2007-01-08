
This document explains how to build BALL and BALLView on the windows platform by
using the Visual Studio project files.

------------------------------------------------------------------------------------
------------------------------------ REQUIREMENTS: ---------------------------------
------------------------------------------------------------------------------------

1.) You need a version of Microsoft Visual Studio NET.
		If you have a commercial version of Visual Studio, you can proceed 
		to step 2...

		Microsoft Visual C++ Express can be obtained free of charge from
		http://msdn.microsoft.com/vstudio/express/visualc/download/
		If you use the express version, you need also to install the 
		Microsoft Windows Platform SDK!
		http://www.microsoft.com/msdownload/platformsdk/sdkupdate/
		For the Express version, building BALL per project files, doesnt
		seem to work. See README2.txt for instructions on how to build BALL
		per Makefiles.

2.) BALL can only be installed if Python 2.3.x is installed!
		(Currently we advise you to use the release 2.3.5.)
		Python can be obtained free of charge from
		http://www.python.org/ftp/python/2.3.5/Python-2.3.5.exe

3.) To create the GUI part of BALL or Python support for BALL, you need the QT 
		library 3.3.X. (Threads support enabled).
		Unfortunately QT 3.3 for Windows is not available under the GPL, so 
		you need a commercial licence if you want to compile the GUI part.
		Future versions of BALL will be build on top of QT 4.X, which is 
		available under the GPL for Windows.

-----------------------------------------------------------------------------------
------------------------------------ INSTALLATION: --------------------------------
-----------------------------------------------------------------------------------

1.) Install the Contrib files:

BALL requires some standard Unix tools and libraries that are not usually found on 
Windows systems. We provide an archive file with all needed files for this tools:
http://www.ball-project.org/Downloads/Contrib/Contrib-1.1.1.zip
The Contib folder in the archive must be copied into the BALL\Windows\ directory. 

This file contains:
	- FLEX: a fast lexical analyser
	- BISON: a parser generator
	- regex: a regular expression library
	- RPC: the remote procedure call library (for XDR)
	- sip: a Python binding generator

2.) Set the following environment variables: 
		
	- "PYTHONDIR" to the path of your Python installation
	- "BALL_PATH" to the BALL installation 
	- "BALL_DATA_PATH" to the "data" subdirectory in the BALL installation  path (e.g. C:\BALL\data)
	- "QTVERSION" to a numerical representation for your QT version e.g. 335 for version 3.3.5
	- "QTDIR" (this is in general done automatical by the QT installation)

3.) Building BALL:

Open BALL\Windows\BALL-project.sln
We have a configuration set for Debuging and one for creating a Release version.
If you switch between the both, you have to clean up the whole project.

Build the projects in the following order:

BALL-lib
VIEW-lib
Python-lib
BALLView

To start BALLView, call 

BALL\Windows\startBALLView  			or
BALL\Windows\startBALLViewDebug

depending, if you created the debug or release version of BALL and BALLView.

If you dont have QT 3.3.X installed, you can only build the core library BALL-lib!


We have also project entries to create Tutorials, Benchmarks and Tests.

You can also create all parts of the project per Makefile in the Command line.
For instructions on how to do this, see README2.txt.

---------------------------------------------------------------------------------
-------------------------------	RUNNING THE TEST SUITE: -------------------------
---------------------------------------------------------------------------------
To run the test suite, just call
runTests.bat 										or
runTestsDebug.bat

If a test should fail, a Log file is created in the folder TEST.

---------------------------------------------------------------------------------
-------------------------------	KNOWN ISSUES: -----------------------------------
---------------------------------------------------------------------------------

- If experienced some instabilities with this release, please try to use the debugging
  version of the library.

- XDRPersistenceManager fails


Please report any problems you have with this setup to

	ball-bugs@bioinf.uni-sb.de

Thank you!
