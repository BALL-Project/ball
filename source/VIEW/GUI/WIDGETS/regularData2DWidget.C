// $Id: regularData2DWidget.C,v 1.1 2000/11/15 18:17:29 anhi Exp $

#include <BALL/VIEW/GUI/WIDGETS/regularData2DWidget.h>

/* Converts to RGB-Space */
QColor con2rgb(double arg, double min, double max)
{
    double r, g, b;
    int ri, gi, bi;
    
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

PixWid::PixWid( QWidget *w ) : QWidget( w )
{
  setMouseTracking(true);
};

void PixWid::mouseMoveEvent( QMouseEvent *e )
{
  emit (mouseMoved(e->x(), e->y()));
}

RegularData2DWidget::RegularData2DWidget(vector<double> &d_, int lx_, int ly_, double min, double max, QWidget *parent) : QScrollView(parent), ModularWidget("RegularData2DWidget"), lengthx(lx_), lengthy(ly_), min_(min), max_(max), minx_(0), maxx_(1), miny_(0), maxy_(1), showMousePos_(true), posLabel_(0), soffsetf1_(0), soffsetf2_(0), swidthf1_(0), swidthf2_(0), bfreqf1_(0), bfreqf2_(0), spectrum_(0)
{
  createLegend( 20, 40 );

//   if (data)
//     createPlot();

  setCaption( "RegularData2DWidget" );

  // register the widget with the MainControl
  ModularWidget::registerWidget(this);
  snumpoints_ = lengthx * lengthy;
}

RegularData2DWidget::RegularData2DWidget(const RegularData2DWidget& widget) : QScrollView(), ModularWidget(widget)
{
}

RegularData2DWidget::~RegularData2DWidget()
{
}

void RegularData2DWidget::initializeWidget(MainControl& main_control)
{
  main_control.insertMenuEntry(MainControl::TOOLS, "&2D-NMR", this, SLOT(createPlot()));
}

void RegularData2DWidget::finalizeWidget(MainControl& main_control)
{
  main_control.removeMenuEntry(MainControl::TOOLS, "&2D-NMR", this, SLOT(createPlot()));
}

/**
 * Creates a legend of width w and height h.
 */
void RegularData2DWidget::createLegend( int w, int h )
{
  int x, y;
  QPainter paint;
  QColor pCol;

  if ( legendMap )
    delete ( legendMap );
  if ( bufferMap )
    delete ( bufferMap );

  legendMap = new QPixmap( w, h );
  bufferMap = new QPixmap( w + 4, h + 4 ); // save Background which is about to be destroyed by legend
  legendMap->fill();
  paint.begin( legendMap );

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
void RegularData2DWidget::scale(Size nx, Size ny)
{
  if (nx && ny && spectrum_)
  {
    QPainter paint;
    QColor pCol;

    Size stepx, stepy;
    int x, y;
    double actx, acty;

    pm->resize(nx, ny);
    pm->fill();

    fullLengthx = nx;
    fullLengthy = ny;

    lengthx = (Position) spectrum_->parsf2_->parameter("SI");
    lengthy = (Position) spectrum_->parsf1_->parameter("SI");

    stepx = lengthx / nx;
    stepy = lengthy / ny;

    actx = spectrum_->parsf2_->parameter("OFFSET");
    acty = spectrum_->parsf1_->parameter("OFFSET");

    paint.begin( pm );
    
    paint.setViewport(0, 0, lengthx, lengthy);
    
    // Transformation: (0,0) in untere linke Ecke.
    QWMatrix m(1, 0, 0, -1, 0, fullLengthy);
    paint.setWorldMatrix( m );

    for (y=0; y<ny; y++) {
	for (x=0; x<nx; x++) {
	    pCol = con2rgb(spec_->getValue(actx+x*stepx, acty+y*stepy), min_, max_);
	    paint.setPen( pCol );
	    paint.drawPoint( x, y );
	    };
	};
    }
}

/**
 * Creates a pixmap containing nx * ny data points from startx to endx and starty to endy
 */
void RegularData2DWidget::getData(Size nx, Size ny, double startx, double endx, double starty, double endy)
{
  Position actx, acty;
  double stepx, stepy;

  pm->resize(nx, ny);
  pm->fill();

  stepx = fabs(endx - startx) / nx;
  stepy = fabs(endy - starty) / ny;

  for (acty=0; acty<ny; acty++)
  {
    for (actx=0; actx<ny; actx++)
    {
      // create point in the pixmap...
      //      spec_->getValue(startx + stepx * actx, starty + stepy * acty);
    };
  };
}

/**
 * Creates a lorentzian peak in the data, having width xwidth, ywidth, amplitude amp, 
 * maximum in xpos, ypos.
 */
void RegularData2DWidget::addLorentzian( double xpos, double ypos, double amp, int xwidth=1, int ywidth=1 )
{
  int x, y;
  
  for (y=0; y<lengthy; y++) {
    if (spec_) {
      for (x=0; x<lengthx; x++) {
	(*spec_)[x + lengthx*y] += 1./(1 + pow(((x - xpos)/xwidth), 2) + pow(((y - ypos)/ywidth), 2));
      };
    };
  };
}

/**
 * Tries to load a spectrum from bruker-directory "dirName". Hopefully the directory's structure is like this in general.
 */
bool RegularData2DWidget::readSpec( string dirName )
{
  if (spectrum_)
    delete (spectrum_);
  spectrum_ = new Bruker2D( dirName );
  spectrum_->read();
  
  spec_ = spectrum_->GetData();

  min_ = spectrum_->parsf1_->parameter( "YMIN_p" );
  max_ = spectrum_->parsf1_->parameter( "YMAX_p" );

  lengthx = (int) spectrum_->parsf2_->parameter( "SI" );
  lengthy = (int) spectrum_->parsf1_->parameter( "SI" );

  soffsetf1_ = spectrum_->parsf1_->parameter( "OFFSET" );
  soffsetf2_ = spectrum_->parsf2_->parameter( "OFFSET" );

  swidthf1_ = spectrum_->parsf1_->parameter( "SW_p" );
  swidthf2_ = spectrum_->parsf2_->parameter( "SW_p" );
  
  bfreqf1_ = spectrum_->parsf1_->parameter( "SF" );
  bfreqf2_ = spectrum_->parsf2_->parameter( "SF" );

  spectrum_->SetShiftRange(soffsetf1_, soffsetf2_, swidthf1_, swidthf2_, bfreqf1_, bfreqf2_, lengthy, lengthx);
  
  // TEST!!!
  //  spec_->createGroundState();
  //cout << "gr: " << spec_->getGroundState() << "sigma: " << spec_->getSigmaGroundState() << endl;
}

void RegularData2DWidget::createPlot()
{
  int x, y;
  QPainter paint;
  QColor pCol;

  // cleanup needed?
  if (pm)
    delete (pm); 
  if (pixWid)
    delete (pixWid);

//   // do we have to clip or interpolate?
//   if ((width() > lengthx) && (height() > lengthy)) { // interpolate in x - and y - direction
//     scaleData( lengthx, lengthy );
//   };

//   if ((width() <= lengthx) && (height() <= lengthy)) { // clip in x - and y - direction
//     fullData = spec_;
//     fullLengthx = lengthx;
//     fullLengthy = lengthy;
//   };

  //  fullData = spec_;
  fullLengthx = lengthx;
  fullLengthy = lengthy;

  pixWid = new PixWid( viewport() );
  connect( pixWid, SIGNAL(mouseMoved(int,int)), this, SLOT(NewMousePos(int,int)));
  pixWid->resize( fullLengthx, fullLengthy );
  // Eigentliches eintragen der Punkte
  pm = new QPixmap( fullLengthx, fullLengthy );
  pm->fill();
  paint.begin( pm );

  paint.setViewport(0, 0, fullLengthx, fullLengthy);

  // Transformation: (0,0) in untere linke Ecke.
  QWMatrix m(1, 0, 0, -1, 0, fullLengthy);
  paint.setWorldMatrix( m );

  for (y=0; y<fullLengthy; y++) {
    for (x=0; x<fullLengthx; x++) {
      pCol = con2rgb((*spec_)[x + y*fullLengthx], min_, max_);
      paint.setPen( pCol );
      paint.drawPoint( x, y );
    };
  };

//   // TEST!
//   list< pair<double, double> > t;
//   t = spectrum_->GetContourLine(0.7 * max_);
//   QPoint f1, f2;
//   paint.setPen(QColor(black));
//   _List_iterator<pair<double,double>,pair<double,double>&,pair<double,double>*> it;
//   it = t.begin();
//   pair<Position, Position> pd;

//   for (int i=0; i<t.size()-1; i++)
//   {
//     pd = spectrum_->GetPosition(it->first, it->second);
//     f1 = QPoint(pd.first, pd.second);
//     it++;
//     i++;
//     pd = spectrum_->GetPosition(it->first, it->second);
//     f2 = QPoint(pd.first, pd.second);
//     paint.drawLine(f1, f2);
//     it++;
//   };
//   // TEST!

  paint.end();
  addChild( pixWid );
  // Hack to enable display of scrollbars etc...
  pixWid->show();
  QScrollView::resizeEvent(new  QResizeEvent( size(), size() ));
}

void RegularData2DWidget::drawContents( QPainter *paint, int clipx, int clipy, int clipw, int cliph )
{
  if (pm && (pm->size() != QSize( 0, 0 ))) { // do we have something to paint?
    paint->end();
    paint->begin( pixWid );
    paint->drawPixmap(clipx, clipy, *pm, clipx, clipy, clipw, cliph);

    // Zeichnen der Bildlegende.
    if (legendMap) {
      bitBlt( pixWid, legendLastX, legendLastY, bufferMap );

      legendLastX = contentsX() + visibleWidth() - legendMap->width() - 12;
      legendLastY = contentsY() + 8;
      
      bitBlt( bufferMap, 0, 0, pixWid, legendLastX, legendLastY );
      
      paint->setPen( QColor( black ) );
      paint->drawRect( legendLastX, legendLastY, legendMap->width() + 4, legendMap->height() + 4 );
      paint->setPen( QColor( white ) );
      paint->drawRect( legendLastX + 1, legendLastY + 1, legendMap->width() + 2, legendMap->height() + 2 );
      paint->drawPixmap( legendLastX + 2, legendLastY + 2, *legendMap );
    };
  };
}

void RegularData2DWidget::paintEvent( QPaintEvent *e )
{
  if (pm && (pm->size() != QSize( 0, 0 ))) { // is there something to paint?
    QPainter paint( pixWid );
    paint.setClipRect( contentsX(), contentsY(), contentsWidth(), contentsHeight() );
    paint.drawPixmap( contentsX(), contentsY(), *pm, contentsX(), contentsY(), contentsWidth(), contentsHeight() );

    // draw the legend.
    if (legendMap) {
      legendLastX = contentsX() + visibleWidth() - legendMap->width() - 12;
      legendLastY = contentsY() + 8;

      bitBlt( bufferMap, 0, 0, pixWid, contentsX() + visibleWidth() - legendMap->width() - 12, contentsY() + 8, legendMap->width() + 4, legendMap->height() + 4 );

      paint.setPen( QColor( black ) );
      paint.drawRect( legendLastX, legendLastY, legendMap->width() + 4, legendMap->height() + 4 );
      paint.setPen( QColor( white ) );
      paint.drawRect( legendLastX + 1, legendLastY + 1, legendMap->width() + 2, legendMap->height() + 2 );
      paint.drawPixmap( legendLastX + 2, legendLastY + 2, *legendMap );
    };
  };
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

void RegularData2DWidget::NewMousePos( int x, int y )
{
  y = fullLengthy - y;
      
  if (showMousePos_) {
    if (MainControl::getMainControl( this )->statusBar() && fullLengthx && fullLengthy) {
      if (posLabel_)
	{
	  MainControl::getMainControl( this )->statusBar()->removeWidget( posLabel_ );
	  delete posLabel_;
	  posLabel_ = NULL;
	};

      QString message, dummy;
      double numDum;
      pair<double, double> numDum2;

      numDum2 = spectrum_->GetShift(x, y);

      message = "x: ";
      message += dummy.setNum( numDum2.first, 'e', 5 );
      message += "y: ";
      message += dummy.setNum( numDum2.second, 'e', 5 );
      message += "z: ";
      if (spec_ && ((x+fullLengthx*y) >=0) && (y < fullLengthy)) {
	numDum = (*spec_)[x + fullLengthx * y];
      };
      message += dummy.setNum( numDum, 'e', 5 );
      posLabel_ = new QLabel( message, MainControl::getMainControl(this)->statusBar() );
      MainControl::getMainControl(this)->statusBar()->addWidget( posLabel_ );
      posLabel_->show();
    };
  };
}

Bruker2D& RegularData2DWidget::GetBrukerFile()
{
  return(*spectrum_);
}

void RegularData2DWidget::SetShiftRange(double offsetf1, double offsetf2, double swidthf1, double swidthf2, double bfreqf1, double bfreqf2)
{
  soffsetf1_ = offsetf1;
  soffsetf2_ = offsetf2;
  swidthf1_ = swidthf1;
  swidthf2_ = swidthf2;
  bfreqf1_ = bfreqf1;
  bfreqf2_ = bfreqf2;

  if (!spectrum_)
  {
    spectrum_ = new Bruker2D();
  };
  spectrum_->SetShiftRange(soffsetf1_, soffsetf2_, swidthf1_, swidthf2_, bfreqf1_, bfreqf2_, lengthy, lengthx);
}
