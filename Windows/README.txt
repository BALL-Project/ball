                    COMPILING BALL ON WINDOWS

Compiling BALL on Windows is still a little more complicated than on
other operating systems. This will change (hopefully) in the near
future. In the meantime, if you want to compile your own BALL on
Windows, you will need a recent version of Visual Studio (currently,
we use Visual Studio 2008). If you got access to that, please

 * Install a Windows version of Qt that is compatible with MSVC.
   Right now, this means either downloading an unofficial binary Qt
   build from google code or downloading the source version and
   compiling your own Qt (be prepared to wait some time for that
   to finish).
 * Set an environment variable QTDIR to point to your newly
   created Qt installation.
 * Download the current contrib package for Windows from the BALL
   homepage and extract it to reside in the BALL\Windows\Contrib
   folder.
 * Open the Visual Studio solution file found in 
   BALL\BALL-VisualStudio\BALL-VisualStudio.sln
   and compile the whole project.
 * The BALLmodule and VIEWmodule projects in the solution build the
   Python interface and can be skipped if Python support is not
   required.
 * To work as expected, BALL will require an environment variable
   BALL_DATA_PATH or a registry entry in HKLM\Software\BALL with
   name BALL_DATA_PATH pointing to the correct data directory.

If you have any problems at any point, please ask us at

ball@bioinf.uni-sb.de

Good luck!
