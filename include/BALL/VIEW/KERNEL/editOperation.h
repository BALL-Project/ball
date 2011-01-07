// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_KERNEL_EDITOPERATION_H
#define BALL_VIEW_KERNEL_EDITOPERATION_H

#ifndef BALL_DATATYPE_STRING_H
# include <BALL/DATATYPE/string.h>
#endif

namespace BALL
{
	class Atom;
	class Bond;

	namespace VIEW
	{
		/** This class represents a single edit operation and stores its type and
		 *  parameters. This can be used to implement undo functionality.
		 */
		class BALL_VIEW_EXPORT EditOperation
		{
			public:

				/// Constructor
				EditOperation();

				///
				EditOperation(Atom* atom, Bond* bond, String description= "Added Object", int operation=0);

				/// Destructor.
				virtual ~EditOperation();

				///
				enum OperationType
				{
					DEFAULT,
					ADDED__ATOM,
					ADDED__BOND,
					CHANGED__TYPE
						// DELETED__ATOM
						// DELETED__BOND
						//CHANGED__SYSTEM
						//MOVED__ATOM
				};

				int	operationType;
				Atom* atom;
				Bond* bond;
				String description;
		};
	}
}

#endif // BALL_VIEW_KERNEL_EDITOPERATION_H
