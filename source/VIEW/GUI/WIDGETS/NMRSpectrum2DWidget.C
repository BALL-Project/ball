#include <BALL/VIEW/OPENGL/KERNEL/mainControl.h>

#include "nmr.moc"

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

nmr::nmr(vector<double> &d_, int lx_, int ly_, double min, double max, QWidget *parent) : QScrollView(parent), ModularWidget("Nmr"), specDat_(d_), lengthx(lx_), lengthy(ly_), min_(min), max_(max), minx_(0), maxx_(1), miny_(0), maxy_(1), showMousePos_(true), posLabel_(0)
{
  createLegend( 20, 40 );

//   if (data)
//     createPlot();

  setCaption( "NMR" );

  // register the widget with the MainControl
  ModularWidget::registerWidget(this);
}

nmr::nmr(const nmr& widget) : QScrollView(), ModularWidget(widget)
{
}

nmr::~nmr()
{
}

void nmr::initializeWidget(MainControl& main_control)
{
  main_control.insertMenuEntry(MainControl::TOOLS, "&nmr", this, SLOT(createPlot()));
}

void nmr::finalizeWidget(MainControl& main_control)
{
  main_control.removeMenuEntry(MainControl::TOOLS, "&nmr", this, SLOT(createPlot()));
}

/**
 * Creates a legend of width w and height h.
 */
void nmr::createLegend( int w, int h )
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

/**
 * Creates a pixmap of size xlength, ylength. Data will be scaled acordingly.
 */
void nmr::scaleData( int xlength, int ylength )
{
  // cleanup?
  if (pm)
    {
      delete (pm);
    };

  pm = new QPixmap( xlength, ylength );
  pm->fill();
}

/**
 * Creates a lorentzian peak in the data, having width xwidth, ywidth, amplitude amp, 
 * maximum in xpos, ypos.
 */
void nmr::addLorentzian( double xpos, double ypos, double amp, int xwidth=1, int ywidth=1 )
{
  int x, y;
  
  for (y=0; y<lengthy; y++) {
    for (x=0; x<lengthx; x++) {
      specDat_[x + lengthx*y] += 1./(1 + pow(((x - xpos)/xwidth), 2) + pow(((y - ypos)/ywidth), 2));
    };
  };
}

/**
 * Tries to load a spectrum from bruker-directory "dirName". Hopefully the directory's structure is like this in general.
 */
bool nmr::readSpec( string dirName )
{
  spectrum_ = new Bruker2D( dirName );
  spectrum_->read( specDat_ );

  min_ = spectrum_->parsf1_->parameter( "YMIN_p" );
  max_ = spectrum_->parsf1_->parameter( "YMAX_p" );

  lengthx = (int) spectrum_->parsf2_->parameter( "SI" );
  lengthy = (int) spectrum_->parsf1_->parameter( "SI" );
}

void nmr::createPlot()
{
  int x, y;
  QPainter paint;
  QColor pCol;

  // Vielleicht muessen wir noch aufraeumen...
  if (pm)
    delete (pm); 
  if (pixWid)
    delete (pixWid);

  // Zuerst muessen wir rausfinden, ob wir interpolieren oder clippen muessen
  if ((width() > lengthx) && (height() > lengthy)) { // interpolieren in x - und y - Richtung. 
    scaleData( lengthx, lengthy );
  };

  if ((width() <= lengthx) && (height() <= lengthy)) { // clippen in beide Richtungen.
    fullData = specDat_;
    fullLengthx = lengthx;
    fullLengthy = lengthy;
  };

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
      pCol = con2rgb(fullData[x + y*fullLengthx], min_, max_);
      paint.setPen( pCol );
      paint.drawPoint( x, y );
    };
  };

  paint.end();
  addChild( pixWid );
  // kleiner Hack, sonst: keine Darstellung von Inhalt & Scrollbars...
  pixWid->show();
  QScrollView::resizeEvent(new  QResizeEvent( size(), size() ));
}

void nmr::drawContents( QPainter *paint, int clipx, int clipy, int clipw, int cliph )
{
  if (pm && (pm->size() != QSize( 0, 0 ))) { // haben wir ein Bild?
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

void nmr::paintEvent( QPaintEvent *e )
{
  if (pm && (pm->size() != QSize( 0, 0 ))) { // haben wir ein Bild?
    QPainter paint( pixWid );
    paint.setClipRect( contentsX(), contentsY(), contentsWidth(), contentsHeight() );
    paint.drawPixmap( contentsX(), contentsY(), *pm, contentsX(), contentsY(), contentsWidth(), contentsHeight() );

    // Zeichnen der Bildlegende.
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

void nmr::resizeEvent( QResizeEvent *e )
{
  QScrollView::resizeEvent( e );
  repaint();
}

void nmr::enterEvent(QEvent *)
{
  //  showMousePos_ = true;
}

void nmr::leaveEvent(QEvent *)
{
  if (posLabel_) {
    if (MainControl::getMainControl( this )->statusBar()) {
      MainControl::getMainControl( this )->statusBar()->removeWidget( posLabel_ );
    };
    posLabel_->hide();;
  };
  //  showMousePos_ = false;
}

void nmr::NewMousePos( int x, int y )
{
  y = fullLengthy - y;
      
  if (showMousePos_) {
    if (MainControl::getMainControl( this )->statusBar() && fullLengthx && fullLengthy) {
      if (posLabel_)
	{
	  MainControl::getMainControl( this )->statusBar()->removeWidget( posLabel_ );
	  delete posLabel_;
	};

      QString message, dummy;
      double numDum;

      message = "x: ";
      numDum = (((double)x / fullLengthx) * (maxx_ - minx_)) + minx_;
      message += dummy.setNum( numDum );
      message += "y: ";
      numDum = (((double)y / fullLengthy) * (maxy_ - miny_)) + miny_;
      message += dummy.setNum( numDum );
      message += "z: ";
      numDum = fullData[x + fullLengthx * y];
      message += dummy.setNum( numDum );
      posLabel_ = new QLabel( message, MainControl::getMainControl(this)->statusBar() );
      MainControl::getMainControl(this)->statusBar()->addWidget( posLabel_ );
      posLabel_->show();
    };
  };
}
