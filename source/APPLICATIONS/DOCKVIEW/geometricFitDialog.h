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
		/**	Dialog for options of the docking algorithm GeometricFit.
    		\ingroup  ViewDialogs
				@see GeometricFit
		 */
		class BALL_EXPORT GeometricFitDialog : 
			public GeometricFitDialogData,
			public ModularWidget,
			public PreferencesEntry
		{ 
			Q_OBJECT
			
			public:
			
				BALL_EMBEDDABLE(GeometricFitDialog,ModularWidget)	
				
				/**	@name	Constructors and Destructors
				 */	
				//@{
			
				/** Default Constructor.
				 *	Calls  \link ModularWidget::registerWidget registerWidget \endlink and \link PreferencesEntry::registerObject_ registerObject_ \endlink
				 *	@param      parent the parent widget of the DockDialog
				 *	@param      name the name of the DockDialog
				 *	@param			modal the modal flag
				 *	@param			fl the widget flags
				 *	@see        QDialog
				 *	@see        ModularWidget
				 *	@see				PreferncesEntry
				 */
				GeometricFitDialog(QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0)
					throw();

				/** Destructor.
				 */
				virtual ~GeometricFitDialog()
					throw();

				/**	ModularWidget methods
				 */
				//@{
				
				/** Fetchs the preferences from the INIFile.
				 *	@see    writePreferences
				 */
				void fetchPreferences(INIFile& file)
					throw();
				
				/** Writes the preferences to the INIFile.
				 * This method will be called inside the method  MainControl::aboutToExit 
				 * @see    fetchPreferences
				 */
				void writePreferences(INIFile& file)
					throw();

				//@}	
					
				/** Resets the dialog to the standard values.
				 */
				void reset()
					throw();
				
				/** Fill options with values of the dialog.
					*	@param      options the options that are filled
					*/
				void getOptions(Options& options)
					throw();

				/** 
					*
					*/
				void setFlag(bool is_redock)
					throw();
					
					
			public slots:
				
				/** Shows dialog to user
					*/
				void show();
			
				/** Indicates that the reset button was pressed.
				 *  Calls QDialog::reset.
				 */
				virtual void resetPressed();
				
				/** Indicates that the cancel button was pressed.
				 *	Hides dialog.
				 */
				virtual void cancelPressed();
		
			
			protected:
			
				/**
					*/
				void fetchPreferences_(INIFile& file, String entry, QString default_value) throw();
				
				/**
					*/
				void swapValues_() throw();
			
				
			private:
			
				///
				bool has_changed_;
				/// flag which indicates if we are docking or redocking
				bool is_redock_;
				///
				vector<QString> backup_;
		};
		
} } // Namespaces
#endif
