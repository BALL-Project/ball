// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: atomBijection.C,v 1.1 2006/01/24 09:31:14 oliver Exp $
//
// Author:
//   Oliver Kohlbacher
//

#include <BALL/STRUCTURE/atomBijection.h>

#include <BALL/COMMON/limits.h>
#include <BALL/STRUCTURE/geometricProperties.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/DATATYPE/hashGrid.h>
#include <BALL/MATHS/quaternion.h>


#include <stack>
#include <vector>
#include <map>

using namespace std;

namespace BALL
{

	AtomBijection::AtomBijection(AtomContainer& A, AtomContainer& B)
		:	std::vector<std::pair<Atom*, Atom*> >()
	{
		assignByName(A, B);
	}

	/* Calculate the root mean squared deviation */
	double AtomBijection::calculateRMSD() const
	{
		double sum_of_squares = 0.0;
		// 
		for(Size i = 0; i < size(); i++)
		{
			Vector3& r(operator [] (i).first->getPosition());
			sum_of_squares += r.getSquareDistance(operator [] (i).second->getPosition());
		}

		// calculate mean square deviation
		sum_of_squares /= (double)size();

		// return RMSD
		return sqrt(sum_of_squares);
	}

	Size AtomBijection::assignByName(AtomContainer& A, AtomContainer& B)
	{
		// Clear old bijection.
		clear();

		// Remember the names of A and their atom pointers.
		StringHashMap<Atom*> A_names;
		for (AtomIterator ai = A.beginAtom(); +ai; ++ai)
		{
			A_names.insert(std::pair<String, Atom*>(ai->getFullName(Atom::ADD_VARIANT_EXTENSIONS_AND_ID), &*ai));
		}
		
		// Iterate over all atoms of B and try to find an 
		// atom in A identical names.
		for (AtomIterator ai = B.beginAtom(); +ai; ++ai)
		{
			if (A_names.has(ai->getFullName(Atom::ADD_VARIANT_EXTENSIONS_AND_ID)))
			{
				// We found two matching atoms. Remember them.
				push_back(AtomPair(A_names[ai->getFullName(Atom::ADD_VARIANT_EXTENSIONS_AND_ID)], &*ai));

				// Throw away the hash map entry in order to avoid
				// 1:n mappings.
				A_names.erase(ai->getFullName(Atom::ADD_VARIANT_EXTENSIONS_AND_ID));
			}
		}

		// Check whether we could map anything. 
		// If not, try to map by atom name alone.
		if (size() == 0)	
		{
			// Next stage: try to map by atom name only.
			A_names.clear();
			for (AtomIterator ai = A.beginAtom(); +ai; ++ai)
			{
				A_names.insert(std::pair<String, Atom*>(ai->getName(), &*ai));
			}
			clear();
			for (AtomIterator ai = B.beginAtom(); +ai; ++ai)
			{
				if (A_names.has(ai->getName()))
				{
					// We found two matching atoms. Remember them.
					push_back(std::pair<Atom*, Atom*>(A_names[ai->getName()], &*ai));
					// Throw away the hash map entry in order to avoid
					// 1:n mappings.
					A_names.erase(ai->getName());
				}
			}			
		}

		return size();
	}

	Size AtomBijection::assignTrivial(AtomContainer& A, AtomContainer& B)	
	{
		// Delete old bijection.
		clear();

		// Check whether we could map anything.
		if (size() == 0)	
		{
			// Last stage: map in order -- first atom of A onto
			// first atom of B and so on.
			AtomIterator ai(A.beginAtom());
			AtomIterator bi(B.beginAtom());
			for (; +ai && +bi; ++ai, ++bi)
			{
				push_back(std::pair<Atom*, Atom*>(&*ai, &*bi));
			}
		}
		return size();
	}

	Size AtomBijection::assignCAlphaAtoms(AtomContainer& A, AtomContainer& B)
	{
		// ???
	}

	Size AtomBijection::assignBackboneAtoms(AtomContainer& A, AtomContainer& B)
	{
		// ???
	}


}	// namespace BALL
