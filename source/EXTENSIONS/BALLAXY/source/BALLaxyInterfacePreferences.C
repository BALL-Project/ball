// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALLaxyInterfacePreferences.h>

#include <BALLaxyInterface.h>
#include <BALL/VIEW/KERNEL/common.h>

#include <QtWidgets/QLineEdit>

namespace BALL
{
	namespace VIEW
	{

		BALLaxyInterfacePreferences::BALLaxyInterfacePreferences(QWidget* parent, const char* name, Qt::WindowFlags fl)
			: ConfigDialog(parent, fl),
			  Ui_BALLaxyInterfacePreferencesData()
				
		{
			setupUi(this);
			setObjectName(name);
			setINIFileSectionName("BALLAXYPlugin");
			setWidgetStackName((String)tr("BALLaxyInterface"));

			registerWidgets_();
		}

		BALLaxyInterfacePreferences::~BALLaxyInterfacePreferences()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "Destructing object " << (void *)this 
										<< " of class BALLaxyInterfacePreferences" << std::endl;
			#endif 
		}

		void BALLaxyInterfacePreferences::storeValues()
		{
			BALLaxyInterface* bi = BALLaxyInterface::getInstance(0);
			if (bi == 0) return;

			bi->setBALLaxyBaseUrl(ascii(url_edit->text()), ascii(login_edit->text()), ascii(password_edit->text()));
		}

		QUrl BALLaxyInterfacePreferences::getBALLaxyBaseUrl()
		{
			return QUrl(url_edit->text());
		}
	}
} // namespaces
