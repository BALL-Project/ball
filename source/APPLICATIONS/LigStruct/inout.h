#ifndef INOUT_H
#define INOUT_H

#include "basic.h"
#include "fragmenter.h"

using namespace OpenBabel;
using namespace BALL;
using namespace std;

/// ################# R e a d i n g   &   L o a d i n g #################
// TODO: remove this DEBUG function
// Write several result molecules to one file
void writeMolVec(vector<Molecule* >& input, SDFile* handle)
{
	for(int i = 0; i < input.size(); i++)
	{
		(*handle) << *input[i];
	}
}


void getLibraryPathes(PredictorConfig& pathes, String config_path)
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
			pathes.fragment_lib_path = tmp.trim();
		}
		else if(tmp.hasPrefix("bondlenths=")){
			tmp = tmp.after("bondlenths=");
			pathes.bondlenth_lib_path = tmp.trim();
		}
		else if(tmp.hasPrefix("connections=")){
			tmp = tmp.after("connections=");
			pathes.connection_lib_path = tmp.trim();
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
	
	// convert OBMol to BALL::Molecule
	frag->molecule = MolecularSimilarity::createMolecule(ob_mol, true);
	
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
#endif // INOUT_H
