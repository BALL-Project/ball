// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/DIALOGS/openSavePreferences.h>

#include <BALL/VIEW/DIALOGS/molecularFileDialog.h>

namespace BALL
{
	namespace VIEW
	{

		OpenSavePreferences::OpenSavePreferences(QWidget* parent, const char* name, Qt::WindowFlags fl)
			: QWidget(parent, fl)
		{
			setINIFileSectionName("OpenSave");
			setupUi(this);
			setObjectName(name);
			setWidgetStackName("Open/Save");
			registerWidgets_();
		}

		OpenSavePreferences::~OpenSavePreferences()
		{
			#ifdef BALL_VIEW_DEBUG
		  Log.error() << "Destructing object " << (void *)this
			            << " of class OpenSavePreferences" << std::endl;
			#endif
		}

		void OpenSavePreferences::storeValues()
		{
			PreferencesEntry::storeValues();

			MolecularFileDialog* mf = MolecularFileDialog::getInstance(0);
			if(mf) {
				mf->setReadPDBModels(pdb_model_box_->isChecked());
			}
		}
		
		bool  OpenSavePreferences::binaryProjectsEnabled()
		{
			return binary_project_radiobutton->isChecked();
		}
	}
}

