// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: molecularFileDialog.h,v 1.6 2003/02/21 16:05:08 anhi Exp $

#ifndef BALL_MOLVIEW_GUI_DIALOGS_MOLECULARFILEDIALOG_H
#define BALL_MOLVIEW_GUI_DIALOGS_MOLECULARFILEDIALOG_H


#ifndef BALL_VIEW_GUI_WIDGETS_MODULARWIDGET_H
# include <BALL/VIEW/GUI/WIDGETS/modularWidget.h>
#endif

#ifndef BALL_VIEW_COMMON_GLOBAL_H
# include <BALL/VIEW/COMMON/global.h>
#endif

#include <qfiledialog.h>

namespace BALL
{
	namespace MOLVIEW
	{
		/** MolecularFileDialog class.
		 		This class is used to read or write Molecular files in one of several
				file formats. Currently, PDB, HIN and MOL2 are supported.
				Upon reading a file, the information will be stored in a  \link System System \endlink .
				This class can also take a  \link System System \endlink  and write it in one of the
				supported file formats.
				This class is derived from  \link VIEW::ModularWidget VIEW::ModularWidget \endlink .
				<b>Definition:</b> BALL/MOLVIEW/GUI/DIALOGS/molecularFileDialog.h
		 */
		class MolecularFileDialog
			: public QWidget,
				public VIEW::ModularWidget
		{
			Q_OBJECT
			public:
			BALL_EMBEDDABLE(MolecularFileDialog)

			/** @name Constructors
			 */
			//@{

			/** Default Constructor.
			 		Constructs new MolecularFileDialog.
					Calls  \link VIEW::ModularWidget::registerWidget VIEW::ModularWidget::registerWidget \endlink 
					@return			MolecularFileDialog new constructed MolecularFileDialog
					@see				VIEW::ModularWidget
			 */
			MolecularFileDialog(QWidget* parent)
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
					This method will be called by  \link show show \endlink  from the  \link MainControl MainControl \endlink  object.
					@param	main_control the  \link MainControl MainControl \endlink  object to be initialized with {\em *this} MolecularFileDialog
					@see		finalizeWidget
					@see		insertMenuEntry
					@see		show
			 */
			virtual void initializeWidget(VIEW::MainControl& main_control)
				throw();

			/** Removes the widget.
			 		Reverses all actions performed in  \link initializeWidget initializeWidget \endlink 
					(removes menu entries of {\em *this} MolecularFileDialog).
					This method will be called by  \link aboutToExit aboutToExit \endlink  from the
					 \link MainControl MainControl \endlink  object.
					@param		main_control the  \link MainControl MainControl \endlink  object to be finalized with
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
					and, if susccesfull, converts is into a  \link System System \endlink . Then it sends a  \link NewCompositeMessage NewCompositeMessage \endlink 
					containing the  \link Composite Composite \endlink  object made from the  \link System System \endlink  to the other \Ref{ConnectionObject}
					objects.
			 		A  \link WindowMessage WindowMessage \endlink  will be sent to change the status bar text of the main application.
					@see		NewCompositeMessage
					@see		WindowMessage
					@see		ConnectionObject
			 */
			virtual void readFile();

			/** Write a molecular file.
			 		This method takes a  \link System System \endlink  and saves it into a molecular file, selected from a QFileDialog.
			 */
			virtual bool writeFile();
		
			/** Read a PDB file
			 */
			bool readPDBFile(String filename, String system_name)
				throw();
		
			/** Read a HIN file
			 */
			bool readHINFile(String filename, String system_name)
				throw();

			/** Read a MOL file
			 */
			bool readMOLFile(String filename, String system_name)
				throw();
		
			/** Read a MOL2 file
			 */
			bool readMOL2File(String filename, String system_name)
				throw();

			/** Write a PDB file
			 */
			bool writePDBFile(String filename, const System& system)
				throw();
			
			/** Write a HIN file
			 */
			bool writeHINFile(String filename, const System& system)
				throw();
			
			/** Write a MOL file
			 */
			bool writeMOLFile(String filename, const System& system)
				throw();
			
			/** Write a MOL2 file
			 */
			bool writeMOL2File(String filename, const System& system)
				throw();
			
			//@}
			
			protected:

			bool finish_(const String& filename, const String& system_name, System* system)
				throw();

		};

	} // namespace MOLVIEW

}	// namespace BALL

#endif // BALL_MOLVIEW_GUI_DIALOGS_MOLECULARFILEDIALOG_H
