// $Id: openHINFile.h,v 1.3 2000/12/22 19:12:09 amoll Exp $

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

using namespace std;
using namespace BALL;
using namespace BALL::VIEW;

namespace BALL
{

	namespace MOLVIEW
	{

		/**		OpenHINFile class.
		*/
		class OpenHINFile: 
			public FileDialog
		{
			public:

			/**	@name	Type Definitions
			*/
			//@{
			//@}

			
			/**	@name Constructors and Destructors
			*/
			//@{

			/** 
			*/
			/** Constructor.
					Construct a file dialog for opening hin files.
			*/
			OpenHINFile(QWidget* parent = 0, const char* name = 0);

			/** Destructor.
			*/
			virtual ~OpenHINFile()
				throw();
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
			/**     Initialize the file dialog.
							This method is called automatically
							immediately before the main application 
							is started. It adds the dialog's 
							menu entries and connections.
			*/
			virtual void initializeWidget(MainControl& main_control);
			
			/**     Remove the dialog.
							This method is called by the dialog's destructor.
							It reverses all actions performed in 
							initializeWidget (remove menu entries and connections).
			*/
			virtual void finalizeWidget(MainControl& main_control);
			//@}

			/**	@name	Predicates
			*/
			//@{
			//@}

   		protected:

			/**	@name	Protected Members
			*/
			//@{

			/** Open the hin file.
					Opens the hin file and sents a message to the other dialogs that a new
					hin file was inserted.
					(sents message: NewCompositeMessage)
			*/
			virtual void openFile_();
			//@}


			private:
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/GUI/DIALOGS/openHINFile.iC>
#		endif 

		} // namespace VIEW

	} // namespace BALL

#endif // BALL_MOLVIEW_GUI_DIALOGS_OPENHINFILE_H
