#include <BALL/VIEW/DIALOGS/webEnginePreferences.h>

#include <BALL/PYTHON/pyInterpreter.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/VIEW/WIDGETS/HTMLView.h>

#include <QFileDialog>

namespace BALL
{
	namespace VIEW
	{

		WebEnginePreferences::WebEnginePreferences(QWidget* parent, const char* name, Qt::WindowFlags fl)
			: QDialog(parent, fl),
			  Ui_WebEnginePreferences(),
			  PreferencesEntry(),
			  ModularWidget(name),
			  script_base_()
		{
			setINIFileSectionName("WebEngine");
			setupUi(this);
			setObjectName(name);
			setWidgetStackName("WebEngine");

			connect(browse_button, SIGNAL(clicked()), this, SLOT(selectScriptBase()));

			registerWidgets_();
		}

		WebEnginePreferences::~WebEnginePreferences()
		{
			#ifdef BALL_VIEW_DEBUG
			Log.error() << "Destructing object " << (void *)this
			            << " of class WebEnginePreferences" << std::endl;
			#endif
		}

		void WebEnginePreferences::applySettings()
		{
#ifdef BALL_PYTHON_SUPPORT
			Path p;
			String script_base = p.find(String(script_base_edit->text()));

			if (script_base == "")
			{
				Log.error() << "WebEngine: Invalid python module path!" << std::endl;
			}
			else if (script_base_ != script_base)
			{
				bool result;
				if (script_base_ != "")
				{
					PyInterpreter::run(String("sys.path.remove('") + script_base_.c_str() + "')", result);
				}

				PyInterpreter::run(String("sys.path.append('") + script_base.c_str() + "')", result);

				if (!result)
				{
					Log.error() << "WebEngine: Could not add python module path!" << std::endl;
				}

				script_base_ = script_base;
			}
#endif

			for (unsigned int i = 0; i < HTMLViewDock::countInstances(); ++i)
			{
				HTMLViewDock::getInstance(i)->resetHTMLView(skip_checks->isChecked());
			}
		}

		void WebEnginePreferences::selectScriptBase()
		{
			QString script_base = QFileDialog::getExistingDirectory(this, "Python module path");

			if (script_base != "")
			{
				script_base_edit->setText(script_base);
			}
		}
	}
}
