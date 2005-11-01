
This document explains how to build BALL and BALLView on the windows platform.

------------------------------------------------------------------------------------
------------------------------------ REQUIREMENTS: ---------------------------------
------------------------------------------------------------------------------------

1.) You need either a version of Microsoft Visual Studio .NET or the
    Microsoft Visual C++ Toolkit, which can be obtained free of charge from
	  http://msdn.microsoft.com/visualc/vctoolkit2003/

2.) BALLView can only be installed if Python 2.3.x is installed!
		(Currently we advise you to use the release 2.3.5.)
		Furthermore you need the QT library 3.2.X or 3.3.X. (Threads support enabled)
		If you create OpenSource code and accept the GPL, QT can be downloaded free of 
		charge from ftp://ftp.trolltech.com/qt/source/

3.) Python support is only available with QT (see point 2).

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

3.) Building the libraries with QT and BALLView

Open a Microsoft Visual Studio .NET or Microsoft Compiler Suite Command prompt 
(see Windows Start Menu) and change the directory to "BALL\Windows" . Next call 

build release                   or
build debug

To build the release or debugging version of the libraries.
To clean up all object files etc. call

build release clean             or
build debug clean

This has also to be done if you want switch between the two versions and create the other one!

BALLView can be started with one of the following batch-files which ensures, that the PATH 
environment variable contains the directories for the needed libs.

startBALLView.bat               or
startBALLViewDebug.bat

4.) Building the libraries without QT and BALLView:

If you dont have the QT library available or dont need the GUI and Python Support, 
you can create the core library BALL only: Proceed as in point 3 but call

build release BALL              or
build debug BALL

5.) Building the tests

Proceed as in point 3 but call

build release tests             or
build debug tests

5.) Benchmarks

To build the benchmarks, proceed as in point 3 but call

build release benchmarks        or
build debug benchmarks

---------------------------------------------------------------------------------
-------------------------------	KNOWN ISSUES: -----------------------------------
---------------------------------------------------------------------------------

- If experienced some instabilities with this release, please try to use the debugging
  version of the library.

- XDRPersistenceManager fails


Please report any problems you have with this setup to

	ball-bugs@bioinf.uni-sb.de

Thank you!
