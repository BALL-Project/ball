// $Id: openMOL2File.h,v 1.2 2001/12/28 02:33:38 oliver Exp $

#ifndef BALL_MOLVIEW_GUI_DIALOGS_OPENMOL2FILE_H
#define BALL_MOLVIEW_GUI_DIALOGS_OPENMOL2FILE_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_FORMAT_MOL2FILE_H
#	include <BALL/FORMAT/MOL2File.h>
#endif

#ifndef BALL_VIEW_KERNEL_LOGVIEW_H
#	include <BALL/VIEW/KERNEL/logView.h>
#endif

#ifndef BALL_VIEW_GUI_DIALOGS_FILEDIALOG_H
#	include <BALL/VIEW/GUI/DIALOGS/fileDialog.h>
#endif

using namespace BALL::VIEW;

namespace BALL
{

	namespace MOLVIEW
	{

		/** OpenMOL2File class.
				The class OpenMOL2File is a class for reading hin files into \Ref{System} objects.
				It is derived from
				the class \Ref{FileDialog}. If a hin file is read it will be converted
				into a \Ref{Composite} object. See \Ref{openFile} for information
				concerning conversion and notification. \\
				{\bf Definition:} \URL{BALL/MOLVIEW/GUI/DIALOGS/openMOL2File.h}
		*/
		class OpenMOL2File
			: public FileDialog
		{
			public:
			BALL_EMBEDDABLE(OpenMOL2File)

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Constructs new openMOL2File.
					Sets the title and the mode of {\em *this} openMOL2File to {\em Import MOL2 file}.
					@param       parent the parent \Ref{QWidget} of {\em *this} openMOL2File (See QFileDialog in the QT documentation)
					@param       name the name of {\em *this} openMOL2File (See QFileDialog in the QT documentation)
					@return      OpenMOL2File new constructed openMOL2File
					@see         FileDialog
			*/
			OpenMOL2File(QWidget* parent = 0, const char* name = 0)
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} openMOL2File.
			*/
			virtual ~OpenMOL2File()
				throw();
			//@}

			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{
			/**	Initializes the widget.
					Initializes the menu {\em File} with its submenu {\em Import}.
					Adds menu entry {\em MOL2 File}.
					This method is called automatically	immediately before the main application 
					is started. 
					This method will be called by \Ref{show} from the \Ref{MainControl} object.
					@param main_control the \Ref{MainControl} object to be initialized with {\em *this} openMOL2File
					@see   finalizeWidget
					@see   insertMenuEntry
					@see   show
			*/
			virtual void initializeWidget(MainControl& main_control)
				throw();
			
			/**	Removes the widget.
					Reverses all actions performed in \Ref{initializeWidget}
					(removes menu entries of {\em *this} openMOL2File).
					This method will be called by \Ref{aboutToExit} from the \Ref{MainControl}
					object.
					@param main_control the \Ref{MainControl} object to be finalized with {\em *this} openMOL2File
					@see   initializeWidget
					@see   removeMenuEntry
					@see   aboutToExit
			*/
			virtual void finalizeWidget(MainControl& main_control)
				throw();
			//@}

   		protected:

			/**	@name	MOL2 file reading
			*/
			//@{

			/** Opens the hin file.
					Opens the selected hin file and if successful converts it into a \Ref{System}
					and sents the message \Ref{NewCompositeMessage} containing the \Ref{Composite}
					object to the other \Ref{ConnectionObject} objects.
					If the hin file contains a bounding box a transparent \Ref{GLSimpleBox} will 
					be constructed and added to the \Ref{System}.
					All text output will be written into \Ref{Log}. The contents of the \Ref{Log}
					can be viewed with the widget \Ref{LogView}.
					A \Ref{WindowMessage} will
					be sent to change the status bar text of the main application.
					@see GLSimpleBox
					@see Log
					@see LogView
					@see NewCompositeMessage
					@see WindowMessage
					@see ConnectionObject
			*/
			virtual void openFile_()
				throw();
			//@}


			private:
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/GUI/DIALOGS/openMOL2File.iC>
#		endif 

		} // namespace VIEW

	} // namespace BALL

#endif // BALL_MOLVIEW_GUI_DIALOGS_OPENMOL2FILE_H
