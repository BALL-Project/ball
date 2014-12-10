#ifndef STRCUTUREASSEMBLER_H
#define STRCUTUREASSEMBLER_H

#include "basic.h"
#include "fragmenter.h"
#include "matcher.h"
#include "assembler.h"

using namespace OpenBabel;
using namespace BALL;
using namespace std;

// String == key == UCK key
typedef boost::unordered_map <String, TemplateCoord*> CoordinateMap;
typedef boost::unordered_map <String, float >         BondLengthMap;
typedef boost::unordered_map <String, Fragment* >     ConnectionMap;

class StructureAssembler
{
public:
	StructureAssembler()
	{
		
	}

	// TODO: delete the libs!
	~StructureAssembler()
	{
		// clean db
		Log << "Removing loaded libs from memory...";
		CoordinateMap::iterator tmp_it;
		for(tmp_it = fragment_lib.begin(); tmp_it!=fragment_lib.end(); tmp_it++)
			delete tmp_it->second; // TODO: this line does not delete the contained molecules!!!!
		
		// bonds are of primitive type and on the stack
		
		ConnectionMap::iterator mit;
		for(mit = connect_lib.begin(); mit != connect_lib.end(); mit++)
			delete mit->second;
	}

	/**
	 * Takes a read molecule (with a normal atomlist only) and splits the former
	 * molecule into several fragments (rigid or linker) that are then back 
	 * inserted into the parent molecule.
	 * Coordinates are generated to yield the complete structure for the molecule
	 * 
	 * @brief assembleStructure
	 * @param mol
	 */
	void assembleStructure(Molecule* mol)
	{
		
	}
	
	/**
	 * Same as the variant for BALL::Molecule but also assigns the internal 
	 * conncetions between the internal fragments of the contained molecule.
	 * 
	 * @brief assembleStructure
	 * @param mol
	 */
	void assembleStructure(GroupFragment* gmol)
	{
		assembleStructure(gmol->molecule);
	}

	/**
	 * @brief setLibsFromConfig reads a config file that indicates locations of
	 *				all necessary libary files.
	 * @param path to config file
	 */
	void setLibsFromConfig(const String& path)
	{
		// get the pathes from config file
		libraryPathesFromConfig(path);
		
		// read lib files:
//		readOldFragmentLib();
		readFragmentLib();
		readBondLib();
		readConnectionLib();
	}

private:
	String fragment_lib_path;
	String bondlenth_lib_path;
	String connection_lib_path;
	
	CoordinateMap fragment_lib;
	BondLengthMap bond_lib;
	ConnectionMap connect_lib;
	
	
	/*
	 * Reads the standard bond lengths from a lib file
	 */
	void readBondLib()
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
	
	/*
	 * Read the connection site from a library file
	 */
	void readConnectionLib()
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
	
	/*
	 * Reads the template coordinates for all fragments
	 */
	void readFragmentLib()
	{
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
	
	
	
	/*
	 * !Brocken, TODO: convert from 'BALL::Molecule' to 'TemplateCoord'
	 * !Deprecated: former FragmentLib reader for fragmentLibs that are in SDF
	 * Format
	 */
	void readOldFragmentLib(boost::unordered_map <String, Molecule*>& fragmentLib)
	{
		fragmentLib.clear();
		SDFile libFile(fragment_lib_path, ios::in);
		
		Molecule* tmp_mol;
		tmp_mol = libFile.read();
		
		// read in fragmentLib and create hash-map from that:
		while(tmp_mol)
		{
			String key = tmp_mol->getProperty("key").getString();
			fragmentLib[key] = tmp_mol;
			
			tmp_mol = libFile.read();
		}
		libFile.close();
	}
	
	
	/*
	 * Get all needed library information from a config file
	 * that lists the paths to the respective lib files
	 */
	void libraryPathesFromConfig(const String& config_path)
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
	
	
};
#endif // STRCUTUREASSEMBLER_H
