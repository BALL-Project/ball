// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: simpleDescriptors.C,v 1.6 2003/07/01 17:09:52 oliver Exp $
//

#include <BALL/QSAR/simpleDescriptors.h>

#include <BALL/CONCEPT/composite.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/expression.h>
#include <BALL/KERNEL/fragment.h>
#include <BALL/STRUCTURE/numericalSAS.h>

namespace BALL
{

	NumberOfBonds::NumberOfBonds()
		:	Descriptor("NumberOfBonds")
	{
	}

	double NumberOfBonds::compute(const Molecule& molecule)
	{
		return (double)molecule.countBonds();
	}

	MolecularMass::MolecularMass()
		:	Descriptor("MolecularMass")
	{
	}
	
	double MolecularMass::compute(const Molecule& molecule)
	{
		double mass = 0.0;
		AtomConstIterator it = molecule.beginAtom();
		for (; +it; ++it)
		{
			if (it->getElement() == Element::UNKNOWN)
			{
				Log.warn() << "MolecularMass: unknown atomic mass for element UNKNOWN (atom: " 
									 << it->getFullName() << ")" << std::endl;
			}
			else
			{
				mass += it->getElement().getAtomicWeight();
			}
		}
		
		return mass;
	}



	PolarSurfaceArea::PolarSurfaceArea()
		:	Descriptor("PolarSurfaceArea")
	{
	}
	
	double PolarSurfaceArea::compute(const Molecule& molecule)
	{
		// polar atoms are N, O, and H connected to N/O
		Expression polar("element(O) OR element(N) OR (element(H) AND (connectedTo((N)) OR connectedTo((O))))");

		HashMap<const Atom*, float> atom_areas;
		calculateSASAtomAreas(molecule, atom_areas, 1.4, 400);
		
		double psa = 0.0;
		HashMap<const Atom*, float>::Iterator it(atom_areas.begin());
		for (; it != atom_areas.end(); ++it)
		{
			if (polar(*(it->first)))
			{
				psa += it->second;
			}
		}

		return psa;
	}
}
