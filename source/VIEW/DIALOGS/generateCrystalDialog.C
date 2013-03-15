// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:

#include <BALL/VIEW/DIALOGS/generateCrystalDialog.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/FORMAT/parameters.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/threads.h>
#include <BALL/XRAY/crystalInfo.h>

#include <QComboBox>

namespace BALL
{
	namespace VIEW
	{
		GenerateCrystalDialog::GenerateCrystalDialog(QWidget* parent, const char* name, bool, Qt::WindowFlags fl)
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
			generate();
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
			
			
			generate_();
			
			system_ = 0;
			unlockComposites();
			
			return true;
		}
		
		void GenerateCrystalDialog::generate_()
		{
				
		
			generator_->setSystem(system_);
			
			boost::shared_ptr<CrystalInfo> ci_ptr;
			
			if (crystalinfo_checkbox->isChecked())
			{
				ci_ptr = boost::dynamic_pointer_cast<CrystalInfo>(system_->getAtomContainer(0)->getProperty("CRYSTALINFO").getSmartObject());
			}
			else
			{
				boost::shared_ptr<CrystalInfo> tmp_ptr(new CrystalInfo());
				ci_ptr = tmp_ptr;
				
				ci_ptr->setSpaceGroup(space_group_combobox->currentText().toStdString());
				
				ci_ptr->setCellEdgeLengthA(axis_x_spinbox->value());
				ci_ptr->setCellEdgeLengthB(axis_y_spinbox->value());
				ci_ptr->setCellEdgeLengthC(axis_z_spinbox->value());
				
			}		
			

			generator_->setCrystalInfo(ci_ptr);

			System* output = 0;
			if (generatePacking->isChecked())
			{
				std::list<System*> crystal = generator_->generatePacking(genPack_a_from->value(), genPack_a_to->value(),
						genPack_b_from->value(), genPack_b_to->value(),
						genPack_c_from->value(), genPack_c_to->value());

				if(crystal.size() != 0)		
				{
					std::list<System*>::iterator it_c = crystal.begin();	

					for(;it_c != crystal.end(); it_c++)
					{
						getMainControl()->insert(**it_c, (*it_c)->getName());
					}
				}
			}
			else
			{
				if (generateASU->isChecked())
				{
					output = generator_->generateAsymmetricUnit();	
				}

				if (generateUC->isChecked())
				{

					output = generator_->generateUnitCell(genUC_a->value(), genUC_b->value(), genUC_c->value());	
				}
				
				if (output)
				{
					getMainControl()->insert(*output, output->getName());
				}
			
			}

			

					
			//Log.info() << "Bis hierhin klappts !" << std::endl;
		}

	} // namespace VIEW
}	// namespace BALL
