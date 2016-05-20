// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <jupyterPreferences.h>

#include <jupyterWidget.h>
#include <BALL/VIEW/KERNEL/common.h>

#include <QtWidgets/QLineEdit>

namespace BALL
{
	namespace VIEW
	{

		JupyterPreferences::JupyterPreferences(QWidget* parent, const char* name, Qt::WindowFlags fl)
			: ConfigDialog(parent, fl),
			  Ui_JupyterPreferencesData()
				
		{
			setupUi(this);
			setObjectName(name);
			setINIFileSectionName("JupyterPlugin");
			setWidgetStackName((String)tr("JupyterWidget"));

			registerWidgets_();
		}

		JupyterPreferences::~JupyterPreferences()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "Destructing object " << (void *)this 
										<< " of class JupyterPreferences" << std::endl;
			#endif 
		}

		void JupyterPreferences::storeValues()
		{
			JupyterWidget* bi = JupyterWidget::getInstance(0);
			if (bi == 0) return;

			bi->setBaseURL(ascii(url_edit->text()));
		}

		QUrl JupyterPreferences::getBaseURL()
		{
			return QUrl(url_edit->text());
		}
	}
} // namespaces
