// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: descriptor.h,v 1.6 2003/02/25 15:53:52 sturm Exp $

#ifndef BALL_QSAR_DESCRIPTOR_H
#define BALL_QSAR_DESCRIPTOR_H

#ifndef BALL_CONCEPT_PROCESSOR_H
#	include <BALL/CONCEPT/processor.h>
#endif

#ifndef BALL_KERNEL_MOLECULE_H
#	include <BALL/KERNEL/molecule.h>
#endif

namespace BALL
{
    /** \ingroup MolecularDescriptors
     *  @{
     */
	/**	Generic QSAR molecular descriptor class.
			<b>Definition:</b>BALL/include/BALL/QSAR/descriptor.h
			 \par
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
  /** @} */
} // namespace BALL

#endif // BALL_QSAR_DESCRIPTOR_H
