// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/STRUCTURE/clashResolver.h>

#include <BALL/CONCEPT/composite.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/MATHS/quaternion.h>
#include <BALL/STRUCTURE/geometricTransformations.h>

using namespace std; 
using namespace BALL;

/// C l a s s   C l a s h D e t e c t o r
/// ############################################################################
ClashDetector::ClashDetector(float tolerance):
	_tolerance(tolerance)
{
	
}

ClashDetector::~ClashDetector()
{
	
}

int ClashDetector::detectPairList(std::list<std::pair<Atom *, Atom *> > &p_list)
{
	int clash_count = 0;
	
	for(std::list<std::pair<Atom *, Atom *> >::iterator it = p_list.begin();
			it != p_list.end(); ++it)
	{
		if ( doClash( *it->first, *it->second) )
			++clash_count;
	}
	return clash_count;
}

void ClashDetector::createBetweenPairList(AtomContainer &ac1, AtomContainer &ac2, list<pair<Atom *, Atom *> > &p_list)
{
	p_list.clear();
	
	for(AtomIterator ati1 = ac1.beginAtom(); +ati1; ++ati1)
	{
		for(AtomIterator ati2 = ac2.beginAtom(); +ati2; ++ati2)
		{
			// make sure that more than 2 bonds are between the atoms:
			if( atom3Away(*ati1, *ati2) )
				p_list.push_back( make_pair(&*ati1, &*ati2) );
		}
	}
}

void ClashDetector::createInnerPairList(AtomContainer &ac, list<pair<Atom *, Atom *> > &p_list)
{
	p_list.clear();
	
	for(AtomIterator ati1 = ac.beginAtom(); +ati1; ++ati1)
	{
		AtomIterator ati2 = ati1;
		for( ++ati2; +ati2; ++ati2)
		{
			if( atom3Away(*ati1, *ati2))
				p_list.push_back( make_pair(&*ati1, &*ati2) );
		}
	}
}

int ClashDetector::detectInMolecule(AtomContainer &ac)
{
	int clash_count = 0;
	// clashes wihtin the small fragment
	for(AtomIterator ati1 = ac.beginAtom(); +ati1; ++ati1)
	{
		AtomIterator ati2 = ati1;
		for( ++ati2; +ati2; ++ati2)
		{
			if( atom3Away(*ati1, *ati2))
			{
				if ( doClash( *ati1, *ati2) )
					++clash_count;
			}
		}
	}
	return clash_count;
}

int ClashDetector::detectBetweenMolecules(AtomContainer &ac1, AtomContainer &ac2)
{
	int clash_count = 0;
	for(AtomIterator ati1 = ac1.beginAtom(); +ati1; ++ati1)
	{
		for(AtomIterator ati2 = ac2.beginAtom(); +ati2; ++ati2)
		{
			// make sure that more than 2 bonds are between the atoms:
			if( atom3Away(*ati1, *ati2) )
			{
				if ( doClash( *ati1, *ati2) ){
					++clash_count;
				}
			}
		}
	}
	return clash_count;
}

bool ClashDetector::doClash(Atom &atm1, Atom &atm2)
{
	float dist = atm1.getDistance(atm2);
	float ideal = atm1.getElement().getVanDerWaalsRadius() + atm2.getElement().getVanDerWaalsRadius();
	
	if (dist + _tolerance > ideal)
		return false;
	
	return true;
}

bool ClashDetector::atom3Away(Atom &at1, Atom &at2)
{
	for(Atom::BondIterator bit = at1.beginBond(); +bit; ++bit)
	{
		if( bit->getBoundAtom(at1) == &at2 ) // single bond between at1 and at2
			return false;
		
		if( bit->getBoundAtom(at1)->getBond(at2) != 0 )  // two bonds between them
			return false;
	}
	
	return true;
}

/// C l a s s   R o t a t o r
/// ############################################################################
Rotator::Rotator()
{
	
}

Rotator::~Rotator()
{
	
}

void Rotator::rotate(const Angle& angle)
{
	//calculate rotation
	Quaternion quat;
	quat.fromAxisAngle(_axis, angle);

	Matrix4x4 rotation;
	quat.getRotationMatrix(rotation);

	rotation = rotation * _translation;

	// apply the transfromations to selected atoms:
	TransformationProcessor tfp( rotation );
	TranslationProcessor tlp( _basis );
	
	for( HashSet<BALL::Atom *>::iterator ati = _to_rotate.begin(); +ati; ++ati)
	{
		(*ati)->apply (tfp ); // translate to origin and rotate
		(*ati)->apply( tlp ); // translate back (but rotated)
	}
}

