// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: modularWidget.C,v 1.4 2003/09/11 16:51:06 amoll Exp $
//

#define BALL_VIEW_DEBUG // ????

#include <BALL/VIEW/KERNEL/modularWidget.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/FORMAT/INIFile.h>
#include <BALL/VIEW/DIALOGS/preferences.h>
#include <qmenubar.h>

using namespace std;

namespace BALL
{
  namespace VIEW
	{

		ModularWidget::ModularWidget(const char* name)
			throw()
			: Embeddable(),
				ConnectionObject(),
				window_menu_entry_id_(-1),
				show_window_enty_(false),
				default_visible_(true)
		{
			if (name != 0)
			{
				setIdentifier(name);
			}
		}
		
		ModularWidget::ModularWidget(const ModularWidget& widget)
			throw()
			: Embeddable(widget),
				ConnectionObject(widget)
		{
		}
		
		ModularWidget::~ModularWidget()
			throw()
		{
      #ifdef BALL_VIEW_DEBUG
			  Log.info() << "Destructing object " << (void *)this 
									 << " of class " << RTTI::getName<ModularWidget>() << endl;
      #endif 
		}

		void ModularWidget::clear()
			throw()
		{
		}

		void ModularWidget::registerWidget(ModularWidget* mwidget)
			throw()
		{
      #ifdef BALL_VIEW_DEBUG
  			Log.info() << "registering ModularWidget at " << mwidget << endl;
			#endif

			if (!mwidget) throw(Exception::NullPointer(__FILE__, __LINE__));

			if (!RTTI::isKindOf<QObject>(*mwidget)) 
			{
				Log.error() << "ModularWidget::ModularWidget: widget is not " 
										<< "in a MainControl object!" << endl;
				return;
			}

			QObject* object = dynamic_cast<QObject*>(mwidget);
			if (object != 0)
			{
				MainControl* mc = MainControl::getMainControl(object);
				if (mc != 0)
				{
					mc->addModularWidget(mwidget);

          #ifdef BALL_VIEW_DEBUG
						Log.info() << "ModularWidget::registered: " << mwidget << endl;
					#endif
				}
				else 
				{
					Log.error() << "ModularWidget::ModularWidget: widget is not in a MainControl object!" << endl;
				}
			}
		}

		void ModularWidget::initializeWidget(MainControl& /* main_control */)
		//	throw()
		{
		}
		
		void ModularWidget::finalizeWidget(MainControl& /* main_control */)
		//	throw()
		{
		}
		
		void ModularWidget::checkMenu(MainControl& /* main_control */)
			throw()
		{
		}
		
		void ModularWidget::initializePreferencesTab(Preferences & /* preferences */)
			throw()
		{
		}
		
		void ModularWidget::finalizePreferencesTab(Preferences & /* preferences */)
			throw()
		{
		}
		
		void ModularWidget::applyPreferences(Preferences & /* preferences */)
			throw()
		{
		}
		
		void ModularWidget::fetchPreferences(INIFile& inifile)
			throw()
		{
			QWidget* widget= dynamic_cast<QWidget*>(this);
			if (!widget) return;

			if (inifile.hasEntry("WINDOWS", getIdentifier() + "::on"))
			{
				if (inifile.getValue("WINDOWS", getIdentifier() + "::on").toUnsignedInt() == 0)
				{
					switchShowWidget();
				}
			}
			else
			{
				if (default_visible_) switchShowWidget();
			}

			if (inifile.hasEntry("WINDOWS", getIdentifier() + "::x"))
			{
				widget->setGeometry(inifile.getValue("WINDOWS", getIdentifier() + "::x").toUnsignedInt(),
													  inifile.getValue("WINDOWS", getIdentifier() + "::y").toUnsignedInt(),
													  inifile.getValue("WINDOWS", getIdentifier() + "::width").toUnsignedInt(),
													  inifile.getValue("WINDOWS", getIdentifier() + "::height").toUnsignedInt());
			} 
		}
		
		void ModularWidget::writePreferences(INIFile& inifile)
			throw()
		{
			QWidget* widget= dynamic_cast<QWidget*>(this);
			if (!widget) return;

			if (window_menu_entry_id_ != -1)
			{
				inifile.insertValue("WINDOWS", getIdentifier() + "::on", 
					String(getMainControl()->menuBar()->isItemChecked(window_menu_entry_id_)));
			}

			inifile.insertValue("WINDOWS", getIdentifier() + "::x", String(widget->x()));
			inifile.insertValue("WINDOWS", getIdentifier() + "::y", String(widget->y()));
			inifile.insertValue("WINDOWS", getIdentifier() + "::width", String(widget->width()));
			inifile.insertValue("WINDOWS", getIdentifier() + "::height", String(widget->height()));
		}

		void ModularWidget::setStatusbarText(String text)
			throw()
		{
			getMainControl()->setStatusbarText(text);
		}

		MainControl* ModularWidget::getMainControl() const
			throw()
		{ 
			return (MainControl*) ((ConnectionObject*)this)->getRoot();
		}

		FragmentDB& ModularWidget::getFragmentDB() const
			throw()
		{
			return *((FragmentDB*)&getMainControl()->getFragmentDB());
		}

		void ModularWidget::switchShowWidget()
			throw()
		{
			QWidget* widget= dynamic_cast<QWidget*>(this);
			if (!widget || 
					window_menu_entry_id_ == -1) 
			{
				return;
			}

			if (!getMainControl()) 
			{
				Log.error() << "Problem in " << __FILE__ << __LINE__ << std::endl;
				return;
			}
			QMenuBar* menu = getMainControl()->menuBar();
			if (!menu) 
			{
				Log.error() << "Problem in " << __FILE__ << __LINE__ << std::endl;
				return;
			}
			if (menu->isItemChecked(window_menu_entry_id_))
			{
				if (!widget->isVisible()) return;

				widget->hide();
				menu->setItemChecked(window_menu_entry_id_, false);
			}
			else
			{
				widget->show();
				menu->setItemChecked(window_menu_entry_id_, true);
			}
		}

} } // namespaces
