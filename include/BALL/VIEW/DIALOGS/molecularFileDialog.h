// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: molecularFileDialog.h,v 1.13 2004/04/18 15:29:45 amoll Exp $
//

#ifndef BALL_VIEW_DIALOGS_MOLECULARFILEDIALOG_H
#define BALL_VIEW_DIALOGS_MOLECULARFILEDIALOG_H

#ifndef BALL_VIEW_KERNEL_MODULARWIDGET_H
# include <BALL/VIEW/KERNEL/modularWidget.h>
#endif

#ifndef BALL_VIEW_KERNEL_COMMON_H
# include <BALL/VIEW/KERNEL/common.h>
#endif

#include <qfiledialog.h>

namespace BALL
{
	class System;

	namespace VIEW
	{

		/** Dialog for opening molecular data files.
				This class is used to read or write Molecular files in one of several
				file formats. Currently, PDB, HIN and MOL2 are supported.
				Upon reading a file, the information will be stored in a System.
				This class can also take a System and write it in one of the
				supported file formats.
				This class is derived from ModularWidget.
				\ingroup ViewDialogs
		 */
		class BALL_EXPORT MolecularFileDialog
			: public QWidget,
				public ModularWidget
		{
			Q_OBJECT
			public:
			BALL_EMBEDDABLE(MolecularFileDialog, ModularWidget)

			/** @name Constructors
			 */
			//@{

			/** Default Constructor.
					Calls ModularWidget::registerWidget()
			 */
			MolecularFileDialog(QWidget* parent = 0, const char* name = "<MolecularFileDialog>")
				throw();
			//
			//@} 
			/** @name Destructors 
			*/ 
			//@{ 

			/** Destructor.
			 */
			virtual ~MolecularFileDialog()
				throw();

			//@} 
			/** @name Accessors: inspectors and mutators 
			*/ 
			//@{ 
			
			/** Initializes the menuentries in <b>File</b>.
					This method is called automatically immediately before the main application is started.
					This method will be called MainControl::show().
					\param	main_control the MainControl object to be initialized 
					\see		finalizeWidget
			 */
			virtual void initializeWidget(MainControl& main_control)
				throw();

			/** Removes the widget and its menu entries.
					This method will be called by MainControl::aboutToExit().
					\param		main_control the MainControl object to be finalized
					\see			initializeWidget
			 */
			virtual void finalizeWidget(MainControl& main_control)
				throw();

		public slots:

			/** Open a molecular file.
					This method tries to open and read a molecular file, selected from a QFileDialog,
					and, if susccesfull, converts is into a System. Then it sends a CompositeMessage
					containing the System to the other ConnectionObject instances.
					\see		CompositeMessage
					\see		ConnectionObject
			 */
			virtual void readFiles();

			/** Open a given molecular file.
					The file type is identified by the filename extension.
			*/
			virtual void openFile(const String& file)
				throw();

			/** Wrapper for the read methods.
					The filetype String is used to indentify the file type (HIN, PDP, MOL, MOL2).
					It is possible to give the system a designated name, otherwise it is named by the file.
			*/
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
			
			/// Overloaded from ModularWidget
			virtual void fetchPreferences(INIFile &inifile)
				throw();
				
			/// Overloaded from ModularWidget
			virtual void writePreferences(INIFile &inifile)
				throw();

			/// Overloaded from ModularWidget
			virtual void checkMenuEntries()
				throw();
				
			//@}
			
			protected:

			// Only for Python interface
			MolecularFileDialog(const MolecularFileDialog& mfd)
				throw();


			bool finish_(const String& filename, const String& system_name, System* system)
				throw();

			enum FileFormats
			{
				PDB_FILE = 0,
				HIN_FILE,
				MOL_FILE,
				MOL2_FILE
			};

			Index save_id_;
			String working_dir_;
		};

	} // namespace VIEW
}	// namespace BALL

#endif // BALL_VIEW_DIALOGS_MOLECULARFILEDIALOG_H
