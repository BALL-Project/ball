// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_DIALOGS_GEOMETRICFITDIALOG_H
#define BALL_VIEW_DIALOGS_GEOMETRICFITDIALOG_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifndef BALL_VIEW_KERNEL_PREFERENCESENTRY
# include <BALL/VIEW/KERNEL/preferencesEntry.h>
#endif

#ifndef BALL_DATATYPE_OPTIONS_H
#include <BALL/DATATYPE/options.h>
#endif

#include <BALL/VIEW/UIC/ui_geometricFitDialog.h>

namespace BALL
{
	namespace VIEW
	{
		/**	Dialog for options of the docking algorithm GeometricFit.
    		\ingroup  ViewDialogs
				@see GeometricFit
				\ingroup ViewDialogs
		 */
		class BALL_VIEW_EXPORT GeometricFitDialog : 
			public QDialog,
			public Ui_GeometricFitDialogData,
			public PreferencesEntry
		{ 
			Q_OBJECT
			
			public:
			
				
				/**	@name	Constructors and Destructors
				 */	
				//@{
			
				/** Default Constructor.
				 *	Calls \link PreferencesEntry::registerObject_ PreferencesEntry::registerObject_ \endlink
				 *	@param      parent the parent widget of the GeometricFitDialog
				 *	@param      name the name of the GeometricFitDialog
				 *	@param			modal the modal flag
				 *	@param			fl the widget flags
				 *	@see        QDialog
				 *	@see				PreferncesEntry
				 */
				GeometricFitDialog(QWidget* parent = 0, const char* name = "GeometricFitDialog");
					
				/** Destructor.
				 */
				virtual ~GeometricFitDialog();
        //@}
				
				/**	@name	Assignment
				 */
				//@{
				/**  Assignment operator
				 */
				const GeometricFitDialog& operator =(const GeometricFitDialog& geo_fit_dialog);
				//@}
					
				/**	@name	Accessors: inspectors and mutators
				 */
				//@{
				 
				/** Fills options with values of the dialog.
					*	@param      options the options that are filled
					*/
				void getOptions(Options& options);

				/** Sets the flags \link GeometricFitDialog::is_redock_ is_redock_ \endlink and 
				  * \link GeometricFitDialog::has_changed_ has_changed_ \endlink.
					*/
				void isRedock(bool is_redock);
				
				/** Fetchs the preferences from the INIFile.
					* Calls \link PreferencesEntry::readPreferenceEntries PreferencesEntry::readPreferenceEntries \endlink.
					* Calls \link GeometricFitDialog::fetchPreferences_ fetchPreferences_ \endlink to read the redocking options.
					* This method is called in \link DockDialog::fetchPreferences DockDialog::fetchPreferences \endlink.
				 	*	@see    writePreferences
				 	*/
				void fetchPreferences(INIFile& file);
				
				/** Writes the preferences to the INIFile.
					* Calls \link PreferencesEntry::writePreferenceEntries PreferencesEntry::writePreferenceEntries \endlink.
				 * This method is called in \link DockDialog::writePreferences DockDialog::writePreferences \endlink.
				 * @see    fetchPreferences
				 */
				void writePreferences(INIFile& file);
					
				//@}
					
			public slots:
				
				/** Shows dialog to user.
					*/
				void show();
			 
			 /** Is called when reset button is pressed.
				 * Calls \link GeometricFitDialog::reset reset \endlink.
				 */
				void reset();
				
				/** Is called when cancel button is pressed.
				 *	Hides dialog.
				 */
				virtual void reject();

				//
				virtual void accept();
				
				
			protected:
			
				/** Function to read the redocking options from INIFile into vector backup_.
					* If INIFile has not yet section <b> GEOMETRIC_FIT_OPTIONS_REDOCK </b>, fill vector \link DockDialog::backup_ backup_ \endlink with default values.
					*	@param    	file the INIFile that is read
					*	@param     	entry key of entry that is read
					*	@param    	default_value default value
					* @see				fetchPreferences
					*/
				void fetchPreferences_(INIFile& file, const String& entry, const QString& default_value);
				
				/** Swaps the option values between vector backup_ and dialog.
				 *  Is called in \link GeometricFitDialog::show show \endlink if \link GeometricFitDialog::has_changed_ has_changed_ \endlink 
				 *  is true and in \link GeometricFitDialog::writePreferences writePreferences \endlink if 
				 *  \link GeometricFitDialog::is_redock_ is redock_ \endlink is true.
				 */
				void swapValues_();
			
				
			private:
			
				/** Copy constructor.
				 	* Remark: Copy contructor is private because it is not completed. 
					* The copy constuctor of the QT widgets is private and cannot be called.    
					*/
				GeometricFitDialog(const GeometricFitDialog& geo_fit_dialog);
			
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
