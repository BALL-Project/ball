// $Id: fileDialog.C,v 1.4 2001/05/13 14:28:35 hekl Exp $

#include <BALL/VIEW/GUI/DIALOGS/fileDialog.h>
 
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
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<FileDialog>() << endl;
			#endif 

			destroy();
		}

		void FileDialog::clear()
			throw()
		{
		}

		void FileDialog::destroy()
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
#			include <BALL/VIEW/GUI/DIALOGS/fileDialog.iC>
#		endif

	} // namespace VIEW

} // namespace BALL
