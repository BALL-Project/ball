// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: regularData2DWidget.C,v 1.1 2003/08/26 12:38:47 oliver Exp $

#include <BALL/VIEW/GUI/KERNEL/mainControl.h>
#include <BALL/DATATYPE/contour.h>
#include <BALL/VIEW/GUI/WIDGETS/regularData2DWidget.h>
#include <BALL/VIEW/GUI/DIALOGS/dlgMoveOverlay.h>
#include <qpainter.h>
#include <qpixmap.h>
#include <qpopupmenu.h>
#include <qlabel.h>
#include <qstatusbar.h>

using std::cout;
using std::endl;

namespace BALL
{
	namespace VIEW
	{

		NewRegularData2DMessage::NewRegularData2DMessage()
			:	CompositeMessage()
			{
			}

		NewRegularData2DMessage::NewRegularData2DMessage(const CompositeMessage &message, bool /* deep */)
			:	CompositeMessage(message)
			{
			}

		NewRegularData2DMessage::~NewRegularData2DMessage()
			throw()
			{
#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this
					<< " of class " << RTTI::getName<NewRegularData2DMessage>() << endl;
#endif
			}

		// ?????
		// WRepaintNoErase seems to dependent on a different QT version.

		RegularData2DWidget::RegularData2DWidget(int lx_, int ly_, double min, double max, QWidget *parent) 
			: QScrollView(parent, "RegularData2DWidget", WResizeNoErase|WNorthWestGravity), 
		ModularWidget("RegularData2DWidget"), 
		legend_last_x_(0),
		legend_last_y_(0),
		lengthx_(lx_), 
		lengthy_(ly_), 
		full_length_x_(0),
		full_length_y_(0),
		min_(min), 
		max_(max), 
		showMousePos_(true), 
		posLabel_(0), 
		data_(0),
		act_lower_left_x_(0), 
		act_lower_left_y_(0), 
		zoom_x_(1), 
		zoom_y_(1), 
		cont_(0), 
		mvover_(0), 
		ind_side_(0), 
		ind_updown_(0), 
		data_length_x_(0), 
		data_length_y_(0), 
		pm_(0),
		legend_map_(0),
		pm_cont_(0), 
		plot_cont_(false), 
		plot_data_(true),
		select_(false),
		legend_wid_(0)
		{
			// set up the ColorTable... TODO: This should be done by a dialog or something similar
			ColorRGBA colorList[3];
			colorList[0] = ColorRGBA(1,0,0,1);
			colorList[1] = ColorRGBA(0,1,0,1);
			colorList[2] = ColorRGBA(0,0,1,1);

			color_table_.setBaseColors(colorList, 3);
			color_table_.setNumberOfColors(53);
			color_table_.setRange(0,1);
			color_table_.createTable();
			
			createLegend(20, 40);

			setCaption("RegularData2DWidget");

			// register the widget with the MainControl
			ModularWidget::registerWidget(this);

			// create the context menu
			men_ = new QPopupMenu(this);
			men_->insertItem("Zoom out", this, SLOT(slotZoomOut()));
			men_->insertItem("Contourplot", this, SLOT(createContour()));
			men_->insertItem("Construct overlay", this, SLOT(plotOverlay()));

			viewport()->setMouseTracking(true);
		}

		RegularData2DWidget::RegularData2DWidget(const RegularData2DWidget& widget) 
			: QScrollView(), ModularWidget(widget), data_(0)
			{
				///????? This one is obviously still missing...
			}

		RegularData2DWidget::~RegularData2DWidget()
			throw()
			{
				if (men_)
				{
					delete (men_);
					men_ = NULL;
				}
			}

		void RegularData2DWidget::initializeWidget(MainControl& main_control)
			//	throw()
		{
			main_control.insertMenuEntry(MainControl::TOOLS, "&2D-NMR", this, SLOT(createPlot()));
			stat_ = getMainControl()->statusBar();
		}

		void RegularData2DWidget::finalizeWidget(MainControl& main_control)
			//	throw()
		{
			main_control.removeMenuEntry(MainControl::TOOLS, "&2D-NMR", this, SLOT(createPlot()));
		}

		void RegularData2DWidget::onNotify(Message *message)
			throw()
			{
				reactToMessages_(message);
			}

