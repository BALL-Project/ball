// $Id: openPDBFile.h,v 1.4 2001/05/13 14:55:24 hekl Exp $

#ifndef BALL_MOLVIEW_GUI_DIALOGS_OPENPDBFILE_H
#define BALL_MOLVIEW_GUI_DIALOGS_OPENPDBFILE_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_FORMAT_PDBFILE_H
#	include <BALL/FORMAT/PDBFile.h>
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

		/** OpenPDBFile class.
				{\bf Framework:} BALL/MOLVIEW/GUI/DIALOGS\\
				{\bf Definition:} \URL{BALL/MOLVIEW/GUI/DIALOGS/openPDBFile.h}\\ \\
				The class OpenPDBFile is a class for reading pdb files into \Ref{System} objects.
				It is derived from
				the class \Ref{FileDialog}. If a pdb file is read it will be converted
				into a \Ref{Composite} object. See \Ref{openFile} for information
				concerning conversion and notification.
				@memo    OpenPDBFile class (BALL MOLVIEW gui dialogs framework)
				@author  $Author: hekl $
				@version $Revision: 1.4 $
				@date    $Date: 2001/05/13 14:55:24 $
		*/
		class OpenPDBFile: public FileDialog
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new openPDBFile.
					Set the title and the mode of {\em *this} openPDBFile to {\em Import PDB file}.
					@param       parent the parent \Ref{QWidget} of {\em *this} openPDBFile (See QFileDialog in the QT documentation)
					@param       name the name of {\em *this} openPDBFile (See QFileDialog in the QT documentation)
					@return      OpenPDBFile new constructed openPDBFile
					@see         FileDialog
			*/
			OpenPDBFile(QWidget* parent = 0, const char* name = 0)
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} openPDBFile.
			*/
			virtual ~OpenPDBFile()
				throw();
			//@}

			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{
			/**	Initialize the widget.
					Initialize the menu {\em File} with its submenu {\em Import}.
					Add menu entry {\em PDB File}.
					This method is called automatically	immediately before the main application 
					is started. 
					This method will be called by \Ref{show} from the \Ref{MainControl} object.
					@param main_control the \Ref{MainControl} object to be initialized with {\em *this} openPDBFile
					@see   finalizeWidget
					@see   insertMenuEntry
					@see   show
			*/
			virtual void initializeWidget(MainControl& main_control)
				throw();
			
			/**	Remove the widget.
					Reverse all actions performed in \Ref{initializeWidget}
					(remove menu entries of {\em *this} openPDBFile).
					This method will be called by \Ref{aboutToExit} from the \Ref{MainControl}
					object.
					@param main_control the \Ref{MainControl} object to be finalized with {\em *this} openPDBFile
					@see   initializeWidget
					@see   removeMenuEntry
					@see   aboutToExit
			*/
			virtual void finalizeWidget(MainControl& main_control)
				throw();
			//@}


   		protected:

			/**	@name	PDB file reading
			*/
			//@{

			/** Open the pdb file.
					Open the selected pdb file and if successfull convert it into a \Ref{System}
					and sent the message \Ref{NewCompositeMessage} containing the \Ref{Composite}
					object to the other \Ref{ConnectionObject} objects.
					All text output will be written into \Ref{Log}. The contents of the \Ref{Log}
					can be viewed with the widget \Ref{LogView}.
					A \Ref{WindowMessage} will
					be sent to change the status bar text of the main application.
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
#			include <BALL/MOLVIEW/GUI/DIALOGS/openPDBFile.iC>
#		endif 

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_GUI_DIALOGS_OPENPDBFILE_H
