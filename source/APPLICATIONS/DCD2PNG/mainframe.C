// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: mainframe.C,v 1.7 2004/12/13 13:38:41 amoll Exp $
//

#include "mainframe.h"

#include <qapplication.h>

#include <BALL/VIEW/KERNEL/message.h>

#include <BALL/VIEW/DIALOGS/mainControlPreferences.h>
#include <BALL/VIEW/DIALOGS/preferences.h>

namespace BALL
{
	using namespace std;
	using namespace BALL::VIEW;

	Mainframe::Mainframe(QWidget* parent, const char* name)
		:	MainControl(parent, name, ".BALLView"),
			control_(0),
			geometric_control_(0),
			display_properties_(0),
			molecular_structure_(0)
	{
		#ifdef BALL_VIEW_DEBUG
			Log.error() << "new Mainframe " << this << std::endl;
		#endif

 		Log.remove(std::cout);
   	Log.remove(std::cerr);

		control_ = new MolecularControl(this, "Structures");
		CHECK_PTR(control_);

 		geometric_control_ = new GeometricControl(this, "Representations");
 		CHECK_PTR(geometric_control_);

		scene_ = new Scene(this, "3D View");
		CHECK_PTR(scene_);
		scene_->setMinimumSize(10, 10);
		setCentralWidget(scene_);

		display_properties_ = new DisplayProperties(this, "DisplayProperties");
		CHECK_PTR(display_properties_);

		file_dialog_ = new MolecularFileDialog(this, "MolecularFileDialog");
		CHECK_PTR(file_dialog_);

 		molecular_structure_ = new MolecularStructure(this, "MolecularStructure");
 		CHECK_PTR(molecular_structure_);

		// initialize own preferences tab
		initializePreferencesTab(*preferences_dialog_);

		// initialize all modular widgets 
		List<ModularWidget*>::Iterator it = modular_widgets_.begin(); 
		for (; it != modular_widgets_.end(); ++it)
		{
			(*it)->initializeWidget(*this);
			(*it)->initializePreferencesTab(*preferences_dialog_);
		}

		// check own preferences 
		preferences_dialog_->fetchPreferences(preferences_);

		// fetch own preferences tab
		fetchPreferences(preferences_);

		// apply on own preferences tab
		applyPreferences();

		// check menu entries, fetch and apply preferences
		for (it = modular_widgets_.begin(); it != modular_widgets_.end(); ++it)
		{
			(*it)->checkMenu(*this);
			(*it)->fetchPreferences(preferences_);
			(*it)->applyPreferences();
		}
	}

	Mainframe::~Mainframe()
		throw()
	{
	}

}
