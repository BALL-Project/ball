// $Id: openHINFile.h,v 1.4 2001/05/13 14:55:24 hekl Exp $

#ifndef BALL_MOLVIEW_GUI_DIALOGS_OPENHINFILE_H
#define BALL_MOLVIEW_GUI_DIALOGS_OPENHINFILE_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_FORMAT_HINFILE_H
#	include <BALL/FORMAT/HINFile.h>
#endif

#ifndef BALL_VIEW_KERNEL_LOGVIEW_H
#	include <BALL/VIEW/KERNEL/logView.h>
#endif

#ifndef BALL_VIEW_GUI_DIALOGS_FILEDIALOG_H
#	include <BALL/VIEW/GUI/DIALOGS/fileDialog.h>
#endif

//using namespace std;
//using namespace BALL;
using namespace BALL::VIEW;

namespace BALL
{

	namespace MOLVIEW
	{

		/** OpenHINFile class.
				{\bf Framework:} BALL/MOLVIEW/GUI/DIALOGS\\
				{\bf Definition:} \URL{BALL/MOLVIEW/GUI/DIALOGS/openHINFile.h}\\ \\
				The class OpenHINFile is a class for reading hin files into \Ref{System} objects.
				It is derived from
				the class \Ref{FileDialog}. If a hin file is read it will be converted
				into a \Ref{Composite} object. See \Ref{openFile} for information
				concerning conversion and notification.
				@memo    OpenHINFile class (BALL MOLVIEW gui dialogs framework)
				@author  $Author: hekl $
				@version $Revision: 1.4 $
				@date    $Date: 2001/05/13 14:55:24 $
		*/
		class OpenHINFile: public FileDialog
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new openHINFile.
					Set the title and the mode of {\em *this} openHINFile to {\em Import HIN file}.
					@param       parent the parent \Ref{QWidget} of {\em *this} openHINFile (See QFileDialog in the QT documentation)
					@param       name the name of {\em *this} openHINFile (See QFileDialog in the QT documentation)
					@return      OpenHINFile new constructed openHINFile
					@see         FileDialog
			*/
			OpenHINFile(QWidget* parent = 0, const char* name = 0)
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} openHINFile.
			*/
			virtual ~OpenHINFile()
				throw();
			//@}

			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{
			/**	Initialize the widget.
					Initialize the menu {\em File} with its submenu {\em Import}.
					Add menu entry {\em HIN File}.
					This method is called automatically	immediately before the main application 
					is started. 
					This method will be called by \Ref{show} from the \Ref{MainControl} object.
					@param main_control the \Ref{MainControl} object to be initialized with {\em *this} openHINFile
					@see   finalizeWidget
					@see   insertMenuEntry
					@see   show
			*/
			virtual void initializeWidget(MainControl& main_control)
				throw();
			
			/**	Remove the widget.
					Reverse all actions performed in \Ref{initializeWidget}
					(remove menu entries of {\em *this} openHINFile).
					This method will be called by \Ref{aboutToExit} from the \Ref{MainControl}
					object.
					@param main_control the \Ref{MainControl} object to be finalized with {\em *this} openHINFile
					@see   initializeWidget
					@see   removeMenuEntry
					@see   aboutToExit
			*/
			virtual void finalizeWidget(MainControl& main_control)
				throw();
			//@}

   		protected:

			/**	@name	HIN file reading
			*/
			//@{

			/** Open the hin file.
					Open the selected hin file and if successfull convert it into a \Ref{System}
					and sent the message \Ref{NewCompositeMessage} containing the \Ref{Composite}
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
#			include <BALL/MOLVIEW/GUI/DIALOGS/openHINFile.iC>
#		endif 

		} // namespace VIEW

	} // namespace BALL

#endif // BALL_MOLVIEW_GUI_DIALOGS_OPENHINFILE_H