		bool RegularData2DWidget::isVisibleAs(const Vector2& pos, TVector2<Position>& res)
		{
			// we need the information stored in xvis & yvis, step_x_ & step_y_
			float x = pos.x;
			float y = pos.y;
			if ((x > xvis_low_) && (x < xvis_high_) && (y > yvis_low_) && (y < yvis_high_))
			{
				RegularData2D::IndexType r = data_->getClosestIndex(pos); // this would be the position with a zoom of 1

				res.x =  (Position)((r.x - act_lower_left_x_)*zoom_x_);
				res.y =  (Position)((r.y - act_lower_left_y_)*zoom_y_);

				return true;
			}
			return false;
		}

		bool RegularData2DWidget::reactToMessages_(Message* message)
		{
			bool update = false;

			if (RTTI::isKindOf<NewRegularData2DMessage>(*message))
			{
				NewRegularData2DMessage *composite_message = RTTI::castTo<NewRegularData2DMessage>(*message);

				// set the RegularData2D we have been sent. Then set all the other attributes.
				if (data_)
				{
					delete (data_);
				}

				data_ = (RegularData2D *)composite_message->getComposite();

				// determine the minimal and maximal values in data_
				min_ = (*data_)[0];
				max_ = (*data_)[0];

				for (Position i=1; i<data_->size(); i++)
				{
					float cur = (*data_)[i];
					
					if (cur < min_)
					{
						min_ = cur;
					}
					if (cur > max_)
					{
						max_ = cur;
					}
				}
				
				lengthx_ = data_->getSize().x;
				lengthy_ = data_->getSize().y;
				data_length_x_ = lengthx_; 
				data_length_y_ = lengthy_; 

				if (pm_)
				{
					delete (pm_);
					pm_ = 0;
				}

				full_length_x_ = visibleWidth();
				full_length_y_ = visibleHeight();

				Vector2 v = data_->getCoordinates(TVector2<Position>(0, 0));
				xvis_low_ = v.x;
				yvis_low_ = v.y;

				v = data_->getCoordinates(TVector2<Position>(lengthx_, lengthy_));
				xvis_high_ = v.x;
				yvis_high_ = v.y;

				createPlot();

				update = true;
			}

			return update;
		}

		/**
		 * Creates a legend of width w and height h.
		 */
		void RegularData2DWidget::createLegend(int w, int h)
		{
			int x, y;
			QPainter paint;
			QColor pCol;

			if (legend_map_)
			{
				delete (legend_map_);
			}
			if (legend_wid_)
			{
				delete (legend_wid_);
			}

			//  legend_map_ = new QPixmap(w, h);
			legend_wid_ = new QWidget(this, "");// WStyle_NoBorderEx);
			legend_wid_->resize(w + 4, h + 4); legend_wid_->setBackgroundColor(white);
			legend_wid_->show();


			//  legend_map_->fill();
			//  paint.begin(legend_map_);
			paint.begin(legend_wid_);
			paint.setPen(red);
			paint.drawLine(0, 0, 100, 100);
			// Transformation: (0, 0) -> lower lefthand corner
			QWMatrix m(1, 0, 0, -1, 0, h);
			paint.setWorldMatrix(m);
			ColorRGBA mapcolor;

			for (y=0; y<h; y++) 
			{
				mapcolor = color_table_.map(fabs(max_ - min_) * (y+1) / h);
				pCol = QColor(mapcolor.getRed(), mapcolor.getGreen(), mapcolor.getBlue());
				paint.setPen(pCol);
				for (x=0; x<w; x++) 
				{
					paint.drawPoint(x, y);
				}
			}

			paint.end();
			//  addChild(legend_wid_, viewport()->width() - legend_wid_->width() + 12, 8);
			//  legend_wid_->show();
		}  

		void RegularData2DWidget::plot()
		{
			if (plot_data_)
			{
				if (!pm_)
				{
					createPlot();
				}
			}

			if (plot_cont_)
			{
				if (!pm_cont_)
				{
					createContour();
				}
			}
		}

