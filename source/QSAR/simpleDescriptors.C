// $Id: simpleDescriptors.C,v 1.3 2001/12/18 01:12:51 oliver Exp $

#include <BALL/QSAR/simpleDescriptors.h>
#include <BALL/KERNEL/PTE.h>


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
}
