// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: primitiveManager.h,v 1.1.4.1 2003/02/05 15:31:34 anker Exp $

#ifndef BALL_MOLVIEW_KERNEL_PRIMITIVEMANAGER_H
#define BALL_MOLVIEW_KERNEL_PRIMITIVEMANAGER_H

#ifndef BALL_VIEW_KERNEL_MESSAGE_H
#	include <BALL/VIEW/KERNEL/message.h>
#endif

//using BALL::VIEW::SelectionMessage;

namespace BALL
{
	namespace MOLVIEW
	{
		/** PrimitiveManager
		*/
		class PrimitiveManager
		{
			public:

			/**	@name	Constructors and Destuctor
			*/	
			//@{

			/** Default Constructor
			*/
			PrimitiveManager()
				throw();

			/** Copy constructor
			*/
			PrimitiveManager(const PrimitiveManager& pm)
				throw();

			/** Destructor
			*/
			virtual ~PrimitiveManager()
				throw();

			//@}
		
			private:
		};

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_KERNEL_PRIMITIVEMANAGER_H
