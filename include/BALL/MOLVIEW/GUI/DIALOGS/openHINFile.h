// $Id: openHINFile.h,v 1.1 2000/09/23 15:38:03 hekl Exp $

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
			OpenHINFile(QWidget* parent = 0, const char* name = 0);

			virtual ~OpenHINFile();
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
			//@}

			/**	@name	Predicates
			*/
			//@{
			//@}

   		protected:

			/**	@name	Protected Members
			*/
			//@{
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
