You will need Microsoft Visual Studio .NET (MSVC 7) in order to compile BALL.

BALL requires some standard Unix tools and libraries that are not usually 
found on Windows systems. We provide an archive file with the installer 
files for this tools:
http://voyager.bioinf.uni-sb.de/OK/BALL/Downloads/Contrib/Contrib-1.0.zip

This file contains:

  - FLEX: a fast lexical analyser

  - BISON: a parser generator

  - regex: a regular expression library

  - RPC: the remote procedure call library (for XDR)
  
All needed tools must be copied/installed in the appropriate subfolders 
in the BALL\Windows\Contrib directory. (These subfolders already exist.)

The regex package is provided as source code and does not require further 
installation, please copy it in the folder regex-0.12. (Be sure, not to create
an other subdir regex-0.12 in this directory!)

For the other packages, please start the individual installers and make sure
they install their contents to respective subfolder in the Contrib directory 
(the directory this file is in). The installers are:
  - bison-1.35-4-bin.exe (bison)
  - flex-2.5.4a-1-bin    (flex)
  - oncrpc.exe  				 (oncrpc)

------------------------------------------------------------------------
Updated versions of these packages are available from GNUWIN32 project at
http://gnuwin32.sourceforge.net/packages/flex.htm and 
http://gnuwin32.sourceforge.net/packages/bison.htm and RWTH Aaachen (ONCRPC) at
http://www.plt.rwth-aachen.de/ks/english/oncrpc.html.

WARNING: bison 1.875 does not work for the BALL installation!
------------------------------------------------------------------------

Now you can open the BALL project BALL/Windows/BALL.sln
and (hopefully) compile BALL. 

Please dont choose an other configuration than "Python Multihreaded" in 
the Configuration Manager, as this is currently the only working configuration.

If you want to compile the visualization component of BALL as well,
you will need a QT license (QT 3.2.0 or higher, commercial or academic license).
If you install a version different than 3.2.3 you have to adjust the
linker settings to reflect the correct QT library name.
To do so, open the properties dialog in MSVC for the entry of the libVIEW 
library. In the field for additional dependencies, adjust the name of the
QT-library.

After you compiled the BALL- and libVIEW-libs, you can compile BALLView.
The program can be started with the batch-file startBALLView.bat under
BALL/Windows/APPLICATIONS/BALLVIEW/ 
This batch file ensures, that the PATH environment variable contains the
directories for the needed libs.

Please report any problems you  have with this (still clumsy) setup to

   ball-bugs@bioinf.uni-sb.de

Thank you!
