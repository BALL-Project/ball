// $Id: modularWidget.C,v 1.7 2001/12/22 14:14:04 oliver Exp $

#include <BALL/VIEW/GUI/WIDGETS/modularWidget.h>

#include <qwidget.h>

using namespace std;

#define BALL_VIEW_DEBUG

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
				
			destroy();
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
						Log.info() << "ModularWidget::registered: " <<mwidget << endl;
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

	} // namespace VIEW

} // namespace BALL
