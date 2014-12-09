// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#ifndef BASIC_H
#define BASIC_H

#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/FORMAT/SDFile.h>
#include <BALL/FORMAT/lineBasedFile.h>
#include <BALL/SYSTEM/file.h>

#include <BALL/KERNEL/forEach.h>
#include <BALL/DATATYPE/string.h>

#include <BALL/KERNEL/atomContainer.h>
#include <BALL/KERNEL/fragment.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/atomIterator.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/bondIterator.h>
#include <BALL/KERNEL/PTE.h>

#include <BALL/STRUCTURE/UCK.h>
#include <BALL/STRUCTURE/molecularSimilarity.h>
#include <BALL/STRUCTURE/structureMapper.h>
#include <BALL/STRUCTURE/geometricTransformations.h>
#include <BALL/MATHS/angle.h>
#include <BALL/MATHS/vector3.h>
#include <BALL/MATHS/matrix44.h>

#include <openbabel/obconversion.h>
#include <openbabel/mol.h>
#include <openbabel/canon.h>
#include <openbabel/graphsym.h>

#include <vector>

#include <boost/unordered_map.hpp>
#include <boost/pending/disjoint_sets.hpp>


using namespace OpenBabel;
using namespace BALL;
using namespace std;

/// ################# H E L P E R    T Y P E #################

/**
 * @brief The TemplateCoord class is a simple array wrapper for BALL::Vector3
 * 
 * To efficiently save coordinates without much overhead
 */
class TemplateCoord
{
	
public:
	TemplateCoord(Size n)
	{
		_size = n;
		positions = new Vector3[n];
	}
	
	~TemplateCoord()
	{
		delete[] positions;
	}
	
	Vector3& operator[](Size idx)
	{
		return *(positions + idx);
	}

	const Size& size()
	{
		return _size;
	}

private:
	Size _size;
	Vector3 *positions;
};


struct GroupFragment
{
	list< Bond* > rotor_lst;
	list< pair< unsigned int, Atom*> > connections;
	Molecule* molecule;
};

/// Typedefs:
typedef vector<GroupFragment*> GroupFragmentList;
typedef boost::unordered_map< int, GroupFragmentList* > CombiLib;


/// ################# H E L P E R    F U N C T I O N S #################

// TODO: maybe change this
/// Delete empty fragments and check that all were empty
void checkAndDeleteFragments(vector <Molecule*> frags)
{
	int empty = 0;
	for (vector <Molecule*>::iterator i = frags.begin(); i != frags.end(); i++)
	{
		if( (*i)->countAtoms() == 0)
			delete (*i);
		else
			empty++;
	}
	if( empty != 0 )
		Log << "WARNING: There are still " << empty <<" unconnected fragments!"<<endl;
}


// TODO: remove this DEBUG function
// Write several result molecules to one file
void writeMolVec(vector<Molecule* >& input, SDFile* handle)
{
	for(int i = 0; i < input.size(); i++)
	{
		(*handle) << *input[i];
	}
}


// get the position of an atom in the molcule list:
const int getAtomPosition(Atom* atm, Molecule* mol)
{
	AtomIterator ati = mol->beginAtom();
	for (int i = 0; +ati; ati++, i++)
	{
		if(&*ati == atm)
			return i;
	}
	return -1;
}


/// empty 'fromMol' and append the atoms to 'toMol'
void transferMolecule( Molecule* toMol, Molecule* fromMol)
{
	int num_atm = fromMol->countAtoms();
	for(int i = 0; i < num_atm; i++)
		toMol->insert( *fromMol->beginAtom() ); // insert auto removes from its old parent
}


/// check if the atom is a rigid one:
bool isAtomRigid(OBAtom* atm)
{
/// TODO: add OBRotorList object to use custom torlib!
	if (atm->IsInRing())
		return true;
	else
	{
		OBBondIterator b_it = atm->BeginBonds();
		for(; b_it!=atm->EndBonds(); b_it++)
		{
			if( ! (*b_it)->IsRotor() )
				return true;
		}
		return false;
	}
}

// cut bonds that are shared with atoms from other fragments:
void clearExternalBonds(Molecule* mol)
{
	Atom::BondIterator bit;
	AtomIterator ait;

	BALL_FOREACH_INTERBOND(*mol, ait, bit)
	{
		bit->destroy();
	}
}

