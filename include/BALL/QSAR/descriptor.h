// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: descriptor.h,v 1.10 2003/07/01 18:15:02 oliver Exp $

#ifndef BALL_QSAR_DESCRIPTOR_H
#define BALL_QSAR_DESCRIPTOR_H

#ifndef BALL_CONCEPT_PROCESSOR_H
#	include <BALL/CONCEPT/processor.h>
#endif

#ifndef BALL_KERNEL_MOLECULE_H
#	include <BALL/KERNEL/molecule.h>
#endif

#ifndef BALL_KERNEL_FRAGMENT_H
#	include <BALL/KERNEL/fragment.h>
#endif

namespace BALL
{
	/**	Generic QSAR molecular descriptor class.
			
			 \par
			\ingroup MolecularDescriptors
	*/
	class Descriptor
		:	public UnaryProcessor<Molecule>
	{
		public:
		
		BALL_CREATE(Descriptor)

		/**	@name Constructors and Destructors
		*/
		//@{
		Descriptor();
			
		Descriptor(const String& name);
			
		Descriptor(const Descriptor& descriptor);

		virtual ~Descriptor();
		//@}

		/**	@name	Processor-related methods
		*/	
		//@{
		virtual Processor::Result operator () (Molecule& molecule);
		//@}

		/**	@name Accessors
		*/
		//@{
		const String& getName() const;
		void setName(const String& name);
		virtual double compute(const Molecule& molecule);
		//@}

		protected:
		String	name_;
	};
} // namespace BALL

#endif // BALL_QSAR_DESCRIPTOR_H
