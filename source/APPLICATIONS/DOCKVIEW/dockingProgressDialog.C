// $Id: dockingProgressDialog.C,v 1.1.2.1 2005/02/17 12:51:25 haid Exp $
//

#include "dockingProgressDialog.h"

namespace BALL
{
	namespace VIEW
	{
		
		//Constructor
		DockingProgressDialog::DockingProgressDialog(QWidget* parent,  const char* name, bool modal, WFlags fl)
			throw()
			: DockingProgressDialogData(parent, name, modal, fl),
				ModularWidget(name),
				PreferencesEntry()
		{
		
		}
		
		//Destructor	
		DockingProgressDialog::~DockingProgressDialog()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << this << " of class DockingProgressDialog" << std::endl;
			#endif 
		}
		
	}
}