// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: molecularFileDialog.h,v 1.2 2003/08/26 15:01:22 amoll Exp $

#ifndef BALL_VIEW_DIALOGS_MOLECULARFILEDIALOG_H
#define BALL_VIEW_DIALOGS_MOLECULARFILEDIALOG_H


#ifndef BALL_VIEW_WIDGETS_MODULARWIDGET_H
# include <BALL/VIEW/WIDGETS/modularWidget.h>
#endif

#ifndef BALL_VIEW_COMMON_GLOBAL_H
# include <BALL/VIEW/COMMON/global.h>
#endif

#include <qfiledialog.h>

namespace BALL
{
	class System;

	namespace VIEW
	{
		/** MolecularFileDialog class.
		 		This class is used to read or write Molecular files in one of several
				file formats. Currently, PDB, HIN and MOL2 are supported.
				Upon reading a file, the information will be stored in a System.
				This class can also take a System and write it in one of the
				supported file formats.
				This class is derived from VIEW::ModularWidget.
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
					Calls VIEW::ModularWidget::registerWidget
					\return			MolecularFileDialog new constructed MolecularFileDialog
					\see				VIEW::ModularWidget
			 */
			MolecularFileDialog(QWidget* parent)
				throw();

			//@} /** @name Destructors */ //@{ 

			/** Destructor.
			 */
			virtual ~MolecularFileDialog()
				throw();

			//@} /** @name Accessors: inspectors and mutators */ //@{ 
			/** Initializes the widget.
			 		Initializes the menu <b> File</b> with the entries <b> Load System</b> and <b> Write System</b>.
					This method is called automatically immediately before the main application is started.
					This method will be called by show from the MainControl object.
					\param	main_control the MainControl object to be initialized with this MolecularFileDialog
					\see		finalizeWidget
					\see		insertMenuEntry
					\see		show
			 */
			virtual void initializeWidget(VIEW::MainControl& main_control)
				throw();

			/** Removes the widget.
			 		Reverses all actions performed in initializeWidget
					(removes menu entries of this MolecularFileDialog).
					This method will be called by aboutToExit from the
					MainControl object.
					\param		main_control the MainControl object to be finalized with
										this MolecularFileDialog
					\see			initializeWidget
					\see			removeMenuEntry
					\see			aboutToExit
			 */
			virtual void finalizeWidget(VIEW::MainControl& main_control)
				throw();

		public slots:

			/** Open a molecular file.
			 		This method tries to open and read a molecular file, selected from a QFileDialog,
					and, if susccesfull, converts is into a System. Then it sends a NewCompositeMessage
					containing the Composite object made from the System to the other ConnectionObject
					objects.
			 		A WindowMessage will be sent to change the status bar text of the main application.
					\see		NewCompositeMessage
					\see		WindowMessage
					\see		ConnectionObject
			 */
			virtual void readFile();

			///
			virtual void openFile(const String& file)
				throw();
		
			///
			virtual void openFile(const String& filename, const String& filetype, 
												const String& system_name)
				throw();

			/** Write a molecular file.
			 		This method takes a System and saves it into a molecular file, selected from a QFileDialog.
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
			
			///
			virtual void fetchPreferences(INIFile &inifile)
					throw();
				
			///
			virtual void writePreferences(INIFile &inifile)
					throw();
				
			//@}
			
			protected:

			bool finish_(const String& filename, const String& system_name, System* system)
				throw();

			enum FileFormats
			{
				PDB_FILE = 0,
				HIN_FILE,
				MOL_FILE,
				MOL2_FILE
			};

			Index file_format_;
			Size x_, y_, width_, height_;
			String working_dir_;
		};

	} // namespace VIEW
}	// namespace BALL

#endif // BALL_VIEW_DIALOGS_MOLECULARFILEDIALOG_H
