// $Id: fileDialog.h,v 1.2 2000/09/23 14:15:02 hekl Exp $

#ifndef BALL_VIEW_GUI_DIALOGS_FILEDIALOG_H
#define BALL_VIEW_GUI_DIALOGS_FILEDIALOG_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_KERNEL_CONNECTIONOBJECT_H
#	include <BALL/VIEW/KERNEL/connectionObject.h>
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
			public ConnectionObject
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
			FileDialog(string header, QFileDialog::Mode mode, QWidget* parent = 0, const char* name = 0);

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
			int exec();

			const string& getPathName() const;

			const string& getFileName() const;
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

			string dir_path_;
			string filename_;
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/DIALOGS/fileDialog.iC>
#		endif 

		} // namespace VIEW

	} // namespace BALL

#endif // BALL_VIEW_GUI_DIALOGS_FILEDIALOG_H
