#ifndef NMR_H
#define NMR_H

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

// Im Moment verwende ich die Byteorder - Makros aus der glib

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_OPENGL_WIDGETS_MODULARWIDGET_H
#	include <BALL/VIEW/OPENGL/WIDGETS/modularWidget.h>
#endif

#include <BALL/FORMAT/bruker2DFile.h>
#include <BALL/FORMAT/JCAMPFile.h>

using namespace BALL;


/**
 * Struktur, die die Daten einer JCAMP-DX - Datei enthaelt.
 */
typedef struct {
  string title;
  int XDIM;
  int SI;
  int YMIN_P;
  int YMAX_P;
} jcamp_dx_data;

/**
 * Muss implementiert werden, um Mausposition an nmr weiterzugeben.
 */
class PixWid : public QWidget
{
  Q_OBJECT
    
  public:
    PixWid( QWidget *parent );
  public slots:
    void mouseMoveEvent( QMouseEvent *e );
  signals:
    void mouseMoved( int, int );
};

class nmr : public QScrollView,
	    public ModularWidget
{
  Q_OBJECT
 public:
  nmr(vector<double> &d_, int lx_, int ly_, double min=0, double max=1., QWidget *parent = 0);
  nmr(const nmr& widget);

  ~nmr();

  /**
   * Erstellt eine Bildlegende der Breite w und Höhe h.
   */
  void createLegend( int w, int h );

  /**
   * Erzeugt eine Pixmap der Groesse xlength, ylength. Die Daten werden dazu passend skaliert.
   */
  void scaleData( int xlength, int ylength );

  /**
   * Erzeugt in den (interpolierten) Daten einen lorentzfunktionsartigen "Peak" mit Breiten xwidth, ywidth, Amplitude amp und
   * Maximum in xpos, ypos.
   */
  void addLorentzian( double xpos, double ypos, double amp, int xwidth=1, int ywidth=1 );

  /**
   * Versucht, ein Spektrum aus dem angegebenen Verzeichnis zu laden. Hoffentlich ist die Verzeichnisstruktur allgemeingueltig...
   */
  bool readSpec( string dirName );

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
	nmr creates an entry in Tools|nmr and connects
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
  int legendLastX, legendLastY;
    /* Stores the spectrum's data. */
  vector<double> specDat_;
  /* Hier sind die Daten inklusive evtl. vorhandenen Interpolationen gespeichert. */
  vector<double> fullData;
  /* Länge des NMR-Datenfeldes in x und y-Richtung. */
  int lengthx, lengthy;
  /* Länge des Feldes inklusive Interpolation. */
  int fullLengthx, fullLengthy;
  /* Wertebereich */
  double min_, max_, minx_, maxx_, miny_, maxy_;
  /* Flag, das darueber entscheidet, ob die Mausposition angezeigt werden soll. */
  bool showMousePos_;
  /* Für die Anzeige der Mausposition */
  QLabel *posLabel_;
  /* Parameterdaten für beide Achsen. */
  jcamp_dx_data f1Dat, f2Dat;
  /* Reads and stores the spectrum. */
  Bruker2D *spectrum_;
};
  

#endif
