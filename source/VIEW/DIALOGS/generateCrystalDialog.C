// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:

#include <BALL/VIEW/DIALOGS/generateCrystalDialog.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/FORMAT/parameters.h>
#include <BALL/SYSTEM/path.h>

#include <QComboBox>

namespace BALL
{
	namespace VIEW
	{
		GenerateCrystalDialog::GenerateCrystalDialog(QWidget* parent, const char* name, bool, Qt::WFlags fl)
			: QDialog(parent, fl),
			Ui_GenerateCrystalDialogData(),
			ModularWidget(name),
			//generator_(0),
			generator_(new CrystalGenerator()),
			//sg_list(0),
			//sg_entry(0),
			selectedSystem_(0)

		{
#ifdef BALL_VIEW_DEBUG
			Log.error() << "new GenerateCrystalDialog" << this << std::endl;
#endif
			setupUi(this);
			setObjectName(name);
			
			initSpaceGroupList();
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
		throw(Exception::FileNotFound)
		{
			if (generator_)
			{
				Path path;
				String filename = generator_->getSpaceGroupFilename();
				String filepath = path.find(filename);
				if (filepath == "")
				{
						throw Exception::FileNotFound(__FILE__, __LINE__, filename);
				}

				Parameters* param = new Parameters(filepath);
				if (!param->isValid()) return false;

				sg_list_.extractSection(*param, "SpacegroupList");	
				if (!sg_list_.isValid()) return false;
				
				delete param;
						
				space_group_combobox->clear();
				for (Position i = 0; i < sg_list_.getNumberOfKeys(); i++)
				{
					space_group_combobox->addItem(QString(sg_list_.getValue(i,0).c_str()));
				}
					
				if (space_group_combobox->count() != sg_list_.getNumberOfKeys())
				{
					space_group_combobox->clear();
				};
					
				return true;		
			}
			
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
