// $Id: fileDialog.h,v 1.7 2001/12/28 02:33:39 oliver Exp $

#ifndef BALL_VIEW_GUI_DIALOGS_FILEDIALOG_H
#define BALL_VIEW_GUI_DIALOGS_FILEDIALOG_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_GUI_WIDGETS_MODULARWIDGET_H
# include <BALL/VIEW/GUI/WIDGETS/modularWidget.h>
#endif

#include <qfiledialog.h>

namespace BALL
{

	namespace VIEW
	{

		/** FileDialog class.
				The class FileDialog is a base class for file dialogs. Its base classes are
				\Ref{QFileDialog} (See QT documentation) and \Ref{ModularWidget}.
				The virtual method \Ref{openFile_} can be overridden for specifying
				what should be done with the selected file. With the method \Ref{exec}
				{\em *this} fileDialog can be opened. If a file was selected the path and the file
				name can be accessed with the methods \Ref{getPathName} and \Ref{getFileName}. \\
				{\bf Definition:} \URL{BALL/VIEW/GUI/DIALOGS/fileDialog.h}\
		*/
		class FileDialog
			: public QFileDialog,	public ModularWidget
		{
			Q_OBJECT

			public:

			BALL_EMBEDDABLE(FileDialog)

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new fileDialog.
					Set the title and the mode of {\em *this} fileDialog to {\em header}.
					See QT documentation for information concerning modes for file dialogs.
					Calls \Ref{ModularWidget::registerWidget}
					@param       header the title of {\em *this} fileDialog
					@param       mode the mode of {\em *this} fileDialog (See QFileDialog in the QT documentation)
					@param       parent the parent \Ref{QWidget} of {\em *this} fileDialog (See QFileDialog in the QT documentation)
					@param       name the name of {\em *this} fileDialog (See QFileDialog in the QT documentation)
					@return      FileDialog new constructed fileDialog
					@see         ModularWidget
			*/
			FileDialog(string header, QFileDialog::Mode mode, QWidget* parent = 0, const char* name = 0)
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} fileDialog.
					Calls \Ref{destroy}.
					@see         destroy
			*/
			virtual ~FileDialog()
				throw();

			/** Explicit default initialization.
					Empty for further purpose.
			*/
			virtual void clear()
				throw();

			/** Explicit destructor.
					Empty for further purpose.
			*/
			virtual void destroy()
				throw();
			//@}

			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{

			/** Non-mutable inspection of the full path name.
					Access a constant reference of the full path name of the selected file of 
					{\em *this} fileDialog.
					@return  string& a constant reference of the full path name of the selected file of {\em *this} fileDialog
			*/
			const string& getPathName() const
				throw();

			/** Non-mutable inspection of the file name.
					Access a constant reference of the file name of the selected file of 
					{\em *this} fileDialog.
					@return  string& a constant reference of the file name of the selected file of {\em *this} fileDialog
			*/
			const string& getFileName() const
				throw();
			//@}


			public slots:

			/** @name Public slots
			*/
			//@{

			/** Start the file dialog.
					Open {\em *this} fileDialog.
					If a file was successfully selected \Ref{openFile_} will be called.
					@return int the result code as returned by the \Ref{QFileDialog} (See QT documentation)
					@see    openFile_
			*/
			int exec();

			//@}

   		protected:

			/**	@name	Protected Members
			*/
			//@{
			
			/** Open the selected file.
					Overridable method for defining what should be done with the selected file.
					(open the file and process it, for example).
					@see exec
			*/
			virtual void openFile_()
				throw();
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
