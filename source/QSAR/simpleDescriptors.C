// $Id: simpleDescriptors.C,v 1.2 2001/12/17 03:17:37 oliver Exp $

#include <BALL/QSAR/simpleDescriptors.h>
#include <BALL/KERNEL/PTE.h>


namespace BALL
{

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
