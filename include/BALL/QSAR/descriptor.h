// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// 

#ifndef BALL_QSAR_DESCRIPTOR_H
#define BALL_QSAR_DESCRIPTOR_H

#ifndef BALL_CONCEPT_PROCESSOR_H
#	include <BALL/CONCEPT/processor.h>
#endif

#ifndef BALL_KERNEL_ATOMCONTAINER_H
	#include <BALL/KERNEL/atomContainer.h>
#endif

namespace BALL
{

	template <typename Key, typename Value> class HashMap;

	/**	Generic QSAR molecular descriptor class.
			\\
	*/
	class BALL_EXPORT Descriptor
		:	public UnaryProcessor<AtomContainer>
	{
		public:
		
		BALL_CREATE(Descriptor)

		/**	@name Constructors and Destructors
		*/
		//@{
		/** Default constructor
		*/
		Descriptor();
		
		/** Copy constructor
		*/
		Descriptor(const Descriptor& descriptor);

		/** Named constructor
		*/
		Descriptor(const String& name);
		
		/** Named unit constructor
		*/
		Descriptor(const String& name, const String& unit);
		
		/** Destructor
		*/
		virtual ~Descriptor();
		//@}

		/** @name Assignment
		*/
		//@{
		/** Assignment operator
		*/
		Descriptor& operator = (const Descriptor& descriptor);
		//@}

		/**	@name	Processor-related methods
		*/	
		//@{
		Processor::Result operator () (AtomContainer& ac);
		//@}

		/**	@name Accessors
		*/
		//@{
		/*_ This getter resturns the name of the descriptor as
				a string reference
		*/
		const String& getName() const;

		/*_ This setter sets the name of a descriptor
				@param name as string
		*/
		void setName(const String& name);		

		/*_ Setter, to set the unit of a descriptor
				@param unit as string
		*/
		void setUnit(const String& name);

		/*_ Getter, returns the unit of the descriptor
				as a string reference
		*/
		const String& getUnit() const;

		/*_ Generic compute method
		*/
		virtual double compute(AtomContainer& ac);
		//@}
		
		
		protected:
		
		/*_ Generic calculate function
		*/
		virtual void calculate_(AtomContainer& ac);

		/** @name Predicates
		*/
		//@{
		/*_ Predicate which returns true if the descriptor
				data is calculated and valid.			
		*/
		virtual bool isValid_(AtomContainer& ac);
		//@}
		

		private:

		//static HashMap<Handle, PreciseTime> mod_times_;

		/*_ String which saves the name of the descriptor
		*/
		String	name_;
		
		/*_ String which saves the unit of the values
				from the descriptor
		*/
		String  unit_;
	};
} // namespace BALL

#endif // BALL_QSAR_DESCRIPTOR_H
 
