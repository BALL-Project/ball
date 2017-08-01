#include <BALL/VIEW/DIALOGS/webEnginePreferences.h>

#include <BALL/VIEW/WIDGETS/HTMLView.h>


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
			  ModularWidget(name)
		{
			setINIFileSectionName("WebEngine");
			setupUi(this);
			setObjectName(name);
			setWidgetStackName("WebEngine");

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
			for (unsigned int i = 0; i < HTMLViewDock::countInstances(); ++i)
			{
				HTMLViewDock::getInstance(i)->resetHTMLView(skip_checks->isChecked());
			}
		}
	}
}
