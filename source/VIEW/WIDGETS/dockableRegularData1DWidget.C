#include <BALL/VIEW/WIDGETS/dockableRegularData1DWidget.h>
#include <qpopupmenu.h>

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
//			 canWidget_.zoomToFit();

//			 connect(this, SIGNAL(rausfinden...), this, SLOT(slotMouseMenu()));
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
			Position menu_entry_pos = context_menu.insertItem( "Show Size (debug!!!)", this, SLOT(showSize()));
			Position menu_entry_pos2 = context_menu.insertItem("ZoomToFit (debug!!!)", this, SLOT(zoomToFit()));
			Position menu_entry_pos3 = context_menu.insertItem("ZoomIn (debug!!!)");
			Position menu_entry_pos4 = context_menu.insertItem("ZoomOut (debug!!!)");
			QPoint pos = e->reason() == QContextMenuEvent::Mouse ? e->globalPos() :
		 							mapToGlobal( QPoint(e->pos().x(), 0) ) + QPoint( width() / 2, height() / 2 );
			
			connect(&context_menu, SIGNAL(activated(int)), this, SLOT(reactToMessages(int)));
			context_menu.exec(pos);
//context_menu.setItemEnabled(menu_entry_pos, true);
			e->accept();
		}
	
		void DockableRegularData1DWidget::zoomToFit()
		{
			canWidget_.zoomToFit();
		}

		void DockableRegularData1DWidget::reactToMessages(int number)
		{
			if (number==1)
			{
				canWidget_.zoomToFit();
			}
		}

		void DockableRegularData1DWidget::showSize()
		{
				Log.info() << this->width() << std::endl;
		}

  }// End of namespace VIEW
}//End of namespae BALL

