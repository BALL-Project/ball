
This document explains how to build BALL and BALLView on the windows platform by
using the Makefiles. For documentation on how to use the Visual Studio project
files, see README.txt.

-----------------------------------------------------------------------------------
-----------------------------INSTALLATION PER MAKEFILE: ---------------------------
-----------------------------------------------------------------------------------

Proceed like in README.txt for points 1.) and 2.)

Open a Microsoft Visual Studio .NET command prompt 
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

-----------------------------------------------------------------------------------
Building the libraries without QT and BALLView: 
If you dont have the QT library available or dont need the GUI and Python Support, 
you can create the core library BALL only: 

build release BALL              or
build debug BALL

-----------------------------------------------------------------------------------
-------------------------------ADDONS INSTALLATION: -------------------------------
-----------------------------------------------------------------------------------

1.) Building the tests

build release tests             or
build debug tests

2.) Building the Benchmarks

build release benchmarks        or
build debug benchmarks

3.) Building the Tutorials

build release tutorial 					or
build debug tutorial

4.) To build all libraries and every entry from the list above call

build release all 							or
build debug all 
