// $Id: regularData2DWidget.h,v 1.14 2001/05/27 10:26:37 hekl Exp $

#ifndef BALL_VIEW_GUI_WIDGET_REGULARDATA2DWIDGET_H
#define BALL_VIEW_GUI_WIDGET_REGULARDATA2DWIDGET_H

#include <qlabel.h>
#include <qwidget.h>
#include <qstatusbar.h>
#include <qpixmap.h>
#include <qpainter.h>
#include <qscrollview.h>
#include <qpopupmenu.h>

#include <iostream>
#include <fstream>
#include <string>

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_COMPOSITE_H
#       include <BALL/CONCEPT/composite.h>
#endif

#ifndef BALL_VIEW_KERNEL_MESSAGE_H
#       include <BALL/VIEW/KERNEL/message.h>
#endif

#ifndef BALL_VIEW_GUI_WIDGETS_MODULARWIDGET_H
#	include <BALL/VIEW/GUI/WIDGETS/modularWidget.h>
#endif

#ifndef BALL_VIEW_GUI_KERNEL_MAINCONTROL_H
#       include <BALL/VIEW/GUI/KERNEL/mainControl.h>
#endif

#ifndef BALL_FORMAT_BRUKER2DFILE_H
#       include <BALL/FORMAT/bruker2DFile.h>
#endif

#ifndef BALL_DATATYPE_REGULARDATA2D_H
#       include <BALL/DATATYPE/regularData2D.h>
#endif

#ifndef BALL_FORMAT_JCAMPFILE_H
#       include <BALL/FORMAT/JCAMPFile.h>
#endif

#ifndef BALL_DATATYPE_CONTOUR_H
#       include <BALL/DATATYPE/contour.h>
#endif

#include <BALL/MOLVIEW/GUI/DIALOGS/DlgMoveOverlay.h>

using namespace BALL;
using namespace BALL::VIEW;

/** New RegularData2D Message Class
 */
class NewRegularData2DMessage: public CompositeMessage
{
 public:
  /**     @name Constructors and Destructors
   */
  //@{
  NewRegularData2DMessage();

  NewRegularData2DMessage(const CompositeMessage& message, bool deep = true);

  virtual ~NewRegularData2DMessage()
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

class RegularData2DWidget 
      : public QScrollView,
        public ModularWidget
{
  Q_OBJECT
 public:
  RegularData2DWidget(int lx_, int ly_, double min=0, double max=1., QWidget *parent = 0);
  RegularData2DWidget(const RegularData2DWidget& widget);

  ~RegularData2DWidget()
		throw();

  /**
   * Create a legend of width w, height h.
   */
  void createLegend( int w, int h );

  /** Scales the data in the rectangle (x1, y1, x2, y2) to the dimensions nx, ny
   */
  void scale(Size nx, Size ny, double x1, double y1, double x2, double y2);

  /**
   * Creates a lorentzian "peak" in the (interpolated) data with width xwidth, ywidth, amplitude amp 
   * and maximum at xpos, ypos.
   */
  void addLorentzian( double xpos, double ypos, double amp, int xwidth=1, int ywidth=1 );

  /**    Decides whether the coordinates (x, y) (in regular-data coordinates) are visible and if so,
	 returns the Position in the Pixmap
  */
  bool isVisibleAs(double x, double y, pair<Position, Position>& res);


  void onNotify(Message *message)
	throw();

  /**	@name	ModularWidget related methods
   */
  //@{
  /**	Setup the menu entries.
	RegularData2DWidget creates an entry in Tools|RegularData2DWidget and connects
	the entry to createPlot()
  */
  virtual void initializeWidget(MainControl& main_control)
	throw();

  /**	Remove menu entries.
   */
  virtual void finalizeWidget(MainControl& main_control)
	throw();
  //@}

  bool reactToMessages_(Message* message);

public slots:

  void paintEvent( QPaintEvent * );
  void drawContents( QPainter *paint, int clipx, int clipy, int clipw, int cliph );
  void resizeEvent( QResizeEvent * );
  void NewMousePos( Position x, Position y );
  void Selected(QPoint beg, QPoint end);
  void slotZoomOut();
 
  void enterEvent( QEvent * );
  void leaveEvent( QEvent * );

  void mousePressEvent( QMouseEvent *e );
  

  void createPlot();

  /**   Creates a new contour plot erasing all former plots.
   */
  void createContour();

  /**   Display the data as contour-plot.
   */
  void plotContour();

  /**   Create a contour plot as an overlay over the pixmap we are using right now.
   */
  void plotOverlay();

  void slotOverlayMove(int i);

  void plot();

  void viewportMousePressEvent(QMouseEvent *e);
  void viewportMouseMoveEvent(QMouseEvent *e);
  void viewportMouseReleaseEvent(QMouseEvent *e);
  void mouseReleaseEvent(QMouseEvent *e);

 protected:
  void eraseSelectFrame();
  void drawSelectFrame();

  QPixmap *pm_, *legend_map_;

  Position legend_last_x_, legend_last_y_;

  /* Length of original data. */
  Size lengthx_, lengthy_;

  /* Length of field (after interpolation). */
  Size full_length_x_, full_length_y_;

  /* Range. */
  double min_, max_;

  /* Flag that decides whether to plot mousepos or not. */
  bool showMousePos_;

  /* Shows mouse position.*/
  QLabel *posLabel_;

  /* Stores the spectrum */
  RegularData2D *spec_;

  /* Context-menu */
  QPopupMenu *men_;

  /* If we zoom into the data, we have to store the position of the lower left corner of the
     area we are looking at at the moment and we have to know a zoom - factor.
  */
  double xvis_low_, xvis_high_, yvis_low_, yvis_high_;

  Position act_lower_left_x_;

  Position act_lower_left_y_;

  double zoom_x_;

  double zoom_y_;

  /* This class creates and stores a number of contour-lines */
  Contour *cont_;

  /* The number and range of the contour-lines we want to plot. */
  Size cont_num_;

  double cont_start_, cont_end_;

  DlgMoveOverlay* mvover_;

  Position ind_side_, ind_updown_;

  /* These variables store the parameters of the spectrum */
  Size spec_length_x_, spec_length_y_;

  /* This pixmap is intended to hold the contour-lines */
  QPixmap *pm_cont_;

  /* decide if we plot contour-lines */
  bool plot_cont_;

  /* decide if we plot the data itself */
  bool plot_data_;

  /* If a selection is in progress, the coordinate of the first endpoint of the
     selected rectangle is stored in here.
  */
  QPoint last_selection_;

  /* If a selection is in progress, the coordinate of the actual endpoint of the 
     selected rectangle is stored in here.
  */
  QPoint selection_;

  bool select_;

  QStatusBar *stat_;

  QWidget *legend_wid_;

  bool isOverlay_;
};

#endif
