// $Id: regularData2DWidget.C,v 1.11 2000/12/04 21:20:41 anhi Exp $

#include <BALL/VIEW/GUI/WIDGETS/regularData2DWidget.h>

/* Converts to RGB-Space */
QColor con2rgb(double arg, double min, double max)
{
    double r, g, b;
    int ri, gi, bi;
    
    if (arg <= min) {
      ri = gi = bi = 0;
      return (QColor(ri, gi, bi));
    };

    if (arg <= min) {
      ri = gi = bi = 255;
      return (QColor(ri, gi, bi));
    };

    arg -= min;
    arg /= (max - min); // Skalierung.
    
    if (fabs(arg) <= 1. / 3) {
      r = 0;
      g = 255 * 3 * arg;
	b = 255;
    } else {
      if ( fabs(arg) <= 2. / 3 ) {
	r = 255 * (arg - 1./3) * 3 * arg;
	g = 255;
	b = 255 - (255 * (arg - 1./3) * 3 * arg);
      } else {
	r = 255;
	g = 255 * (1 - (arg - 2./3) * 3 * arg);
	b = 0;
      };
    };
    
    ri = (int) r;
    gi = (int) g;
    bi = (int) b;
    return (QColor(ri, gi, bi));
}

NewRegularData2DMessage::NewRegularData2DMessage()
  :
  CompositeMessage()
{
}

NewRegularData2DMessage::NewRegularData2DMessage(const CompositeMessage &message, bool /* deep */)
  :
  CompositeMessage(message)
{
}

NewRegularData2DMessage::~NewRegularData2DMessage()
{
  #ifdef BALL_VIEW_DEBUG
  cout << "Destructing object " << (void *)this
       << " of class " << RTTI::getName<NewRegularData2DMessage>() << endl;
  #endif
}

PixWid::PixWid( QWidget *w ) : QWidget( w ), select_(false)
{
  setMouseTracking(true);
};

void PixWid::mousePressEvent( QMouseEvent *e )
{
  if (e->button() == QMouseEvent::LeftButton)
  {
    beg_old_ = beg_;
    grabMouse( crossCursor );
    beg_ = e->pos();
    select_ = true;
  } else {
    emit context(e);
  };
};

void PixWid::mouseReleaseEvent( QMouseEvent *e )
{
  if (select_) {
    releaseMouse();
    end_old_ = end_;
    end_ = e->pos();
    select_ = false;

    emit selected(beg_, end_);
  };
};

void PixWid::mouseMoveEvent( QMouseEvent *e )
{
  if (select_) // we first have to draw the current selection
  {
//     QPainter q(this);
    
//     q.setPen(QColor(white));
//     q.drawRect(beg_.x(), beg_.y(), beg_.x() - end_.x(), beg_.y() - end_.y());
  };

  emit (mouseMoved(e->x(), e->y()));
}

RegularData2DWidget::RegularData2DWidget(int lx_, int ly_, double min, double max, QWidget *parent) : QScrollView(parent), ModularWidget("RegularData2DWidget"), pm_(0), legend_map_(0), buffer_map_(0), lengthx_(lx_), lengthy_(ly_), min_(min), max_(max), showMousePos_(true), posLabel_(0), soffsetf1_(0), soffsetf2_(0), swidthf1_(0), swidthf2_(0), bfreqf1_(0), bfreqf2_(0), act_lower_left_x_(0), act_lower_left_y_(0), zoom_x_(1), zoom_y_(1), cont_(0), mvover_(0), ind_side_(0), ind_updown_(0)
{
  createLegend( 20, 40 );

//   if (data)
//     createPlot();

  setCaption( "RegularData2DWidget" );

  // register the widget with the MainControl
  ModularWidget::registerWidget(this);

  // create the context menu
  men_ = new QPopupMenu(this);
  men_->insertItem("Create Baseline");
  men_->insertItem("Zoom out", this, SLOT(slotZoomOut()));
  men_->insertItem("Contourplot", this, SLOT(createContour()));
  men_->insertItem("Construct overlay", this, SLOT(plotOverlay()));
}

RegularData2DWidget::RegularData2DWidget(const RegularData2DWidget& widget) : QScrollView(), ModularWidget(widget)
{
}

