// $Id: openPDBFile.h,v 1.1 2000/09/23 15:38:04 hekl Exp $

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

using namespace std;
using namespace BALL;
using namespace BALL::VIEW;

namespace BALL
{

	namespace MOLVIEW
	{

		/**		OpenPDBFile class.
		*/
		class OpenPDBFile: 
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
			OpenPDBFile(QWidget* parent = 0, const char* name = 0);

			virtual ~OpenPDBFile();
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
#			include <BALL/MOLVIEW/GUI/DIALOGS/openPDBFile.iC>
#		endif 

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_GUI_DIALOGS_OPENPDBFILE_H
