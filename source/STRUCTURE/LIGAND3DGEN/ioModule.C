// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/STRUCTURE/LIGAND3DGEN/ioModule.h>
#include <BALL/STRUCTURE/LIGAND3DGEN/ligand3DBase.h>

#include <BALL/KERNEL/PTE.h>
#include <BALL/STRUCTURE/molecularSimilarity.h>

using namespace std; 
using namespace BALL;


/// C o m b i L i b M a n a g e r
/// ############################################################################

CombiLibManager::CombiLibManager(LineBasedFile *combilib_file):
	_combilib_file(combilib_file),
	_scaffold(0),
	_lib_is_generated(false)
{}

CombiLibManager::~CombiLibManager()
{
	// _combilib_file to be deleted out side of CombiLibManager
	_combilib_file = 0;
}

void CombiLibManager::setCombiLib(LineBasedFile &combilib_file)
{
	_combilib_file = & combilib_file;
	
	_parseCombiLibFile();
}

RFragment &CombiLibManager::getScaffold()
{
	if( !_lib_is_generated )
	{
		_parseCombiLibFile();
	}
	return *_scaffold;
}

CombiLibMap &CombiLibManager::getCombiLib()
{
	if( !_lib_is_generated )
	{
		_parseCombiLibFile();
	}
	return _lib;
}


void CombiLibManager::generateCombinationsSMILES(list<String>& out_SMILES)
{
	out_SMILES.clear();
	
	if( !_lib_is_generated )
	{
		_parseCombiLibFile();
	}
	// generate all combinations as AtomContainer:
	list<AtomContainer*> combi_molecules;
	
	generateCombinationsAtomContainer(combi_molecules);
	
	// Use open babel to convert to SMILES:
	list<AtomContainer*>::iterator it = combi_molecules.begin();
	for( ; it != combi_molecules.end(); ++it)
	{
		OpenBabel::OBMol* tmp = MolecularSimilarity::createOBMol( **it ,1,1);
		
		OpenBabel::OBConversion ba_conv;
		ba_conv.SetOutFormat("smi");
		
		String tmp_SMILES = ba_conv.WriteString( tmp, 1 );
		
		out_SMILES.push_back( tmp_SMILES );
		
		delete tmp; tmp = 0;
	}
}

void CombiLibManager::generateCombinationsAtomContainer(
		list<AtomContainer*>& out_molecules)
{
	if( !_lib_is_generated )
	{
		_parseCombiLibFile();
	}
	
	_r_atms.clear();
	
	_scaffold = this->_lib[0][0];
	
	for(list<RAtom>::iterator it = _scaffold->r_atom_lst.begin(); 
			it != _scaffold->r_atom_lst.end(); ++it)
	{
		_r_atms.push_back( &*it );
	}
	
	_recurGenerateCombi( out_molecules );
}

void CombiLibManager::_sortRGroups()
{
	for(unsigned int i = 1; i < _lib.size(); ++i)
	{
		std::sort(_lib[i].begin(), _lib[i].end(), _compareRFrag);
	}
}

bool CombiLibManager::_compareRFrag(RFragment *&frag1, RFragment *&frag2)
{
	return frag1->size > frag2->size;
}

void CombiLibManager::_recurGenerateCombi(std::list<AtomContainer *> &out_molecules)
{
	//1.) get next unhandled RAtom:
	RAtom* ra = 0;
	if( ! _r_atms.empty() )
	{
		ra = _r_atms.front();
		_r_atms.pop_front(); // deeper recursions shall not handle this r-atom again
	}
	
	//2.) check for end recursion case:
	if( !ra )
	{
		// make a copy of the current molecule in _scaffold, push it to the 
		// result list and return
		out_molecules.push_back( new AtomContainer(* _scaffold->molecule) );
		return;
	}
	
	//3.) recursion case:
	else
	{
		// iterate over all RFrags in the next needed RGroup:
		vector<RFragment*>& group = _lib[ra->id];
		vector<RFragment*>::iterator it2;
		for(it2 = group.begin(); it2 != group.end(); ++it2)
		{
			RFragment* tmp = new RFragment( **it2 );
			Bond* bnd = 0;
			
			//3.1.) extend r_atom_lst with the RAtoms of the new RFragment
			//    (we do not need the rotors here, so ignore them)
			for( list< RAtom >::iterator it = tmp->r_atom_lst.begin(); 
					 it != tmp->r_atom_lst.end(); ++it)
				_r_atms.push_back( &*it );
			
			//3.2.) connect the molecule to the scaffold and create a bond
			_scaffold->molecule->insert( *tmp->molecule );
			Bond b;
			b.setOrder(1);
			bnd = ra->atm->createBond(b, *tmp->group_atom );
			
			//3.3.) recurr deeper
			_recurGenerateCombi( out_molecules );
			
			
			//3.4.) delete bond, and remove inserted molecule
			bnd->destroy();
			_scaffold->molecule->remove( *tmp->molecule );
			
			//3.5.) remove the added r_atom from the list again
			for( list< RAtom >::iterator it = tmp->r_atom_lst.begin();
					 it != tmp->r_atom_lst.end(); ++it)
				_r_atms.pop_back();
			
			delete tmp;
		}
	}
	// upper calls to _recurGenerateCombi should handle this atom first
	_r_atms.push_front( ra );
}

