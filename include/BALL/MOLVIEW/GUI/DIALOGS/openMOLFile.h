// $Id: openMOLFile.h,v 1.1.2.3 2002/10/21 15:39:34 amoll Exp $

#ifndef BALL_MOLVIEW_GUI_DIALOGS_OPENMOLFILE_H
#define BALL_MOLVIEW_GUI_DIALOGS_OPENMOLFILE_H

#ifndef BALL_VIEW_GUI_DIALOGS_FILEDIALOG_H
#	include <BALL/VIEW/GUI/DIALOGS/fileDialog.h>
#endif

using namespace BALL::VIEW;

namespace BALL
{
	namespace MOLVIEW
	{

		/** OpenMOLFile class.
				The class OpenMOLFile is a class for reading hin files into \Ref{System} objects.
				It is derived from
				the class \Ref{FileDialog}. If a hin file is read it will be converted
				into a \Ref{Composite} object. See \Ref{openFile} for information
				concerning conversion and notification. \\
				{\bf Definition:} \URL{BALL/MOLVIEW/GUI/DIALOGS/openMOLFile.h}
		*/
		class OpenMOLFile
			: public FileDialog
		{
			public:
			BALL_EMBEDDABLE(OpenMOLFile)

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Constructs new openMOLFile.
					Sets the title and the mode of {\em *this} openMOLFile to {\em Import MOL file}.
					@param       parent the parent \Ref{QWidget} of {\em *this} openMOLFile (See QFileDialog in the QT documentation)
					@param       name the name of {\em *this} openMOLFile (See QFileDialog in the QT documentation)
					@return      OpenMOLFile new constructed openMOLFile
					@see         FileDialog
			*/
			OpenMOLFile(QWidget* parent = 0, const char* name = 0)
				throw();

			//@}
			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} openMOLFile.
			*/
			virtual ~OpenMOLFile()
				throw();
							
			//@}
			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{

			/**	Initializes the widget.
					Initializes the menu {\em File} with its submenu {\em Import}.
					Adds menu entry {\em MOL File}.
					This method is called automatically	immediately before the main application 
					is started. 
					This method will be called by \Ref{show} from the \Ref{MainControl} object.
					@param main_control the \Ref{MainControl} object to be initialized with {\em *this} openMOLFile
					@see   finalizeWidget
					@see   insertMenuEntry
					@see   show
			*/
			virtual void initializeWidget(MainControl& main_control)
				throw();
			
			/**	Removes the widget.
					Reverses all actions performed in \Ref{initializeWidget}
					(removes menu entries of {\em *this} openMOLFile).
					This method will be called by \Ref{aboutToExit} from the \Ref{MainControl}
					object.
					@param main_control the \Ref{MainControl} object to be finalized with {\em *this} openMOLFile
					@see   initializeWidget
					@see   removeMenuEntry
					@see   aboutToExit
			*/
			virtual void finalizeWidget(MainControl& main_control)
				throw();
							
   		protected:

			//@}
			/*_	@name	MOL file reading
			*/
			//@{

			/*_ Opens the hin file.
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
		};

		} // namespace VIEW

	} // namespace BALL

#endif // BALL_MOLVIEW_GUI_DIALOGS_OPENMOLFILE_H
