// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: fileDialog.C,v 1.2 2003/08/26 15:01:09 amoll Exp $

#include <BALL/VIEW/DIALOGS/fileDialog.h>
 
using std::istream;
using std::ostream;

namespace BALL
{

	namespace VIEW
	{
	  FileDialog::FileDialog(string header, QFileDialog::Mode mode, QWidget* parent, const char* name)
			throw()
			:	QFileDialog(parent, name, TRUE),
				ModularWidget(),
				dir_path_(),
				filename_()
		{
			setCaption(QString(header.c_str()));
			setMode(mode);

			// register the widget with the MainControl
			ModularWidget::registerWidget(this);
		}
			
		FileDialog::~FileDialog()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				std::cout << "Destructing object " << (void *)this 
									<< " of class FileDialog" << std::endl; //<< RTTI::getName<FileDialog>() << std::endl;
			#endif 

			clear();
		}

		void FileDialog::clear()
			throw()
		{
		}

		int FileDialog::exec()
    {
			setDir(QString(dir_path_.c_str()));

			int result = QFileDialog::exec();

			if (result)
			{
				dir_path_ = dirPath().ascii();

				filename_ = selectedFile().ascii();

				openFile_();
			}

			return result;
		}
  
		void FileDialog::openFile_()
			throw()
    {
    }

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/DIALOGS/fileDialog.iC>
#		endif

	} // namespace VIEW

} // namespace BALL
