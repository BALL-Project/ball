#ifndef BALL_VIEW_WIDGETS_DOCKABLEREGULARDATA1DWIDGET_H
#define BALL_VIEW_WIDGETS_DOCKABLEREGULARDATA1DWIDGET_H

#ifndef BALL_VIEW_WIDGETS_REGULARDATA1DWIDGET_H
# include <BALL/VIEW/WIDGETS/regularData1DWidget.h>
#endif

#ifndef BALL_VIEW_WIDGETS_DOCKWIDGET_H
# include <BALL/VIEW/WIDGETS/dockWidget.h>
#endif

namespace BALL
{
  namespace VIEW
  {      
    class DockableRegularData1DWidget
      : public DockWidget
    {
      Q_OBJECT                        //macro for QT messages
    public:

      //Constructor
      DockableRegularData1DWidget(const RegularData1D& data, QWidget *parent=0 );

      //   DockableRegularData1DWidget(QWidget *parent=0 );

      //Copyconstructor 
      //  DockableRegularData1DWidget(const  DockableRegularData1DWidget& dockcanwid);

      //Destructor 
      ~DockableRegularData1DWidget()
	throw();

      //methods
      void plot()
	throw();
  
      RegularData1DWidget& getWidget()
	throw();

    protected:
      RegularData1DWidget canWidget_;

    };//end of class dockableRegularData1DWidget
    
  }
}


#endif
