// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:

#include <BALL/VIEW/DIALOGS/generateCrystalDialog.h>
#include <BALL/VIEW/KERNEL/mainControl.h>

namespace BALL
{
	namespace VIEW
	{
		GenerateCrystalDialog::GenerateCrystalDialog(QWidget* parent, const char* name, bool, Qt::WFlags fl)
			: QDialog(parent, fl),
			Ui_GenerateCrystalDialogData(),
			ModularWidget(name),
			cryst_gen_(0),
			selectedSystem_(0)
		{
#ifdef BALL_VIEW_DEBUG
			Log.error() << "new GenerateCrystalDialog" << this << std::endl;
#endif
			setupUi(this);
			setObjectName(name);
			
			// register the widget with the MainControl
			ModularWidget::registerWidget(this);
			hide();
			
			//signals and slot connections
			connect( buttonBox, SIGNAL( accepted() ), this, SLOT ( slotOk() ));
			connect( buttonBox, SIGNAL( rejected() ), this, SLOT ( slotCancel() ));

		}
			
		GenerateCrystalDialog::~GenerateCrystalDialog()
		{
#ifdef BALL_VIEW_DEBUG
			Log.info() << "Destructing object " << this << " of class GenerateCrystalDialog" << std::endl; 
#endif 

		}
		
// 		void GenerateCrystalDialog::checkMenu(MainControl& mc)
// 		{
// 			std::cout << "hier geh ich kaputt" << std::endl;
// 			menu_id_->setEnabled(!mc.compositesAreLocked());
// 			std::cout << "hier bin ich kaputt" << std::endl;
// 		}

		bool GenerateCrystalDialog::initSpaceGroupList()
		{
			return false;
		}
		
		void GenerateCrystalDialog::slotOk()
		{
		}
		
		void GenerateCrystalDialog::slotCancel()
		{
		}
	} // namespace VIEW
}	// namespace BALL
