// $Id: regularData2DWidget.h,v 1.1 2000/11/10 17:21:42 anhi Exp $

#ifndef BALL_VIEW_GUI_WIDGET_REGULARDATA2DWIDGET_H
#define BALL_VIEW_GUI_WIDGET_REGULARDATA2DWIDGET_H

#include <qlabel.h>
#include <qwidget.h>
#include <qstatusbar.h>
#include <qpixmap.h>
#include <qpainter.h>
#include <qscrollview.h>

#include <iostream>
#include <fstream>
#include <string>
#include <glib.h>

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
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

using namespace BALL;

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
  signals:
    void mouseMoved( int, int );
};

class RegularData2DWidget 
      : public QScrollView,
        public ModularWidget
{
  Q_OBJECT
 public:
  RegularData2DWidget(vector<double> &d_, int lx_, int ly_, double min=0, double max=1., QWidget *parent = 0);
  RegularData2DWidget(const RegularData2DWidget& widget);

  ~RegularData2DWidget();

  /**
   * Create a legend of width w, height h.
   */
  void createLegend( int w, int h );

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
  void paintEvent( QPaintEvent * );
  void drawContents( QPainter *paint, int clipx, int clipy, int clipw, int cliph );
  void resizeEvent( QResizeEvent * );
  void NewMousePos( int x, int y );
 
  void enterEvent( QEvent * );
  void leaveEvent( QEvent * );
  
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
};
  
#endif
