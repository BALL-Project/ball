// $Id: molecularMessage.h,v 1.1 2000/05/14 15:29:37 hekl Exp $

#ifndef BALL_MOLVIEW_KERNEL_MOLECULARMESSAGE_H
#define BALL_MOLVIEW_KERNEL_MOLECULARMESSAGE_H

#ifndef BALL_COMMON_H
#include <BALL/common.h>
#endif

#ifndef BALL_VIEW_KERNEL_MESSAGE_H
#	include <BALL/VIEW/KERNEL/message.h>
#endif

using namespace BALL::VIEW;


namespace BALL
{

	namespace MOLVIEW
	{
		/**	New Molecular Message Class		
		*/
		class NewMolecularMessage: public CompositeMessage
		{
			public:

			/**	@name	Constructors and Destructors
			*/	
			//@{
			NewMolecularMessage();

			NewMolecularMessage(const CompositeMessage& message, bool deep = true);

			virtual ~NewMolecularMessage();
			//@}
		
			/**	@name	Assignment
			*/
			//@{
			//@}

			/**	@name	Accessors
			*/
			//@{
			//@}

			/**	@name	Debugging and Diagnostics
			*/	
			//@{
			//@}

			/**	@name	Storers
			*/
			//@{
			//@}
			
			private:
		};


		/**	Removed Molecular Message Class		
		*/
		class RemovedMolecularMessage: public RemovedCompositeMessage
		{
			public:

			/**	@name	Constructors and Destructors
			*/	
			//@{
			RemovedMolecularMessage();

			RemovedMolecularMessage(const CompositeMessage& message, bool deep = true);

			virtual ~RemovedMolecularMessage();
			//@}
		
			/**	@name	Assignment
			*/
			//@{
			//@}

			/**	@name	Accessors
			*/
			//@{
			//@}

			/**	@name	Debugging and Diagnostics
			*/	
			//@{
			//@}

			/**	@name	Storers
			*/
			//@{
			//@}
			
			private:
		};


		/**	Changed Molecular Message Class		
		*/
		class ChangedMolecularMessage: public ChangedCompositeMessage
		{
			public:

			/**	@name	Constructors and Destructors
			*/	
			//@{
			ChangedMolecularMessage();

			ChangedMolecularMessage(const CompositeMessage& message, bool deep = true);

			virtual ~ChangedMolecularMessage();
			//@}
		
			/**	@name	Assignment
			*/
			//@{
			//@}

			/**	@name	Accessors
			*/
			//@{
			//@}

			/**	@name	Debugging and Diagnostics
			*/	
			//@{
			//@}

			/**	@name	Storers
			*/
			//@{
			//@}
			
			private:
		};


#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/KERNEL/molecularMessage.iC>
#		endif
		
	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_KERNEL_MOLECULARMESSAGE_H
