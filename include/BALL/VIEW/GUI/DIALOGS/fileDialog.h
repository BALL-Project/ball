// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: fileDialog.h,v 1.13 2003/03/03 14:18:26 anhi Exp $

#ifndef BALL_VIEW_GUI_DIALOGS_FILEDIALOG_H
#define BALL_VIEW_GUI_DIALOGS_FILEDIALOG_H

#include <qfiledialog.h>

#ifndef BALL_VIEW_GUI_WIDGETS_MODULARWIDGET_H
# include <BALL/VIEW/GUI/WIDGETS/modularWidget.h>
#endif

namespace BALL
{
	namespace VIEW
	{
    /**  \addtogroup  ViewGuiDialogs
     *  @{
     */
		/** FileDialog class.
				The class FileDialog is a base class for file dialogs. Its base classes are
				 \link QFileDialog QFileDialog \endlink  (See QT documentation) and  \link ModularWidget ModularWidget \endlink .
				The virtual method  \link openFile_ openFile_ \endlink  can be overridden for specifying
				what should be done with the selected file. With the method  \link exec exec \endlink 
				{\em *this} fileDialog can be opened. If a file was selected the path and the file
				name can be accessed with the methods  \link getPathName getPathName \endlink  and  \link getFileName getFileName \endlink .  \par
				<b>Definition:</b> BALL/VIEW/GUI/DIALOGS/fileDialog.h
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
					Calls  \link ModularWidget::registerWidget ModularWidget::registerWidget \endlink 
					@param       header the title of {\em *this} fileDialog
					@param       mode the mode of {\em *this} fileDialog (See QFileDialog in the QT documentation)
					@param       parent the parent  \link QWidget QWidget \endlink  of {\em *this} fileDialog (See QFileDialog in the QT documentation)
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
					Calls  \link clear clear \endlink .
			*/
			virtual ~FileDialog()
				throw();

			/** Explicit default initialization.
					Empty for further purpose.
			*/
			virtual void clear()
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


			public slots:

			//@}
			/** @name Public slots
			*/
			//@{

			/** Start the file dialog.
					Open {\em *this} fileDialog.
					If a file was successfully selected  \link openFile_ openFile_ \endlink  will be called.
					@return int the result code as returned by the  \link QFileDialog QFileDialog \endlink  (See QT documentation)
					@see    openFile_
			*/
			int exec();


   		protected:

			//@}
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

#ifndef BALL_NO_INLINE_FUNCTIONS
#	include <BALL/VIEW/GUI/DIALOGS/fileDialog.iC>
#endif 
  /** @} */
		} // namespace VIEW

	} // namespace BALL

#endif // BALL_VIEW_GUI_DIALOGS_FILEDIALOG_H
