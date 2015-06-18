// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/STRUCTURE/LIGAND3DGEN/ligand3Dbase.h> 
#include <BALL/STRUCTURE/UCK.h>

#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/FORMAT/SDFile.h>
#include <BALL/SYSTEM/path.h>

using namespace BALL;
using namespace std;

/// R i g i d F r a g m e n t s D a t a b a s e
/// ############################################################################
RigidFragmentDB::RigidFragmentDB()
{
	RigidFragmentDB("");
}

RigidFragmentDB::~RigidFragmentDB()
{
	// clear memory from allocated template fragments
	for(RigidFragmentDB::iterator it = this->begin();
			it != this->end(); ++it)
	{
		delete it->second;
	}
}

RigidFragmentDB::RigidFragmentDB(const BALL::String& filename)
{
	//1.) Find correct path:
	_filename = filename;
	if (_filename == "")
	{
		_filename = "fragments/template_rigids.line";
	}
	
	Path path;
	_filename = path.find( _filename );
	
	if ( _filename == "")
	{
		throw Exception::FileNotFound(__FILE__, __LINE__, filename);
	}
	
	//2.) Load the DB:
	LineBasedFile db_file(_filename, ios::in);
	
	// read in fragmentLib and create hash-map from that:
	String key;
	while( db_file.readLine() )
	{
		TemplateCoord* tmp_frag=0;
		if ( db_file.getLine().hasPrefix("key ") )
		{
			// get key:
			key = db_file.getLine().after("key ");
			
			// get number of positions:
			db_file.readLine();
			Size size = db_file.getLine().toUnsignedInt();
			
			// get positions:
			tmp_frag = new TemplateCoord(size);
			Size i;
			for(i = 0; i < size; i++)
			{
				db_file.readLine();
				String coords[3];
				db_file.getLine().split(coords, 3);
				Vector3& vec = (*tmp_frag)[i];
				vec.set(coords[0].toFloat(), coords[1].toFloat(), coords[2].toFloat());
			}
			
			// append to hash map
			this->insert( make_pair(key, tmp_frag) );
		}
		else
		{
			Log << "WARNING: missed in the template coordinate lib file a line!!!"
					<< endl;
		}
	}
	db_file.close();
}


/// S i t e F r a g m e n t s D a t a b a s e
/// ############################################################################
SiteFragmentDB::SiteFragmentDB()
{
	SiteFragmentDB("");
}

SiteFragmentDB::~SiteFragmentDB()
{
	// clear memory from allocated template fragments
	for(SiteFragmentDB::iterator it = this->begin();
			it != this->end(); ++it)
	{
		delete it->second;
	}
}

SiteFragmentDB::SiteFragmentDB(const BALL::String& filename)
{
	//1.) find correct path:
	_filename = filename;
	if (_filename == "")
	{
		_filename = "fragments/template_sites.sdf";
	}
	
	Path path;
	_filename = path.find( _filename );
	
	if ( _filename == "")
	{
		throw Exception::FileNotFound(__FILE__, __LINE__, filename);
	}
	
	//2.) Load the DB:
	SDFile handle(_filename, ios::in); //open the lib file as sdf-file
	
	// read all lib molecules and save them with their key:
	Fragment* tmp_mol;
	tmp_mol = (Fragment*)handle.read(); // first entry
	
	int cnt = 0;
	while(tmp_mol)
	{
		BALL::String key = tmp_mol->getProperty("key").getString();
		this->insert(make_pair(key, tmp_mol));
		
		tmp_mol = (Fragment*)handle.read();
		cnt++;
	}
	handle.close();
}

/// C l a s s   M a t c h e r
/// ############################################################################
Matcher::Matcher(RigidFragmentDB &coord_map):_coord_lib(coord_map){}

Matcher::~Matcher(){}

void Matcher::matchFragment(AtomContainer& fragment)
{
	// for all fragments, match these against the lib:
	UCK keyGen( fragment, true, 5 );
	TemplateCoord* templat = _coord_lib[ keyGen.getUCK() ];
	
	if(templat && (templat->size() == fragment.countAtoms()) )
	{
		templat->applyCoordinates2Molecule( fragment );
	}
	else
	{
		throw Exception::FragmentTemplateNotFound("canonicalizer.C", 81, fragment);
	}
}

const String Matcher::getUCK(AtomContainer &mol)
{
	UCK keyGen( mol, true, 5 );
	return keyGen.getUCK();
}


/// C l a s s   T e m p l a t e C o o r d
/// ############################################################################
TemplateCoord::TemplateCoord(BALL::Size n)
{
	_size = n;
	positions.reserve( _size );
}

