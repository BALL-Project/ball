// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_DIALOGS_MOLECULARFILEDIALOG_H
#define BALL_VIEW_DIALOGS_MOLECULARFILEDIALOG_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifndef BALL_VIEW_KERNEL_MODULARWIDGET_H
# include <BALL/VIEW/KERNEL/modularWidget.h>
#endif

#ifndef BALL_VIEW_KERNEL_COMMON_H
# include <BALL/VIEW/KERNEL/common.h>
#endif

class QMenu;

namespace BALL
{
	class System;

	namespace VIEW
	{

		/** Dialog for opening molecular data files.
				This class is used to read or write Molecular files in one of several
				file formats. Currently supported file formats are:\par
				HIN, MOL, MOL2, PDB, SD, XYZ\par
				Upon reading a file, the information will be stored in a System.
				This class can also take a System and write it in one of the
				supported file formats.
				This class is derived from ModularWidget.
				@see HINFile
				@see MOLFile
				@see MOL2File
				@see PDBFile
				@see SDFile
				@see XYZFile
				\ingroup ViewDialogs
		 */
		class BALL_VIEW_EXPORT MolecularFileDialog
			: public QWidget,
				public ModularWidget
		{
			Q_OBJECT
			public:
			BALL_EMBEDDABLE(MolecularFileDialog, ModularWidget)

			/** Default Constructor.
					Calls ModularWidget::registerWidget()
			 */
			MolecularFileDialog(QWidget* parent = 0, const char* name = "MolecularFileDialog");

			/// Destructor.
			virtual ~MolecularFileDialog();

			/** Initializes the menu entries in <b>File</b>.
					This method is called automatically immediately before the main application is started.
					This method will be called MainControl::show().
					\param	main_control the MainControl object to be initialized 
					\see		finalizeWidget
			 */
			virtual void initializeWidget(MainControl& main_control);

			/** Test if this ModularWidget can handle a given file format.
					(Overloaded from ModularWidget)
			 		@param fileform short string with the file extension (e.g. PDB)
					@see openFile
			*/
			virtual bool canHandle(const String& fileformat) const;

			/** Tell this ModularWidget to open a given file.
					(Overloaded from ModularWidget)
			 		@see canHandle
			*/
			virtual bool openFile(const String& filename);

			/** Open a given molecular file.
					The file type is identified by the filename extension.
					This method can be used to load molecular files, which were given as 
					command line arguments.
			*/
			virtual System* openMolecularFile(const String& file);

			/** Wrapper for the read methods.
					The filetype String is used to indentify the file type (HIN, PDP, MOL, MOL2).
					It is possible to give the system a designated name, otherwise it is named by the file.
			*/
			virtual System* openMolecularFile(const String& filename, 
																				const String& filetype, 
																				const String& system_name);


			///
			System* openPDBFile();

			///
			System* openHINFile();

			///
			System* openMOLFile();

			///
			System* openMOL2File();

			///
			System* openSDFile();
			
			///
			System* openACFile();

			///
			System* openXYZFile();

			/** Read a PDB file
			 */
			System* readPDBFile(String filename, String system_name);

			/** Read a HIN file
			 */
			System* readHINFile(String filename, String system_name);

			/** Read a MOL file
			 */
			System* readMOLFile(String filename, String system_name);

			/** Read a MOL2 file
			 */
			System* readMOL2File(String filename, String system_name);

			/** Read a SD file
			 */
			System* readSDFile(String filename, String system_name);
			
			/** Read an AC file
			 */
			System* readACFile(String filename, String system_name);
			
			/** Read a XYZ file
			 */
			System* readXYZFile(String filename, String system_name);

			/** Write a PDB file
			 */
			bool writePDBFile(String filename, const System& system);
			
			/** Write a HIN file
			 */
			bool writeHINFile(String filename, const System& system);
			
			/** Write a MOL file
			 */
			bool writeMOLFile(String filename, const System& system);
			
			/** Write a MOL2 file
			 */
			bool writeMOL2File(String filename, const System& system);

			/** Write a SD file
			 */
			bool writeSDFile(String filename, const System& system);

			/** Write an AC file
			 */
			bool writeACFile(String filename, const System& system);

			/** Write a XYZ file
			 */
			bool writeXYZFile(String filename, const System& system);
			
			/// Overloaded from ModularWidget
			virtual void checkMenu(MainControl& main_control);

			///
			virtual String getSupportedFileFormats() const;

			///
			virtual String getSupportedFileFormatsList() const;

			void setReadPDBModels(bool read) { read_all_pdb_models_ = read; }

			public slots:

			/** Open a molecular file.
					This method tries to open and read a molecular file, selected from a QFileDialog,
					and, if susccesfull, converts is into a System. Then it sends a CompositeMessage
					containing the System to the other ConnectionObject instances.
					\see		CompositeMessage
					\see		ConnectionObject
			 */
			virtual void readFiles();

			/** Write a molecular file.
					This method takes a System and saves it into a molecular file, selected from a QFileDialog.
			 */
			virtual bool writeFile();


			protected:

			virtual void onNotify(Message *message);

			// Only for Python interface
			MolecularFileDialog(const MolecularFileDialog& mfd);


			virtual bool finish_(const String& filename, const String& system_name, System* system);

			System* openFile_(String type);

			enum FileFormats
			{
				PDB_FILE = 0,
				HIN_FILE,
				MOL_FILE,
				MOL2_FILE,
				SD_FILE,
				XYZ_FILE
			};

			QAction* save_id_, *open_id_;
			String file_format_;
			bool read_all_pdb_models_;
		};

	} // namespace VIEW
}	// namespace BALL

#endif // BALL_VIEW_DIALOGS_MOLECULARFILEDIALOG_H
