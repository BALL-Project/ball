Unpack the contrib.zip package into this directory and
compile bison, flex and librpc.lib.
------------------------------------------------------------------------
You will need Microsoft Visual Studio .NET (MSVC 7) in order to compile
BALL.

You need to build the following additional packages:
  - flex -- required for: lexer
     You can use the MSVC project folder in    
       flex-2.5.4/MISC/MSVC/msvc.sln
     to build it. make sure you build the "Release" configuration
     rather than the "Debug" one.

  - bison -- parser
     The best way to get it is the installation of CygWin
     (www.cygwin.com). Cygwin also provides the executable
		 (make sure to select bison for installation!) in 
     c:\cygwin\bin\bison.exe (if you do not install everything to 
     c:\cygwin, you will have to change the path in the settings
     of the user-defined build steps of all the .y files).
		 You will have to copy the executable (bison.exe) as well as the 
     three required DLLs (cygwin1.dll, cygintl-2.dll, cygiconv-2.dll) 
		 from /usr/bin to BALL\Windows\Contrib\bin.

  - regex -- regular expressions
     No installation is required for the regular expression package.
     If the .c- and .h-file are in Contrib/regex-0.12, the BALL
     project folder will automatically find and compile it.

  - ONC RPC -- portable data format (XDR)
     This package is prepared for compilation with MSVC.
     Under Windows, open a command shell (cmd), set the environment
     variables (run vcvars32.bat from your Microsoft Visual Studio .NET
     directory), and type "make" to build the RPC library.

Now you can open the BALL project folder in BALL/Windows
and (hopefully) compile BALL. 

Please choose the configuration "Python Multihreaded" in the Configuration
Manager, as this is the only configuration, that is currently working...

If you want to compile the visualization component of BALL as well,
you will need a QT license (QT 3.2.0 or higher, commercial or academic license).
If you install a version different than 3.2.3 you might have to adjust the
linker settings to reflect the correct library names.


Please report any problems you  have with this (still clumsy) setup to

   ball-bugs@bioinf.uni-sb.de

Thank you!
