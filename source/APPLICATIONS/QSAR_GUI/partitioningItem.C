#include <BALL/APPLICATIONS/QSAR_GUI/partitioningItem.h>
#include <BALL/APPLICATIONS/QSAR_GUI/exception.h>
#include <BALL/APPLICATIONS/QSAR_GUI/mainWindow.h>

#include <QtGui/QDrag>
#include <QtCore/QMimeData>

using namespace BALL::QSAR;
using namespace BALL::QSAR::Exception;
using namespace BALL::VIEW;
using namespace BALL::VIEW::Exception;


PartitioningItem::PartitioningItem(InputDataItem* input, DataItemView* miv):
	DataItem(miv),
	input_(input)
{
	setPixmap(QPixmap("./images/partitioning.png").scaled(QSize(width(), height()), Qt::KeepAspectRatio,Qt::FastTransformation ));
	name_ = "Partitioning of "+ input_->name();
}


PartitioningItem::~PartitioningItem()
{
}

bool PartitioningItem::execute()
{

	return true;
}

void writeConfigSection(ofstream& out)
{
	Q_UNUSED(out);
}

void PartitioningItem::addToPipeline()
{
}

void PartitioningItem::removeFromPipeline()
{
}

void PartitioningItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QByteArray itemData;
	QDataStream dataStream(&itemData, QIODevice::WriteOnly);
	QDrag *drag = new QDrag(event->widget());

	/// set member-pointer of main_window to the current object in order to gain access to this object from another widget 
	view_->data_scene->main_window->dragged_item = this;
	view_->data_scene->main_window->drag_source = view_->name;

	QMimeData *mimeData = new QMimeData;
	mimeData->setData("application/x-partitemdata", itemData);
	drag->setMimeData(mimeData);
	drag->setPixmap(pixmap());
	drag->setHotSpot(QPoint(0, 0));
	drag->start();
	itemChange(ItemPositionChange, pos());
	QGraphicsItem::mousePressEvent(event);
}