		/** Scale to dimensions nx, ny
		*/
		void RegularData2DWidget::scale(Size nx, Size ny, const Vector2& pos1, const Vector2& pos2)
		{
			RegularData2D::IndexType pa = data_->getClosestIndex(pos1);
			pa = data_->getClosestIndex(pos2);

			if (nx && ny && data_)
			{
				QPainter paint;
				QColor pCol;

				double stepx, stepy;
				Size x, y;
				double actx, acty;

				float x1 = pos1.x;
				float x2 = pos2.x;
				float y1 = pos1.y;
				float y2 = pos2.y;
				if (pm_)
				{
					delete (pm_);
				}

				pm_ = new QPixmap(nx, ny);
				pm_->fill();

				stepx = (x2 - x1) / nx;
				stepy = (y2 - y1) / ny;

				actx = x1;
				acty = y1;

				paint.begin(pm_);

				paint.setViewport(0, 0, nx, ny);

				// Transformation: (0,0) -> lower left corner
				QWMatrix m(1, 0, 0, -1, 0, ny);
				paint.setWorldMatrix(m);
				ColorRGBA mapcolor;

				for (y = 0; y < ny; y++) 
				{
					for (x = 0; x < nx; x++) 
					{
						mapcolor = color_table_.map(data_->getData(TVector2<Position>((Position)(actx + x * stepx), (Position)(acty + y * stepy))));
						pCol = QColor(mapcolor.getRed(), mapcolor.getGreen(), mapcolor.getBlue());
						paint.setPen(pCol);
						paint.drawPoint(x, y);
					}
				}
				paint.end();

			}
		}

		/**
		 * Creates a lorentzian peak in the data, having width xwidth, ywidth, amplitude amp, 
		 * maximum in xpos, ypos.
		 */
		void RegularData2DWidget::addLorentzian(double xpos, double ypos, double amp, int xwidth, int ywidth)
		{
			Position x, y;

			for (y=0; y<lengthy_; y++) 
			{
				if (data_) 
				{
					for (x=0; x<lengthx_; x++) 
					{
						(*data_)[x + lengthx_*y] += amp/(1 + pow(((x - xpos)/xwidth), 2) + pow(((y - ypos)/ywidth), 2));
					}
				}
			}
		}

		void RegularData2DWidget::plotOverlay()
		{
			plot_cont_ = true;

			if (pm_cont_)
			{
				delete pm_cont_;
			}
			pm_cont_ = new QPixmap(data_length_x_, data_length_y_);
			pm_cont_->fill(black);

			mvover_ = new DlgMoveOverlay(this, "Overlay");

			connect(mvover_, SIGNAL(sigMove(int)), this, SLOT(slotOverlayMove(int)));
			mvover_->show();
			//  createPlot();

			// This should be replaced by setting the variables via a dialog
			cont_num_ = 5;

			// determine the minimal and maximal values in data_
			min_ = (*data_)[0];
			max_ = (*data_)[0];

			for (Position i=1; i<data_->size(); i++)
			{
				float cur = (*data_)[i];

				if (cur < min_)
				{
					min_ = cur;
				}
				if (cur > max_)
				{
					max_ = cur;
				}
			}
			
			cont_start_   = max_ / 2;
			cont_end_     = max_;
			// up to here...

			cont_ = new Contour(cont_num_, cont_start_, cont_end_);
			cont_->apply(*data_);

			isOverlay_ = true;
			plotContour();
		}  

		void RegularData2DWidget::slotOverlayMove(int i)
		{
			switch(i) 
			{
				case 1 : // move to the left
					ind_side_ += 1;
					break;
				case 2 :  // move to the right
					ind_side_ -= 1;
					break;
				case 3: // move up
					ind_updown_ += 1;
					break;
				case 4: // move down
					ind_updown_ -= 1;
					break;
			}

			//  createPlot();
			//  plotContour();
			repaint();
		}

