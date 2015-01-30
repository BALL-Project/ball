// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include "ioModule.h"
#include "base.h"

#include <BALL/FORMAT/lineBasedFile.h>
#include <BALL/FORMAT/SDFile.h>

#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/fragment.h>

#include <BALL/STRUCTURE/molecularSimilarity.h>

/// L i b r a r y R e a d e r
/// ############################################################################
TemplateLibraryManager::TemplateLibraryManager()
{
	
}

TemplateLibraryManager::~TemplateLibraryManager()
{
	
}

CoordinateMap &TemplateLibraryManager::getFragmentLib()
{
	return _fragment_lib;
}

BondLengthMap &TemplateLibraryManager::getBondLengthlib()
{
	return _bond_lib;
}

ConSiteMap &TemplateLibraryManager::getConnectionsLib()
{
	return _connect_lib;
}

void TemplateLibraryManager::readAll()
{
	// read lib files:
//	readSDFFragmentLib();
	if( _fragment_lib_path != "")
		readFragmentLib();
	
	if( _bondlenth_lib_path != "")
		readBondLib();
	
	if( _connection_lib_path != "")
		readConnectionLib();
}

void TemplateLibraryManager::readBondLib()
{
	LineBasedFile bondFile(_bondlenth_lib_path, ios::in);
	
	while( bondFile.readLine() )
	{
		String st_ar[2];
		bondFile.getLine().split(st_ar, 2);
		
		_bond_lib[st_ar[0]] = st_ar[1].toFloat();
		
		// generate also the reversed label, if both differ
		if( (st_ar[0])[0] != (st_ar[0])[1] )
		{
			String altKey = (st_ar[0])[1];
			altKey += (st_ar[0])[0];
			_bond_lib[altKey] = st_ar[1].toFloat();
		}
	}
}


void TemplateLibraryManager::readConnectionLib()
{
	_connect_lib.clear();
	
	SDFile handle(_connection_lib_path, ios::in); //open the lib file as sdf-file
	
	// read all lib molecules and save them with their key:
	Fragment* tmp_mol;
	tmp_mol = (Fragment*)handle.read(); // first entry
	
	int cnt = 0;
	while(tmp_mol)
	{
		BALL::String key = tmp_mol->getProperty("key").getString();
		_connect_lib[key] = tmp_mol;
		
		tmp_mol = (Fragment*)handle.read();
		cnt++;
	}
	handle.close();
}


void TemplateLibraryManager::readFragmentLib()
{
	//DEBUG:
	cout<<"reading fragment lib from: "<<_fragment_lib_path<<endl;
	_fragment_lib.clear();
	LineBasedFile libFile(_fragment_lib_path, ios::in);
	
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
			_fragment_lib[key] = tmp_frag;
		}
		else
		{
			Log << "WARNING: missed in the template coordinate lib file a line!!!"<<endl;
		}
	}
	libFile.close();
}


void TemplateLibraryManager::readSDFFragmentLib()
{
	_fragment_lib.clear();
	SDFile libFile(_fragment_lib_path, ios::in);
	
	Molecule* tmp_mol;
	TemplateCoord* tmp_frag=0;
	tmp_mol = libFile.read();
	
	// read in fragmentLib and create hash-map from that:
	while(tmp_mol)
	{
		String key = tmp_mol->getProperty("key").getString(); // get key
		
		Size size = tmp_mol->countAtoms();
		tmp_frag = new TemplateCoord(size);
		_fragment_lib[key] = tmp_frag; // add template to the lib
		
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


void TemplateLibraryManager::libraryPathesFromConfig(const String& config_path)
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
			_fragment_lib_path = tmp.trim();
		}
		else if(tmp.hasPrefix("bondlenths=")){
			tmp = tmp.after("bondlenths=");
			_bondlenth_lib_path = tmp.trim();
		}
		else if(tmp.hasPrefix("connections=")){
			tmp = tmp.after("connections=");
			_connection_lib_path = tmp.trim();
		}
		else
			continue;
	}
}


/// C o m b i L i b M a n a g e r
/// ############################################################################

CombiLibManager::CombiLibManager(){}

CombiLibManager::~CombiLibManager()
{
	_scaffold = "";
}

