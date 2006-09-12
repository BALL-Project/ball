#ifndef BALL_VIEW_DIALOGS_DOCKINGALGORITHMDIALOG_H
#define BALL_VIEW_DIALOGS_DOCKINGALGORITHMDIALOG_H

#ifndef BALL_VIEW_KERNEL_PREFERENCESENTRY
# include <BALL/VIEW/KERNEL/preferencesEntry.h>
#endif

#ifndef BALL_DATATYPE_OPTIONS_H
#include <BALL/DATATYPE/options.h>
#endif

#include <QtGui/qdialog.h>

namespace BALL
{
	namespace VIEW
	{
		/**	Abstract base class for docking options dialogs.
    		\ingroup  ViewDialogs
		 */
		class BALL_VIEW_EXPORT DockingAlgorithmDialog : 
			public QDialog,
			public PreferencesEntry
		{ 
			Q_OBJECT
			
			public:
	 	
				/**	@name	Constructors and Destructors
				 */	
				//@{
			
				/** Default Constructor.
				 */
				DockingAlgorithmDialog(QWidget* parent)
					throw();

				/** Copy Constructor.
				 */
				DockingAlgorithmDialog(const DockingAlgorithmDialog& dialog)
					throw();
				
				/** Destructor.
				 */
				virtual ~DockingAlgorithmDialog()
					throw();
        //@}

				/**	@name	Assignment
				 */
				//@{
				/**  Assignment operator
				 */
				virtual const DockingAlgorithmDialog& operator =(const DockingAlgorithmDialog& dialog)
					throw();
				//@}

       /** Fills options with values of the dialog.
				 *	@param      options the options that are filled
				 */
				virtual void getOptions(Options& options)
					throw();

	      /** Sets the flags \link DockingAlgorithmDialog::is_redock_ is_redock_ \endlink and 
				  * \link DockingAlgorithmDialog::has_changed_ has_changed_ \endlink.
					*/
				virtual void isRedock(bool is_redock)
					throw();

				/** Fetchs the preferences from the INIFile.
					* Calls \link PreferencesEntry::readPreferenceEntries PreferencesEntry::readPreferenceEntries \endlink.
					* This method is called in \link DockDialog::fetchPreferences DockDialog::fetchPreferences \endlink.
				 	*	@see    writePreferences
				 	*/
				virtual void fetchPreferences(INIFile& file)
					throw();

				/** Writes the preferences to the INIFile.
					* Calls \link PreferencesEntry::writePreferenceEntries PreferencesEntry::writePreferenceEntries \endlink.
				  * This method is called in \link DockDialog::writePreferences DockDialog::writePreferences \endlink.
				  * @see    fetchPreferences
				  */
				virtual void writePreferences(INIFile& file)
					throw();

			public slots:

				/** Shows dialog to user.
					*/
				void show();
			 
			 	/** Is called when reset button is pressed.
				 	*/
				void reset();
				
				/** Is called when cancel button is pressed.
				 *	Hides dialog.
				 */
				void reject();

				/** Is called when ok button is pressed.
				 */
				void accept();

			protected:
				
				/** Swaps the option values between backup_ and dialog.
				 */
				void swapValues_() 
					throw();

				/** Flag:
					* True if we do docking and did redocking before or otherwise.
					* False if we do (re)docking and also did (re)docking before.
					*/
				bool has_changed_;
				
				/** Flag which indicates if we do docking or redocking.
					*/
				bool is_redock_;
				
				/** Needed to guarantee that both, docking and redocking preferences can be written to INIFile
				 *  When we do docking, redocking options are in a value map and when we do redocking, the docking options are in there.
				 */
				ValueMap backup_;

				/** name of the INI file section of redocking options
				 */
				String inifile_section_name_backup_;

		};
		
} } // Namespaces
#endif