		void RegularData2DWidget::createPlot()
		{
			Position x, y;
			QPainter paint;
			QColor pCol;

			// cleanup needed?
			if (pm_)
			{
				delete (pm_); 
			}

			// determine the minimal and maximal values in data_
			min_ = (*data_)[0];
			max_ = (*data_)[0];

			for (Position i=1; i<data_->size(); i++)
			{
				float cur = (*data_)[i];

				if (cur < min_)
				{
					min_ = cur;
				}
				if (cur > max_)
				{
					max_ = cur;
				}
			}

			act_lower_left_x_ = 0;
			act_lower_left_y_ = 0;
			zoom_x_ = 1; 
			zoom_y_ = 1;

			full_length_x_ = lengthx_;
			full_length_y_ = lengthy_;

			resizeContents(full_length_x_, full_length_y_);

			// Draw the points
			pm_ = new QPixmap(full_length_x_, full_length_y_);
			pm_->fill();
			paint.begin(pm_);

			paint.setViewport(0, 0, full_length_x_, full_length_y_);

			// Transformation: (0,0) -> lower left corner
			QWMatrix m(1, 0, 0, -1, 0, full_length_y_);
			paint.setWorldMatrix(m);
			ColorRGBA mapcolor;

			for (y=0; y<full_length_y_; y++) 
			{
				for (x=0; x<full_length_x_; x++) 
				{
					mapcolor = color_table_.map((*data_)[x + y*full_length_x_]);
					pCol = QColor(mapcolor.getRed(), mapcolor.getGreen(), mapcolor.getBlue());

					paint.setPen(pCol);
					paint.drawPoint(x, y);
				}
			}

			paint.end();
		}

		void RegularData2DWidget::plotContour()
		{
			if (pm_cont_)
			{
				QPainter paint;

				// now draw the data.
				ContourLine l(0);
				Vector2 v, v2;

				paint.begin(pm_cont_);

				paint.setViewport(0, 0, data_length_x_, data_length_y_);

				// Transformation: (0,0) -> lower left corner
				QWMatrix m(1, 0, 0, -1, 0, full_length_y_);
				paint.setWorldMatrix(m);

				Position i=0;

				cont_->resetCounter();

				ColorRGBA mapcolor;

				// naive algorithm: tries to plot every part of the lines
				while(cont_->getNextContourLine(l))
				{
					l.resetCounter();

					// set it's colour
					mapcolor = color_table_.map(cont_start_ + (cont_end_ - cont_start_) / cont_num_ * i); 
					QColor pcol = isOverlay_ ? QColor(red) : QColor(mapcolor.getRed(), mapcolor.getGreen(), mapcolor.getBlue());
					paint.setPen(pcol);
					++i;

					// draw this ContourLine.

					while (l.getNextPoint(v))
					{
						if (l.getNextPoint(v2))
						{
							// draw a line from p to p2
							TVector2<Position> qp1, qp2;

							if ((isVisibleAs(v, qp1)) && (isVisibleAs(v2, qp2)))
							{
								paint.drawLine(qp1.x+ind_side_, qp1.y+ind_updown_, qp2.x+ind_side_, qp2.y+ind_updown_);
							}
						}
					}
				}

				paint.end();
			}
		}

		void RegularData2DWidget::drawContents(QPainter *paint, int clipx, int clipy, int clipw, int cliph)
		{
			if (pm_ && (pm_->size() != QSize(0, 0))) 
			{ // do we have something to paint?
				paint->drawPixmap(clipx, clipy, *pm_, clipx, clipy, clipw, cliph);

				if (legend_wid_)
				{
					moveChild(legend_wid_, contentsX()+viewport()->width()-legend_wid_->width()-12, contentsY()+8);
					legend_wid_->show();
				};

				if (plot_cont_ && pm_cont_) 
				{
					QPoint dummy = contentsToViewport(QPoint(contentsX(), contentsY()));

					bitBlt(viewport(), dummy.x(), dummy.y(), pm_cont_, contentsX()+ind_side_, 
							contentsY()+ind_updown_, viewport()->width(), viewport()->height(), OrROP);
				}
			}
		}

		void RegularData2DWidget::paintEvent(QPaintEvent*)
		{
			if (pm_ && (pm_->size() != QSize(0, 0))) 
			{ // is there something to paint?
				QPainter paint(viewport());

				if (legend_wid_)
				{
					removeChild(legend_wid_);
					addChild(legend_wid_, contentsX()+viewport()->width()-legend_wid_->width()-12, contentsY()+8);
					legend_wid_->show();
				};

				if (plot_data_) 
				{
					paint.drawPixmap(contentsX(), contentsY(), *pm_, contentsX(), contentsY(), 
							contentsWidth(), contentsHeight());
				}

				if (plot_cont_ && pm_cont_) 
				{
					QPoint dummy = contentsToViewport(QPoint(contentsX(), contentsY()));

					bitBlt(viewport(), dummy.x(), dummy.y(), pm_cont_, contentsX()+ind_side_, 
							contentsY()+ind_updown_, viewport()->width(), viewport()->height(), OrROP);
				}
			}
		}