void CombiLibManager::readCombiLib(const String &file_name)
{
	LineBasedFile combilib_file(file_name, ios::in);
	int group_id = -1;
	SMILESVec* current_group = 0;
	
	while( combilib_file.readLine() )
	{
		/// 'comments' and 'empty lines': ignore
		if ( combilib_file.getLine().hasPrefix("#") || combilib_file.getLine().isEmpty() )
		{
			continue;
		}
		
		/// 'scaffold': set scaffold (the last defined scaffold will be used)
		else if ( combilib_file.getLine().hasPrefix("scaffold:"))
		{
			// get the scaffold SMILES
			String str = String(combilib_file.getLine().after("scaffold:")).trim();
			
			// if we have another scaffold, delete it and use the new one:
			if(_scaffold != "")
			{
				Log<<"INPUT-WARNING: overwriting a former scaffold, using now: "<<str
					 <<endl<<"Remember that only one scaffold per combi-lib configuration"
					 <<" file should be specified. If more scaffolds occur, the last one"
					 <<" is used"<<endl;
			}
			_scaffold = str;
		}
		
		/// 'group_': create a new group
		else if( combilib_file.getLine().hasPrefix("group_") )
		{
			// get group number:
			String str_num = String(combilib_file.getLine().after("group_")).trim().trimRight(":");
			group_id = str_num.toUnsignedInt();
			
			// create new groupfragmentlist for the group:
			SMILESVec tmp;
			current_group = &tmp;
			_lib[group_id] = tmp;
		}
		
		/// '[ some number * ]': append a SMILES to current group_id
		else if( combilib_file.getLine().hasPrefix("[") )
		{
			String str = combilib_file.getLine().trim() ;
			
			// make sure that the fragment really contains the needed connection:
			if ( str.hasSubstring( "[" + String(group_id) + "*]" )  )
			{
				current_group->push_back( str );
			}
			else
			{
				Log<<"INPUT-ERROR: the r-group SMILES "<< str 
					 <<" belongs to the r-group block "<< group_id
					 <<" but that group number was not found in the SMILES"<<endl;
				Log<<endl;
				Log<<"Please re-check your combi-lib configuration file."<<endl;
				exit(EXIT_FAILURE);
			}
		}
		/// 'undefined line': throw error
		else
		{
			// TODO: throw exception
			Log<<"INPUT-ERROR: could not understand the combi lib line: ";
			Log<< combilib_file.getLine()<<endl;
			exit(EXIT_FAILURE);
		}
	}
	combilib_file.close(); // close combiLib file
}

const vector<String>& CombiLibManager::operator[](int index)
{
	if( _lib.find(index) != _lib.end() )
		return _lib[index];
	else // TODO: throw exception
	{
		cout<<"ERROR: tried to access an invalid R-Group in a CombiLibManager instance"<<endl;
		exit(EXIT_FAILURE);
	}
}

const Size CombiLibManager::size()
{
	return _lib.size();
}


/// S m i l e s P a r s e r
/// ############################################################################

SmilesParser::SmilesParser(){
	_babel_conv.SetInFormat("smi");
}

SmilesParser::~SmilesParser(){}

Molecule *SmilesParser::fromSMILEStoMolecule(const String &smiles_string)
{
	_babel_conv.ReadString(&_babel_mol, smiles_string);
	
	return MolecularSimilarity::createMolecule(_babel_mol, true);
}

GroupFragment *SmilesParser::fromSMILEStoGroupfragment(const String &smiles_string)
{
	// get the obmol SMILES input:
	_babel_conv.ReadString(&_babel_mol, smiles_string);
	
	GroupFragment* frag = new GroupFragment();
	list< pair<int, OBAtom*> > con_lst;
	
	// find r-group connections:
	vector<OBAtom*> for_deletion;
	for(OBAtomIterator it = _babel_mol.BeginAtoms(); it != _babel_mol.EndAtoms(); it++)
	{
		// connection sites (r-groups) are marked with 'Du' in OB and the isotope 
		// label carries the identifier of the r-group:
		if(!strcmp( (*it)->GetType(), "Du") ) 
		{
			OBAtom* ptr = ( *(*it)->BeginBonds())->GetNbrAtom( *it );
			int r_id = (*it)->GetIsotope();
			
			for_deletion.push_back( *it );
			con_lst.push_back(  make_pair( r_id, ptr )  );
		}
	}
	
	// Delete unnecessary pseudo atoms (r-group definitions in SMILES)
	for(int i = 0; i < for_deletion.size(); i++)
		_babel_mol.DeleteAtom( for_deletion[i], true);
	
	// convert OBMol to BALL::Molecule
	frag->molecule = MolecularSimilarity::createMolecule(_babel_mol, true);
	
	// convert the Open babel connection list to the final connection list:
	list< pair<int, OBAtom*> >::iterator iti = con_lst.begin();
	for(; iti != con_lst.end(); iti++)
	{
		Atom* atm_ptr = frag->molecule->getAtom( (*iti).second->GetIdx() - 1 );
		unsigned int group_id = (*iti).first;
		
		frag->connections.push_back(  make_pair( group_id, atm_ptr )  );
	}
	
	return frag;
}

/// (static) L i g I O
/// ############################################################################
/// 
void LigIO::writeMolVec(vector<AtomContainer *> &input, SDFile& handle)
{
	for(int i = 0; i < input.size(); i++)
	{
		handle << *input[i];
	}
}

void LigIO::writeMolVec(vector<AtomContainer*> &input, LineBasedFile& handle)
{
	for(int i = 0; i < input.size(); i++)
	{
		writePositionLines(*input[i], handle);
	}
}

void LigIO::readOBMolecule(const String &path, OBMol &mol)
{
	// Read open-babel molecule:
	OBConversion conv;
	OBFormat *format = conv.FormatFromExt(path);
	
	if (!format || !conv.SetInFormat(format))
		cout << "Could not find input format for file " << path << endl;

	ifstream ifs(path); // open file
	
	if (!ifs)
		cout << "Could not open " << path << " for reading." << endl;
	
	conv.Read(&mol, &ifs); // actual 'read' command
	ifs.close();
}


void LigIO::writePositionLines(AtomContainer& mol, LineBasedFile& handle)
{
	handle <<"key "<< mol.getProperty("key").getString() <<endl;
	handle << String(mol.countAtoms()) << endl;
	
	AtomIterator ati = mol.beginAtom();
	for(; +ati; ati++)
	{
		handle << String(ati->getPosition().x) << " ";
		handle << String(ati->getPosition().y) << " ";
		handle << String(ati->getPosition().z) << endl;
	}
}