RegularData2DWidget::~RegularData2DWidget()
{
  if (men_)
  {
    delete (men_);
    men_ = NULL;
  }
}

void RegularData2DWidget::initializeWidget(MainControl& main_control)
{
  main_control.insertMenuEntry(MainControl::TOOLS, "&2D-NMR", this, SLOT(createPlot()));
}

void RegularData2DWidget::finalizeWidget(MainControl& main_control)
{
  main_control.removeMenuEntry(MainControl::TOOLS, "&2D-NMR", this, SLOT(createPlot()));
}

void RegularData2DWidget::onNotify(Message *message)
{
  reactToMessages_(message);
}

bool RegularData2DWidget::isVisibleAs(double x, double y, pair<Position, Position>& res)
{
  // we need the information stored in xvis & yvis, step_x_ & step_y_
  if ((x > xvis_low_) && (x < xvis_high_) && (y > yvis_low_) && (y < yvis_high_))
  {
    spec_->getNearestPosition(x, y, res); // this would be the position with a zoom of 1

    res.first  -= act_lower_left_x_;      // move the origin
    res.second -= act_lower_left_y_;
    
    res.first  *= zoom_x_;
    res.second *= zoom_y_;

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
      if (spec_)
	delete (spec_);
      spec_ = (RegularData2D *)composite_message->getComposite();
      min_ = spec_->getLowerBound();
      max_ = spec_->getUpperBound();
      lengthx_ = spec_->getXSize();
      lengthy_ = spec_->getYSize();

      if (pm_)
      {
	delete (pm_);
	pm_ = 0;
      };
      if (pix_wid_)
      {
	delete (pix_wid_);
	pix_wid_ = 0;
      };

      lengthx_ = spec_->getXSize();
      lengthy_ = spec_->getYSize();
      
      full_length_x_ = width();
      full_length_y_ = height();

      pair<float, float> dum;

      spec_->getConvertedPosition(0, 0, dum);
      xvis_low_ = dum.first;
      yvis_low_ = dum.second;
      spec_->getConvertedPosition(lengthx_, lengthy_, dum);
      xvis_high_ = dum.first;
      yvis_high_ = dum.second;

      createPlot();

      update = true;
    }

  return update;
}

/**
 * Creates a legend of width w and height h.
 */
void RegularData2DWidget::createLegend( int w, int h )
{
  int x, y;
  QPainter paint;
  QColor pCol;

  if ( legend_map_ )
    delete ( legend_map_ );
  if ( buffer_map_ )
    delete ( buffer_map_ );

  legend_map_ = new QPixmap( w, h );
  buffer_map_ = new QPixmap( w + 4, h + 4 ); // save Background which is about to be destroyed by legend
  legend_map_->fill();
  paint.begin( legend_map_ );

  // Transformation: (0, 0) -> lower lefthand corner
  QWMatrix m(1, 0, 0, -1, 0, h);
  paint.setWorldMatrix( m );

  for (y=0; y<h; y++) {
    pCol = con2rgb( fabs(max_ - min_) * (y+1) / h, min_, max_ );
    paint.setPen( pCol );
    for (x=0; x<w; x++) {
      paint.drawPoint( x, y );
    };
  };
}  

/** Scale to dimensions nx, ny
 */
