#include <BALL/APPLICATIONS/QSAR_GUI/fileBrowser.h>
#include <QtGui/QHeaderView>


using namespace BALL::VIEW;

FileBrowser::FileBrowser(std::string path)
{
	///at the moment, there's only support for sd-files
	QStringList filters;
	filters << "*.sdf"	
			<< "*.csv"	
			<< "*.txt";
	
	dirmodel_ = new QDirModel(this);

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
