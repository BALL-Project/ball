// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: dockDialog.C,v 1.1.2.3 2005/01/06 15:22:31 leonhardt Exp $
//

#include "dockDialog.h"
#include <qpushbutton.h>
#include <qcombobox.h>
#include <qstringlist.h>
//#include <qerrormessage.h>
#include <qmessagebox.h>

#ifndef BALL_KERNEL_SYSTEM_H
# include <BALL/KERNEL/system.h>
#endif

//#ifndef BALL_LIST_H
//# include <BALL/KERNEL/system.h>
//#endif

//#ifndef BALL_COMMON_LOGSTREAM_H
//# include <BALL/COMMON/logStream.h>
//#endif

namespace BALL
{
	namespace VIEW
	{

		DockDialog::DockDialog( QWidget* parent,  const char* name)
			throw()
			: DockDialogData(parent, name),
				ModularWidget(name)
		{
		#ifdef BALL_VIEW_DEBUG
			Log.error() << "new DockDialog " << this << std::endl;
		#endif
		
			//setCaption("Docking");
		
			// register the widget with the MainControl
			ModularWidget::registerWidget(this);

			hide(); 
		}

		DockDialog::~DockDialog()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << this << " of class DockDialog" << std::endl;
			#endif 
		}

		void DockDialog::initializeWidget(MainControl& main_control)
			throw()
		{
			main_control.initPopupMenu(MainControl::DISPLAY)->setCheckable(true);
			
			id_ = main_control.insertMenuEntry(MainControl::DISPLAY, "&Docking", this,
																				 SLOT(show()), CTRL+Key_D, -1,
																				 "Start Docking");
		}
																				 																		 
		void DockDialog::finalizeWidget(MainControl& main_control)
			throw()
		{
			main_control.removeMenuEntry(MainControl::DISPLAY, "&Docking", this,
																	 SLOT(show()), CTRL+Key_D);   
		}   
		
		

		/*void DockDialog::onNotify(Message *message)
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "DockDialog " << this << " onNotify " << message << std::endl;
			#endif
			// selection => store last selection for later processing
			if (RTTI::isKindOf<CompositeMessage>(*message))
			{
				CompositeMessage* composite_message = RTTI::castTo<CompositeMessage>(*message);
				int  type = composite_message->getType();
				//is message type REMOVE_COMPOSITE or NEW_COMPOSITE
				
				Composite* composite = composite_message->getComposite();
				if(RTTI::isKindOf<System>(*composite))
				{
					//if (type == CompositeMessage::CompositeMessageType::REMOVE_COMPOSITE) 
					if(type == CompositeMessage::REMOVED_COMPOSITE)
					{ 
						systems_.remove(composite);
					}
					if(type == CompositeMessage::NEW_COMPOSITE)
					{
						systems_.append(composite);
					}
				}
				
			}
		}	*/
		


		void DockDialog::checkMenu (MainControl& main_control)
			throw()
		{
			CompositeManager& composite_manager = main_control.getCompositeManager();
			HashSet<Composite*>& composites =  composite_manager.getComposites();
			
			//iterate over composite_hash
			HashSet<Composite*>::iterator composite_it;

			int num_systems = 0;
			for(composite_it = composites.begin(); +composite_it; ++composite_it)
			{
				if(RTTI::isKindOf<System>(*(*composite_it)))
				{
					num_systems++;
				}
			}
			//no systems loaded, disable menu entry "Docking"
			menuBar()->setItemEnabled(id_, num_systems);
		}
		
		
		void DockDialog::show()
		{
			MainControl* main_control = getMainControl();

			//get the composites
			CompositeManager& composite_manager = main_control->getCompositeManager();
			
			//iterate over all composites; add systems to list
			HashSet<Composite*>::iterator composite_it = composite_manager.begin();

			//selection lists for systems should be empty
			system_list1->clear();
			system_list2->clear();
			//pointer to selected systems should be NULL
			selected_system1_ = NULL;
			selected_system2_ = NULL;
			
			QStringList current_system_list;
			
			for(; +composite_it; ++composite_it)
			{
				if(RTTI::isKindOf<System>(*(*composite_it)))
				{
					System* system = RTTI::castTo<System>(*(*composite_it));
					current_system_list.append(system->getName());
					
					//check if user has already selected two systems 
					if (system->isSelected())
					{
						if(selected_system1_ == NULL)
						{
							selected_system1_ = system;
						}
						else
						{
							if(selected_system2_ == NULL)
							{
								selected_system2_ = system;
							}
							else
							{
								//if more than 2 systems are selected => Error message!
								#ifdef BALL_VIEW_DEBUG
									Log.error() << "DockDialog " << " More than two systems selected! " << std::endl;
								#endif
					
								QMessageBox error_message(0,0);
								error_message.warning(0,"Error","More than two systems selected!", QMessageBox::Ok, QMessageBox::NoButton);
								return;
							}
						}
					}
				}
			}
			
			//put <select> in list as the first element
			current_system_list.prepend("<select>");
			
			//set selection lists of dialog
			system_list1->insertStringList(current_system_list);
			system_list2->insertStringList(current_system_list);
		
			if(selected_system1_ != NULL)
			{
				system_list1->setCurrentText(selected_system1_->getName());
				//Log.error() << "current item: " << system_list1->currentItem() << std::endl;
			}
			if(selected_system2_ != NULL)
			{
				system_list2->setCurrentText(selected_system2_->getName());
				//Log.error() << "current item1: " << system_list1->currentItem() << std::endl;
				//Log.error() << "current item2: " << system_list2->currentItem() << std::endl;
			}
			
			//show dialog to user
				DockDialogData::show();
		}
		
	} // namespace VIEW
} // namespace BALL
