#include <BALL/VIEW/WIDGETS/dockableRegularData1DWidget.h>
#include <qpopupmenu.h>
#include <qapplication.h>

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
	 		QSizePolicy sizePolicy (QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);		
			setSizePolicy(sizePolicy);
			setMinimumSize(sizeHint());
      setGuest(canWidget_);  //  canWidget_.resize(120,100);Sets the size of the canWidget_ excluding any window frame.  
	    canWidget_.createPolygon();
			getWidget().resize(size()); //canWidget_.resize(size()); size(): Returns the size of the widget excluding any window frame
//			 canWidget_.zoomToFit(); // this will be ignored because of size hints 
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
      canWidget_.showObjects();
//			resize(600, 400); // ???????????? 
			show();
			//for testing
				getWidget().resize(size()); //canWidget_.resize(size()); size(): Returns the size of the widget excluding any window frame
				//showSize();
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
			Position menu_entry_pos3 = context_menu.insertItem("ZoomIn (debug!!!)", this, SLOT(zoomIn()));
			Position menu_entry_pos4 = context_menu.insertItem("ZoomOut (debug!!!)", this, SLOT(zoomOut()));
			QPoint pos = e->reason() == QContextMenuEvent::Mouse ? e->globalPos() :
		 							mapToGlobal( QPoint(e->pos().x(), 0) ) + QPoint( width() / 2, height() / 2 );
	//		connect(&context_menu, SIGNAL(activated(int)), this, SLOT(reactToMessages(int)));
			context_menu.exec(pos);
			e->accept();
		}
		
		void DockableRegularData1DWidget::reactToMessages(int number)
		{
			if (number==1)
			{
				canWidget_.zoomToFit();
			}
		}

		void DockableRegularData1DWidget::zoomToFit()
		{
			canWidget_.zoomToFit();
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
			return QSize((qApp->mainWidget()->width()/3.)+55, (qApp->mainWidget()->height()/3.)+55);
		}
		
  }// End of namespace VIEW
}//End of namespae BALL

