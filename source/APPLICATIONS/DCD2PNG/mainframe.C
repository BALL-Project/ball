// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include "mainframe.h"

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
 		geometric_control_ = new GeometricControl(this, "Representations");
		scene_ = new Scene(this, "3D View");
		scene_->setMinimumSize(10, 10);
		setCentralWidget(scene_);
		display_properties_ = new DisplayProperties(this, "DisplayProperties");
		file_dialog_ = new MolecularFileDialog(this, "MolecularFileDialog");
 		molecular_structure_ = new MolecularStructure(this, "MolecularStructure");

		init_();
	}

	Mainframe::~Mainframe()
		throw()
	{
	}

}
