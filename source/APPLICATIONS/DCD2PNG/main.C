// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: main.C,v 1.6 2004/07/16 13:55:32 amoll Exp $
//

// order of includes is important: first qapplication, than BALL includes
#include <qapplication.h>

#include <BALL/SYSTEM/path.h>
#include <BALL/SYSTEM/directory.h>
#include "mainframe.h"

#include <BALL/FORMAT/DCDFile.h>
#include <BALL/MOLMEC/COMMON/snapShot.h>
#include <BALL/VIEW/RENDERING/POVRenderer.h>

#include <iostream>
#include <sys/wait.h>

using namespace BALL;
using namespace BALL::VIEW;

void showUsage()
{
 	Log.insert(std::cerr);
	Log.error() << "DCD2PNG <PDBFILE.pdb | HINFILE.hin> <DCDFILE.dcd> [<BALLViewProject.bvp>] [<DIRECTORY>]" << std::endl;
	Log.error() << "Read a molecular file, and create PNG images from them in the given directory." << std::endl;
 	Log.remove(std::cerr);
}


int main(int argc, char **argv)
{
	if (argc < 3)
	{
		showUsage();
		return 1;
	}

	// =============== testing if we can write in current directoy =====================
	bool dir_error = false;
	char* home_dir = 0;
	try
	{
		BALL::String temp_file_name;
		BALL::File::createTemporaryFilename(temp_file_name);
		BALL::File out(temp_file_name, std::ios::out);
		out << "test" << std::endl;
		out.remove();
	}
	catch(...)
	{
		// oh, we have a problem, look for the users home dir
		dir_error = true;

		// default for UNIX/LINUX
		home_dir = getenv("HOME");
		if (home_dir == 0) 
		{
			// windows
			home_dir = getenv("HOMEPATH");
		}

		// changedir to the homedir
		if (home_dir != 0)
		{
			BALL::Directory dir(home_dir);
			dir_error = !dir.setCurrent();
		}

		if (dir_error)
		{
			Log.error() << 
					"You dont have write access to the current working directory\n"<<
					"and I can not find your home directory. This can cause\n" <<
					"unexpected behaviour. Please start DCD2PNG from your homedir with\n" << 
					"absolute path.\n";
		}
	}

	// =============== initialize Mainframe ============================================
	QApplication application(argc, argv);
	BALL::Mainframe mainframe;
//  	application.setMainWidget(&mainframe);
	mainframe.setIdentifier("MAIN");
	mainframe.registerThis();

 	mainframe.hide();

	// if we need to use the users homedir as working dir, do so
	if (home_dir != 0 && !dir_error)
	{
		mainframe.setWorkingDir(home_dir);
	}

	// =============== parsing command line arguments ==================================
	String dcd_file_name;
	String molecular_file_name;
	String working_dir = ".";
	bool error = false;
	System* system;
	Position nr = 100000000;

	DisplayProperties::getInstance(0)->enableCreationForNewMolecules(false);

	for (BALL::Index i = 1; i < argc && !error; ++i)
	{
		BALL::String argument(argv[i]);
		if (argument.hasSuffix(".bvp"))
		{
			DisplayProperties::getInstance(0)->enableCreationForNewMolecules(true);
			mainframe.loadBALLViewProjectFile(argument);
			continue;
		}

		if (argument.hasSuffix(".pdb") ||
		    argument.hasSuffix(".hin"))
		{
			MolecularFileDialog* mfd = MolecularFileDialog::getInstance(0);
			if (mfd == 0) return 0;
			system = mfd->openFile(argument);
			molecular_file_name = argument;
			continue;
		}
		
		if (argument.hasSuffix(".dcd"))
		{
			dcd_file_name = argument;
			continue;
		}

		if (argument.hasPrefix("-s"))
		{
			argument.after("-s");
			try
			{
				nr = argument.toUnsignedInt();
			}
			catch(...)
			{
			}
			continue;
		}
			
	
		Directory d(argument);
		if (d.isValid())
		{
			mainframe.setWorkingDir(argument);
			working_dir = argument;
			continue;
		}

		error = true;
	}

	if (molecular_file_name == "" ||
			dcd_file_name == "" ||
			error)
	{
		showUsage();
		application.quit();
		return 0;
	}

	if (dcd_file_name == "")
	{
		DisplayProperties::getInstance(0)->enableCreationForNewMolecules(true);
 		DisplayProperties::getInstance(0)->applyButtonClicked();
	}

	Size width = 1000;
	Size height = 750;
	
	DCDFile dcdfile(dcd_file_name);
	Scene::getInstance(0)->resize(width, height);

	
	int writepipe[2];
	pid_t childpid;
 	if (pipe(writepipe) < 0 ) 
	{
		std::cerr << "Could not pipe!" << std::endl;
		return -1;
	}
	

	String povray_options;
	povray_options = "-V -D +I- +W" + String(width) + " +H" + String(height) + " +O" + working_dir + "/";

	SnapShotManager sm(system, 0, &dcdfile, false);
	POVRenderer pov;
	std::stringstream pov_renderer_output;
	pov.setOstream(pov_renderer_output);
	pov.setHumanReadable(false);
	Position nr2 = 0;
	sm.applyFirstSnapShot();

	
	while(sm.applyNextSnapShot())
	{
		String pov_arg = povray_options + String(nr) + ".png" ;
 		Scene::getInstance(0)->exportScene(pov);

		// abort, if we can not fork!
		if ( (childpid = fork()) < 0) 
		{
			std::cerr << "Could not fork!" << std::endl;
			return -1;
		}
		else if (childpid != 0)
		{
			// we are in the parent process and wait for the child to finish
			
			// Parent closes it's read end of the pipe
 			close (writepipe[0]); 
			
			sleep(1);

			while (pov_renderer_output.good())
			{
				char buffer[1000];
				pov_renderer_output.getline(buffer, 1000);

				Size n = strlen(buffer);
				if(write(writepipe[1], buffer, n) != (int) n) 
				{ 
					std::cerr << "Could not write to pipe!" << std::endl;
					return -1;
				}

				write(writepipe[1], "\n", 1);	
			}

			// Parent closes it's write end of the pipe, this sends EOF to reader
			close (writepipe[1]); 

			int status;
			waitpid( -1, &status, 0 );
		}
		else
		{
			// we are in the child process and start povray
			
			// Child closes it's write end of the pipe
		 	close (writepipe[1]); 

			// handle stdin already closed
			if(writepipe[0] != STDIN_FILENO) 
			{
				if(dup2(writepipe[0], STDIN_FILENO) < 0) 
				{ 
					std::cout << "Could not dup2!" << std::endl;
					return -1;
				} 

				close(writepipe[0]);
			}

	  	execl ("/home/student/amoll/bin/povray", "povray", pov_arg.c_str(), 0);
		}

		nr++;
		nr2++;
	}

	std::cout << "Written " + String(nr2) + " images." << std::endl;

// 	mainframe.show();
//   return application.exec();
	return 0;
}

