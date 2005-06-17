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

				/** Copy constructor.
					*/
				GeometricFitDialog(const GeometricFitDialog& geo_fit_dialog)
					throw();
					
				/** Destructor.
				 */
				virtual ~GeometricFitDialog()
					throw();

				/**  Assignment operator
				 */
				const GeometricFitDialog& operator =(const GeometricFitDialog& geo_fit_dialog)
					throw();
					
				/**	ModularWidget methods
				 */
				//@{
				
				/** Fetchs the preferences from the INIFile.
					* Calls fetchPreferences_. to read the redocking options 
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

				/** Sets the flags 'is_redock_' and 'has_changed_'
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
			
				/** function to read the redocking options from INIFile into vector backup_
					* if INIFile has not yet section GEOMETRIC_FIT_OPTIONS_REDOCK, fill backup_ vector with default values
					*/
				void fetchPreferences_(INIFile& file, const String& entry, const QString& default_value) throw();
				
				/** Swaps the option values between vector backup_ and dialog
				 *  Is called in show() if has_changed_ is true
				 *  and in writePreferences if is_redock_ is true
				 */
				void swapValues_() throw();
			
				
			private:
			
				/** flag:
					* true if we now do docking and did redocking before or otherwise
					* false if we do (re)docking and also did (re)docking before
					*/
				bool has_changed_;
				
				/// flag which indicates if we do docking or redocking
				bool is_redock_;
				
				/** Needed to guaranty that both, docking and redocking preferences can be written to INIFile
					* When we do docking, redocking options are in the vector and when we do redocking, the docking options are in there.
					* In fetchPreferences, we read the last redocking options from INIFile in this vector 
					* and in writePreferences, we write the redocking options in INIFile from this vector
					*/
				vector<QString> backup_;
		};
		
} } // Namespaces
#endif
