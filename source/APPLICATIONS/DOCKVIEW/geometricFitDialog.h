//   // -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_DIALOGS_GEOMETRICFITDIALOG_H
#define BALL_VIEW_DIALOGS_GEOMETRICFITDIALOG_H

#ifndef BALL_VIEW_KERNEL_MODULARWIDGET_H
# include <BALL/VIEW/KERNEL/modularWidget.h>
#endif

#ifndef BALL_VIEW_KERNEL_PREFERENCESENTRY
# include <BALL/VIEW/KERNEL/preferencesEntry.h>
#endif

#ifndef BALL_DATATYPE_OPTIONS_H
#include <BALL/DATATYPE/options.h>
#endif

#ifndef BALL_STRUCTURE_DOCKING_GEOMETRICFIT_H
# include <BALL/STRUCTURE/DOCKING/geometricFit.h>
#endif

#include "geometricFitDialogData.h"

namespace BALL
{
	namespace VIEW
	{
		class BALL_EXPORT GeometricFitDialog : 
				public GeometricFitDialogData,
				public ModularWidget,
				public PreferencesEntry
		{ 
				Q_OBJECT

				public:
				
				/// Constructor
				GeometricFitDialog(QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0)
					throw();

				/// Destructor
				virtual ~GeometricFitDialog()
					throw();

				/// Read the preferences from a INIFile
				void fetchPreferences(INIFile& file)
					throw();
				
				/// Write the preferences to a INIFile
				void writePreferences(INIFile& file)
					throw();

				/// Reset the dialog to the standard values
				void reset()
					throw();
				
				/// Set options
				void setOptions(Options& options)
					throw();

				public slots:
				
				//
				virtual void resetPressed();
					
				//
				virtual void cancelPressed();
				
				protected:
					
					
				private:
					

		};
		
} } // Namespaces
#endif
