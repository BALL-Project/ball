// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: modularWidget.C,v 1.12 2003/08/26 09:18:45 oliver Exp $

#include <BALL/VIEW/GUI/WIDGETS/modularWidget.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/FORMAT/INIFile.h>
#include <BALL/VIEW/GUI/DIALOGS/preferences.h>
#include <BALL/VIEW/GUI/KERNEL/mainControl.h>

using namespace std;

namespace BALL
{
  namespace VIEW
	{

		ModularWidget::ModularWidget(const char* name)
			throw()
			: Embeddable(),
				ConnectionObject()
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
		
		void ModularWidget::fetchPreferences(INIFile & /* inifile */)
			throw()
		{
		}
		
		void ModularWidget::writePreferences(INIFile & /* inifile */)
			throw()
		{
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

	} // namespace VIEW
} // namespace BALL
