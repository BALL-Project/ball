// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: main.C,v 1.2 2004/07/15 15:21:16 amoll Exp $
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
	// Create the mainframe.
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
	String working_dir;
	bool error = false;
	System* system;

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
	
		Directory d(argument);
		if (d.isValid())
		{
			mainframe.setWorkingDir(argument);
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
	
	DCDFile dcdfile(dcd_file_name);

	SnapShotManager sm(system, 0, &dcdfile, false);
	POVRenderer pov;
//  	pov.init(*Scene::getInstance(0)->getStage(), 1000, 1000);

 	Scene::getInstance(0)->exportScene(pov);

	sm.applyFirstSnapShot();
	while(sm.applyNextSnapShot())
	{
 		Scene::getInstance(0)->exportScene(pov);
	}

// 	mainframe.show();
//   return application.exec();
	return 0;
}

