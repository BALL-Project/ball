// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: regularData1DWidget.C,v 1.10 2004/06/10 15:45:02 amoll Exp $
//

#include <BALL/VIEW/WIDGETS/regularData1DWidget.h>
#include <BALL/COMMON/limits.h>

namespace BALL
{
	namespace VIEW
	{

RegularData1DWidget::RegularData1DWidget(const RegularData1D* data, QWidget *parent)
	throw()
	:CanvasWidget(parent),
	 data_(data),
	 diagram_color_(QColor(red)),
	 background_color_(QColor(white)) //,
	 //	 height_(0),//400), 
	 //	 line_style_(true)
{
}

//!!!!!!!!!!!!!!!TODO cant work until copyconstructor of pixmap works
RegularData1DWidget::RegularData1DWidget(RegularData1DWidget* widget)
	throw() 
	: //pixmapWidget(widget),
		data_(widget->data_),
		diagram_color_(widget->diagram_color_),
		background_color_(widget->background_color_)//,
//	heigth_(widget->heigth_),
//	line_style_(widget->line_style)
{ 
}

// Destructor
RegularData1DWidget::~RegularData1DWidget()
 throw()
{
}

// creates a polygon from a given vector RegularData1D * data
void RegularData1DWidget::createPolygon()
	throw()
{
	// no data => no polygon
	if (data_ == 0 ||
			data_->size() == 0) 
	{
		return;
	}

	// find the data min and max
	float min = Limits<float>::max();
	float max = Limits<float>::min();
	float dif_min = min;
	float old = min; //last point

	for (int i=0; i<(int)data_->size(); i++)
	{
		if ((*data_)[i] >= max) max = (*data_)[i];
		if ((*data_)[i] <= min) min = (*data_)[i];

		float dif = (*data_)[i] - old;

		if ( (dif > 1e-3) && (dif < dif_min) ) 
		{
			dif_min = dif;
		}

		old = (*data_)[i];
	}

	height_ = (int)ceil(((max-min) / dif_min) * 5);
 
	if (height_ > 1000)
	{
		// the resulting picture would be much too large
		height_ = 1000;
		dif_min = (max - min)/200.;
	}	

	// resize the canvas to fit the data
	canvas_.resize(5*(data_->size()+2), height_+10);

	int x_new;
	int y_new;
	int x_old = 5;
	int y_old = height_+5 - (int)((((*data_)[0]-min)/dif_min)*5);
	QCanvasLine *ql;

	for (int i=1; i<(int)data_->size(); i++)
	{
		x_new = 5*(i+1);
		y_new = height_+5 - (int)((((*data_)[i]-min)/dif_min)*5);
		
		ql = new QCanvasLine(&canvas_);
		ql->setBrush(QColor(black));
		ql->setPoints(x_old, y_old, x_new, y_new);
		objects_.push_back(dynamic_cast<QCanvasItem*> (ql));
		ql->show();

		x_old = x_new;
		y_old = y_new;
	}

	//add the x-axis
	ql = new QCanvasLine(&canvas_);
	int startx = 5;
	int starty = height_+5 - (int)round(((startx-min)/dif_min)*5);
	int endx   = data_->size()*5;
	ql->setPoints(startx, starty, endx, starty);
	ql->setBrush(QColor(red));
	ql->show();
	objects_.push_back(dynamic_cast<QCanvasItem*> (ql));
		
	//add the y-axis	
	ql = new QCanvasLine(&canvas_);
	startx = 0;
	starty = 0;
	int endy   = height_+5 - (int)round((((*data_)[endx]-min)/dif_min)*5);
	ql->setPoints(startx, starty, startx, endy);
	ql->setBrush(QColor(red));
	ql->show();
	objects_.push_back(dynamic_cast<QCanvasItem*> (ql));
}

void RegularData1DWidget::onNotify(Message *message)
	throw()
{
	if (!RTTI::isKindOf<RegularData1DMessage>(*message))
	{
		return;
	}

	RegularData1DMessage& msg = *(RegularData1DMessage*) message;
	if (msg.getData() != data_) return;

	createPolygon();
}

	}//end of namespace VIEW
}//end of namespace BALL
