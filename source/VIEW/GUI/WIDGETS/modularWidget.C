// $Id: modularWidget.C,v 1.5 2001/05/13 14:26:25 hekl Exp $

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
			  cout << "Destructing object " << (void *)this 
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
  			cerr << "registering ModularWidget at " << mwidget << endl;
			#endif

			QObject* object = dynamic_cast<QObject*>(mwidget);
			if (object != 0)
			{
				MainControl* mc = MainControl::getMainControl(object);
				if (mc != 0)
				{
					mc->addModularWidget(mwidget);

          #ifdef BALL_VIEW_DEBUG
						cerr << "ModularWidget::registered: " <<mwidget << endl;
					#endif
				}
				else 
				{
					cerr << "ModularWidget::ModularWidget: widget is not in a MainControl object!" << endl;
				}
			}
		}

		void ModularWidget::initializeWidget(MainControl& /* main_control */)
			throw()
		{
		}
		
		void ModularWidget::finalizeWidget(MainControl& /* main_control */)
			throw()
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
