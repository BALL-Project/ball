// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_DIALOGS_ADVANCEDOPTIONSDIALOG_H
#define BALL_VIEW_DIALOGS_ADVANCEDOPTIONSDIALOG_H

#include <BALL/VIEW/UIC/advancedOptionsDialogData.h>

#include <BALL/FORMAT/INIFile.h>
#include <BALL/SYSTEM/path.h>

namespace BALL
{
	namespace VIEW
	{
		// Dialog for changing options in MolecularDynamicsDialog
		// and EnergyMinimizationDialog
		
		class advancedOptionsDialog
		: public advancedOptions
		{
			Q_OBJECT
			
			public:
			//constructor
			advancedOptionsDialog(QWidget* parent = NULL, const char* name = NULL);
			//destructor
			virtual ~advancedOptionsDialog();
			
			///
			const String& getFilename() const;
			
			///
			void setFilename(const String& filename);
			
			///
			bool getUseDistanceDependentDC() const;

			///
			//void setUseDistanceDependentDC(bool use_dddc);

			///
			//bool useMicroCanonical() const;

			///
			//void setMicroCanonical(bool state);

			
			protected:
			
			virtual void browseParameterFiles();
		};
	}
}
#endif
