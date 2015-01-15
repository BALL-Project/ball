// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef LIGANDSTRUCTUREBASE_H
#define LIGANDSTRUCTUREBASE_H

///// BALL: File IO
//#include <BALL/FORMAT/SDFile.h>
//#include <BALL/FORMAT/lineBasedFile.h>
//#include <BALL/SYSTEM/file.h>

///// BALL: Macros
//#include <BALL/KERNEL/forEach.h>
//#include <BALL/DATATYPE/forEach.h>

/// BALL: Base Classes (String + Math)
#include <BALL/DATATYPE/string.h>
//#include <BALL/MATHS/angle.h>
//#include <BALL/MATHS/vector3.h>
//#include <BALL/MATHS/matrix44.h>

///// BALL: Atom, Bond, Element
#include <BALL/KERNEL/atom.h>
//#include <BALL/KERNEL/atomIterator.h>
//#include <BALL/KERNEL/bond.h>
//#include <BALL/KERNEL/bondIterator.h>
//#include <BALL/KERNEL/PTE.h>

/// BALL: Molecule Container
#include <BALL/CONCEPT/composite.h>
#include <BALL/KERNEL/atomContainer.h>
//#include <BALL/KERNEL/fragment.h>
//#include <BALL/KERNEL/molecule.h>

///// BALL: Algorithms
//#include <BALL/STRUCTURE/UCK.h>
//#include <BALL/STRUCTURE/molecularSimilarity.h>
//#include <BALL/STRUCTURE/structureMapper.h>
//#include <BALL/STRUCTURE/geometricTransformations.h>

///// Open Babel
//#include <openbabel/obconversion.h>
//#include <openbabel/mol.h>
//#include <openbabel/canon.h>
//#include <openbabel/graphsym.h>

/// STL
#include <vector>
//#include <limits>

///// BOOST
//#include <boost/unordered_map.hpp>
//#include <boost/pending/disjoint_sets.hpp>

///// Name Spaces
//using namespace OpenBabel;
using namespace BALL;
using namespace std;




/// Typedefs:

// Standard Data:
typedef vector<Atom*> AtmVec;
typedef vector<Atom*>::iterator AVIter;




class LigBase
{
public:
	
	// generate a mini dump of a molecule
	static String printInlineMol(Composite* mol);
	static String printInlineMol(AtmVec& mol);
	
	// get the position of an atom in the molcule list:
	static const int getAtomPosition(Atom* atm, AtomContainer *mol);
	
	// Translate the AtomContainer 'fromMol' into an AtmVec 'toMol'
	static void toAtmVec( AtomContainer& fromMol, AtmVec& toMol);


};

#endif // LIGANDSTRUCTUREBASE_H