void Rotator::setAxis(Atom &atm1, Atom &atm2, Composite *parent)
{
	init(atm1, atm2);
	
	// get the atoms that are to be rotated:
	_to_rotate.clear();
	
	// no direction/parent given, do not care where you rotate
	if( parent == 0)
	{
		setAtomsToRotate(atm2, atm2, atm1);
	}
	// both atoms belong to the parent:
	else if( parent->isAncestorOf(atm1) && parent->isAncestorOf(atm2) )
	{
		HashSet< Atom* > tested_atoms;
		tested_atoms.insert( &atm1 ); // insert the blocking atom
		
		if( findRotateDirection(tested_atoms, atm2, parent) )
		{
			setAtomsToRotate(atm2, atm2, atm1);
		}
		else
		{
			setAtomsToRotate(atm1, atm1, atm2);
		}
	}
	// one atom does not belong to the parent
	else
	{
		if( parent->isAncestorOf(atm1) )
		{
			setAtomsToRotate(atm1, atm1, atm2);
		}
		else if (parent->isAncestorOf(atm2) )
		{
			setAtomsToRotate(atm2, atm2, atm1);
		}
		else
		{
			cout<<"SEVERE ERROR - clashBase - Rotator::setAxis - l.166"<<endl;
			exit(EXIT_FAILURE);
		}
	}
}

void Rotator::init(Atom &atm1, Atom &atm2)
{
	// setup the axis and translation:
	Vector3& coord1 = atm1.getPosition();
	Vector3& coord2 = atm2.getPosition();
	
	_basis = coord2;
	
	_translation.set(1, 0, 0, -_basis.x, 0, 1, 0, -_basis.y, 0, 0, 1, -_basis.z, 
									 0, 0, 0, 1);
	
	_axis = _basis - coord1;
}

void Rotator::setAtomsToRotate(Atom &start, Atom &probe, Atom &block)
{
	_to_rotate.insert( &probe );

	// get all partner atoms of probe
	for (AtomBondConstIterator it = probe.beginBond(); +it ; it++)
	{
		Atom* partner = it->getPartner(probe);

		if (partner == &block) // except for the first atom this should not happen
		{
			if (&probe == &start) 
				continue;
			else
			{
				throw Exception::RotationAxisInRing("clashBase.C", 243, probe);
			}
		}

		// recurse if the partner still needs to be explored
		if ( ! _to_rotate.has( partner) )
			setAtomsToRotate(start, *partner, block);
	}
}

bool Rotator::findRotateDirection(BALL::HashSet<Atom *>& tested, 
																	Atom &direction, Composite *parent)
{
	bool is_child = true;
	
	// check if the current 'direction' is actually a child of 'parent'
	if( parent->isAncestorOf(direction) )
	{
		// get all partner atoms of 'direction'
		for (AtomBondConstIterator it = direction.beginBond(); +it ; it++)
		{
			Atom* partner = it->getPartner(direction);
			
			// make sure to only check atoms that are not 'block'
			if ( tested.has(partner) )
				continue;
			
			tested.insert(partner);
			
			if ( !findRotateDirection(tested, *partner, parent) )
			{
				is_child = false;
				break;
			}
		}
	}
	else
	{
		is_child = false;
	}

	return is_child;
}


/// C l a s s   C l a s h R e s o l v e r
/// ############################################################################
ClashResolver::ClashResolver(float tolerance, int max_rotors): 
	ClashDetector(tolerance),
	_max_rotations(max_rotors)
{
	_all_rotors = 0;
	
	_molecule = 0;
}

ClashResolver::~ClashResolver()
{}

void ClashResolver::setMolecule(AtomContainer& molecule, ConnectList& rotors)
{
	_all_rotors = &rotors;
	
	_molecule = &molecule;
	
	_max_rotations = rotors.size() * 3;
}

int ClashResolver::detect()
{
	return detectInMolecule( *_molecule );
}

