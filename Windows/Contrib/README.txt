Unpack the contrib.zip package into this directory and
compile bison, flex and librpc.lib.
------------------------------------------------------------------------
You will need Microsoft Visual Studio .NET (MSVC 7) in order to compile
BALL.

You need to build the following additional packages:
  - flex -- required for: lexer
     You can use the MSVC project folder in    
       flex-2.5.4/MISC/MSVC/msvc.sln
     to build it.

  - bison -- parser
     Best way to build it is the installation of CygWin
     (www.cygwin.com). Cygwin also provides a bison executable,
     but the simplest way is to open a CygWin shell, cd to 
     Contrib/bison-1.75 and execute "./configure" and "make".
     This will create the bison executable in bison-1.75/src/bison.exe

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
and (hopefully) compile BALL. Please report any problems you 
have with this (still somewhat clumsy) setup to

   ball-bugs@bioinf.uni-sb.de

Thank you!
