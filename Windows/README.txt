------------------------------------------------------------------------------------
------------------------------------ CAVEATS: --------------------------------------
------------------------------------------------------------------------------------

1.) You need a version of Microsoft Visual Studio .NET.

2.) BALL can only be installed on Windows if Python is installed!
		Currently we advise you to use the release 2.3.5.

3.) Python support can only be used with an installed QT lib!  
		(You need QT version: 3.2.X or 3.3.X. and Threads have to be switched on)

-----------------------------------------------------------------------------------
------------------------------------ INSTALLATION: --------------------------------
-----------------------------------------------------------------------------------

1.) Install the Contrib files:

BALL requires some standard Unix tools and libraries that are not usually 
found on Windows systems. We provide an archive file with all needed files 
for this tools:

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
	- "PYTHONPATH" to the Python module path (e.g. C:\Python\Lib\site-packages)
	- "BALL_PATH" to the BALL installation 
	- "BALL_DATA_PATH" to the "data" subdirectory in the BALL installation  path (e.g. C:\BALL\data)
	- "QTDIR" (this is in general done automatical by the QT installation)
	- "QTVERSION" to a numerical representation e.g. 335 for version 3.3.5

3.) Building the libraries

Open a Microsoft Visual Studio NET or Microsoft Compiler Suite Command prompt 
(see Windows Start Menu) and  change the directory to "BALL\Windows" . Next call 

build.bat release  
or
build.bat debug

To build the release or debugging version of the libraries.
To clean up all object files etc. call

build.bat release clean
or
build.bat debug clean

This has also to be done if you want switch between the two versions
and create the other one...

BALLView can be started with the batch-file
BALL/Windows/startBALLView.bat

This ensures, that the PATH environment variable contains the
directories for the needed libs.

---------------------------------------------------------------------------------
-------------------------------	KNOWN ISSUES: -----------------------------------
---------------------------------------------------------------------------------

- If experienced some instabilities with this release, please try to use the debugging
  version of the library.

- XDRPersistenceManager fails


Please report any problems you have with this setup to

   ball-bugs@bioinf.uni-sb.de

Thank you!

