// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: regularData2DWidget.C,v 1.4 2004/01/13 12:41:01 anhi Exp $
//

#include <BALL/VIEW/WIDGETS/regularData2DWidget.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <qpointarray.h>
#include <qpainter.h>
#include <qwmatrix.h>
#include <qlayout.h>
#include <qpointarray.h>

namespace BALL
{
	namespace VIEW
	{
		// Constructor
		RegularData2DWidget::RegularData2DWidget(const RegularData2D& data, QWidget *parent)
			throw()
			:CanvasWidget(parent),
			 data_(data),
			 diagram_color_(QColor(red)),
			background_color_(QColor(white)) //,
			 //	 height_(0),//400), 
			 //	 line_style_(true)
		{
			std::cout <<"constructoraufruf!" << std::endl;
		}

		//Copyconstructor
		//!!!!!!!!!!!!!!!TODO cant work until copyconstructor of pixmap works

		/*	RegularData2DWidget::RegularData2DWidget(RegularData2DWidget* widget)
			throw() 
			:pixmapWidget(widget),
			data_(widget->data),
			diagram_color_(widget->diagram_color_),
			background_color_(widget->background_color_)//,
		//	heigth_(widget->heigth_),
		//	line_style_(widget->line_style)
		{ 
		}*/ 

		// Destructor
		RegularData2DWidget::~RegularData2DWidget()
		 throw()
		{
		}

		// creates a polygon from a given vector RegularData1D * data

		void RegularData2DWidget::createPixmap()
			throw()
		{
 			// no data => no polygon
			if (data_.size() == 0) return;
			
			// set up the ColorTable... TODO: This should be done by a dialog or something similar
			
			ColorRGBA colorList[3];
			ColorTable color_table_;  //Colortabel BALL
			colorList[0] = ColorRGBA(1,0,0,1);
			colorList[1] = ColorRGBA(0,1,0,1);
			colorList[2] = ColorRGBA(0,0,1,1);

			color_table_.setBaseColors(colorList, 3);
			color_table_.setNumberOfColors(53);
			color_table_.setRange(0,1);
			color_table_.createTable();
			
			Position x, y;
			QPainter paint;     //Painter
			QColor pCol;        //QTColor
			ColorRGBA mapcolor; //Ball Color
			PixmapItem* pixItem = new PixmapItem;
			
			// determine the minimal and maximal values in data_
			float min; 
			float max;

			min = data_[0];
			max = data_[0];

			for (Position i=1; i<data_.size(); i++)
			{
				float cur = data_[i];

				if (cur < min)
				{
					min = cur;
				}
				if (cur > max)
				{
					max  = cur;
				}
			}			
			
			Size max_x, max_y;

			max_x = data_.getMaxXIndex();
			max_y = data_.getMaxYIndex();

			// Draw the points
			QPixmap& pixmap = pixItem->getPixmap();

			pixmap.resize(max_x, max_y);

			pixmap.fill();           // delete the old picture
			paint.begin(pixmap);         // set the Painter 

			for (y=0; y<=max_y; y++) 
			{
				for (x=0; x<=max_x; x++) 
				{
					mapcolor = color_table_.map(data_[x + y*max_x]);
					pCol = QColor(mapcolor.getRed(), mapcolor.getGreen(), mapcolor.getBlue());

					paint.setPen(pCol);
					paint.drawPoint(x, y);
				}
			}


			paint.end();
			//put the pixmapItem into objects
			objects_.push_back(dynamic_cast<QCanvasItem*> (&pixmap); 
		
			// resize the canvas to fit the data
			canvas_.resize(max_x, max_y);


			//				polygon_ = new QCanvasPolygon(&canvas_);
			//			polygon_ = new QCanvasSpline(&canvas_);	
			// polygon_->setControlPoints((QPointArray)point_data_, false);
			
				//insert the polygon into vector<canvasItem*> of canvasWidget
			//				objects_.push_back(dynamic_cast<QCanvasItem*> (polygon_));
			}


	}//end of namespace VIEW

}//end of namespace BALL