int ClashResolver::resolve()
{
	pair< Atom*, Atom* >* last_rotor = 0;
	pair< Atom*, Atom* >* best_rotor = 0;
	
	TemplateCoord best_conf( *_molecule );
	
	int best_cnt = detectInMolecule( *_molecule );
	int current_cnt = 0;
	
	// try optimise bonds, use two times the size of '_all_rotors' optimization steps
	for(int tries=0; tries < 2*_all_rotors->size(); tries++)
	{
		best_rotor = 0;
		
		// for each given rotor, test 36 angles and find the one best rotation 
		// among these
		for( ConnectList::iterator it = _all_rotors->begin();
				 it!= _all_rotors->end(); ++it)
		{
			// do not optimize the same rotor in two consecutive steps
			if( &*it == last_rotor )
				continue;
			
			// use the currently known best conformation:
			best_conf.applyCoordinates2Molecule( *_molecule );
			
			Rotator roto;
			roto.setAxis( *it->first, *it->second );
			
			// test 36 rotations for the selected rotor:
			for( int i = 1; i < 36; ++i)
			{
				roto.rotate( Angle(10.0, false) ); // perfrom a rotation on '_molecule'
				
				current_cnt = detectInMolecule( *_molecule ); // get score for the resulting conformation
				
				if ( current_cnt < best_cnt )
				{
					best_cnt = current_cnt;
					best_conf.readCoordinatesFromMolecule( *_molecule );
					best_rotor = &*it;
				}
				
				if( current_cnt == 0)
					return 0;
			}
		}
		
		// set to the best found conformation:
		best_conf.applyCoordinates2Molecule( *_molecule );
		
		if( best_rotor != 0)
			last_rotor = best_rotor;
		
		// last round was not able to find any better rotation and did not return
		// because of 0 clashes. Return early, we will not be able to find any 
		// better rotation in further optimisation trials
		else
		{
			return best_cnt;
		}
	}
	
	return best_cnt;
}

int ClashResolver::resolveAllCombinations( const int &steps)
{
	// calc the rotation angle to use, depending on the steps:
	Angle rot_angle = Angle( 360.0 / (float)steps, false );
	
	return allCombinationsRecur( _all_rotors->begin(), _all_rotors->end(), rot_angle, steps);
}

int ClashResolver::allCombinationsRecur(const ConnectList::iterator &p, 
																	 const ConnectList::iterator& p_end, 
																	 Angle& angle, const int &steps)
{
	int current_cnt = numeric_limits<int>::max();
	
	Rotator roto;
	roto.setAxis(* p->first, * p->second);
	
	for( int i = 0; i < steps-1; ++i )
	{
		roto.rotate( angle );
		
		current_cnt = detect();
		
		// this rotation solved ALL clashes
		if( current_cnt == 0)
		{
			return 0;
		}
		// If the rotation did not work, try rotating the other bonds, if some
		// a not rotated yet
		else
		{
			ConnectList::iterator next_p = p; ++next_p;  
			if( next_p != p_end )
			{
				current_cnt = allCombinationsRecur( next_p, p_end, angle, steps);
				
				// some other rotation based on this rotation solved ALL clashes
				if ( current_cnt == 0)
				{
					return 0;
				}
				// the other bonds could not create a solution with this rotation
				else
				{
					continue; // try next rotation of this bond
				}
			}
		}
		
	}
	
	return current_cnt;
}

/// C l a s s   C o n n e c t i o n R e s o l v e r
/// ############################################################################

int ConnectionResolver::num_resolve_calls = 0; //#DEBUG

ConnectionResolver::ConnectionResolver(float tolerance, int max_rotors): 
	ClashDetector(tolerance),
	_max_rotations(max_rotors),
	atm_large(0), 
	atm_small(0), 
	_large_root(0),
	_small_root(0),
	_save_large(0),
	_save_small(0),
	_large_rotors( new ConnectList() ),
	_small_rotors( new ConnectList() ),
	_all_rotors( new ConnectList() )
{}


ConnectionResolver::~ConnectionResolver()
{
	delete _large_rotors; _large_rotors = 0;
	delete _small_rotors; _small_rotors = 0;
	delete _all_rotors; _all_rotors = 0;
	
	delete _save_large; _save_large = 0;
	delete _save_small; _save_small = 0;
}

