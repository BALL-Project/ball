// $Id: dockResultDialog.C,v 1.1.2.2 2005/03/18 14:48:16 haid Exp $
//

#include <qtable.h>

#include "dockResultDialog.h"

namespace BALL
{
	namespace VIEW
	{
		
		//Constructor
		DockResultDialog::DockResultDialog(QWidget* parent,  const char* name, bool modal, WFlags fl)
			throw()
			: DockResultDialogData(parent, name, modal, fl),
				ModularWidget(name)
		{
			// register the widget with the MainControl
			ModularWidget::registerWidget(this);
		}
		
		//Destructor	
		DockResultDialog::~DockResultDialog()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << this << " of class DockResultDialog" << std::endl;
			#endif 
		}
		
		
		// add docked system to BALLView structures 
		void DockResultDialog::displayDockedSystem()
			throw()
		{
			SnapShot best_result = ranked_conformations_[0];
			
			docked_system_ = new System(ranked_conformations_.getSystem());
			best_result.applySnapShot(*docked_system_);
			
			Log.info() << "ResultDialog before insert" << std::endl;
			getMainControl()->insert(*docked_system_);
			//getMainControl()->update(*docked_system_, true);
		
		}

				
		// ------------------------- SLOTS ------------------------------------------------
		// --------------------------------------------------------------------------------

		/// Show and raise
		void DockResultDialog::show()
		{
			// get the conformations (= pair<Index, float> = Snapshot number & energy value) for the result dialog
			std::vector<ConformationSet::Conformation> conformations = ranked_conformations_.getScoring();
			
			//clear table
			int num_rows = result_table->numRows();
			for(int j = num_rows; j >=0; j--)
			{
				result_table->removeRow(j);
			}
			
			// fill the table of the result dialog
			result_table->insertRows(0,conformations.size());
			for(unsigned int i = 0; i < conformations.size() ; i++)
			{
				//1.column = snapshot number; 2.column = energy value 
				QString string;
				result_table->setText(i,0,string.setNum(conformations[i].first));
				result_table->setText(i,1,string.setNum(conformations[i].second));
			}
			
			//adjust column width
			for(int j = 0; j < result_table->numCols() ; j++)
			{
				result_table->adjustColumn(j);
			}
			//show dialog to user
			DockResultDialogData::show();
			
		}
		
		///
		void DockResultDialog::showSnapshot()
		{
			for(int i=0; i < result_table->numRows(); i++)
			{
				if(result_table->isRowSelected(i))
				{
					int snapshot = (result_table->text(i,0)).toInt();
					//Log.info() << "snapshot: " << snapshot << std::endl;
					SnapShot selected_conformation = ranked_conformations_[snapshot];
			
					selected_conformation.applySnapShot(*docked_system_);
					getMainControl()->update(*docked_system_, true);
					return;
				
				
					/*
					MainControl* main_control = getMainControl();

					//get the composites
					CompositeManager& composite_manager = main_control->getCompositeManager();
			
					//iterate over all composites; add systems to list
					HashSet<Composite*>::Iterator composite_it = composite_manager.begin();
					
					for(; +composite_it; ++composite_it)
					{
						System* docked_system = dynamic_cast<System*>(*composite_it);
						if()
						{
							selected_conformation.applySnapShot(*docked_system);
							return;
						}
					}
					*/
				}
			}
		}
		
	}
}