void RegularData2DWidget::scale(Size nx, Size ny, double x1, double y1, double x2, double y2)
{
  if (nx && ny && spec_)
  {
    QPainter paint;
    QColor pCol;
    pair<Position, Position> pos_dummy;

    double stepx, stepy;
    Size x, y;
    double actx, acty;
    Size numpx_, numpy_;

    if (pm_)
      delete (pm_);
    if (pix_wid_)
      delete (pix_wid_);

    pix_wid_ = new PixWid( viewport() );
    connect(pix_wid_, SIGNAL(mouseMoved(Position, Position)), this, SLOT(NewMousePos(Position,Position)));
    connect(pix_wid_, SIGNAL(context(QMouseEvent*)), this, SLOT(mousePressEvent(QMouseEvent*)));
    connect(pix_wid_, SIGNAL(selected(QPoint, QPoint)), this, SLOT(Selected(QPoint, QPoint)));
    pix_wid_->resize(nx, ny);

    pm_ = new QPixmap(nx, ny);
    pm_->fill();


    stepx = (x2 - x1) / nx;
    stepy = (y2 - y1) / ny;

    actx = x1;
    acty = y1;

    paint.begin( pm_ );
    
    paint.setViewport(0, 0, nx, ny);
    
    // Transformation: (0,0) -> lower left corner
    QWMatrix m(1, 0, 0, -1, 0, ny);
    paint.setWorldMatrix( m );

    for (y=0; y<ny; y++) {
	for (x=0; x<nx; x++) {
	    pCol = con2rgb(spec_->getValue(actx+x*stepx, acty+y*stepy), min_, max_);
	    paint.setPen( pCol );
	    paint.drawPoint( x, y );
	    };
	};
    paint.end();
    addChild( pix_wid_ );
    pix_wid_->show();
    QScrollView::resizeEvent(new QResizeEvent(size(), size()));
  };
}

/**
 * Creates a lorentzian peak in the data, having width xwidth, ywidth, amplitude amp, 
 * maximum in xpos, ypos.
 */
void RegularData2DWidget::addLorentzian( double xpos, double ypos, double amp, int xwidth=1, int ywidth=1 )
{
  Position x, y;
  
  for (y=0; y<lengthy_; y++) {
    if (spec_) {
      for (x=0; x<lengthx_; x++) {
	(*spec_)[x + lengthx_*y] += amp/(1 + pow(((x - xpos)/xwidth), 2) + pow(((y - ypos)/ywidth), 2));
      };
    };
  };
}

// /**
//  * Tries to load a spectrum from bruker-directory "dirName". Hopefully the directory's structure is like this in general.
//  */
// bool RegularData2DWidget::readSpec( string dirName )
// {
//   if (spectrum_)
//     delete (spectrum_);
//   spectrum_ = new Bruker2D( dirName );
//   spectrum_->read();
  
//   spec_ = spectrum_->GetData();

//   min_ = spectrum_->parsf1_->parameter( "YMIN_p" );
//   max_ = spectrum_->parsf1_->parameter( "YMAX_p" );

//   lengthx_ = (int) spectrum_->parsf2_->parameter( "SI" );
//   lengthy_ = (int) spectrum_->parsf1_->parameter( "SI" );

//   soffsetf1_ = spectrum_->parsf1_->parameter( "OFFSET" );
//   soffsetf2_ = spectrum_->parsf2_->parameter( "OFFSET" );

//   swidthf1_ = spectrum_->parsf1_->parameter( "SW_p" );
//   swidthf2_ = spectrum_->parsf2_->parameter( "SW_p" );
  
//   bfreqf1_ = spectrum_->parsf1_->parameter( "SF" );
//   bfreqf2_ = spectrum_->parsf2_->parameter( "SF" );

//   spectrum_->SetShiftRange(soffsetf1_, soffsetf2_, swidthf1_, swidthf2_, bfreqf1_, bfreqf2_, lengthy_, lengthx_);
  
//   // TEST!!!
//   //  spec_->createGroundState();
//   //cout << "gr: " << spec_->getGroundState() << "sigma: " << spec_->getSigmaGroundState() << endl;
// }

void RegularData2DWidget::plotOverlay()
{
  mvover_ = new DlgMoveOverlay(this, "Overlay");

  connect(mvover_, SIGNAL(sigMove(int)), this, SLOT(slotOverlayMove(int)));
  mvover_->show();
  createPlot();

  // This should be replaced by setting the variables via a dialog
  cont_num_ = 5;

  spec_->createGroundState();
  cont_start_ = spec_->getGroundState() + 0.5*spec_->getSigmaGroundState();
  //cont_start_   = spec_->getUpperBound() / 2;
  cont_end_   = spec_->getUpperBound();
  // up to here...

  cont_ = new Contour(cont_num_, cont_start_, cont_end_);
  cont_->apply(*spec_);

  plotContour();
}  

