// $Id: fileDialog.C,v 1.1 2000/09/23 13:28:31 hekl Exp $

#include <BALL/VIEW/GUI/DIALOGS/fileDialog.h>
 
using std::istream;
using std::ostream;

namespace BALL
{

	namespace VIEW
	{
	  FileDialog::FileDialog(string header, QFileDialog::Mode mode, QWidget* parent, const char* name)
			:	QFileDialog(parent, name, TRUE),
				ConnectionObject(),
				dir_path_(),
				filename_()
		{
			setCaption(QString(header.c_str()));
			setMode(mode);
		}
			
		FileDialog::~FileDialog()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<FileDialog>() << endl;
			#endif 
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
    {
    }

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/DIALOGS/fileDialog.iC>
#		endif

	} // namespace VIEW

} // namespace BALL
