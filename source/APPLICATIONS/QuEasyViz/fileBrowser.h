/* fileBrowser.h
 * 
 * Copyright (C) 2009 Marcel Schumann
 * 
 * This file is part of QuEasy -- A Toolbox for Automated QSAR Model
 * Construction and Validation.
 * QuEasy is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 * 
 * QuEasy is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

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
