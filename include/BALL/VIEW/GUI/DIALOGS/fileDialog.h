// $Id: fileDialog.h,v 1.3 2000/10/07 15:27:44 hekl Exp $

#ifndef BALL_VIEW_GUI_DIALOGS_FILEDIALOG_H
#define BALL_VIEW_GUI_DIALOGS_FILEDIALOG_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_GUI_WIDGETS_MODULARWIDGET_H
# include <BALL/VIEW/GUI/WIDGETS/modularWidget.h>
#endif

#include <qfiledialog.h>

using namespace std;
using namespace BALL;
using namespace BALL::VIEW;

namespace BALL
{

	namespace VIEW
	{

		/**		FileDialog class.
		*/
		class FileDialog: 
			public QFileDialog,
			public ModularWidget
		{
			Q_OBJECT

			public:

			/**	@name	Type Definitions
			*/
			//@{
			//@}

			
			/**	@name Constructors and Destructors
			*/
			//@{

			/** Constructor. 
					contruct a file dialog.
			*/
			FileDialog(string header, QFileDialog::Mode mode, QWidget* parent = 0, const char* name = 0);

			/** Destructor.
			*/
			virtual ~FileDialog();
			//@}

			/**	@name Exceptions
			*/
			//@{
			//@}

			/**	@name	Assignment
			*/
			//@{
			//@}

			/**	@name	Accessors
			*/
			//@{

			/** return the full path name.
			*/
			const string& getPathName() const;

			/** return only the filename.
			*/
			const string& getFileName() const;
			//@}

			/** @name Modular Widget related methods
			 */
			//@{
			
			/**     Initialize the file dialog.
							This method is called automatically
							immediately before the main application 
							is started. It adds the dialog's 
							menu entries and connections.
							Add here your own menu initialisiations.
			*/
			virtual void initializeWidget(MainControl& main_control);
			
			/**     Remove the dialog.
							This method is called by the dialog's destructor.
							It reverses all actions performed in 
							initializeWidget (remove menu entries and connections).
			*/
			virtual void finalizeWidget(MainControl& main_control);
			
			/**     Update all menu entry states.
							This method is called just before a popup menu
							is shown (via the QT signal aboutToShow()).
							It should be used to update the state of 
							menu entries (e.g. disable or enable entries).
			*/
			virtual void checkMenu(MainControl& main_control);
			//@}

			public slots:

			/** @name Public slot
			*/
			//@{

			/** start the file dialog.
				  This function opens the dialog.
			*/
			int exec();

			//@}

			/**	@name	Predicates
			*/
			//@{
			//@}

   		protected:

			/**	@name	Protected Members
			*/
			//@{
			
			/** open the selected file.
					Overridable method for defining what should be done with the selected file.
					(open the file and process it, for example).
			*/
			virtual void openFile_();
			//@}


			private:

			string dir_path_;
			string filename_;
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/DIALOGS/fileDialog.iC>
#		endif 

		} // namespace VIEW

	} // namespace BALL

#endif // BALL_VIEW_GUI_DIALOGS_FILEDIALOG_H
