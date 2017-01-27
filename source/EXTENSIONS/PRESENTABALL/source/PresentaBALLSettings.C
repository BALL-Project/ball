#include <PresentaBALLSettings.h>
#include <PresentaBALLView.h>

#include <BALL/SYSTEM/path.h>
#include <BALL/SYSTEM/directory.h>

#include <QLineEdit>
#include <QFileDialog>

namespace BALL
{
	namespace VIEW
	{
		PresentaBALLSettings::PresentaBALLSettings(QWidget* parent, const char* name, Qt::WindowFlags fl)
			: ConfigDialog(parent, fl),
			  Ui_PresentaBALLSettingsData()
		{
			setupUi(this);
			setObjectName(name);
			setWidgetStackName((String)tr("PresentaBALL"));
			setINIFileSectionName("PresentaBALL");
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

			//set the webpage language according to the language set in preferences
			String home_dir = Directory::getUserHomeDir();
			INIFile f(home_dir + FileSystem::PATH_SEPARATOR + ".BALLView");
			f.read();

			Path p;
			String s;
			if (f.hasEntry("GENERAL", "language") && f.getValue("GENERAL", "language") == "de_DE")
			{
				s = p.find("HTMLBasedInterface/html_de");
			}
			else
			{
				s = p.find("HTMLBasedInterface/html_eng");
			}

			if (!s.isEmpty())
			{
				setIndexHTMLLocation((s + "/index.html").c_str());
			}
			else
			{
				Log.error() << "No html directory set!" << std::endl;
			}
		}

	}
}
