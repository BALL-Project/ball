// $Id: simpleDescriptor.C,v 1.2 2001/12/17 02:50:03 oliver Exp $

#include <BALL/QSAR/simpleDescriptors.h>

namespace BALL
{

	MolecularMass::MolecularMass()
		:	Descriptor("MolecularMass")
	{
	}
	
	double MolecularMass::compute(const Molecule& molecule)
	{
		double mass = 0.0;
		AtomIterator it = molecule.beginAtom();
		for (; +it; ++it)
		{
			if (it->getElement() == Element::UNKNOWN)
			{
				Log.warn() << "MolecularMass: unknown atomic mass for element UNKNOWN (atom: " 
									 << it->getFullName() << ")" << std::endl;
			}
			else
			{
				mass += it->getElement().getMass();
			}
		}
		
		return mass;
	}
}
