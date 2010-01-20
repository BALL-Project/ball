// -*- Mode: C++; tab-wdith: 2: -*-
// vi: set ts=2:
//
//

#ifndef BALL_QSAR_PARTIALCHARGEBASE_H
#define BALL_QSAR_PARTIALCHARGEBASE_H

#ifndef BALL_QSAR_DESCRIPTOR_H
#include <BALL/QSAR/descriptor.h>
#endif

namespace BALL
{
	/** Generic QSAR molecular partial charge descriptor class
			\\
	*/
	class BALL_EXPORT PartialChargeBase
		:	public Descriptor
	{
		public:
		/** @name Constructors and Destructors
		*/
		//@{
		/** Default constructor
		*/
		PartialChargeBase();

		/** Copy constructor
		*/
		PartialChargeBase(const PartialChargeBase& pcb);
		
		/** Named constructor
		*/
		PartialChargeBase(const String& name);

		/** Named unity constructor
		*/
		PartialChargeBase(const String& name, const String& unit);

		/** Destructor
		*/
		virtual ~PartialChargeBase();
		//@}
		
		/** @name Accessors
		*/
		//@{
		void computeAllDescriptors(AtomContainer& ac);
		
		void setDataFolder(const char* folder);
		//@}
	
		/** @name Assignment
		*/
		//@{
		/** Assignment Operator
		*/
		PartialChargeBase& operator = (const PartialChargeBase& pcb);
		//@}


		protected:

		/** @name Predicates
		*/
		
		//@{
		bool isValid_(AtomContainer& ac);
		//@}
		
		/** @name Accessors
		*/

		//@{
		void calculate_(AtomContainer& ac);
		//@}
		
		String data_folder_;
		
	};

} // namespace BALL

#endif


