#include <BALL/APPLICATIONS/QuEasyViz/fileBrowser.h>
#include <QtGui/QHeaderView>
#include <BALL/DATATYPE/string.h>
#include <BALL/APPLICATIONS/QuEasyViz/mainWindow.h>

using namespace BALL::VIEW;

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



FileIconProvider::FileIconProvider(FileBrowser* file_browser)
{
	file_browser_ = file_browser;
}


QIcon FileIconProvider::icon (const QFileInfo& info) const
{
	String file = info.absoluteFilePath().toStdString();
	String dir = file_browser_->main_window->getImageDirectory();
	if(file.hasSuffix("sdf"))
	{
		return QIcon((dir+"sdf_icon.png").c_str());
	}
	else if(file.hasSuffix("csv") || file.hasSuffix("txt") )
	{
		return QIcon((dir+"csv_icon.png").c_str());
	}
	else
	{
		return QFileIconProvider::icon(info);
	}	
}
