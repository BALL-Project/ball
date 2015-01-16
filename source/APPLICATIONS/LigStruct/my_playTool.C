// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
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


#include <vector>
#include <set>
#include <BALL/QSAR/ringPerceptionProcessor.h>

using namespace OpenBabel;
using namespace BALL;
using namespace std;

bool hasTrippleBond(Atom &atm)
{
	for(Atom::BondIterator bit = atm.beginBond(); +bit; ++bit)
	{
		if( bit->getOrder() == 3)
			return true;
	}
	
	return false;
}



bool isRigidAtom(Atom &atm)
{
	// if the atom is in a ring it is rigid
	if( atm.hasProperty("InRing") )// TODO: perhaps also test if this prop is true?
		 return true; 
	
	// if any neighboring bond is non single bond, the atom is rigid
	for(Atom::BondIterator bit = atm.beginBond(); +bit ; ++bit)
	{
		if( bit->getOrder() > 1 )
			return true;
	}
	
	// if the atom has only one connection that leads to a rigid atom, it self is considered rigid
	if( atm.countBonds() == 1 )
	{
		if ( isRigidAtom( * atm.beginBond()->getBoundAtom( atm )) ) //TODO: this recursion is not pretty/efficient
			return true;
		else
			return false;
	}
	
	return false;
}


bool isRotableBond(Bond &bnd)
{
	// ring bonds or not single bonds: not rotable
	if( bnd.getOrder() > 1 || bnd.hasProperty("InRing"))
		return false;
	
	// if any of the binding partners is a terminal atom: not rotable
	if( bnd.getFirstAtom()->countBonds() ==1 || bnd.getSecondAtom()->countBonds() ==1)
		return false;

//	// The following 3uncommented 3 lines DO NOT correctly cover all cases:
//	// considering a bond that comes from a tripple bond and goes to a rigid fragment 
//	// as rigid is often correct BUT: the assignment is incorrect if the tripple bond
//	// is surrounded by rigid groups.
//	// BETTER: consider bonds before and after a tripple bond as one single rotable
//	// bond. Rotating the pre- and aft-bond individually would not make any sense.
//	// Not sure if I will implement this improved version.
	
//	if( hasTrippleBond( *bnd.getFirstAtom() ) || hasTrippleBond( *bnd.getSecondAtom() ))
//		if( isRigidAtom(*bnd.getFirstAtom()) && isRigidAtom(*bnd.getSecondAtom()))
//			return false;
	
	return true;
}


bool isBridgingBond(Bond &bnd)
{
	if( !isRotableBond(bnd) )
		return false;
	
	bool atm1_fixed = isRigidAtom(* bnd.getFirstAtom() );
	bool atm2_fixed = isRigidAtom(* bnd.getSecondAtom() );
	
	// rotable-bond between two fixed atoms: is a bridge
	if( atm1_fixed && atm2_fixed)
		return true;
	
	// rotable-bond between a fixed and flexible atom: is a bridge
	if( (atm1_fixed && !atm2_fixed) ||
			(!atm1_fixed && atm2_fixed) )
		return true;
	
	// rotable-bond between two flexibles: NOT a bridge
	return false;
}

/// ################# M A I N #################
int main(int argc, char* argv[])
{
	CommandlineParser parpars("queryFragments", " generate query fragments and connections", 0.1, String(__DATE__), "Preparation");
	parpars.registerParameter("i", "input SDF", INFILE, true);
//	parpars.registerParameter("o", "output SDF", OUTFILE, true);
	
//	parpars.registerParameter("c", "location of conf file", INFILE, false);
//	parpars.setSupportedFormats("c","conf");
	
	parpars.setSupportedFormats("i","sdf");
//	parpars.setSupportedFormats("o","sdf");
//	parpars.setOutputFormatSource("o","i");

	String manual = "...just playing...";
	parpars.setToolManual(manual);

	parpars.parse(argc, argv);
	

/// F R A G M E N T I N G
	Log << "Reading molecule, searching ring atoms..."<<endl;
	SDFile in_file(parpars.get("i"), ios::in);
	Molecule* ball_mol = in_file.read();

	RingPerceptionProcessor rpp;
	vector< vector < Atom* > > sssr;
	rpp.calculateSSSR(sssr, *ball_mol);
	Log << "......done!"<<endl<<endl;
	
	for(AtomIterator ati = ball_mol->beginAtom(); +ati; ++ati)
	{
		if( ati->hasProperty("InRing") )
		{
			cout<<ati->getElement().getSymbol()<< " ring atom - rigid"<<endl;
		}
		else
		{
			cout<<ati->getElement().getSymbol()<< " not in ring - ";
			if( isRigidAtom(*ati) )
				cout<<"rigid"<<endl;
			else
				cout<<"flexi"<<endl;
		}
	}
	
	cout<<endl;
	AtomIterator ati;
	Atom::BondIterator bit;
	BALL_FOREACH_BOND(*ball_mol, ati, bit)
	{
		Atom* atm1 = bit->getFirstAtom();
		Atom* atm2 = bit->getSecondAtom();
		cout<<"Bond: "<<atm1->getElement().getSymbol()<<"-"<<atm2->getElement().getSymbol()<<" ";
		
		if( isBridgingBond(*bit) )
			cout<<" - IS BRIDGE - ";
		
		if( isRotableBond(*bit) )
			cout<<"is rotable"<<endl;
		else
			cout<<"is fixed"<<endl;
	}

	Log << "......done!"<<endl;
}

