#include <BALL/APPLICATIONS/QSAR_GUI/predictionItem.h>
#include <BALL/APPLICATIONS/QSAR_GUI/exception.h>
#include <BALL/APPLICATIONS/QSAR_GUI/mainWindow.h>
#include <BALL/APPLICATIONS/QSAR_GUI/predictionPlotter.h>
#include <BALL/APPLICATIONS/QSAR_GUI/predictionResultDialog.h>

#include <QtGui/QDrag>
#include <QtCore/QMimeData>

using namespace BALL::QSAR;
using namespace BALL::QSAR::Exception;
using namespace BALL::VIEW;
using namespace BALL::VIEW::Exception;

PredictionItem::PredictionItem(InputDataItem* input_item, ModelItem* model_item, DataItemView* view):
	DataItem(view),
	model_item_(model_item),
	input_data_item_(input_item),
	dotted_edge_(NULL)
{
	setPixmap(QPixmap("./images/prediction.png").scaled(QSize(width(), height()), Qt::KeepAspectRatio,Qt::FastTransformation ));
	name_ = "Prediction for " + input_item->name();
	pred_plotter_ = NULL;
}

PredictionItem::PredictionItem(PredictionItem& item):
DataItem(item.view_)
{
	name_ = item.name_;
	setPixmap(item.pixmap());
	model_item_ = item.model_item_;
	results_ = item.results_;
	input_data_item_ = item.input_data_item_;
	pred_plotter_ = item.pred_plotter_;
}

PredictionItem::~PredictionItem()
{
	delete dotted_edge_;
	delete pred_plotter_;
	if (view_->name == "view")
	{
		MainWindow* mw = view_->data_scene->main_window;
		mw->removeFromPipeline(this);
	}
	// everything else is done by base-class destructor!!
}

void PredictionItem::connectWithModelItem()
{
	if(done_) return;   // do nothing twice !
	
	for(unsigned int i=0; i<input_data_item_->data()->getNoSubstances();i++)
	{
		vector<double>* substance = input_data_item_->data()->getSubstance(i);
		RowVector res = model_item_->model()->predict(*substance,1);
		results_ << res;
		delete substance;
	}
	
	done_ = 1;
}

ModelItem* PredictionItem::modelItem()
{
	return model_item_;
}

const QList<RowVector>* PredictionItem::results()
{
	return &results_;
}

InputDataItem* PredictionItem::inputDataItem()
{
	return input_data_item_;
}

void PredictionItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	/// set member-pointer of main_window to the current object in order to gain access to this object from another widget 
	view_->data_scene->main_window->dragged_item = this;
	view_->data_scene->main_window->drag_source = view_->name;

	QByteArray itemData;
	QDataStream dataStream(&itemData, QIODevice::WriteOnly);
	QDrag *drag = new QDrag(event->widget());

	QMimeData *mimeData = new QMimeData;
	mimeData->setData("application/x-predictiondata", itemData);
	drag->setMimeData(mimeData);
	drag->setPixmap(this->pixmap());
	drag->setHotSpot(QPoint(0, 0));
	drag->start();
	itemChange(ItemPositionChange, pos());
	QGraphicsItem::mousePressEvent(event);
}


void PredictionItem::setDottedEdge(DottedEdge* edge)
{
	dotted_edge_ = edge;
}

DottedEdge* PredictionItem::dottedEdge()
{
	return dotted_edge_;
}

void PredictionItem::showPredictionPlotter()
{
	if(results_.size()==0)
	{
		QMessageBox::information(view_,"No predictions","No predictions have been done yet that could be plotted!\nTherefore, click \"Execute Pipeline\" first.");
		return;	
	}
	if(pred_plotter_ == NULL)
	{
		pred_plotter_=new PredictionPlotter(this);
	}
	else
	{
		pred_plotter_->show();
	}
}

void PredictionItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	PredictionResultDialog predictionResultDialog(this);
	predictionResultDialog.exec();
}


void PredictionItem::writeConfigSection(QTextStream& out)
{
	out << "[Predictor]" << "\n";
	out << "model_file = "<< modelItem()->savedAs() << "\n";
	out << "data_file = "<< inputDataItem()->savedAs() << "\n";
	out << "print_excepted = "<< 1 << "\n";
	out << "output = " << savedAs() << "\n\n";
}

