// $Id: simpleDescriptors.h,v 1.4 2001/12/18 01:18:44 oliver Exp $

#ifndef BALL_QSAR_SIMPLEDESCRIPTORS_H
#define BALL_QSAR_SIMPLEDESCRIPTORS_H

#ifndef BALL_QSAR_DESCRIPTOR_H
#	include <BALL/QSAR/descriptor.h>
#endif

namespace BALL
{

	
	/**	Descriptor returning the molecular mass of a molecule
	*/
	class MolecularMass
		:	public Descriptor
	{
		public:
		MolecularMass();
		virtual double compute(const Molecule& molecule);
	};

	/**	Descriptor returning the number of bonds of a molecule.
	*/
	class NumberOfBonds
		:	public Descriptor
	{
		public:
		NumberOfBonds();
		virtual double compute(const Molecule& molecule);
	};





} // namespace BALL

#endif // BALL_QSAR_SIMPLEDESCRIPTORS_H
