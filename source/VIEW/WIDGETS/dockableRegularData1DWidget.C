#include <BALL/VIEW/WIDGETS/dockableRegularData1DWidget.h>


namespace BALL
{
  namespace VIEW
  {
    //---------------------Class dockableRegularData1DWidget

    //Constructor
    DockableRegularData1DWidget::DockableRegularData1DWidget(const RegularData1D& data, QWidget *parent)
      : DockWidget(parent,"Dockable RegularData1D Widget"),
        canWidget_(data, this)
    {
       setGuest(canWidget_);
       canWidget_.createPolygon();
			 getWidget().resize(size());
			 canWidget_.zoomToFit();
    }

    /*  DockableRegularData1DWidget::DockableRegularData1DWidget(QWidget *parent)
      : DockWidget(parent)
    {
    }
    */

   //Destructor
    DockableRegularData1DWidget::~DockableRegularData1DWidget()
      throw()
    {
    }
   //methods:

    void DockableRegularData1DWidget::plot()
      throw()
    {
      //    setGuest(canWidget_);
      canWidget_.showObjects();
    }

    RegularData1DWidget& DockableRegularData1DWidget::getWidget()
	throw()
    {
	return canWidget_;
    }

  }// End of namespace VIEW
}//End of namespae BALL

