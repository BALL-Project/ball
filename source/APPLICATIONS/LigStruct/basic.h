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
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/atom.h>
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
typedef vector< Atom*> Fragment;

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


//namespace BALL
//{
//	/**
//	 * TODO: Warning, when aligning we will probably get errors because we still
//	 *       have bonds to the ofther fragments, thus the 'site'-generation might
//	 *       not work properly
//	 * 
//	 * @brief The SubMolecule class
//	 */
//	class SubMolecule: public Molecule
//	{
	
//	public:
//		SubMolecule(vector< Atom* >* frag)
//		{
//			atomList = frag;
			
//			vector< Atom* >::iterator fit = frag->begin();
//			for(; fit != frag->end(); fit++)
//			{
//				insertWithoutRemoving(**fit);
				
//				// TODO: remove all bonds lead outside of the SubMolecule (hacking hacking....)
				
//			}
//		}
		
//		/** 
//		 * Delete instance without deleting the contained atoms!
//		 * 
//		 * Do not declare as virtual, otherwise the Super-Destructor will be called
//		 * deleting all atoms... although they shall be reused!
//		 */
//		~SubMolecule()
//		{
//			atomList = 0;
			
//			// Do everything what the Super destructors SHOULD do, even in our case:
//			PropertyManager::destroy();
//			if ( ((Composite*)this)->parent_ != 0)
//			{
//				this->parent_->removeChild(*this);
//			}
			
//			/// from Composite::clear()
//			///
//	//		// do NOT delete the contained atoms:
//	//		for (Composite* composite_ptr = first_child_; composite_ptr != 0; )
//	//		{
//	//			Composite* next_ptr = composite_ptr->next_;
			
//	//			if (composite_ptr->isAutoDeletable())
//	//			{
//	//				delete composite_ptr;
//	//			} 
//	//			else
//	//			{
//	//				composite_ptr->previous_ = composite_ptr->next_ = composite_ptr->parent_ = 0;
//	//				composite_ptr->clear();
//	//			}
				
//	//			composite_ptr = next_ptr;
//	//		}
	
//			// clear pointers
//			first_child_ = last_child_ = 0;
	
//			// clear properties
//			BALL_BIT_CLEAR_ALL(properties_);
	
//			// update counters and selection
//			number_of_children_ = 0;
//			number_of_selected_children_ = 0;
//			number_of_children_containing_selection_ = 0;
//			contains_selection_ = selected_;
//			updateSelection_();
	
//			// update modification time stamp
//			stamp(MODIFICATION);
//		}
		
//		Size countAtoms() const
//		{
//			return atomList->size();
//		}
		
//		Atom& operator[](Position idx)
//		{
//			return *(atomList->at(idx));
//		}
	
//		// 
//		// (Do not delete the atomList at destruction)
//		vector< Atom* >* atomList;
		
//	private:
//		/**
//		 * @brief insertWithoutRemoving, copied and modified from 
//		 * Composite::appendChild(). Modification: we append the atom but do not
//		 * delete it from its parent.
//		 * @param atm
//		 */
//		void insertWithoutRemoving(Atom& atm)
//		{
//			// if it is already the last child, everything is done
//			if (&atm == last_child_)
//			{
//				return;
//			}
			
//			// its the first child
//			if (last_child_ == 0)
//			{
//				first_child_ = last_child_ = &atm;
//			} 
//			else 
//			{
//				// append it to the list of children
//				last_child_->next_ = &atm;
//				atm.previous_ = last_child_;
//				last_child_ = &atm;
//			}
	
//			/// We just host the atom, but do not want to adopt it:
//			++number_of_children_;
	
//			/// Should not be seen as modification:
//	//		// update modification time stamp
//	//		last_child_->stamp(MODIFICATION);
		
//			/// Do not think selection plays a role for what we do:
//	//		// update selection counters
//	//		if (atm.containsSelection())
//	//		{
//	//			number_of_children_containing_selection_++;
				
//	//			if (atm.selected_)
//	//			{
//	//				number_of_selected_children_++;
//	//			}
				
//	//			// recursively update the nodes` states
//	//			updateSelection_();
//	//		}
//		}
//	};
//}

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


#endif // BASIC_H
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