void RegularData2DWidget::slotOverlayMove(int i)
{
  switch(i) {
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
  };

  cout << i << " " <<  ind_side_ << " " << ind_updown_ << endl;

  createPlot();
  plotContour();
}

void RegularData2DWidget::createPlot()
{
  Position x, y;
  QPainter paint;
  QColor pCol;

  // cleanup needed?
  if (pm_)
    delete (pm_); 
  if (pix_wid_)
    delete (pix_wid_);

  spec_->createGroundState();
  min_ = spec_->getGroundState() + spec_->getSigmaGroundState();

  act_lower_left_x_ = 0;
  act_lower_left_y_ = 0;
  zoom_x_ = 1;
  zoom_y_ = 1;

  full_length_x_ = lengthx_;
  full_length_y_ = lengthy_;

  pix_wid_ = new PixWid( viewport() );
  connect(pix_wid_, SIGNAL(mouseMoved(Position,Position)), this, SLOT(NewMousePos(Position,Position)));
  connect(pix_wid_, SIGNAL(context(QMouseEvent*)), this, SLOT(mousePressEvent(QMouseEvent*)));
  connect(pix_wid_, SIGNAL(selected(QPoint, QPoint)), this, SLOT(Selected(QPoint, QPoint)));
  pix_wid_->resize( full_length_x_, full_length_y_ );

  // Draw the points
  pm_ = new QPixmap( full_length_x_, full_length_y_ );
  pm_->fill();
  paint.begin( pm_ );

  paint.setViewport(0, 0, full_length_x_, full_length_y_);

  // Transformation: (0,0) -> lower left corner
  QWMatrix m(1, 0, 0, -1, 0, full_length_y_);
  paint.setWorldMatrix( m );

  for (y=0; y<full_length_y_; y++) {
    for (x=0; x<full_length_x_; x++) {
      pCol = con2rgb((*spec_)[x + y*full_length_x_], min_, max_);
      paint.setPen( pCol );
      paint.drawPoint( x, y );
    };
  };

  paint.end();
  addChild( pix_wid_ );
  // Hack to enable display of scrollbars etc...
  pix_wid_->show();
  QScrollView::resizeEvent(new  QResizeEvent( size(), size() ));
}

void RegularData2DWidget::plotContour()
{
  QPainter paint;

  // now draw the data.
  ContourLine l(0);
  pair<float, float> p, p2;

  paint.begin( pm_ );

  paint.setViewport(0, 0, full_length_x_, full_length_y_);

  // Transformation: (0,0) -> lower left corner
  QWMatrix m(1, 0, 0, -1, 0, full_length_y_);
  paint.setWorldMatrix( m );

  Position i=0;

  cont_->resetCounter();

  // naive algorithm: draw *every part* of the line... should be changed asap.
  while(cont_->getNextContourLine(l))
  {
    l.resetCounter();

    // set it's colour
    QColor pcol = con2rgb(cont_start_ + (cont_end_ - cont_start_) / cont_num_ * i, cont_start_, max_);
    paint.setPen( red );
    ++i;

    // draw this ContourLine.
    while (l.getNextPoint(p))
    {
      if (l.getNextPoint(p2))
      {
	// draw a line from p to p2
	pair<Position, Position> qp1, qp2;

	/// TEST!
	isVisibleAs(p.first, p.second, qp1);
	cout << qp1.first << " " << qp1.second << endl;

	spec_->getNearestPosition(p.first, p.second, qp1);
	spec_->getNearestPosition(p2.first, p2.second, qp2);
	cout << qp1.first << " " << qp1.second << endl;
	//if ((isVisibleAs(p.first, p.second, qp1)) && (isVisibleAs(p2.first, p2.second, qp2)))
	{
	  paint.drawLine(qp1.first+ind_side_, qp1.second+ind_updown_, qp2.first+ind_side_, qp2.second+ind_updown_);
	};
      };
    };
  };

  paint.end();
  addChild( pix_wid_ );
  // Hack to enable display of scrollbars etc...
  pix_wid_->show();
  QScrollView::resizeEvent(new  QResizeEvent( size()-QSize(1,0), size() ));
}

