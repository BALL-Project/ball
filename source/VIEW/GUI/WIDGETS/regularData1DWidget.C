// $Id: regularData1DWidget.C,v 1.5 2001/06/06 14:14:46 anhi Exp $

#include <BALL/VIEW/GUI/WIDGETS/regularData1DWidget.h>

using std::endl;
using std::cout;

NewRegularData1DMessage::NewRegularData1DMessage()
  :    CompositeMessage()
{
}

NewRegularData1DMessage::NewRegularData1DMessage(const CompositeMessage &message, bool /* deep */)
  :    CompositeMessage(message)
{
}

NewRegularData1DMessage::~NewRegularData1DMessage()
  throw()
{
  #ifdef BALL_VIEW_DEBUG
  cout << "Destructing object " << (void *)this
       << " of class " << RTTI::getName<NewRegularData1DMessage>() << endl;
  #endif
}

RegularData1DWidget::RegularData1DWidget(int l, double min, double max, QWidget *parent) 
  throw()
  : QScrollView(parent), 
    ModularWidget("RegularData1DWidget"), 
    pm_(0), 
    length_(l), 
    min_(min), 
    max_(max)
{
  setCaption("RegularData1DWidget");

  // register the widget with the MainControl
  ModularWidget::registerWidget(this);

  viewport()->setMouseTracking(true);
}

RegularData1DWidget::RegularData1DWidget(const RegularData1DWidget& widget) 
  throw()
  : QScrollView(), 
    ModularWidget("RegularData1DWidget"),
    spec_(0)
{
}

RegularData1DWidget::~RegularData1DWidget()
  throw()
{
}

void RegularData1DWidget::initializeWidget(MainControl& main_control)
{
  main_control.insertMenuEntry(MainControl::TOOLS, "&1D-NMR", this, SLOT(createPlot()));
}

void RegularData1DWidget::finalizeWidget(MainControl& main_control)
{
  main_control.removeMenuEntry(MainControl::TOOLS, "&1D-NMR", this, SLOT(createPlot()));
}

void RegularData1DWidget::onNotify(Message* message)
  throw()
{
  reactToMessages_(message);
}

bool RegularData1DWidget::reactToMessages_(Message* message)
{
  if (RTTI::isKindOf<NewRegularData1DMessage>(*message))
  {
    NewRegularData1DMessage *composite_message = RTTI::castTo<NewRegularData1DMessage>(*message);
    
    // set the RegularData1D we have been sent. Then set all the other attributes.
    if (spec_)
    {
      cout << spec_ << endl;
      //      delete (spec_);
    }
    spec_ = (RegularData1D *)composite_message->getComposite();
    min_ = spec_->getLowerBound();
    max_ = spec_->getUpperBound();
    length_ = spec_->getSize();

    createPlot();
    
    return true;
  }
  
  return false;
}

const RegularData1DWidget& RegularData1DWidget::operator = (const RegularData1DWidget &widget)
  throw()
{
  pm_     =     widget.pm_;
  length_ = widget.length_;
  min_    =    widget.min_;
  max_    =    widget.max_;
  spec_   =   widget.spec_;

  return *this;
}

void RegularData1DWidget::clear()
  throw()
{
  pm_ = QPixmap();
  length_ = 0;
  min_ = 0;
  max_ = 0;
  if (spec_)
  {
    delete spec_;
    spec_ = 0;
  }
}

bool RegularData1DWidget::operator == (const RegularData1DWidget &widget) const
  throw()
{
  if ((spec_ == widget.spec_) && (length_ == widget.length_) &&
      (min_  == widget.min_)  && (max_    == widget.max_))
  {
    return true;
  }

  return false;
}

void RegularData1DWidget::createPlot()
{
  QPointArray fdummy(length_);

	// we have to be careful not to lose important digits
  double max_el(spec_->getUpperBound());

  pm_.fill();
  resize( length_, height() );
  pm_.resize( length_, height() );

  QPainter p( &pm_ );

  // scale
  p.setWindow(  0, 0, length_, max_el);
  p.setViewport(0, 0, length_, height());
  
  // transform: set (0,0) to lower left corner
  QWMatrix m(1, 0, 0, -1, 0, max_el);
  p.setWorldMatrix(m);

  for (Size i=0; i<length_; i++)
  {
    fdummy.putPoints(i, 1, i, (*spec_)[i]*10e4);
  }
  p.setPen( QColor(red) );
  p.drawPolyline( fdummy );


p.drawText(length_/2, (*spec_)[length_/2], "Hallo!");
cout << (*spec_)[length_/2] << endl; 
  p.drawLine(0,0,length_, max_el);

  p.end();

  //  repaint();
}

void RegularData1DWidget::drawContents(QPainter *paint, int clipx, int clipy, int clipw, int cliph)
{
  if ((pm_.size() != QSize(0,0)))
  {
    paint->drawPixmap(clipx, clipy, pm_, clipx, clipy, clipw, cliph);
  };
}

void RegularData1DWidget::paintEvent(QPaintEvent *e)
{
  if ((pm_.size() != QSize(0,0)))
  {
    QPainter paint(viewport());

    pm_.save("abc.ppm", "PPMRAW");
    
    paint.setClipRect(contentsX(), contentsY(), contentsWidth(), contentsHeight());
    paint.drawPixmap(contentsX(), contentsY(), pm_, contentsX(), contentsY(), contentsWidth(), contentsHeight());
  };
}

void RegularData1DWidget::resizeEvent(QResizeEvent *e)
{
  QScrollView::resizeEvent(e);
  repaint();
}
