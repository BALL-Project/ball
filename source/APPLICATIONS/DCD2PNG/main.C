// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

// order of includes is important: first qapplication, than BALL includes
#include <QtWidgets/QApplication>

#include <BALL/SYSTEM/path.h>
#include <BALL/SYSTEM/directory.h>
#include "mainframe.h"

#include <BALL/FORMAT/DCDFile.h>
#include <BALL/MOLMEC/COMMON/snapShot.h>
#include <BALL/MOLMEC/COMMON/snapShotManager.h>
#include <BALL/VIEW/RENDERING/POVRenderer.h>

#include <iostream>
#include <sys/wait.h>

using namespace BALL;
using namespace BALL::VIEW;

void showUsage()
{
 	Log.insert(std::cerr);
	Log.error() << (Sting)qApp->translate("BALL::VIEW::DCD2PNG", "DCD2PNG DCDFILE.dcd  file.[bvp|pdb|hin]  [DCD2PNG.ini]") << std::endl;
	Log.error() << (Sting)qApp->translate("BALL::VIEW::DCD2PNG", "Read a BALLView project or a molecular file format and create PNG images from it by using POVRay.") << std::endl;
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
	String home_dir;
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
		home_dir = BALL::Directory::getUserHomeDir();

		if (home_dir == "")
		{
			Log.error() << (Sting)qApp->translate("BALL::VIEW::DCD2PNG", 
					"You dont have write access to the current working directory")<< "\n" <<
					(Sting)qApp->translate("BALL::VIEW::DCD2PNG", "and I can not find your home directory. This can cause") << "\n" <<
					(Sting)qApp->translate("BALL::VIEW::DCD2PNG", "unexpected behaviour. Please start DCD2PNG from your homedir with") << "\n" << 
					(Sting)qApp->translate("BALL::VIEW::DCD2PNG", "absolute path.") << "\n";
			return -1;
		}
	}

	// =============== initialize Mainframe ============================================
	BALL::Directory dir;
	QApplication application(argc, argv);
	BALL::Mainframe* mainframe = new Mainframe();
	mainframe->setIdentifier("MAIN");
	mainframe->registerThis();

 	mainframe->hide();

	// if we need to use the users homedir as working dir, do so
	if (home_dir != "")
	{
		mainframe->setWorkingDir(home_dir);
	}

	// =============== parsing command line arguments ==================================
	String dcd_file_name;
	String molecular_file_name;
	String working_dir = ".";
	String project_file = "";
	String inifile_name = dir.getPath() + BALL::FileSystem::PATH_SEPARATOR + "DCD2PNG.ini";
	bool error = false;
	System* system = 0;
	Position nr = 100000000;

	for (BALL::Index i = 1; i < argc && !error; ++i)
	{
		BALL::String argument(argv[i]);
		if (argument.hasSuffix(".bvp"))
		{
		 	mainframe->loadBALLViewProjectFile(argument);
			project_file = argument;
			continue;
		}

		if (argument.hasSuffix(".ini"))
		{
			inifile_name = argument;
			continue;
		}

		if (argument.hasSuffix(".pdb") ||
		    argument.hasSuffix(".hin"))
		{
			MolecularFileDialog* mfd = MolecularFileDialog::getInstance(0);
			if (mfd == 0) return 0;
			system = mfd->openMolecularFile(argument);
			if (system == 0)
			{
				std::cerr << (Sting)qApp->translate("BALL::VIEW::DCD2PNG", "Could not open file") << ": " << argument << std::endl;
				error = true;
				break;
			}

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
			
		error = true;
	}

	if ((molecular_file_name == "" && project_file == "") ||
			dcd_file_name == "" ||
			error)
	{
		showUsage();
		application.quit();
		return 0;
	}

	if (system == 0)
	{
		if (mainframe->getCompositeManager().getNumberOfComposites() == 0)
		{
			std::cerr << "No molecular entities stored in project file and no molecular file given..." << std::endl;
			showUsage();
			return -1;
		}

		system = (System*) *mainframe->getCompositeManager().begin();
	}

	Size width = 720;
	Size height = 480;
	String povray_options = "-V +FN +QR +A0.3 -UV -D +I- ";
	String povray_exec;
	INIFile inifile(inifile_name);
	if (!inifile.read())
	{
		std::cerr << "Could not find DCD2PNG.ini file, please specify the absolute path." << std::endl;
		return -1;
	}

	if (inifile.hasEntry("DCD2PNG", "WORKINGDIR"))
	{
		String argument = inifile.getValue("DCD2PNG", "WORKINGDIR");
		Directory d(argument);
		if (d.isValid())
		{
			working_dir = argument;
		}
		else
		{
			d.set(argument);
			if (d.isValid())
			{
				working_dir = argument;
			}
			else
			{
				std::cerr << "Could not read directory for images from DCD2PNG.ini!" << std::endl;
				return -1;
			}
		}
	}

	try
	{
		if (inifile.hasEntry("DCD2PNG", "WIDTH"))
		{
			String argument = inifile.getValue("DCD2PNG", "WIDTH");
			width = argument.toUnsignedInt();
		}
		if (inifile.hasEntry("DCD2PNG", "HEIGHT"))
		{
			String argument = inifile.getValue("DCD2PNG", "HEIGHT");
			height = argument.toUnsignedInt();
		}
	}
	catch(...)
	{
		std::cerr << "Could not read values for width and height from INIFile!" << std::endl;
	}

	if (inifile.hasEntry("DCD2PNG", "POVRAY_OPTIONS"))
	{
//   		povray_options = inifile.getValue("DCD2PNG", "POVRAY_OPTIONS") + " +I- ";
		povray_options = inifile.getValue("DCD2PNG", "POVRAY_OPTIONS") + " +Iasd.pov ";
	}

	if (inifile.hasEntry("DCD2PNG", "POVRAY"))
	{
		povray_exec = inifile.getValue("DCD2PNG", "POVRAY");
	}

	if (!File::isAccessible(povray_exec))
	{
		std::cout << "Could not find POVRay at the given location: " << povray_exec << std::endl;
		std::cout << "Please modify the settings in DCD2PNG.ini" << std::endl;
		return -1;
	}

	Scene::getInstance(0)->resize(width, height);
	povray_options +=	"+W" + String(width) + " +H" + String(height) + " +O" + working_dir + FileSystem::PATH_SEPARATOR;
	POVRenderer pov;
	pov.setHumanReadable(false);

	Position nr2 = 0;

	DCDFile dcdfile(dcd_file_name);
	SnapShotManager sm(system, 0, &dcdfile);
	sm.applyFirstSnapShot();

	vector<String> strings;
	Size s = povray_exec.split(strings, String(FileSystem::PATH_SEPARATOR).c_str());
	if (s < 2)
	{
		std::cerr << "Error: Specify in DCD2PNG.ini the absolute path to povray." << std::endl;
		std::cerr << "e.g. POVRAY=/usr/bin/povray" << std::endl;
		return -1 ;
	}
	String pov_exec2 = strings[s - 1];

	error = false;
	while(sm.applyNextSnapShot() && ! error)
	{
		String pov_arg = povray_options + String(nr) + ".png" ;

		std::stringstream pov_renderer_output;
//   		pov.setOstream(pov_renderer_output);
 		pov.setFileName("asd.pov");
 		Scene::getInstance(0)->exportScene(pov);

		int writepipe[2];
		pid_t childpid;
	
		if (pipe(writepipe) < 0 ) 
		{
			std::cerr << "Could not pipe!" << std::endl;
			return -1;
		}
	
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
			int result = waitpid( -1, &status, 0 );

			if (result <= 1) 
			{
				std::cerr << "No child process!" << std::endl;
			}
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
 					std::cerr << "Could not dup2!" << std::endl;
 					return -1;
				} 

 				close(writepipe[0]);
			}

			std::cerr << "Calling " << povray_exec << " " << pov_arg << std::endl;
 	   	int result = execl (povray_exec.c_str(), pov_exec2.c_str(), pov_arg.c_str(), NULL);
			if (result == -1)
			{
				std::cout << "Could not exec POVRay! " << std::endl;
				error = true;
			}
			
			/*
			// for debugging:
 			std::cout << std::endl ;
  		execl ("/bin/cat", "cat", 0);
			error = true;
			*/
		}

		nr++;
		nr2++;
	}

	std::cout << "Written " + String(nr2) + " images." << std::endl;
	
	/*
	mainframe->show();
 	return application.exec();
	*/

	return 0;
}

