// $Id: molecularFileDialog.h,v 1.1.2.1 2002/12/06 18:55:08 anhi Exp $

#ifndef BALL_MOLVIEW_GUI_DIALOGS_MOLECULARFILEDIALOG_H
#define BALL_MOLVIEW_GUI_DIALOGS_MOLECULARFILEDIALOG_H

#include <qfiledialog.h>

#ifndef BALL_VIEW_GUI_WIDGETS_MODULARWIDGET_H
# include <BALL/VIEW/GUI/WIDGETS/modularWidget.h>
#endif

namespace BALL
{
	namespace MOLVIEW
	{
		/** MolecularFileDialog class.
		 		This class is used to read or write Molecular files in one of several
				file formats. Currently, PDB, HIN and MOL2 are supported.
				Upon reading a file, the information will be stored in a \Ref{System}.
				This class can also take a \Ref{System} and write it in one of the
				supported file formats.
				This class is derived from \Ref{VIEW::ModularWidget}.
				{\bf Definition:} \URL{BALL/MOLVIEW/GUI/DIALOGS/molecularFileDialog.h}
		 */
		class MolecularFileDialog
			: public QWidget, VIEW::ModularWidget
		{
			Q_OBJECT
			public:
			BALL_EMBEDDABLE(MolecularFileDialog)

			/** @name Constructors
			 */

			//@{

			/** Default Constructor.
			 		Constructs new MolecularFileDialog.
					Calls \Ref{VIEW::ModularWidget::registerWidget}
					@return			MolecularFileDialog new constructed MolecularFileDialog
					@see				VIEW::ModularWidget
			 */
			MolecularFileDialog(QWidget *)
				throw();

			//@}
			/** @name Destructors
			 */
			//@{
			
			/** Destructor.
			 		Default destruction of {\em *this} MolecularFileDialog.
			 */
			virtual ~MolecularFileDialog()
				throw();

			/** Explicit destructor.
			 */
			virtual void destroy()
				throw();

			//@}

			/** @name Accessors: inspectors and mutators
			 */
			
			//@{
			
			/** Initializes the widget.
			 		Initializes the menu {\em File} with the entries {\em Load System} and {\em Write System}.
					This method is called automatically immediately before the main application is started.
					This method will be called by \Ref{show} from the \Ref{MainControl} object.
					@param	main_control the \Ref{MainControl} object to be initialized with {\em *this} MolecularFileDialog
					@see		finalizeWidget
					@see		insertMenuEntry
					@see		show
			 */
			virtual void initializeWidget(VIEW::MainControl& main_control)
				throw();

			/** Removes the widget.
			 		Reverses all actions performed in \Ref{initializeWidget}
					(removes menu entries of {\em *this} MolecularFileDialog).
					This method will be called by \Ref{aboutToExit} from the
					\Ref{MainControl} object.
					@param		main_control the \Ref{MainControl} object to be finalized with
										{\em *this} MolecularFileDialog
					@see			initializeWidget
					@see			removeMenuEntry
					@see			aboutToExit
			 */
			virtual void finalizeWidget(VIEW::MainControl& main_control)
				throw();

		public slots:

			/** Open a molecular file.
			 		This method tries to open and read a molecular file, selected from a QFileDialog,
					and, if susccesfull, converts is into a \Ref{System}. Then it sends a \Ref{NewCompositeMessage}
					containing the \Ref{Composite} object made from the \Ref{System} to the other \Ref{ConnectionObject}
					objects.
			 		A \Ref{WindowMessage} will be sent to change the status bar text of the main application.
					@see		NewCompositeMessage
					@see		WindowMessage
					@see		ConnectionObject
			 */
			virtual void readFile()
				throw();

			/** Write a molecular file.
			 		This method takes a \Ref{System} and saves it into a molecular file, selected from a QFileDialog.
					@param	system the \Ref{System} that is saved into a file
			 */
			virtual bool writeFile()
				throw();
		
			//@}
		};

	} // namespace MOLVIEW

}	// namespace BALL

#endif // BALL_MOLVIEW_GUI_DIALOGS_MOLECULARFILEDIALOG_H
