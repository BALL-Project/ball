#include <BALL/VIEW/WIDGETS/dockableRegularData1DWidget.h>
#include <qpopupmenu.h>
#include <qapplication.h>

namespace BALL
{
  namespace VIEW
  {

DockableRegularData1DWidget::DockableRegularData1DWidget(const RegularData1D* data, QWidget *parent)
	: DockWidget(parent,"Dockable RegularData1D Widget"),
		canWidget_(data, this)
{
	QSizePolicy sizePolicy (QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);		
	setSizePolicy(sizePolicy);
	setMinimumSize(sizeHint());
	setGuest(canWidget_);  
	canWidget_.createPlot();
	getWidget().resize(size()); 
}


DockableRegularData1DWidget::~DockableRegularData1DWidget()
	throw()
{
}

void DockableRegularData1DWidget::plot()
	throw()
{
	canWidget_.showObjects();
	show();
	canWidget_.zoomToFit();
}

RegularData1DWidget& DockableRegularData1DWidget::getWidget()
	throw()
{
	return canWidget_;
}

void DockableRegularData1DWidget::contextMenuEvent(QContextMenuEvent* e)
{
	//slot, connected to pressMousEvent
	QPopupMenu context_menu;
// 	Position menu_entry_pos = context_menu.insertItem( "Show Size (debug!!!)", this, SLOT(showSize()));
	context_menu.insertItem("ZoomToFit", this, SLOT(zoomToFit()));
	context_menu.insertItem("ZoomIn", this, SLOT(zoomIn()));
	context_menu.insertItem("ZoomOut", this, SLOT(zoomOut()));
	QPoint pos = e->reason() == QContextMenuEvent::Mouse ? e->globalPos() :
		mapToGlobal( QPoint(e->pos().x(), 0) ) + QPoint( width() / 2, height() / 2 );
 	context_menu.exec(pos);
	e->accept();
}

void DockableRegularData1DWidget::showSize()
{
		Log.info() << "dockableRegularData1DWidget width :" << width() << std::endl;
		Log.info() << "dockableRegularData1DWidget height :" << height() << std::endl;
		Log.info() << "canWidget_.width() :" << canWidget_.width() << std::endl;
		Log.info() << "canWidget_.height() :" << canWidget_.height()  << std::endl;
		Log.info() << "canWidget_.viewport()->width() :" <<  canWidget_.viewport()->width() << std::endl;
		Log.info() << "canWidget_.viewport()->height() :" << canWidget_.viewport()->height()  << std::endl;
		Log.info() << "canWidget_.getCanvas().width() :" << canWidget_.getCanvas().width() << std::endl;
		Log.info() << "canWidget_.getCanvas().height() :" << canWidget_.getCanvas().height()  << std::endl;
}

void DockableRegularData1DWidget::zoomToFit()
{
	canWidget_.zoomToFit();
}

void DockableRegularData1DWidget::zoomIn()
{
	canWidget_.zoomIn();
}

void DockableRegularData1DWidget::zoomOut()
{
	canWidget_.zoomOut();
}

//we need this for SizePolicy
QSize DockableRegularData1DWidget::sizeHint() const
{
	return QSize((int)(qApp->mainWidget()->width()/3.)+55, (int)(qApp->mainWidget()->height()/3.)+55);
}
		
  }// End of namespace VIEW
}//End of namespae BALL

