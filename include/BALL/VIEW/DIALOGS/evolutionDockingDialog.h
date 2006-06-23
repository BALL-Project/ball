//   // -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_DIALOGS_EVOLUTIONDOCKINGDIALOG_H
#define BALL_VIEW_DIALOGS_EVOLUTIONDOCKINGDIALOG_H

#ifndef BALL_VIEW_DIALOGS_DOCKINGALGORTIHMDIALOG_H
# include <BALL/VIEW/DIALOGS/dockingAlgorithmDialog.h>
#endif

#ifndef BALL_DATATYPE_OPTIONS_H
#include <BALL/DATATYPE/options.h>
#endif

#include <BALL/VIEW/UIC/evolutionDockingDialogData.h>

namespace BALL
{
	namespace VIEW
	{
		/**	Dialog for options of the docking algorithm EvolutionDocking.
    		\ingroup  ViewDialogs
				@see EvolutionaryDocking
		 */
		class BALL_VIEW_EXPORT EvolutionDockingDialog : 
			public Ui_EvolutionDockingDialogData,
			public DockingAlgorithmDialog
		{ 
			Q_OBJECT
			
			public:
			
				
				/**	@name	Constructors and Destructors
				 */	
				//@{
			
				/** Default Constructor.
				 *	Calls \link PreferencesEntry::registerObject_ PreferencesEntry::registerObject_ \endlink
				 *	@param      parent the parent widget of the EvolutionDockingDialog
				 *	@param      name the name of the EvolutionDockingDialog
				 *	@param			modal the modal flag
				 *	@param			fl the widget flags
				 *	@see        QDialog
				 *	@see				PreferncesEntry
				 */
				EvolutionDockingDialog(QWidget* parent = 0, const char* name = 0)
					throw();
					
				/** Destructor.
				 */
				virtual ~EvolutionDockingDialog()
					throw();
        //@}
				
				/**	@name	Assignment
				 */
				//@{
				/**  Assignment operator
				 */
				const EvolutionDockingDialog& operator =(const EvolutionDockingDialog& ev_dock_dialog)
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

				/** Fills options with values of the force field dialog.
					*	@param      options the options that are filled
					*/
				void getFFOptions(Options& options)
					throw();

			  //@}
					
			public slots:
				
				/** Is called when radio button for using a created grid is pressed. 
				 	* It enables the browse button and to corresponding line edit.
				 	*/
				void enableFileBrowsing();

				/** Is called when radio button for using a new grid is pressed. 
				 	* It disables the browse button and to corresponding line edit.
				 	*/
				void disableFileBrowsing();

				/** Is called when browse button is pressed. 
				 	* It shows a file dialog.
				 	*/
				void browseGridFile();

				
				/** Is called when force field options button is pressed. 
					* It shows the force field option dialog.
					*/
		   void showForceFieldOptions();
										
			private:
			
				/** Copy constructor.
				 	* Remark: Copy contructor is private because it is not completed. 
					* The copy constuctor of the QT widgets is private and cannot be called.    
					*/
				EvolutionDockingDialog(const EvolutionDockingDialog& ev_dock_dialog)
					throw();
		};
		
} } // Namespaces
#endif
