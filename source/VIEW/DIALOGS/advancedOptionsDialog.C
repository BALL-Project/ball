// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/DIALOGS/advancedOptionsDialog.h>

#include <qfiledialog.h>
#include <qlineedit.h>
#include <qradiobutton.h>
#include <qcheckbox.h>


namespace BALL
{
	namespace VIEW
	{

		advancedOptionsDialog::advancedOptionsDialog(QWidget* parent, const char* name)
			:advancedOptions( parent, name )
		{
		}

		advancedOptionsDialog::~advancedOptionsDialog()
		{
		}

		void advancedOptionsDialog::browseParameterFiles()
		{
			// look up the full path of the parameter file
			Path p;
			String filename = p.find(getFilename());
			if (filename == "")
			{
				filename = getFilename();
			}
			QString tmp = filename.c_str();
			QString result = QFileDialog::getOpenFileName(tmp, "*.ini", 0, "Select an AMBER parameter file");
			if (!result.isEmpty())
			{
				// store the new filename in the lineedit field
				setFilename(String(result.ascii()));
			}
		}
		
		const String& advancedOptionsDialog::getFilename() const
		{
			static String filename;
			filename = parameter_file_edit->text().ascii();
			return filename;
		}
		
		void advancedOptionsDialog::setFilename(const String& filename)
		{
			parameter_file_edit->setText(filename.c_str());
			parameter_file_edit->update();
		}
		
		bool advancedOptionsDialog::getUseDistanceDependentDC() const
		{
			return distance_button->isChecked();
		}

		/*bool advancedOptionsDialog::useMicroCanonical() const
		{
			return microcanonical_button->isChecked();
		}*/

	}//namespace View
}//namespace Ball