// Copy properties from an original molecule to a copy molecule
void copyMoleculeProperies(Molecule &orig, Molecule &cop)
{
	NamedPropertyIterator  it;
	for(it = orig.beginNamedProperty (); it !=orig.endNamedProperty(); it++)
	{
		cop.setProperty(*it);
	}
	cop.setName(orig.getName());
}

void setCoordinates(Molecule* query, TemplateCoord* templat)
{
	AtomIterator qit = query->beginAtom();
	for (int i = 0 ; i < templat->size(); i++, qit++)
	{
		qit->setPosition( (*templat)[i]);
	}
}




///####################### L O A D I N G ##############################
void getLibraryPathes(vector<String>& result_pathes, String config_path)
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
			result_pathes[0] = tmp.trim();
		}
		else if(tmp.hasPrefix("bondlenths=")){
			tmp = tmp.after("bondlenths=");
			result_pathes[1] = tmp.trim();
		}
		else if(tmp.hasPrefix("connections=")){
			tmp = tmp.after("connections=");
			result_pathes[2] = tmp.trim();
		}
		else
			continue;
	}
}

void readFragmentLib(String& path, boost::unordered_map <BALL::String, Molecule*>& fragmentLib)
{
	fragmentLib.clear();
	SDFile libFile(path, ios::in);
	
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


/**
 * @brief Reads the template coordinates for all fragments
 * @param path
 * @param fragmentLib
 */
void readNewFragmentLib(const String& path, boost::unordered_map <BALL::String, TemplateCoord*>& fragmentLib)
{
	fragmentLib.clear();
	LineBasedFile libFile(path, ios::in);
	
	// read in fragmentLib and create hash-map from that:
	String key;
	while( libFile.readLine() )
	{
		TemplateCoord* tmp_frag=0;
		if ( libFile.getLine().hasPrefix("key "))
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
			fragmentLib[key] = tmp_frag;
		}
		else
		{
			Log << "WARNING: missed in the template coordinate lib file a line!!!"<<endl;
		}
	}
	libFile.close();
}




void readConnectionLib(String& path, boost::unordered_map <String, Molecule* >& con)
{
	con.clear();
	
	SDFile handle(path, ios::in); //open the lib file as sdf-file
	
	// read all lib molecules and save them with their key:
	Molecule* tmp_mol;
	tmp_mol = handle.read(); // first entry
	
	int cnt = 0;
	while(tmp_mol)
	{
		BALL::String key = tmp_mol->getProperty("key").getString();
		con[key] = tmp_mol;
		
		tmp_mol = handle.read();
		cnt++;
	}
	
	handle.close();
}


/// read std Bonds from file:
/// ---------------------------------------
void readBondLib(String& path, boost::unordered_map <String, float >& bonds)
{
	LineBasedFile bondFile(path, ios::in);
	
	while( bondFile.readLine() )
	{
		String st_ar[2];
		bondFile.getLine().split(st_ar, 2);
		
		bonds[st_ar[0]] = st_ar[1].toFloat();
		
		// generate also the reversed label, if both differ
		if( (st_ar[0])[0] != (st_ar[0])[1] ){
			String altKey = (st_ar[0])[1];
			altKey += (st_ar[0])[0];
			bonds[altKey] = st_ar[1].toFloat();
		}
	}
}


void readOBMolecule(const String& path, OBMol& mol)
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


/**
 * Convert an input SMILES to a GroupFragment type (molecule with connections
 * and rotors). The rotors however will be added later during fragmentation.
 * @brief smilesToGroupFragment
 * @param conv
 * @param smiles
 * @param frag
 */
GroupFragment* smilesToGroupFragment(OBConversion& conv, String& smiles)
{
	// get the obmol SMILES input:
	OBMol ob_mol;
	conv.ReadString( &ob_mol, smiles );
	
	GroupFragment* frag = new GroupFragment();
	list< pair<int, OBAtom*> > con_lst;
	
	// find r-group connections:
	vector<OBAtom*> for_deletion;
	for(OBAtomIterator it = ob_mol.BeginAtoms(); it != ob_mol.EndAtoms(); it++)
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
	
	// Delete unnecessary atoms:
	for(int i = 0; i < for_deletion.size(); i++)
		ob_mol.DeleteAtom( for_deletion[i], true);
	
	// convert to BALL::Molecule
	frag->molecule = MolecularSimilarity::createMolecule(ob_mol, true);
	
	// convert the Open babel connection list to a GroupFragment c-list:
	list< pair<int, OBAtom*> >::iterator iti = con_lst.begin();
	for(; iti != con_lst.end(); iti++)
	{
		Atom* atm_ptr = frag->molecule->getAtom( (*iti).second->GetIdx() - 1 );
		unsigned int group_id = (*iti).first;
		
		frag->connections.push_back(  make_pair( group_id, atm_ptr )  );
	}
	
	return frag;
}


