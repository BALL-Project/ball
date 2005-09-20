//   // -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_DIALOGS_GEOMETRICFITDIALOG_H
#define BALL_VIEW_DIALOGS_GEOMETRICFITDIALOG_H

#ifndef BALL_VIEW_KERNEL_PREFERENCESENTRY
# include <BALL/VIEW/KERNEL/preferencesEntry.h>
#endif

#ifndef BALL_DATATYPE_OPTIONS_H
#include <BALL/DATATYPE/options.h>
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
			public PreferencesEntry
		{ 
			Q_OBJECT
			
			public:
			
				
				/**	@name	Constructors and Destructors
				 */	
				//@{
			
				/** Default Constructor.
				 *	Calls \link PreferencesEntry::registerObject_ registerObject_ \endlink
				 *	@param      parent the parent widget of the GeometricFitDialog
				 *	@param      name the name of the GeometricFitDialog
				 *	@param			modal the modal flag
				 *	@param			fl the widget flags
				 *	@see        QDialog
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
        //@}
				
				/**	@name	Assignment
				 */
				//@{
				/**  Assignment operator
				 */
				const GeometricFitDialog& operator =(const GeometricFitDialog& geo_fit_dialog)
					throw();
				//@}
					
				/**	@name	Accessors: inspectors and mutators
				 */
				//@{
				 
				/** Fills options with values of the dialog.
					*	@param      options the options that are filled
					*/
				void getOptions(Options& options)
					throw();

				/** Sets the flags 'is_redock_' and 'has_changed_'.
					*/
				void isRedock(bool is_redock)
					throw();
				
				/** Fetchs the preferences from the INIFile.
					* Calls \link PreferencesEntry::readPreferenceEntries readPreferenceEntries \endlink.
					* Calls \link fetchPreferences_ fetchPreferences_ \endlink to read the redocking options.
					* This method is called in \link DockDialog::fetchPreferences fetchPreferences \endlink.
				 	*	@see    writePreferences
				 	*/
				void fetchPreferences(INIFile& file)
					throw();
				
				/** Writes the preferences to the INIFile.
					* Calls \link PreferencesEntry::writePreferenceEntries writePreferenceEntries \endlink.
				 * This method is called in \link DockDialog::writePreferences writePreferences \endlink.
				 * @see    fetchPreferences
				 */
				void writePreferences(INIFile& file)
					throw();
					
				/** Resets the dialog to the standard values.
				 */
				void reset()
					throw();
				//@}
					
			public slots:
				
				/** Shows dialog to user.
					*/
				void show();
			 
			 /** Is called when reset button is pressed.
				 * Calls \link GeometricFitDialog::reset reset \endlink.
				 */
				void resetPressed();
				
				/** Is called when cancel button is pressed.
				 *	Hides dialog.
				 */
				void cancelPressed();
				
				
			protected:
			
				/** Function to read the redocking options from INIFile into vector backup_.
					* If INIFile has not yet section GEOMETRIC_FIT_OPTIONS_REDOCK, fill backup_ vector with default values.
					*	@param    	file the INIFile that is read
					*	@param     	entry key of entry that is read
					*	@param    	default_value default value
					* @see				fetchPreferences
					*/
				void fetchPreferences_(INIFile& file, const String& entry, const QString& default_value) throw();
				
				/** Swaps the option values between vector backup_ and dialog.
				 *  Is called in \link show show \endlink if has_changed_ is true
				 *  and in \link writePreferences writePreferences \endlink if is_redock_ is true.
				 */
				void swapValues_() throw();
			
				
			private:
			
				/** Flag:
					* True if we do docking and did redocking before or otherwise.
					* False if we do (re)docking and also did (re)docking before.
					*/
				bool has_changed_;
				
				/** Flag which indicates if we do docking or redocking.
					*/
				bool is_redock_;
				
				/** Needed to guarantee that both, docking and redocking preferences can be written to INIFile
					* When we do docking, redocking options are in the vector and when we do redocking, the docking options are in there.
					* In \link GeometricFitDialog::fetchPreferences fetchPreferences \endlink, we read the last redocking options from INIFile in this vector 
					* and in \link GeometricFitDialog::writePreferences writePreferences \endlink, we write the redocking options in INIFile from this vector
					*/
				vector<QString> backup_;
		};
		
} } // Namespaces
#endif
