// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: simpleDescriptors.h,v 1.8 2003/03/03 14:18:15 anhi Exp $

#ifndef BALL_QSAR_SIMPLEDESCRIPTORS_H
#define BALL_QSAR_SIMPLEDESCRIPTORS_H

#ifndef BALL_QSAR_DESCRIPTOR_H
#	include <BALL/QSAR/descriptor.h>
#endif

namespace BALL
{

    /**  \addtogroup  MolecularDescriptors
     *  @{
     */	
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

	/** Descriptor computing the polar surface area of a molecule.
			The polar surface area computation is based on the numerical 
			solvent-accessible surface computation implemented in BALL 
			(\Ref{NumericalSAS). It is the sum of all surface elements
			that are either caused by a nitrogen, an oxygen, or a hydrogen
			bound to them. Value is in $\AA^2$.
	*/
	class PolarSurfaceArea
		:	public Descriptor
	{
		public:
		PolarSurfaceArea();
		virtual double compute(const Molecule& molecule);
	};
  /** @} */
} // namespace BALL

#endif // BALL_QSAR_SIMPLEDESCRIPTORS_H