/**
* Parse the combi lib file
* @brief readGroups
* @param path
*/
void readGroups(CombiLib& input_lib, const String& path)
{
	typedef vector<GroupFragment*> GroupFragmentList;
	typedef boost::unordered_map< int, GroupFragmentList* > CombiLib;
	typedef boost::unordered_map< int, GroupFragmentList* >::iterator CombiLibIterator;
	/// Init the Babel-String reader:
	OBConversion conv;
	conv.SetInFormat("smi");
	
	/// Read combiLib from line file
	LineBasedFile combiLibFile(path, ios::in);
	GroupFragmentList* tmp_lst;
	
	int group_id = -1;
	GroupFragment* tmp_frag;
	
	while( combiLibFile.readLine() )
	{
		/// ignore comments and empty lines
		if ( combiLibFile.getLine().hasPrefix("#") || combiLibFile.getLine().isEmpty() )
		{
			continue;
		}
		
		/// scaffold (takes the last defined scaffold)
		else if ( combiLibFile.getLine().hasPrefix("scaffold:"))
		{
			// get the scaffold SMILES
			String str = String(combiLibFile.getLine().after("scaffold:")).trim();
			
			// generate a new GroupFragment (on heap) from the SMILES
			tmp_frag = smilesToGroupFragment(conv, str);
			
			// if we have another scaffold, delete it and use the new one:
			if(input_lib.find(0) != input_lib.end())
			{
				Log<<"INPUT-WARNING: overwriting a former scaffold, using now: "<<str
					 <<endl<<"Remember that only one scaffold per combi-lib configuration"
					 <<" file should be specified. If more scaffolds occur, the last one"
					 <<" is used"<<endl;
				delete input_lib[0];
			}

			tmp_lst = new GroupFragmentList;
			tmp_lst->push_back(tmp_frag);
			input_lib[0] = tmp_lst;
		}
		
		/// create a new group:
		else if ( combiLibFile.getLine().hasPrefix("group_"))
		{
			// get group number:
			String str_num = String(combiLibFile.getLine().after("group_")).trim().trimRight(":");
			group_id = str_num.toUnsignedInt();
			if(group_id == 0)
			{
				Log<<"INPUT-ERROR: r-group id 0 is forbidden. Please use a integer "
						 "between 1 and INT-MAX."<<endl;
				exit(EXIT_FAILURE);
			}
			
			// create new groupfragmentlist for the group:
			///TODO: check for existing scaffold (0) and replace existing, avoid mem-leak
			tmp_lst = new GroupFragmentList;
			input_lib[group_id] = tmp_lst;
		}
		
		/// append group-Fragment to current group
		else
		{
			GroupFragment* tmp_frag = smilesToGroupFragment( conv, combiLibFile.getLine().trim() );
			
			bool found = false;
			list< pair< unsigned int, Atom*> >::iterator it = tmp_frag->connections.begin();
			
			// make sure that the fragment realy contains the needed connection:
			for(; it != tmp_frag->connections.end(); it++)
				found = (found || ( (*it).first == group_id ) );
			if (found)
			{
				tmp_lst->push_back(tmp_frag);
			}
			else
			{
				Log<<"INPUT-ERROR: the r-group SMILES "<< combiLibFile.getLine().trim() 
					 <<" belongs to the r-group block "<< group_id
					 <<" but that group number was not found in the SMILES"<<endl;
				Log<<endl;
				Log<<"Please re-check your combi-lib configuration file."<<endl;
				exit(EXIT_FAILURE);
			}
		}
	}
	combiLibFile.close(); // close combiLib file
//	//Test: iterate whole lib
//	CombiLibIterator it = input_lib.begin();
//	for(; it != input_lib.end(); it++)
//	{
//		cout<<endl<<"Group ID: "<<(*it).first<<endl;
		
//		tmp_lst = (*it).second;
//		for(int i = 0; i< tmp_lst->size(); i++)
//		{
//			tmp_frag = (*tmp_lst)[i];
//			cout<<"Molecule "<< i <<" #atoms: "<<tmp_frag->molecule->countAtoms()<<endl;
//		}
//	}
}

#endif // BASIC_H