void ConnectionResolver::setMolecule(
		Atom &atm1, Atom &atm2, ConnectList &connection_rotors)
{
	if(_save_large != 0)
	{
		delete _save_large;
		delete _save_small;
	}

	atm_large = &atm1;
	atm_small = &atm2;
	
	_large_root = (AtomContainer* )&atm1.getRoot();
	_small_root = (AtomContainer* )&atm2.getRoot();
	
	// Allocate the best-conformation-containers:
	_save_large = new TemplateCoord( _large_root->countAtoms() );
	_save_small = new TemplateCoord( _small_root->countAtoms() );
	
	if(_small_root == _large_root)
	{
		cout<<"SEVERE ERROR: ConnectionClashResolver got two identical roots"<<endl;
		exit(EXIT_FAILURE);
	}
	
	// Initialize rotorlists:
	_large_rotors->clear();
	_small_rotors->clear();
	_all_rotors->clear();
	
	for (ConnectList::iterator p = connection_rotors.begin(); p!=connection_rotors.end(); ++p)
	{
		if( &p->first->getRoot() == _large_root && &p->second->getRoot() == _large_root)
		{
			_large_rotors->push_back( *p );
			_all_rotors->push_back( *p );
		}
		else if( &p->first->getRoot() == _small_root && &p->second->getRoot() == _small_root)
		{
			_small_rotors->push_back( *p );
			_all_rotors->push_back( *p );
		}
	}
	
	_small_rotors->push_back( make_pair(&atm1,&atm2) );
	_large_rotors->push_back( make_pair(&atm1,&atm2) );
	_all_rotors->push_back( make_pair(&atm1,&atm2) );
	
	_max_rotations = _all_rotors->size() * 2;
	
	createBetweenPairList(*_large_root, *_small_root, _clash_inter);
	_clash_large.clear();
	_clash_small.clear();
}

int ConnectionResolver::detect()
{
	return detectPairList(_clash_inter);
}

pair<int, bool> ConnectionResolver::resolve()
{
	//0) only create the clash lists for the two molecules if we need to resolve
	//   clashes
	if(_clash_large.size() == 0)
	{
		createInnerPairList(*_large_root, _clash_large);
		createInnerPairList(*_small_root, _clash_small);
	}
	
	ConnectionResolver::num_resolve_calls++;
	
	_save_large->readCoordinatesFromMolecule( *_large_root );
	_save_small->readCoordinatesFromMolecule( *_small_root );

	const int given_large_cnt = detectPairList( _clash_large);
	const int given_small_cnt = detectPairList( _clash_small);	
	
	bool changed_large = false;
	
	//1) CONNECTION rotation might solve the clashes
	int clash_cnt = resolveConnection();
	
	//2) SMALL FRAGMENT rotations:
	if( clash_cnt != 0 && _small_rotors->size() > 1 )
	{
		clash_cnt = resolveFragment( *_small_root, *_small_rotors, given_small_cnt);
	}
	
	//3) LARGE FRAGMENT rotations:
	if( clash_cnt != 0 && _large_rotors->size() > 1 )
	{
		clash_cnt = resolveLarge( given_large_cnt + given_small_cnt );
		changed_large = true;
	}

	return make_pair( detectAll(), changed_large );
}

int ConnectionResolver::resolveConnection()
{
	int current_count = 0;
	
	int best_cnt = detectPairList(_clash_inter);
	
	Rotator roto;
	roto.setAxis(*atm_large, *atm_small, _small_root);
	
	// test 36 angles/rotations along the connecting bond, select the best one
	for( int i = 1; i < 36; ++i)
	{
		roto.rotate( Angle(10.0, false) );
		
		current_count = detectPairList(_clash_inter);
		
		if ( current_count < best_cnt)
		{
			best_cnt = current_count;
			_save_small->readCoordinatesFromMolecule( *_small_root );
		}

		// we could find a clash-free solution
		if( current_count == 0)
		{
			return 0;
		}
	}
	// we could not find a clash-free solution, but we use the best conformation
	// that was visited:
	_save_small->applyCoordinates2Molecule( *_small_root );

	return best_cnt;
}

int ConnectionResolver::resolveFragment(AtomContainer& frag,
																				ConnectList&   clist,
																				const int&     given_clashes)
{
	int best_cnt = -1;
	if(&frag == _small_root)
	{
		best_cnt = detectPairList(_clash_inter) + detectPairList(_clash_small) - given_clashes;
	}
	else
	{
		best_cnt = detectPairList(_clash_inter) + detectPairList(_clash_large) - given_clashes;
	}
	
	int current_count = 0;
	
	for(int tries=0; tries < _max_rotations; tries++) // try optimise _max_rotations bonds
	{
		for(ConnectList::iterator p = clist.begin(); p != clist.end(); ++p)
		{
			_save_large->applyCoordinates2Molecule( *_large_root );
			_save_small->applyCoordinates2Molecule( *_small_root );
			
			// else: test 20 rotations for that rotor
			Rotator roto;
			roto.setAxis(*p->first, *p->second, &frag.getRoot());
			
			for( int i = 1; i < 20; ++i)
			{
				roto.rotate( Angle(18.0, false) );
				
				// refresh current count:
				if(&frag == _small_root)
				{
					current_count = detectPairList(_clash_inter) + detectPairList(_clash_small) - given_clashes;
				}
				else
				{
					current_count = detectPairList(_clash_inter) + detectPairList(_clash_large) - given_clashes;
				}
				
				// update best structure
				if ( current_count < best_cnt )
				{
					best_cnt = current_count;
					
					_save_large->readCoordinatesFromMolecule( *_large_root );
					_save_small->readCoordinatesFromMolecule( *_small_root );
				}
				
				if( current_count == 0)
				{
					return 0;
				}
			}
		}
		
		_save_large->applyCoordinates2Molecule( *_large_root );
		_save_small->applyCoordinates2Molecule( *_small_root );
	}
	return best_cnt;
}

