You will need Microsoft Visual Studio .NET (MSVC 7) in order to compile BALL.

BALL requires some standard Unix tools and libraries that are not usually 
found on Windows systems. We provide an archive file with all needed files 
for this tools:

http://voyager.bioinf.uni-sb.de/OK/BALL/Downloads/Contrib/Contrib-1.1pre3.zip

This file contains:

  - FLEX: a fast lexical analyser

  - BISON: a parser generator

  - regex: a regular expression library

  - RPC: the remote procedure call library (for XDR)
  
The Contib folder in the archive must be copied to BALL\Windows\ directory. 

------------------------------------------------------------------------
Updated versions of these packages are available from GNUWIN32 project at
http://gnuwin32.sourceforge.net/packages/flex.htm and 
http://gnuwin32.sourceforge.net/packages/bison.htm and RWTH Aaachen (ONCRPC) at
http://www.plt.rwth-aachen.de/ks/english/oncrpc.html.

WARNING: bison 1.875 does not work for the BALL installation!
------------------------------------------------------------------------

Now you can open the BALL project BALL/Windows/BALL.sln
and (hopefully) compile BALL. 

If you want to compile the visualization component of BALL as well,
you will need a QT license (QT 3.2.0 or higher, commercial or academic license).
If you install a version different than 3.2.3 you have to adjust the
linker settings to reflect the correct QT library name.
To do so, open the properties dialog in MSVC for the entry of the libVIEW 
library. In the field for additional dependencies, adjust the name of the
QT-library. (See also BALL/WINDOWS/setting_qt_version_in_visual_studio_de.png)

After you compiled the BALL- and libVIEW-libs, you can compile BALLView.
The program can be started with the batch-file startBALLView.bat under
BALL/Windows/APPLICATIONS/BALLVIEW/ 
This batch file ensures, that the PATH environment variable contains the
directories for the needed libs.

If experienced some instabilities with this release, please try to disable all
optimisations for the MSVC compiler.

Please report any problems you  have with this (still clumsy) setup to

   ball-bugs@bioinf.uni-sb.de

Thank you!
