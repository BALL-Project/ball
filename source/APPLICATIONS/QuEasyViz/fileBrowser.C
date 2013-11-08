#include <BALL/VIEW/KERNEL/iconLoader.h>
#include <fileBrowser.h>
#include <QtGui/QHeaderView>
#include <BALL/DATATYPE/string.h>
#include <mainWindow.h>

namespace BALL
{
	namespace VIEW
	{

		FileBrowser::FileBrowser(std::string path, MainWindow* main_win)
		{
			main_window = main_win;
			
			///display only sdf, csv and txt-files
			QStringList filters;
			filters << "*.sdf"<<"*.csv"<< "*.txt";
			dirmodel_ = new QDirModel(this);
			icon_provider_ = new FileIconProvider(this);
			dirmodel_->setIconProvider(icon_provider_);

			///set display filters: list all directories, list files, don't list "." and ".."
			dirmodel_->setFilter(QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot);
			dirmodel_->setNameFilters(filters);
			
			///drag and drop related stuff
			setDragEnabled (true);
			setDragDropMode(QAbstractItemView::DragOnly);
			setSelectionMode(QAbstractItemView::ExtendedSelection);

			///hide the header of the tree
			header()->hide();

			setModel(dirmodel_);

			///expand the user's homedir
			QString qpath= QDir::homePath();
			if(path!="") qpath = qpath.fromStdString(path);
			expand(QModelIndex(dirmodel_->index(qpath)));
			scrollTo(QModelIndex(dirmodel_->index(qpath)),QAbstractItemView::PositionAtTop);
			
			///hide all columns except name
			hideColumn(1);
			hideColumn(2);
			hideColumn(3);
		}

		FileBrowser::~FileBrowser()
		{
		}


		void FileBrowser::currentChanged(const QModelIndex & current, const QModelIndex& previous)
		{
			// check whether files were added to/removed from the current folder
		// 	if(current.isValid())
		// 	{
		// 		QModelIndex parent = dirmodel_->parent(current);
		// 		if(parent.isValid())
		// 		{
		// 			dirmodel_->refresh(parent);
		// 		}
		// 	}
			
			QAbstractItemView::currentChanged(current, previous);
		}


		FileIconProvider::FileIconProvider(FileBrowser* file_browser)
		{
			file_browser_ = file_browser;
		}


		QIcon FileIconProvider::icon (const QFileInfo& info) const
		{
			String file = info.absoluteFilePath().toStdString();
			if(file.hasSuffix("sdf"))
			{
				return QIcon(IconLoader::instance().getIcon("actions/sdf_icon"));
			}
			else if(file.hasSuffix("csv") || file.hasSuffix("txt") )
			{
				return QIcon(IconLoader::instance().getIcon("actions/csv_icon"));
			}
			else
			{
				return QFileIconProvider::icon(info);
			}	
		}
	}
}
