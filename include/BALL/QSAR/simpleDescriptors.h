// $Id: simpleDescriptors.h,v 1.3 2001/12/17 03:17:52 oliver Exp $

#ifndef BALL_QSAR_SIMPLEDESCRIPTORS_H
#define BALL_QSAR_SIMPLEDESCRIPTORS_H

#ifndef BALL_QSAR_DESCRIPTOR_H
#	include <BALL/QSAR/descriptor.h>
#endif

namespace BALL
{

	
	/**	
	*/
	class MolecularMass
		:	public Descriptor
	{
		public:
		MolecularMass();
		virtual double compute(const Molecule& molecule);
	};



} // namespace BALL

#endif // BALL_QSAR_SIMPLEDESCRIPTORS_H
