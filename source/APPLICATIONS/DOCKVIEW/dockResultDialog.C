// $Id: dockResultDialog.C,v 1.1.2.1 2005/03/17 13:39:33 leonhardt Exp $
//

#include "dockResultDialog.h"

namespace BALL
{
	namespace VIEW
	{
		
		//Constructor
		DockResultDialog::DockResultDialog(QWidget* parent,  const char* name, bool modal, WFlags fl)
			throw()
			: DockResultDialogData(parent, name, modal, fl),
				ModularWidget(name),
				PreferencesEntry()
		{
		
		}
		
		//Destructor	
		DockResultDialog::~DockResultDialog()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << this << " of class DockResultDialog" << std::endl;
			#endif 
		}
		
		
		// ------------------------- SLOTS ------------------------------------------------
		// --------------------------------------------------------------------------------

		///
		void DockResultDialog::showSnapshot()
		{
		
		}
		
	}
}