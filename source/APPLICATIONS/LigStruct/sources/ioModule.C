// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include "ioModule.h"

#include <BALL/FORMAT/lineBasedFile.h>
#include <BALL/FORMAT/SDFile.h>

#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/fragment.h>
// using namespace OpenBabel;
// using namespace BALL;
// using namespace std;


IOModule::IOModule()
{
	
}

IOModule::~IOModule()
{
	
}

void IOModule::readAll()
{
	// read lib files:
//	readSDFFragmentLib();
	if( fragment_lib_path != "")
		readFragmentLib();
	
	if( bondlenth_lib_path != "")
		readBondLib();
	
	if( connection_lib_path != "")
		readConnectionLib();
}

void IOModule::readBondLib()
{
	LineBasedFile bondFile(bondlenth_lib_path, ios::in);
	
	while( bondFile.readLine() )
	{
		String st_ar[2];
		bondFile.getLine().split(st_ar, 2);
		
		bond_lib[st_ar[0]] = st_ar[1].toFloat();
		
		// generate also the reversed label, if both differ
		if( (st_ar[0])[0] != (st_ar[0])[1] )
		{
			String altKey = (st_ar[0])[1];
			altKey += (st_ar[0])[0];
			bond_lib[altKey] = st_ar[1].toFloat();
		}
	}
}



void IOModule::readConnectionLib()
{
	connect_lib.clear();
	
	SDFile handle(connection_lib_path, ios::in); //open the lib file as sdf-file
	
	// read all lib molecules and save them with their key:
	Fragment* tmp_mol;
	tmp_mol = (Fragment*)handle.read(); // first entry
	
	int cnt = 0;
	while(tmp_mol)
	{
		BALL::String key = tmp_mol->getProperty("key").getString();
		connect_lib[key] = tmp_mol;
		
		tmp_mol = (Fragment*)handle.read();
		cnt++;
	}
	handle.close();
}




void IOModule::readFragmentLib()
{
	//DEBUG:
	cout<<"reading fragment lib from: "<<fragment_lib_path<<endl;
	fragment_lib.clear();
	LineBasedFile libFile(fragment_lib_path, ios::in);
	
	// read in fragmentLib and create hash-map from that:
	String key;
	while( libFile.readLine() )
	{
		TemplateCoord* tmp_frag=0;
		if ( libFile.getLine().hasPrefix("key ") )
		{
			// get key:
			key = libFile.getLine().after("key ");
			
			// get number of positions:
			libFile.readLine();
			Size size = libFile.getLine().toUnsignedInt();
			
			// get positions:
			tmp_frag = new TemplateCoord(size);
			Size i;
			for(i = 0; i < size; i++)
			{
				libFile.readLine();
				String coords[3];
				libFile.getLine().split(coords, 3);
				Vector3& vec = (*tmp_frag)[i];
				vec.set(coords[0].toFloat(), coords[1].toFloat(), coords[2].toFloat());
			}
			
			// append to hash map
			fragment_lib[key] = tmp_frag;
		}
		else
		{
			Log << "WARNING: missed in the template coordinate lib file a line!!!"<<endl;
		}
	}
	libFile.close();
}




void IOModule::readSDFFragmentLib()
{
	fragment_lib.clear();
	SDFile libFile(fragment_lib_path, ios::in);
	
	Molecule* tmp_mol;
	TemplateCoord* tmp_frag=0;
	tmp_mol = libFile.read();
	
	// read in fragmentLib and create hash-map from that:
	while(tmp_mol)
	{
		String key = tmp_mol->getProperty("key").getString(); // get key
		
		Size size = tmp_mol->countAtoms();
		tmp_frag = new TemplateCoord(size);
		fragment_lib[key] = tmp_frag; // add template to the lib
		
		Index i = 0;
		for (AtomIterator it = tmp_mol->beginAtom(); +it; ++i, ++it)
		{
			tmp_frag->get(i).set( it->getPosition() );
		}
		
		delete tmp_mol; // free memory
		
		tmp_mol = libFile.read(); //read for next iteration
	}
	libFile.close();
}


void IOModule::libraryPathesFromConfig(const String& config_path)
{
	LineBasedFile configFile(config_path, ios::in);
	
	String tmp="";
	while( configFile.readLine() )
	{
		tmp = configFile.getLine();
		if(tmp.hasPrefix("#"))
			continue;
		
		if(tmp.hasPrefix("fragments=")){
			tmp = tmp.after("fragments=");
			fragment_lib_path = tmp.trim();
		}
		else if(tmp.hasPrefix("bondlenths=")){
			tmp = tmp.after("bondlenths=");
			bondlenth_lib_path = tmp.trim();
		}
		else if(tmp.hasPrefix("connections=")){
			tmp = tmp.after("connections=");
			connection_lib_path = tmp.trim();
		}
		else
			continue;
	}
}