void RegularData2DWidget::drawContents( QPainter *paint, int clipx, int clipy, int clipw, int cliph )
{
  if (pm_ && (pm_->size() != QSize( 0, 0 ))) { // do we have something to paint?
    paint->end();
    paint->begin( pix_wid_ );
    paint->drawPixmap(clipx, clipy, *pm_, clipx, clipy, clipw, cliph);

    // Zeichnen der Bildlegende.
    if (legend_map_) {
      bitBlt( pix_wid_, legend_last_x_, legend_last_y_, buffer_map_ );

      legend_last_x_ = contentsX() + visibleWidth() - legend_map_->width() - 12;
      legend_last_y_ = contentsY() + 8;
      
      bitBlt( buffer_map_, 0, 0, pix_wid_, legend_last_x_, legend_last_y_ );
      
      paint->setPen( QColor( black ) );
      paint->drawRect( legend_last_x_, legend_last_y_, legend_map_->width() + 4, legend_map_->height() + 4 );
      paint->setPen( QColor( white ) );
      paint->drawRect( legend_last_x_ + 1, legend_last_y_ + 1, legend_map_->width() + 2, legend_map_->height() + 2 );
      paint->drawPixmap( legend_last_x_ + 2, legend_last_y_ + 2, *legend_map_ );
    };
  };
}

void RegularData2DWidget::paintEvent( QPaintEvent *e )
{
  if (pm_ && (pm_->size() != QSize( 0, 0 ))) { // is there something to paint?
    QPainter paint( pix_wid_ );
    paint.setClipRect( contentsX(), contentsY(), contentsWidth(), contentsHeight() );
    paint.drawPixmap( contentsX(), contentsY(), *pm_, contentsX(), contentsY(), contentsWidth(), contentsHeight() );

    // draw the legend.
    if (legend_map_) {
      legend_last_x_ = contentsX() + visibleWidth() - legend_map_->width() - 12;
      legend_last_y_ = contentsY() + 8;

      bitBlt( buffer_map_, 0, 0, pix_wid_, contentsX() + visibleWidth() - legend_map_->width() - 12, contentsY() + 8, legend_map_->width() + 4, legend_map_->height() + 4 );

      paint.setPen( QColor( black ) );
      paint.drawRect( legend_last_x_, legend_last_y_, legend_map_->width() + 4, legend_map_->height() + 4 );
      paint.setPen( QColor( white ) );
      paint.drawRect( legend_last_x_ + 1, legend_last_y_ + 1, legend_map_->width() + 2, legend_map_->height() + 2 );
      paint.drawPixmap( legend_last_x_ + 2, legend_last_y_ + 2, *legend_map_ );
    };
  };
}

void RegularData2DWidget::Selected(QPoint beg, QPoint end)
{
  // The user has selected a rectangle in the data -> zoom into it
  // first: convert the position into position in the data
  pair<float, float> dummy, dummy2;
  pair<Position, Position> pos_dummy;
  double numpx_, numpy_;

  spec_->getConvertedPosition(beg.x() / zoom_x_ + act_lower_left_x_, beg.y() / zoom_y_ + act_lower_left_y_, dummy);
  spec_->getConvertedPosition(end.x() / zoom_x_ + act_lower_left_x_, end.y() / zoom_y_ + act_lower_left_y_, dummy2);
  
  cout << dummy.first << " " << dummy.second << " " << dummy2.first << " " << dummy2.second << endl;

  xvis_low_  = dummy.first;
  xvis_high_ = dummy2.first;
  yvis_low_  = dummy.second;
  yvis_high_ = dummy2.second;

  spec_->getNearestPosition(xvis_low_, yvis_low_, pos_dummy);
  act_lower_left_x_ = pos_dummy.first;
  act_lower_left_y_ = pos_dummy.second;

  lengthx_ = (Position) spec_->getXSize();
  lengthy_ = (Position) spec_->getYSize();

  numpx_ = (Position) (lengthx_ * fabs(xvis_high_ - xvis_low_) / fabs(spec_->getXUpper() - spec_->getXLower()));
  numpy_ = (Position) (lengthy_ * fabs(yvis_high_ - yvis_low_) / fabs(spec_->getYUpper() - spec_->getYLower()));

  full_length_x_ = width();
  full_length_y_ = height();

  zoom_x_ = (double) width() / numpx_;
  zoom_y_ = (double) height() / numpy_;

  scale(width(), height(), dummy.first, dummy.second, dummy2.first, dummy2.second);
  plotContour();
}

