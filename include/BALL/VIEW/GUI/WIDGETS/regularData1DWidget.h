// $Id: regularData1DWidget.h,v 1.4 2001/06/06 14:25:07 anhi Exp $

#ifndef BALL_VIEW_GUI_WIDGET_REGULARDATA1DWIDGET_H
#define BALL_VIEW_GUI_WIDGET_REGULARDATA1DWIDGET_H

#include <qscrollview.h>
#include <qpixmap.h>

#ifndef BALL_COMMON_H
#       include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_COMPOSITE_H
#       include <BALL/CONCEPT/composite.h>
#endif

#ifndef BALL_VIEW_KERNEL_MESSAGE_H
#       include <BALL/VIEW/KERNEL/message.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
#       include <BALL/DATATYPE/string.h>
#endif

#ifndef BALL_VIEW_GUI_WIDGETS_MODULARWIDGET_H
#       include <BALL/VIEW/GUI/WIDGETS/modularWidget.h>
#endif

#ifndef BALL_VIEW_GUI_KERNEL_MAINCONTROL_H
#       include <BALL/VIEW/GUI/KERNEL/mainControl.h>
#endif

#ifndef BALL_DATATYPE_REGULARDATA1D_H
#       include <BALL/DATATYPE/regularData1D.h>
#endif

using namespace BALL;
using namespace BALL::VIEW;

/**    New RegularData1D Message Class
 */
class NewRegularData1DMessage: public CompositeMessage
{
 public:
  /**    @name Constructors and Destructors
   */
  //@{
  NewRegularData1DMessage();

  NewRegularData1DMessage(const CompositeMessage& message, bool deep = true);

  virtual ~NewRegularData1DMessage()
    throw();
  //@}

  /**     @name   Assignment
   */
  //@{
  //@}

  /**     @name   Accessors
   */
  //@{
  //@}

  /**     @name   Debugging and Diagnostics
   */
  //@{
  //@}

  /**     @name   Storers
   */
  //@{
  //@}

 private:
};


/**      Widget used to display 1-dimensional NMR-spectra.
 */
class RegularData1DWidget
  : public QScrollView,
    public ModularWidget
{
  Q_OBJECT
 public:
  // macro for create method
  //  BALL_CREATE(RegularData1DWidget);

  /** Constructors and Destructors
   */
  //@{
      /// Detailed constructor
      RegularData1DWidget(int l, double min=0, double max=1., QWidget *parent = 0)
	throw();

      /// Copy constructor
      RegularData1DWidget(const RegularData1DWidget& widget)
	throw();

      /// Destructor
      virtual ~RegularData1DWidget()
	throw();
  //@}

  /** Assignment
   */
  //@{
      /// Assignment operator
      const RegularData1DWidget& operator = (const RegularData1DWidget &widget)
	throw();

      /// Clear method
      virtual void clear()
	throw();
  //@}

  /** @name    Predicates
   */
  //@{

      /// Equality operator. Two spectra are equal by definition if they point to the same data and have the same length, min and max.
      bool operator == (const RegularData1DWidget &widget) const
	throw();

  //@}

  void onNotify(Message *message)
  throw();

public slots:
	
  bool reactToMessages_(Message *message);
  
  /**    @name    ModularWidget related methods
   */
  //@{
  /**    Setup the menu entries.
	 RegularData1DWidget creates an entry in Tools|RegularData1DWidget and connects
	 the entry to createPlot()
  */
  virtual void initializeWidget(MainControl& main_control);

  /**    Remove menu entries.
   */
  virtual void finalizeWidget(MainControl& main_control);
  //@}

 public slots:

  void createPlot();

  void drawContents(QPainter *paint, int clipx, int clipy, int clipw, int cliph);
  void paintEvent(QPaintEvent *e);
  void resizeEvent(QResizeEvent *e);
    
 protected:
      QPixmap pm_;
      Size length_;
      double min_;
      double max_;
      RegularData1D *spec_;
};

#endif // BALL_VIEW_GUI_WIDGET_REGULARDATA1DWIDGET_H
