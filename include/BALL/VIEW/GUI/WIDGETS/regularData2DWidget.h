// $Id: regularData2DWidget.h,v 1.3 2000/11/28 17:42:18 anhi Exp $

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

#ifndef BALL_DATATYPE_CONTOURLINE_H
#       include <BALL/DATATYPE/contourLine.h>
#endif

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

  virtual ~NewRegularData2DMessage();
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

/**
 * Must be implemented to pass mouse position upwards.
 */
class PixWid 
      : public QWidget
{
  Q_OBJECT
    
  public:
    PixWid( QWidget *parent );
  public slots:
    void mouseMoveEvent( QMouseEvent *e );
    void mousePressEvent( QMouseEvent *e );
    void mouseReleaseEvent( QMouseEvent *e );
  signals:
    void mouseMoved( int, int );

  private:
    QPoint beg_old, beg, end_old, end;
};

class RegularData2DWidget 
      : public QScrollView,
        public ModularWidget
{
  Q_OBJECT
 public:
  RegularData2DWidget(int lx_, int ly_, double min=0, double max=1., QWidget *parent = 0);
  RegularData2DWidget(const RegularData2DWidget& widget);

  ~RegularData2DWidget();

  /**
   * Create a legend of width w, height h.
   */
  void createLegend( int w, int h );

  /** Scales the data in the rectangle (x1, y1, x2, y2) to the dimensions nx, ny
   */
  void scale(Size nx, Size ny, double x1, double y1, double x2, double y2);

  /**
   * Creates a pixmap containing nx * ny data points from startx to endx and starty to endy
   */
  void getData(Size nx, Size ny, double startx, double endx, double starty, double endy);

  /**
   * Creates a lorentzian "peak" in the (interpolated) data with width xwidth, ywidth, amplitude amp 
   * and maximum at xpos, ypos.
   */
  void addLorentzian( double xpos, double ypos, double amp, int xwidth=1, int ywidth=1 );

  /**
   * Tries to read a spectrum from directory "dirName".
   */
  bool readSpec( string dirName );

  Bruker2D& GetBrukerFile();

  void SetShiftRange(double offsetf1, double offsetf2, double swidthf1, double swidthf2, double bfreqf1, double bfreqf2);

 public slots:
  bool reactToMessages_(Message* message);

  void paintEvent( QPaintEvent * );
  void drawContents( QPainter *paint, int clipx, int clipy, int clipw, int cliph );
  void resizeEvent( QResizeEvent * );
  void NewMousePos( int x, int y );
 
  void enterEvent( QEvent * );
  void leaveEvent( QEvent * );

  void mousePressEvent( QMouseEvent *e );
  
  /**	@name	ModularWidget related methods
   */
  //@{
  /**	Setup the menu entries.
	RegularData2DWidget creates an entry in Tools|RegularData2DWidget and connects
	the entry to createPlot()
  */
  virtual void initializeWidget(MainControl& main_control);

  /**	Remove menu entries.
   */
  virtual void finalizeWidget(MainControl& main_control);
  //@}

  void createPlot();

 protected:
  QPixmap *pm, *legendMap, *bufferMap;
  PixWid *pixWid;
  Size legendLastX, legendLastY;
  /* Stores original data. */
  vector<double> specDat_;
  /* Stores data after interpolation. */
  vector<double> fullData;
  /* Length of original data. */
  Size lengthx, lengthy;
  /* Length of field (after interpolation). */
  Size fullLengthx, fullLengthy;
  /* Range. */
  double min_, max_, minx_, maxx_, miny_, maxy_;
  /* Flag that decides whether to plot mousepos or not. */
  bool showMousePos_;
  /* Shows mouse position.*/
  QLabel *posLabel_;
  /* Reads and stores the spectrum. */
  Bruker2D *spectrum_;
  double soffsetf1_, soffsetf2_;
  double swidthf1_, swidthf2_;
  double bfreqf1_, bfreqf2_;
  Size snumpoints_;
  /* Stores the spectrum */
  RegularData2D *spec_;
  /* Context-menu */
  QPopupMenu *men_;
};
  
#endif