void RegularData2DWidget::slotZoomOut()
{
  scale(width(), height(), spec_->getXLower(), spec_->getYLower(), spec_->getXUpper(), spec_->getYUpper());
}

void RegularData2DWidget::resizeEvent( QResizeEvent *e )
{
  QScrollView::resizeEvent( e );
  repaint();
}

void RegularData2DWidget::enterEvent(QEvent *)
{
  //  showMousePos_ = true;
}

void RegularData2DWidget::leaveEvent(QEvent *)
{
  if (posLabel_) {
    if (MainControl::getMainControl( this )->statusBar()) {
      MainControl::getMainControl( this )->statusBar()->removeWidget( posLabel_ );
      delete (posLabel_);
      posLabel_ = NULL;
    };
  };
  //  showMousePos_ = false;
}

void RegularData2DWidget::createContour()
{
  if (pm_)
    delete pm_;
  if (pix_wid_)
    delete pix_wid_;

  act_lower_left_x_ = 0;
  act_lower_left_y_ = 0;
  zoom_x_ = 1;
  zoom_y_ = 1;

  full_length_x_ = lengthx_;
  full_length_y_ = lengthy_;

  pix_wid_ = new PixWid( viewport() );
  connect(pix_wid_, SIGNAL(mouseMoved(Position,Position)), this, SLOT(NewMousePos(Position,Position)));
  connect(pix_wid_, SIGNAL(context(QMouseEvent*)), this, SLOT(mousePressEvent(QMouseEvent*)));
  connect(pix_wid_, SIGNAL(selected(QPoint, QPoint)), this, SLOT(Selected(QPoint, QPoint)));
  pix_wid_->resize( full_length_x_, full_length_y_ );

  pm_ = new QPixmap( full_length_x_, full_length_y_ );
  pm_->fill();

  // This should be replaced by setting the variables via a dialog
  cont_num_ = 5;

  spec_->createGroundState();
  cont_start_ = spec_->getGroundState() + 0.5*spec_->getSigmaGroundState();
  //  cont_start_   = spec_->getUpperBound() / 2;
  cont_end_   = spec_->getUpperBound();
  // up to here...

  cont_ = new Contour(cont_num_, cont_start_, cont_end_);
  cont_->apply(*spec_);

  plotContour();
}

void RegularData2DWidget::NewMousePos( Position x, Position y )
{
  y = full_length_y_ - y;
  x = full_length_x_ - x; // is this always the case???
  
  x = x/zoom_x_ + act_lower_left_x_;
  y = y/zoom_y_ + act_lower_left_y_;
    
  if (showMousePos_) {
    if (MainControl::getMainControl( this )->statusBar() && full_length_x_ && full_length_y_) {
      if (posLabel_)
	{
	  MainControl::getMainControl( this )->statusBar()->removeWidget( posLabel_ );
	  delete posLabel_;
	  posLabel_ = NULL;
	};

      QString message, dummy;
      double numDum;
      pair<float, float> numDum2;

      //      numDum2 = spectrum_->GetShift(x, y);
      spec_->getConvertedPosition(x, y, numDum2);

      message = "x: ";
      message += dummy.setNum( numDum2.first, 'e', 5 );
      message += "y: ";
      message += dummy.setNum( numDum2.second, 'e', 5 );
      message += "z: ";
      if (spec_ && (y < full_length_y_)) {
	numDum = (*spec_)[x + full_length_x_ * y];
      };
      message += dummy.setNum( numDum, 'e', 5 );
      posLabel_ = new QLabel( message, MainControl::getMainControl(this)->statusBar() );
      MainControl::getMainControl(this)->statusBar()->addWidget( posLabel_, 0 );
      posLabel_->show();
    };
  };
}

void RegularData2DWidget::mousePressEvent( QMouseEvent *e )
{
  if (e->button() == QMouseEvent::RightButton)
  {
    men_->popup(mapToGlobal(e->pos()));
  }
}
