// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <iPythonPreferences.h>

#include <IPythonWidget.h>
#include <BALL/VIEW/KERNEL/common.h>

#include <QtWidgets/QLineEdit>

namespace BALL
{
	namespace VIEW
	{

		IPythonPreferences::IPythonPreferences(QWidget* parent, const char* name, Qt::WindowFlags fl)
			: ConfigDialog(parent, fl),
			  Ui_IPythonPreferencesData()
				
		{
			setupUi(this);
			setObjectName(name);
			setINIFileSectionName("IPythonPlugin");
			setWidgetStackName((String)tr("IPythonWidget"));

			registerWidgets_();
		}

		IPythonPreferences::~IPythonPreferences()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "Destructing object " << (void *)this 
										<< " of class IPythonPreferences" << std::endl;
			#endif 
		}

		void IPythonPreferences::storeValues()
		{
			IPythonWidget* bi = IPythonWidget::getInstance(0);
			if (bi == 0) return;

			bi->setIPythonURL(ascii(url_edit->text()));
		}

		QUrl IPythonPreferences::getIPythonURL()
		{
			return QUrl(url_edit->text());
		}
	}
} // namespaces
