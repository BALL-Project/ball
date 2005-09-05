------------------------------------------------------------------------------------
------------------------------------ CAVEATS: --------------------------------------
------------------------------------------------------------------------------------

1.) You need a version of Microsoft Visual Studio .NET.

2.) BALL can only be installed on Windows if Python is installed!
		Currently we advise you to use the release 2.3.5.

3.) Python support can only be used with an installed QT lib!  
		(You need QT version: 3.2.X or 3.3.X.)

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
	- "BALL_DATA_PATH" to the "data" subdirectory in the BALL installation 
										 path (e.g. C:\BALL\data)
	- "QTDIR" (this is in general done automatical by the QT installation)

3.) Install the sip module:

Open a Microsoft Visual Studio NET Command prompt (see Windows Start Menu) and 
change the directory to "BALL\Windows\Contrib\sip" . Next call 

python configure.py
nmake
nmake install

4.) Now you can open the BALL MSVC project (BALL/Windows/BALL.sln).

5.) Set the correct library versions for the linker:

If you install a QT version other than 3.3.4 you have to adjust the
linker settings to reflect the correct QT library name.
To do so, open the properties dialog in MSVC for the entry of the libVIEW 
library. In the field for additional dependencies, adjust the name of the
QT-library. (See also BALL/WINDOWS/setting_qt_version_in_visual_studio_de.png)
Repeat this for the BALLView and Python Module project entries.
(The same applies to the Python library if you dont use Python 2.3.X.)

6.) Compile the BALL library.

7.) Build the "VIEW UIC MOC" project.

8.) Build the libVIEW project.

9.) Compile BALLView.

The program can be started with the batch-file startBALLView.bat under
BALL/Windows/APPLICATIONS/BALLVIEW/ 
This batch file ensures, that the PATH environment variable contains the
directories for the needed libs.

---------------------------------------------------------------------------------
-------------------------------	KNOWN ISSUES: -----------------------------------
---------------------------------------------------------------------------------

- If experienced some instabilities with this release, please try to disable all
	optimisations for the MSVC compiler.

- XDRPersistenceManager fails


Please report any problems you have with this (still clumsy) setup to

   ball-bugs@bioinf.uni-sb.de

Thank you!