		void RegularData2DWidget::Selected(QPoint beg, QPoint end)
		{
			// The user has selected a rectangle in the data. Now we should zoom in on it.
			// "beg" contains the position of the first corner, "end" of the second. Both
			// are relative to the viewport (????)

			if (data_) { // to be sure that there *is* data to zoom into

				// First we have to convert the positions into the corresponding positions in
				// the dataset. For this, we need to know the resolution we are working at right
				// now.
				int xh, yh;
				Position newx_low, newy_low, newx_up, newy_up, numpx, numpy;
				viewportToContents(beg.x(), beg.y(), xh, yh); // this should account for scrolling
				// in the data

				xh = full_length_x_ - xh;
				yh = full_length_y_ - yh; // for some reason, we always have to reverse the y direction

				newx_low = (Position) (beg.x() / zoom_x_ + act_lower_left_x_); // this corresponds to the index in the data.
				newy_low = (Position) (beg.y() / zoom_y_ + act_lower_left_y_);
				newx_up  = (Position) (end.x() / zoom_x_ + act_lower_left_x_);
				newy_up  = (Position) (end.y() / zoom_y_ + act_lower_left_y_);

				Vector2 vec_low = data_->getCoordinates(TVector2<Position>(newx_low, newy_low));
				Vector2 vec_up  = data_->getCoordinates(TVector2<Position>(newx_up,  newy_up));  // these should now contain the new values.


				// now set all the class variables concerned with the position in the data
				xvis_low_  = vec_low.x;
				xvis_high_ = vec_up.x;
				yvis_low_  = vec_low.y;
				yvis_high_ = vec_up.y;

				act_lower_left_x_ = newx_low; 
				act_lower_left_y_ = newy_low;

				lengthx_ = (Position) data_->getSize().x;
				lengthy_ = (Position) data_->getSize().y;

				numpx = (Position) (lengthx_ * fabs(xvis_high_ - xvis_low_) / fabs(data_->getDimension().x - data_->getOrigin().x));
				numpy = (Position) (lengthy_ * fabs(yvis_high_ - yvis_low_) / fabs(data_->getDimension().y - data_->getOrigin().y));

				full_length_x_ = visibleWidth();
				full_length_y_ = visibleHeight();

				zoom_x_ = (double) full_length_x_ / numpx;
				zoom_y_ = (double) full_length_y_ / numpy;

				scale(width(), height(), vec_low, vec_up);
				plotContour();
			}
		}

		void RegularData2DWidget::slotZoomOut()
		{
			if (data_) {
				scale(visibleWidth(), visibleHeight(), data_->getOrigin(), data_->getDimension());
			}
		}

		void RegularData2DWidget::resizeEvent(QResizeEvent *e)
		{
			QScrollView::resizeEvent(e);
			repaint();
		}

		void RegularData2DWidget::enterEvent(QEvent *)
		{
			//  showMousePos_ = true;
		}

		void RegularData2DWidget::leaveEvent(QEvent *)
		{
			if (posLabel_)
			{
				if (stat_) 
				{
					stat_->removeWidget(posLabel_);
					delete (posLabel_);
					posLabel_ = NULL;
				}
			}
			//  showMousePos_ = false;
		}

		void RegularData2DWidget::createContour()
		{
			plot_cont_ = true;
			plot_data_ = false;

			if (pm_cont_)
			{
				delete pm_cont_;
			}
			pm_cont_ = new QPixmap(data_length_x_, data_length_y_);
			pm_cont_->fill(black);

			// This should be replaced by setting the variables via a dialog
			cont_num_ = 5;

			// determine the minimal and maximal values in data_
			min_ = (*data_)[0];
			max_ = (*data_)[0];

			for (Position i=1; i<data_->size(); i++)
			{
				float cur = (*data_)[i];

				if (cur < min_)
				{
					min_ = cur;
				}
				if (cur > max_)
				{
					max_ = cur;
				}
			}
			cont_start_   = max_ / 2;
			cont_end_     = max_;
			// up to here...

			cont_ = new Contour(cont_num_, cont_start_, cont_end_);
			cont_->apply(*data_);

			isOverlay_ = false;
			plotContour();

			plot_cont_ = true;
			repaint();
		}

