// $Id: NMRSpectrum1DWidget.C,v 1.1 2000/11/15 18:17:29 anhi Exp $

#include <BALL/VIEW/GUI/WIDGETS/NMRSpectrum1DWidget.h>

/* Default constructor */
NMRSpectrum1DWidget::NMRSpectrum1DWidget() 
  throw()
  : QScrollView(), ModularWidget(), pm_(0), length_(0), data_(0), min_(0), max_(0), pixWid_(0)
{
}

/* Copy constructor */
NMRSpectrum1DWidget::NMRSpectrum1DWidget(const NMRSpectrum1DWidget& widget) 
  throw()
  : QScrollView(), ModularWidget("NMRSpectrum1DWidget")
{
}

/* Detailed constructor */
NMRSpectrum1DWidget::NMRSpectrum1DWidget(vector<double> &d, Size l, Size min, Size max, QWidget *parent) 
  throw()
  : QScrollView(parent), ModularWidget("NMRSpectrum1DWidget"), pm_(0), length_(l), data_(d), min_(min), max_(max), pixWid_(0)
{
  setCaption( "NMRSpectrum1DWidget" );

  // register the widget with the MainControl
  ModularWidget::registerWidget(this);
}

/* Destructor */
NMRSpectrum1DWidget::~NMRSpectrum1DWidget()
  throw()
{
}

const Size NMRSpectrum1DWidget::getLength() const
  throw()
{
  return (length_);
}

const Size NMRSpectrum1DWidget::getMin() const
  throw()
{
  return min_;
}

const Size NMRSpectrum1DWidget::getMax() const
  throw()
{
  return max_;
}

const vector<double>& NMRSpectrum1DWidget::getData() const
  throw()
{
  return data_;
}

void NMRSpectrum1DWidget::initializeWidget(MainControl& main_control)
{
  main_control.insertMenuEntry(MainControl::TOOLS, "&1D-NMR", this, SLOT(createPlot()));
}

void NMRSpectrum1DWidget::finalizeWidget(MainControl& main_control)
{
  main_control.removeMenuEntry(MainControl::TOOLS, "&1D-NMR", this, SLOT(createPlot()));
}

const NMRSpectrum1DWidget& NMRSpectrum1DWidget::operator = (const NMRSpectrum1DWidget &widget)
  throw()
{
  pm_     =     widget.pm_;
  data_   =   widget.data_;
  length_ = widget.length_;
  min_    =    widget.min_;
  max_    =    widget.max_;

  return *this;
}

void NMRSpectrum1DWidget::clear()
  throw()
{
  pm_ = QPixmap();
  data_ = vector<double>(0);
  length_ = 0;
  min_ = 0;
  max_ = 0;
}

bool NMRSpectrum1DWidget::operator == (const NMRSpectrum1DWidget &widget) const
  throw()
{
  if ((data_ == widget.getData()) && (length_ == widget.getLength()) 
      && (min_ == widget.getMin()) && (max_ == widget.getMax()))
  {
    return (true);
  };
  return (false);
}

void NMRSpectrum1DWidget::createPlot()
{
  QPointArray fdummy(length_);
  int i;
  double min_el, max_el;

  if (pixWid_)
    delete (pixWid_);

  min_el = *min_element(data_.begin(), data_.end()) * 10e4; // we have to be careful not to lose important digits
  max_el = *max_element(data_.begin(), data_.end()) * 10e4;

  pm_.fill();
  resize( length_, 300 );
  pm_.resize( length_, 400 );

  QPainter p( &pm_ );
  pixWid_ = new PixWid(viewport());
  pixWid_->resize(length_, 400);

  // scale
  p.setWindow(0, 0, length_, max_el);

  p.setViewport(0, 0, length_, height());
  
  // transform: set (0,0) to lower left corner
  QWMatrix m(1, 0, 0, -1, 0, max_el);
  p.setWorldMatrix(m);


  for (i=0; i<length_; i++)
  {
    fdummy.putPoints(i, 1, i, 10e4 * data_[i]);
  }
  p.setPen( QColor(red) );
  p.drawPolyline( fdummy );

  p.end();

  addChild(pixWid_);
  pixWid_->show();
  QScrollView::resizeEvent(new QResizeEvent(size(), size())); // little hack to ensure correct display...
  repaint();
}

/** Tries to load a spectrum from bruker-directory "dirName". Hopefully the directory's structure is like this in general.
 */
bool NMRSpectrum1DWidget::readSpec( String dirName )
{
  spectrum_ = new Bruker1D(dirName);
  spectrum_->read(data_);

  min_ = (Size) spectrum_->getParameters()->parameter( "YMIN_p" );
  max_ = (Size) spectrum_->getParameters()->parameter( "YMAX_p" );

  length_ = (Size) spectrum_->getParameters()->parameter( "SI" );
}

void NMRSpectrum1DWidget::drawContents(QPainter *paint, int clipx, int clipy, int clipw, int cliph)
{
  if (pm_.size() != QSize(0,0))
  {
    paint->end();
    paint->begin(pixWid_);
    paint->drawPixmap(clipx, clipy, pm_, clipx, clipy, clipw, cliph);
  };
}

void NMRSpectrum1DWidget::paintEvent(QPaintEvent *e)
{
  if (pm_.size() != QSize(0,0))
  {
    QPainter paint(pixWid_);
    
    paint.setClipRect(contentsX(), contentsY(), contentsWidth(), contentsHeight());
    paint.drawPixmap(contentsX(), contentsY(), pm_, contentsX(), contentsY(), contentsWidth(), contentsHeight());
  };
}

void NMRSpectrum1DWidget::resizeEvent(QResizeEvent *e)
{
  QScrollView::resizeEvent(e);
  repaint();
}
