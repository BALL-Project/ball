#ifndef FILEBROWSER_H
#define FILEBROWSER_H

#include <QtGui/QDirModel>
#include <QtGui/QTreeView>

namespace BALL
{
	namespace VIEW
	{

		/** @class FileBrowser
		* @brief a simple file browser class
		*
		* This class represents a very simple and clear file browser which shows SD-Files in the filesystem
		*/
		class FileBrowser : public QTreeView
		{
			public:
				
				/** @name Constructors and Destructors */
			
				/** constructor */
				FileBrowser();
				
				/** destructor */
				~FileBrowser();
		
			private:
				/** @name Private Attributes*/
			
				/** QDirModel connected to the browser */
				QDirModel* dirmodel_;
		};
	}
}

#endif
