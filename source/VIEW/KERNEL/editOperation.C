// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/KERNEL/editOperation.h>
namespace BALL
{
	namespace VIEW
	{
		EditOperation::EditOperation()
		{
		}

		EditOperation::EditOperation(Atom* new_atom, Bond* new_bond, String new_description, int new_operation)
			: operationType((EditOperation::OperationType)new_operation),
			  atom(new_atom),
				bond(new_bond),
				description(new_description)
		{
		}

		EditOperation::~EditOperation()
		{
#ifdef BALL_VIEW_DEBUG
			Log.info() << "Destructing object EditOperation " << this << " of class EditOperation>" << std::endl;
#endif
		}
	}
}

