#include <PresentaBALLSettings.h>
#include <PresentaBALLView.h>

#include <QLineEdit>
#include <QFileDialog>

#include <BALL/VIEW/KERNEL/common.h>

namespace BALL
{
	namespace VIEW
	{
		PresentaBALLSettings::PresentaBALLSettings(PresentaBALLView* parent, const char* name, Qt::WindowFlags fl)
			: ConfigDialog(parent, fl),
			  Ui_PresentaBALLSettingsData(),
			  html_interface_(parent)
		{
			setupUi(this);
			setObjectName(name);
			setWidgetStackName((String)tr("PresentaBALL"));
			setINIFileSectionName("PresentaBALL_PLUGIN");
			registerWidgets_();

			connect(browse_button, SIGNAL(clicked()), this, SLOT(selectIndexHTMLLocation()));
		}

		PresentaBALLSettings::~PresentaBALLSettings()
		{ }

		void PresentaBALLSettings::setIndexHTMLLocation(const QString& path)
		{
			index_html_edit->setText(path);
		}

		QString PresentaBALLSettings::getIndexHTMLLocation()
		{
			return index_html_edit->text();
		}

		void PresentaBALLSettings::selectIndexHTMLLocation()
		{
			QString new_location = QFileDialog::getOpenFileName(this, "Select a start page for PresentaBALL");

			if (new_location != "")
			{
				setIndexHTMLLocation(new_location);
			}
		}

		void PresentaBALLSettings::restoreDefaultValues(bool all)
		{
			PreferencesEntry::restoreDefaultValues(all);
			html_interface_->restoreDefaults();
		}

	}
}
