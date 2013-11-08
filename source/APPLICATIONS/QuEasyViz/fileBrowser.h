#ifndef FILEBROWSER_H
#define FILEBROWSER_H

#include <QtGui/QDirModel>
#include <QtGui/QTreeView>
#include <QtGui/QFileIconProvider>


namespace BALL
{
	namespace VIEW
	{

		class MainWindow;		
		class FileBrowser;
		
		class FileIconProvider : public QFileIconProvider
		{
			public:
				FileIconProvider(FileBrowser* file_browser);
				
				/** overloads the function of the base-class in such a way that specific icons are returned for sd- and csv-files */
				QIcon icon (const QFileInfo& info) const;
					
			private:
				FileBrowser* file_browser_;
		};
			
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
				FileBrowser(std::string path, MainWindow* main_window);
				
				/** destructor */
				~FileBrowser();
				
				MainWindow* main_window;
		
			private:
				/** @name Private Attributes*/
			
				/** QDirModel connected to the browser */
				QDirModel* dirmodel_;
				FileIconProvider* icon_provider_;
				
				// overloads QAbstractItemView::currentChanged() in order to refresh QDirModel
				void currentChanged (const QModelIndex & current, const QModelIndex& previous);
		};
	}
}

#endif
