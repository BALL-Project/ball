// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: editFunctions.C,v 1.1 2005/02/07 17:46:27 anne Exp $
//

#include <BALL/VIEW/WIDGETS/editFunctions.h>

namespace BALL
{
	namespace VIEW
	{
		EditFunctions::EditFunctions(QWidget* parent, const char *name)
			throw()
			: DockWidget(parent, name),
				pte_(),
				tab_()
		{
			tab_.addTab(&pte_, "Periodic Table of Elements");
			setGuest(tab_);
			registerWidget(this);
		}	

		EditFunctions::~EditFunctions()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
			Log.error() << "Destructing object " << (void *)this 
									<< " of class EditFunctions" << endl;
			#endif 
		}

		void EditFunctions::initializeWidget(MainControl& main_control)
			throw()
		{
			DockWidget::initializeWidget(main_control);
		}

		void EditFunctions::finalizeWidget(MainControl& main_control)
			throw()
		{
			DockWidget::initializeWidget(main_control);
		}

	}		
}