int ConnectionResolver::resolveLarge( const int &given_clashes)
{
	int best_cnt = detectAll() - given_clashes;
	int current_count = 0;
	
	for(int tries=0; tries < _max_rotations; tries++) // try optimise _max_rotations bonds
	{
		
		for(ConnectList::iterator p = _all_rotors->begin(); p != _all_rotors->end(); ++p)
		{
			_save_large->applyCoordinates2Molecule( *_large_root );
			_save_small->applyCoordinates2Molecule( *_small_root );
			
			// else: test 20 rotations for that rotor
			Rotator roto;
			roto.setAxis(*p->first, *p->second);
						
			for( int i = 1; i < 20; ++i)
			{
				roto.rotate( Angle(18.0, false) );
				
				current_count = detectAll() - given_clashes;
				
				if ( current_count < best_cnt )
				{
					best_cnt = current_count;
					
					_save_large->readCoordinatesFromMolecule( *_large_root );
					_save_small->readCoordinatesFromMolecule( *_small_root );
				}
				
				if( current_count == 0)
				{
					return 0;
				}
			}
		}
		
		_save_large->applyCoordinates2Molecule( *_large_root );
		_save_small->applyCoordinates2Molecule( *_small_root );
	}
	return best_cnt;
}


/* 
 * resolve           A L L
 */
int ConnectionResolver::resolveAllCombinations( const int &steps)
{
	// calc the rotation angle to use, depending on the steps:
	Angle rot_angle = Angle( 360.0 / (float)steps, false );
	
	// set best clash cnt to current status
	int best_cnt = detectAll();
	
	// recursivly check all possible combinations
	int result = allCombinationsRecur( _all_rotors->begin(), _all_rotors->end(), rot_angle, steps, best_cnt);
	
	// if we solved all clashes: cool! else use the best we could find.
	if( result == 0)
	{
		return 0;
	}
	else
	{
		_save_large->applyCoordinates2Molecule( *_large_root );
		_save_small->applyCoordinates2Molecule( *_small_root );
		
		return best_cnt;
	}
}


/*
 *  resolve           A L L (recursion)
 */
int ConnectionResolver::allCombinationsRecur(const ConnectList::iterator &p, 
																	 const ConnectList::iterator& p_end, 
																	 Angle& angle, const int &steps, int &best_cnt )
{
	int current_cnt = numeric_limits<int>::max();
	
	Rotator roto;
	roto.setAxis(* p->first, * p->second);
	
	for( int i = 0; i < steps; ++i )
	{
		// test a rotation:
		if( i != 0)
			roto.rotate( angle );
		
		current_cnt = detectAll();
		
		// check if current solution is better than the previous:
		if( current_cnt < best_cnt)
		{
			best_cnt = current_cnt;
			_save_large->readCoordinatesFromMolecule( *_large_root );
			_save_small->readCoordinatesFromMolecule( *_small_root );
		}
		
		// this rotation solved ALL clashes
		if( current_cnt == 0)
		{
			return 0;
		}
		
		// If the rotation did not work, try rotating the other bonds, if some
		// are not rotated yet
		else
		{
			// Did I reach the end yet ?
			ConnectList::iterator next_p = p; ++next_p;  
			if( next_p != p_end )
			{
				current_cnt = allCombinationsRecur( next_p, p_end, angle, steps, best_cnt);
				
				// some other rotation based on this rotation solved ALL clashes
				if ( current_cnt == 0)
				{
					return 0;
				}
			}
			//else: (implizit END OF RECURSION)
		}
	}
	
	return best_cnt;
}

int ConnectionResolver::detectAll()
{
	return detectPairList(_clash_inter)
					+ detectPairList(_clash_small)
					+ detectPairList(_clash_large);
}


