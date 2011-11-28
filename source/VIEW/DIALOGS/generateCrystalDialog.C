// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:

#include <BALL/VIEW/DIALOGS/generateCrystalDialog.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/FORMAT/parameters.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/threads.h>

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
			system_(0)

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
		
		bool GenerateCrystalDialog::generate()
		{
			if (system_ == 0) system_ = getMainControl()->getSelectedSystem();
			if (system_ == 0)
			{
				Log.error() << "No system given! Aborting..." << std::endl;
				return false;
			}

			if (!lockComposites()) 
			{
				setStatusbarText(tr("Can not generate a crystal packing, since I can not lock the molecular data. Is a simulation running?"), true);
				return false;
			}
			
			bool use_mt = false;
			// currently doesnt work:
			#ifdef BALL_QT_HAS_THREADS
			if (getMainControl()->useMultithreading())
			{
				use_mt = true;

				if (thread_ == 0)
				{
					thread_ = new GenerateCrystalThread();
				}
				else
				{
					if (thread_->isRunning())
					{
						Log.error() << "Thread already running in"  << " "  << __FILE__ << "  " << __LINE__<< std::endl;
						return false;
					}
				}

				thread_->setCrystalDialog(this);
				thread_->start();

				Position pos = 3;
				QString dots;
				Position i = 0;
				while (thread_->isRunning())
				{
					setStatusbarText(tr("Generating a crystal packing ") + dots, false);
					qApp->processEvents();
					if (i > 10)
					{
						if (pos < 40) 
						{
							pos ++;
							dots +=".";
						}
						else 
						{
							pos = 3;
							dots = "...";
						}
						i = 0;
					}

					i++;
					thread_->wait(10); 
				}
			}
			
			setStatusbarText(tr("Finished crystal generation"), true);

			#endif
			if (!use_mt) generate_();
			
			system_ = 0;
			unlockComposites();
			
			return true;
		}
		
		void GenerateCrystalDialog::generate_()
		{
		}

	} // namespace VIEW
}	// namespace BALL
