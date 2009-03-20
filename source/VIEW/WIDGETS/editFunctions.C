// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: editFunctions.C,v 1.5.20.1 2007/03/25 21:56:45 oliver Exp $
//

#include <BALL/VIEW/WIDGETS/editFunctions.h>

namespace BALL
{
	namespace VIEW
	{
		EditFunctions::EditFunctions(QWidget* parent, const char *name)
			: DockWidget(parent, name),
				pte_(),
				edit_operations_(),
				tab_()
		{
			tab_.addTab(&pte_, "Periodic Table of Elements");
			tab_.addTab(&edit_operations_, "Undo");
			setGuest(tab_);
			registerWidget(this);
		  Log.error() << "Editfunctions " << dynamic_cast<ModularWidget*>(this) << std::endl;
		}	

		EditFunctions::~EditFunctions()
		{
			#ifdef BALL_VIEW_DEBUG
			Log.error() << "Destructing object " << (void *)this 
									<< " of class EditFunctions" << endl;
			#endif 
		}

		void EditFunctions::initializeWidget(MainControl& main_control)
		{
			DockWidget::initializeWidget(main_control);
		}

		void EditFunctions::finalizeWidget(MainControl& main_control)
		{
			DockWidget::initializeWidget(main_control);
		}

	}		
}

