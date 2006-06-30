//   // -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_DIALOGS_GEOMETRICFITDIALOG_H
#define BALL_VIEW_DIALOGS_GEOMETRICFITDIALOG_H

#ifndef BALL_VIEW_DIALOGS_DOCKINGALGORTIHMDIALOG_H
# include <BALL/VIEW/DIALOGS/dockingAlgorithmDialog.h>
#endif

#ifndef BALL_DATATYPE_OPTIONS_H
#include <BALL/DATATYPE/options.h>
#endif

#include <BALL/VIEW/UIC/geometricFitDialogData.h>

namespace BALL
{
	namespace VIEW
	{
		/**	Dialog for options of the docking algorithm GeometricFit.
    		\ingroup  ViewDialogs
				@see GeometricFit
		 */
		class BALL_VIEW_EXPORT GeometricFitDialog :
			public DockingAlgorithmDialog,
			public Ui_GeometricFitDialogData
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
				GeometricFitDialog(QWidget* parent = 0, const char* name = 0)
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
								
				//@}				
							
		};
		
} } // Namespaces
#endif
