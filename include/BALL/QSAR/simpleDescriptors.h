// $Id: simpleDescriptors.h,v 1.2 2001/12/17 03:00:12 oliver Exp $

#ifndef BALL_QSAR_SIMPLEDESCRIPTORS_H
#define BALL_QSAR_SIMPLEDESCRIPTORS_H

#ifndef BALL_QSAR_SIMPLEDESCRIPTORS_H
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
		virtual compute(const Molecule& molecule);
	};



} // namespace BALL

#endif // BALL_QSAR_SIMPLEDESCRIPTORS_H