TemplateCoord::TemplateCoord( AtomContainer& mol)
{
	_size = mol.countAtoms();

	positions.reserve( _size );
	
	// transfer initial coords:
	for (AtomIterator qit = mol.beginAtom(); +qit; qit++)
	{
		positions.push_back( qit->getPosition() );
	}
}

void TemplateCoord::applyCoordinates2Molecule( AtomContainer& mol)
{
	AtomIterator qit = mol.beginAtom();
	for (int i = 0 ; i < _size; i++, qit++)
	{
		qit->setPosition( positions[i] );
	}
}

void TemplateCoord::readCoordinatesFromMolecule( AtomContainer& mol)
{
	if(positions.size() == _size)
	{
		AtomIterator qit = mol.beginAtom();
		for (int i = 0 ; i < _size; i++, qit++)
		{
			positions[i] = qit->getPosition() ;
		}
	}
	// very first insertion:
	else
	{
		AtomIterator qit = mol.beginAtom();
		for (int i = 0 ; i < _size; i++, qit++)
		{
			positions.push_back( qit->getPosition() );
		}
	}
}

TemplateCoord::~TemplateCoord()
{
}

Vector3& TemplateCoord::operator[]( Index idx)
{
	return positions[idx];
}

Vector3& TemplateCoord::get( Index idx)
{
	return positions[idx];
}

const Size& TemplateCoord::size()
{
	return _size;
}

/// S t r u c t   R A t o m
/// ############################################################################
//#WARNING: add a guard to not override via insert ???
void RAtom::addParnter(RFragment &other)
{
	known_partners[ make_pair(parent->curr_set, &other) ] = other.curr_set;
}

int RAtom::getCompatibleSet(RFragment &other)
{
	std::map< pair<int ,RFragment*>, int >::iterator it;
	it = known_partners.find( make_pair(parent->curr_set, &other) );
	
	if( it != known_partners.end() )
	{
		if( it->second < other.coord_sets.size())
			return it->second;
	}

	return -1;
}

/// C l a s s   R F r a g m e n t
/// ############################################################################
RFragment::RFragment()
{
	this->size       = 0;
	this->group_atom = 0;
	this->group_id   = 0;
	this->molecule   = 0;
	this->rotor_lst  = 0; 
}

RFragment::~RFragment()
{
	for(int i = 0; i < coord_sets.size(); ++i)
	{
		delete coord_sets[i];
	}
	
	rotor_lst->clear();
	delete rotor_lst;
	
	delete molecule;
}

RFragment::RFragment(const RFragment& other)
{
	this->molecule = new BALL::AtomContainer();
	
	// clone atoms:
	RAtom const * tmp_r = 0;
	for(BALL::AtomIterator ati = other.molecule->beginAtom(); +ati; ++ati)
	{
		BALL::Atom* tmp_at = new BALL::Atom( *ati );
		this->molecule->insert( *tmp_at );
		
		// check what kind of atom we are currently coping
		// case group atom: transfer the group atom:
		if( &*ati == other.group_atom)
		{
			this->group_atom = tmp_at;
		}
		// else check for r-atom
		else
		{
			tmp_r = _isRAtom( other.r_atom_lst, &*ati);
			if( tmp_r )
			{
				RAtom new_r;
				new_r.atm = tmp_at;
				new_r.id  = tmp_r->id;
				
				this->r_atom_lst.push_back( new_r );
			}
		}
	}
	
	// clone bonds
	BALL::cloneBonds(*other.molecule, *this->molecule);
	
	group_id = other.group_id;
	rotor_lst = other.rotor_lst;
}

void RFragment::setCoordsTo(const int &set)
{
	if( curr_set == set)
		return;
	
	coord_sets[set]->applyCoordinates2Molecule( *molecule );
	curr_set = set;
}

void RFragment::newSetFromCurrent()
{
	curr_set = coord_sets.size();
	coord_sets.push_back( new TemplateCoord( *molecule ) );
}

RAtom const* RFragment::_isRAtom( const std::list< RAtom >& ilist, BALL::Atom* atom)
{
	std::list< RAtom >::const_iterator it = ilist.begin();
	for(; it != ilist.end(); ++it)
	{
		if( (*it).atm == atom )
			return &*it;
	}
	
	return 0;
}


/// C l a s s   L i g B a s e
/// ############################################################################
bool LigBase::containsUnknownElement(AtomContainer &ac)
{
	for( AtomIterator it = ac.beginAtom(); +it; ++it)
	{
		if( it->getElement().getSymbol() == "?")
			return true;
	}
	
	return false;
}

bool LigBase::containsHydrogen(AtomContainer &ac)
{
	for(AtomIterator ati = ac.beginAtom(); +ati; ++ati)
	{
		if(ati->getElement().getSymbol() == "H")
		{
			return true;
		}
	}
	return false;
}

