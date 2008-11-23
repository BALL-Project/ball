#include <BALL/APPLICATIONS/QSAR_GUI/fileBrowser.h>
#include <QtGui/QHeaderView>
#include <BALL/DATATYPE/string.h>

using namespace BALL::VIEW;

FileBrowser::FileBrowser(std::string path)
{
	///display only sdf, csv and txt-files
	QStringList filters;
	filters << "*.sdf"<<"*.csv"<< "*.txt";
	dirmodel_ = new QDirModel(this);
	icon_provider_ = new FileIconProvider;
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


QIcon FileIconProvider::icon (const QFileInfo& info) const
{
	String file = info.absoluteFilePath().toStdString();
	if(file.hasSuffix("sdf"))
	{
		return QIcon("./images/sdf_icon.png");
	}
	else if(file.hasSuffix("csv") || file.hasSuffix("txt") )
	{
		return QIcon("./images/csv_icon.png");
	}
	else
	{
		return QFileIconProvider::icon(info);
	}	
}