		void RegularData2DWidget::NewMousePos(Position x, Position y)
		{
			y = full_length_y_ - y;
			x = full_length_x_ - x; // is this always the case???

			x = (Position) (x/zoom_x_ + act_lower_left_x_);
			y = (Position) (y/zoom_y_ + act_lower_left_y_);

			if (showMousePos_) 
			{
				if (stat_ && full_length_x_ && full_length_y_) 
				{
					if (posLabel_)
					{
						stat_->removeWidget(posLabel_);
						delete posLabel_;
						posLabel_ = NULL;
					}

					QString message, dummy;
					double nx, ny, numDum;

					Vector2 vec = data_->getCoordinates(TVector2<Position>(x, y));
					nx = vec.x;
					ny = vec.y;

					message = "x: ";
					message += dummy.setNum(nx, 'e', 5);
					message += "y: ";
					message += dummy.setNum(ny, 'e', 5);
					message += "z: ";
					if (data_ && (y < full_length_y_)) 
					{
						numDum = (*data_)[x + full_length_x_ * y];
					}

					message += dummy.setNum(numDum, 'e', 5);

					posLabel_ = new QLabel(message, stat_);
					stat_->addWidget(posLabel_, 0);
					posLabel_->show();
				}
			}
		}

		void RegularData2DWidget::mousePressEvent(QMouseEvent *)
		{
		}

		void RegularData2DWidget::viewportMousePressEvent(QMouseEvent *e)
		{
			if (e->button() == QMouseEvent::LeftButton)
			{
				//    viewportToContents(e->pos().x(), e->pos().y(), x_help, y_help);
				last_selection_ = e->pos();
				selection_ = e->pos();
				setCursor(crossCursor);

				select_ = true;
			}

			if (e->button() == QMouseEvent::RightButton)
			{
				men_->popup(mapToGlobal(e->pos()));
			}
		}

		void RegularData2DWidget::viewportMouseMoveEvent(QMouseEvent *e)
		{
			//  viewportToContents(e->pos().x(), e->pos().y(), x_help, y_help);
			QPoint helper;
			int xh, yh;
			viewportToContents(e->pos().x(), e->pos().y(), xh, yh);
			helper = QPoint(xh, yh);

			NewMousePos(helper.x(), helper.y());
			if (select_)
			{
				if ((last_selection_.x() != helper.x()) && (last_selection_.y() != helper.y()))
				{
					QPainter p(viewport());
					p.setPen(QPen(green, 0, DashLine));
					p.setRasterOp(NotROP);

					// erase old rectangle
					p.drawRect(last_selection_.x(), last_selection_.y(), selection_.x() - last_selection_.x(), selection_.y() - last_selection_.y());

					selection_ = helper;

					// draw the new one
					p.drawRect(last_selection_.x(), last_selection_.y(), selection_.x() - last_selection_.x(), selection_.y() - last_selection_.y());

					p.end();
					viewport()->repaint(0);
				}
			}
		}

		void RegularData2DWidget::mouseReleaseEvent(QMouseEvent *e)
		{
			viewportMouseReleaseEvent(e);
		}

		void RegularData2DWidget::viewportMouseReleaseEvent(QMouseEvent *e)
		{
			if (select_)
			{
				setCursor(arrowCursor);
				select_ = false;

				Selected(last_selection_, e->pos());
			}
		}

		void RegularData2DWidget::eraseSelectFrame()
		{
			drawSelectFrame();
		}

		void RegularData2DWidget::drawSelectFrame()
		{
			QPainter paint(viewport());
			paint.setPen(QPen(green, 0, DashLine));
			paint.setRasterOp(NotROP);
			paint.drawRect(last_selection_.x(), last_selection_.y(), selection_.x() - last_selection_.x(), selection_.y() - last_selection_.y());
			paint.end();
			viewport()->repaint(0);
		}

	} } // namespaces
