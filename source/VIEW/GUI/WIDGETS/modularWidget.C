// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: modularWidget.C,v 1.11 2002/12/12 11:43:25 oliver Exp $

#include <BALL/VIEW/GUI/WIDGETS/modularWidget.h>

#include <qwidget.h>

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

		void ModularWidget::destroy()
			throw()
		{
		}

		void ModularWidget::registerWidget(ModularWidget* mwidget)
			throw()
		{
      #ifdef BALL_VIEW_DEBUG
  			Log.info() << "registering ModularWidget at " << mwidget << endl;
			#endif

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
			WindowMessage *window_message = new WindowMessage;
			window_message->setStatusBar(text);
			window_message->setDeletable(true);
			notify_(window_message);
		}

	} // namespace VIEW

} // namespace BALL