void CombiLibManager::_parseCombiLibFile()
{
	//#TODO: better whole class clearing
	_id_mapping.clear();
	_lib.clear();
	
	if (_combilib_file->isClosed())
	{
		// Some exception!
		cout<<"CombiLibManader ERROR: file is closed"<<endl; exit(EXIT_FAILURE);
	}
	else if( !_combilib_file)
	{
		// Some exception!
		cout<<"CombiLibManader ERROR: file is null"<<endl; exit(EXIT_FAILURE);
	}
	
	int g_id = -2;
	int old_id = -2;
	
	// read the file line by line
	while( _combilib_file->readLine() )
	{
		/// 'comments' and 'empty lines': ignore
		if ( _combilib_file->getLine().hasPrefix("#") || _combilib_file->getLine().isEmpty() )
		{
			continue;
		}
		
		
		/// 'scaffold': set scaffold
		else if ( _combilib_file->getLine().hasPrefix("scaffold:"))
		{
			// get the scaffold SMILES
			String str = String(_combilib_file->getLine().after("scaffold:")).trim();
			
			// if we have another scaffold, delete it and use the new one:
			if(_scaffold != 0)
			{
				Log<<"ERROR CombiLibManager: found second scaffold definition: "<<str
					 <<endl<<"Remember that only one scaffold per combi-lib configuration"
					 <<" file should be specified."<<endl;
				exit(EXIT_FAILURE);
			}
			_scaffold = _smi_parser.fromSMILEStoRFragment(str, 0);
			_lib.push_back( vector<RFragment*>() );
			_lib[0].push_back(_scaffold);
			
			// update our id_mapping and the one for the r-fragment:
			for( list< RAtom >::iterator it = _scaffold->r_atom_lst.begin();
					 it != _scaffold->r_atom_lst.end(); ++it)
			{
				int r_id = (*it).id;
				
				// create new r-groups for all found r-atoms of the scaffold
				if ( _id_mapping.find(r_id) == _id_mapping.end() )
				{
					_id_mapping[r_id] = _lib.size();
					_lib.push_back( vector<RFragment*>() );
				}
				(*it).id = _id_mapping[r_id];
			}
		}
		
		/// 'group_': create a new group
		else if( _combilib_file->getLine().hasPrefix("group_") )
		{
			if( !_scaffold )
			{
				cout<<"ERROR CombiLibManager: found r-group definition before scaffold "
						<<"definition, which is not allowed. Please fix your combi-file"<<endl;
				exit(EXIT_FAILURE);
			}
			
			// set old id:
			old_id = g_id;
			
			// get group number:
			String str_num = String(_combilib_file->getLine().after("group_")).trim().trimRight(":");
			g_id = str_num.toUnsignedInt();
			
			// ERROR: g_id 0 is reserved for the scaffold
			if( g_id == 0)
			{
				cout<<"ERROR in CombiLibManager: found group id 0 for a r-group, but id"
					  << " 0 is reserved for the scaffold"<<endl;
				exit(EXIT_FAILURE);
			}
			
			// check for correct g_id naming:
			if( old_id >= g_id )
			{
				cout<<"ERROR in CombiLibManager: found group id "<< g_id<< " and "
					  <<"previous id was "<<old_id<<endl;
				cout<<"The groups ids need to increase when reading the combi-file "
						<<"from top to bottom"<<endl;
				exit(EXIT_FAILURE);
			}
			
			// map the found g_id to internal-g_id if not already existing:
			if( _id_mapping.find(g_id) == _id_mapping.end() )
			{
				// create new groupfragmentlist for the group (r-groups start at 1)
				_id_mapping[g_id] = _lib.size();
				_lib.push_back( vector<RFragment*>() );
			}
		}
		
		
		/// '[ some number * ]': append a SMILES to current group_id
		else if( _combilib_file->getLine().hasPrefix("[") )
		{
			String str = _combilib_file->getLine().trim() ;
			
			// make sure that the fragment really contains the needed connection:
			if ( !str.hasSubstring( "[" + String(g_id) + "*]" )  )
			{
				Log<<"ERROR in CombiLibManager: the r-group SMILES "<< str <<" belongs to the "
					 <<"r-group block "<< g_id <<" but that number was not present "
					 <<"in the SMILES"<<endl<<"Please check your combi-file."<<endl;
				exit(EXIT_FAILURE);
			}
			
			RFragment* tmp =  _smi_parser.fromSMILEStoRFragment(str, g_id);
			tmp->group_id = _id_mapping[g_id];
			
			// update our id_mapping and the one for the r-fragment:
			list< RAtom >::iterator it = tmp->r_atom_lst.begin();
			for(; it != tmp->r_atom_lst.end(); ++it)
			{
				int r_id = (*it).id;
				
				// create new r-groups for the r-atoms with new group ids
				if ( _id_mapping.find(r_id) == _id_mapping.end() )
				{
					_id_mapping[r_id] = _lib.size();
					_lib.push_back( vector<RFragment*>() );
				}
				(*it).id = _id_mapping[r_id];
			}
			
			// add to combi lib:
			_lib[ _id_mapping[g_id] ].push_back( tmp );
		}

		/// 'undefined line': throw error
		else
		{
			//#TODO: throw exception
			Log<<"ERROR in CombiLibManager: could not understand the combi lib line: ";
			Log<< _combilib_file->getLine()<<endl;
			exit(EXIT_FAILURE);
		}
	}
	 
	// check if for all found r-groups we have at last one r-fragment
	boost::unordered_map< int, int >::iterator it2;

	for(it2 = _id_mapping.begin(); it2 != _id_mapping.end(); ++it2)
	{
		if( _lib[it2->second].size() == 0 )
		{
			Log<<"ERROR in CombiLibManager: could not find a r-fragment for r-group "
				 <<it2->first<< " but that r-group is referenced in the combi-file. "
				 <<"If that r-group should not exist anymore, check if any unnecessary"
				 <<"r-label still references that group."<<endl;
			exit(EXIT_FAILURE);
		}
	}
	
	_sortRGroups();
	
	_lib_is_generated = true;
}