String LigBase::printInlineMol(Composite *mol)
{
	String tmp = "";
	AtomIterator pre = ((AtomContainer*)mol)->endAtom();
	--pre;
	for(AtomIterator ati = ((AtomContainer*)mol)->beginAtom(); +ati; ++ati)
	{
		tmp += ati->getElement().getSymbol();
		tmp += String( countBondsAndOrder(*ati ) );
		if( ati != pre )
			tmp += "-";
	}
	return tmp;
}

String LigBase::printMol(Composite *mol)
{
	String tmp = "";
	for(AtomIterator ati = ((AtomContainer*)mol)->beginAtom(); +ati; ++ati)
	{
		tmp += ati->getElement().getSymbol();
		tmp += " " + String( ati->countBonds() );
		tmp += "\n";
	}
	return tmp;
}

String LigBase::printInlineStarMol(Composite* mol)
{
	String tmp = "";
	for(AtomIterator ati = ((AtomContainer*)mol)->beginAtom(); +ati; ++ati)
	{
		tmp += ati->getElement().getSymbol();
		if( &*ati != &*((AtomContainer*)mol)->beginAtom())
			tmp += String(ati->getBond( *((AtomContainer*)mol)->beginAtom() )->getOrder());
	}
	return tmp;
}


String LigBase::printInlineStarMol(AtmVec& mol)
{
	
	String tmp = "";
	Atom* center = *mol.begin();
	for(AVIter ati = mol.begin(); ati != mol.end(); ++ati)
	{
		tmp += (*ati)->getElement().getSymbol();
		if( *ati != center )
			tmp += String((*ati)->getBond( *center )->getOrder());
	}
	return tmp;
}

String LigBase::moleculeToSMILES(AtomContainer &ac)
{
	OpenBabel::OBMol* obmol = MolecularSimilarity::createOBMol(ac, true);
	
	OpenBabel::OBConversion conv;
	conv.SetOutFormat("can"); // canonical smiles
	String can_smiles = conv.WriteString(obmol);
	
	// remove the stupid ID that openbabel always attaches to the generated canonical smile
	can_smiles = can_smiles.substr(0,can_smiles.find_first_of('\t'));
	delete obmol;
	return can_smiles;
}

int LigBase::countBondsAndOrder(Atom &atm)
{
	int cnt = 0;
	for( Atom::BondIterator bit = atm.beginBond(); +bit; ++bit )
	{
		cnt += bit->getOrder();
	}
	return cnt;
}

int LigBase::countBondsInParent(Atom &atm, const Composite &parent)
{
	int cnt = 0;
	for( Atom::BondIterator bit = atm.beginBond(); +bit; ++bit )
	{
		if( parent.isParentOf( * bit->getBoundAtom( atm )) )
			cnt++;
	}
	return cnt;
}

const int LigBase::getAtomPosition(Atom *atm, AtomContainer *mol)
{
	AtomIterator ati = mol->beginAtom();
	for (int i = 0; +ati; ati++, i++)
	{
		if(&*ati == atm)
			return i;
	}
	return -1;
}

// Translate the AtomContainer 'fromMol' into an AtmVec 'toMol'
void LigBase::toAtmVec(AtomContainer &fromMol, AtmVec &toMol)
{
	for(AtomIterator ati = fromMol.beginAtom(); +ati; ++ati)
		toMol.push_back(&*ati);
}

void LigBase::transferMolecule(AtomContainer *toMol, AtomContainer *fromMol)
{
	int num_atm = fromMol->countAtoms();
	for(int i = 0; i < num_atm; i++)
		toMol->insert( *fromMol->beginAtom() ); // insert auto removes from its old parent
}

void LigBase::clearExternalBonds(AtomContainer *mol)
{
	Atom::BondIterator bit;
	AtomIterator ait;

	BALL_FOREACH_INTERBOND(*mol, ait, bit)
	{
		bit->destroy();
	}
}

void LigBase::removeHydrogens(AtomContainer &tmp)
{
	AtmVec to_remove;
	vector<Bond*> bnd_remove;
	for(AtomIterator ati = tmp.beginAtom(); +ati; ++ati)
	{
		if(ati->getElement().getSymbol() == "H")
		{
			// remove (all) bond(s) to the atom:
			for(AtomBondIterator bit = ati->beginBond(); +bit ;++bit)
				bnd_remove.push_back( &*bit );
			
			to_remove.push_back( &*ati );
		}
	}
	
	// remove the hydrogens:
	for(int i = 0; i<to_remove.size(); ++i)
		tmp.remove( *to_remove[i] );
	
	for(int i = 0; i<bnd_remove.size(); ++i)
		bnd_remove[i]->destroy();
}

