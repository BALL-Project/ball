// $Id: dockProgressDialog.C,v 1.1.2.1 2005/03/17 14:03:03 haid Exp $
//

#include "dockProgressDialog.h"

namespace BALL
{
	namespace VIEW
	{
		
		//Constructor
		DockProgressDialog::DockProgressDialog(QWidget* parent,  const char* name, bool modal, WFlags fl)
			throw()
			: DockProgressDialogData(parent, name, modal, fl),
				ModularWidget(name),
				PreferencesEntry()
		{
		
		}
		
		//Destructor	
		DockProgressDialog::~DockProgressDialog()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << this << " of class DockProgressDialog" << std::endl;
			#endif 
		}
		
	}
}