/// S m i l e s P a r s e r
/// ############################################################################

SmilesParserOB::SmilesParserOB(){
	_babel_conv.SetInFormat("smi");
}

SmilesParserOB::~SmilesParserOB(){}

AtomContainer *SmilesParserOB::fromSMILEStoMolecule(const String &smiles_string)
{
	_babel_conv.ReadString(&_babel_mol, smiles_string);
	
	AtomContainer* tmp = MolecularSimilarity::createMolecule(_babel_mol, true);
	_cano.canonicalize(*tmp);
	return tmp;
}

RFragment *SmilesParserOB::fromSMILEStoRFragment(const String &smiles_string, 
																							 const int &g_id)
{
	// get the obmol SMILES input:
	_babel_conv.ReadString(&_babel_mol, smiles_string);
	
	RFragment* frag = new RFragment();
	frag->group_id = g_id;
	frag->curr_set = -1;
	frag->size = _babel_mol.NumAtoms();
	list< pair<int, OpenBabel::OBAtom*> > con_lst;
	OpenBabel::OBAtom* group_atom = 0;
	
	// find all r-labels and their r-atoms:
	vector<OpenBabel::OBAtom*> for_deletion;
	OpenBabel::OBAtomIterator it = _babel_mol.BeginAtoms();
	for(; it != _babel_mol.EndAtoms(); it++)
	{
		// connection sites (r-groups) are marked with 'Du' in OB and the isotope 
		// label carries the identifier of the r-group:
		if(!strcmp( (*it)->GetType(), "Du") ) 
		{
			OpenBabel::OBAtom* ptr = ( *(*it)->BeginBonds())->GetNbrAtom( *it );
			int r_id = (*it)->GetIsotope();
			
			if (r_id == g_id)
			{
				group_atom = ptr;
			}
			
			for_deletion.push_back( *it );
			con_lst.push_back(  make_pair( r_id, ptr )  );
		}
	}
	
	// ERROR, we should have found a specific group id but could not find it.
	// g_id == -1 means we are not interested in the group_id at all
	// g_id ==  0 means we have a scaffold and thus no group atom anyways
	if( !group_atom && g_id != -1 && g_id != 0 )
	{
		cout<<"ERROR in SMILESParser: could not find r-label with group_id "
			  << g_id << " in SMILES " << smiles_string << endl;
		exit(EXIT_FAILURE);
	}
	
	// Delete the r-labels (which are only pseudo atoms for highlighting r-atoms)
	for(unsigned int i = 0; i < for_deletion.size(); i++)
		_babel_mol.DeleteAtom( for_deletion[i], true);
	
	// convert OBMol to BALL::Molecule and canonize
	frag->molecule = MolecularSimilarity::createMolecule(_babel_mol, true);
	
	// convert the Open babel r-connection list to the final list r-atoms:
	list< pair<int, OpenBabel::OBAtom*> >::iterator iti = con_lst.begin();
	for(; iti != con_lst.end(); iti++)
	{
		Atom* atm_ptr = frag->molecule->getAtom( (*iti).second->GetIndex() );
		
		if ( (*iti).second == group_atom)
		{
			frag->group_atom = atm_ptr;
		}
		else
		{
			unsigned int group_id = (*iti).first;
			RAtom new_r;
			new_r.id  = group_id;
			new_r.atm = atm_ptr;
			new_r.parent = frag;
			
			frag->r_atom_lst.push_back( new_r );
		}
	}
	_cano.canonicalize( * frag->molecule ); // will be done during assembly anyways
	
	return frag;
}
