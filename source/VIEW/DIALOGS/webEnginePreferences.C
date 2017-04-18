#include <BALL/VIEW/DIALOGS/webEnginePreferences.h>

#include <BALL/PYTHON/pyInterpreter.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/VIEW/WIDGETS/HTMLView.h>

#include <QFileDialog>

using std::string;
using std::tie;

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
				Log.error() << "WebEngine: Invalid python module path!\n";
			}
			else if (!PyInterpreter::isInitialized())
			{
				Log.error() << "WebEngine: Python interpreter unavailable!\n";
			}
			else if (script_base_ != script_base)
			{
				if (script_base_ != "")
				{
					PyInterpreter::run(string("sys.path.remove('") + script_base_.c_str() + "')");
				}

				PyInterpreter::run(string("sys.path.append('") + script_base.c_str() + "')");
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
