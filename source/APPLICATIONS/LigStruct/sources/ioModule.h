// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef IOMODULE_H
#define IOMODULE_H

#include "base.h"

#include <BALL/DATATYPE/string.h>

//using namespace OpenBabel;
using namespace BALL;
//using namespace std;

/**
 * @brief The IOModule is a class to read, store and mange any library
 * 
 */
class IOModule
{
public:
	
	IOModule();
	
	~IOModule();
	
	/**
	 * @brief readAll reads all libs for which a path was set
	 */
	void readAll();
	
	/*
	 * Get all needed library information from a config file
	 * that lists the paths to the respective lib files
	 */
	void libraryPathesFromConfig(const String& config_path);
	
	
	/*
	 * Reads the standard bond lengths from a lib file
	 */
	void readBondLib();
	
	/*
	 * Read the connection site from a library file
	 */
	void readConnectionLib();
	
	/*
	 * Reads the template coordinates for all fragments
	 */
	void readFragmentLib();
	
	/*
	 * fragment_lib reader for fragmentLibs that are in SDF Format, converts
	 * the data to unordered_map <String, TemplateCoord*> for efficient internal
	 * representation
	 * 
	 * TODO: this is possibly very helpful for testing of input fragment libraries
	 * possibly remove this method
	 */
	void readSDFFragmentLib();
	CoordinateMap fragment_lib;
	BondLengthMap bond_lib;
	ConnectionMap connect_lib;
	
private:
	String fragment_lib_path;
	String bondlenth_lib_path;
	String connection_lib_path;

	
	

};

#endif // IOMODULE_H
