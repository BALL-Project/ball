// $Id: NMRSpectrum1DWidget.h,v 1.1 2000/11/15 18:19:39 anhi Exp $

#ifndef BALL_VIEW_GUI_WIDGET_NMRSPECTRUM1DWIDGET_H
#define BALL_VIEW_GUI_WIDGET_NMRSPECTRUM1DWIDGET_H

#include <qscrollview.h>
#include <qpixmap.h>

#ifndef BALL_COMMON_H
#       include <BALL/common.h>
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

#ifndef BALL_FORMAT_BRUKER1DFILE_H
#       include <BALL/FORMAT/bruker1DFile.h>
#endif

#ifndef BALL_FORMAT_JCAMPFILE_H
#       include <BALL/FORMAT/JCAMPFile.h>
#endif


// This is needed because of PixWid...
#ifndef BALL_VIEW_GUI_WIDGETS_REGULARDATA2DWIDGET_H
#       include <BALL/VIEW/GUI/WIDGETS/regularData2DWidget.h>
#endif


using namespace BALL;

/**      Widget used to display 1-dimensional NMR-spectra.
 */
class NMRSpectrum1DWidget
  : public QScrollView,
    public ModularWidget
{
  Q_OBJECT
 public:
  // macro for create method
  BALL_CREATE(NMRSpectrum1DWidget);

  /** Constructors and Destructors
   */
  //@{
      /// Default constructor
      NMRSpectrum1DWidget()
	throw();

      /// Copy constructor
      NMRSpectrum1DWidget(const NMRSpectrum1DWidget& widget)
	throw();

      /// Detailed constructor
      NMRSpectrum1DWidget(vector<double> &d, Size l = 0, Size min = 0, Size max = 0, QWidget *parent = 0)
	throw();

      /// Destructor
      virtual ~NMRSpectrum1DWidget()
	throw();
  //@}

  /** Assignment
   */
  //@{
      /// Assignment operator
      const NMRSpectrum1DWidget& operator = (const NMRSpectrum1DWidget &widget)
	throw();

      /// Clear method
      virtual void clear()
	throw();
  //@}

  /** Predicates
   */
  //@{

      /// Equality operator. Two spectra are equal by definition if they point to the same data and have the same lenght, min and max.
      bool operator == (const NMRSpectrum1DWidget &widget) const
	throw();

  //@}

  /**    @name Accessors
   */
  //@{
  /**    Return the length of the spectrum
   */
  const Size getLength() const
    throw();

  /**    Return the minimum value
   */
  const Size getMin() const
    throw();

  /**    Return the maximum value
   */
  const Size getMax() const
    throw();

  /**    Return the spectrum itself
   */
  const vector<double>& getData() const
    throw();
  //@}

  /** Tries to load a spectrum from bruker-directory "dirName". Hopefully the directory's structure is like this in general.
   */
  bool NMRSpectrum1DWidget::readSpec( String dirName );

 public slots:
  /**    @name    ModularWidget related methods
   */
  //@{
  /**    Setup the menu entries.
	 NMRSpectrum1DWidget creates an entry in Tools|NMRSpectrum1DWidget and connects
	 the entry to createPlot()
  */
  virtual void initializeWidget(MainControl& main_control);

  /**    Remove menu entries.
   */
  virtual void finalizeWidget(MainControl& main_control);
  //@}

  void createPlot();

  void drawContents(QPainter *paint, int clipx, int clipy, int clipw, int cliph);
  void paintEvent(QPaintEvent *e);
  void resizeEvent(QResizeEvent *e);
    
 protected:
      QPixmap pm_;
      Size length_;
      vector<double> data_;
      Size min_;
      Size max_;
      Bruker1D *spectrum_;
      PixWid *pixWid_;
};

#endif // BALL_VIEW_GUI_WIDGET_NMRSPECTRUM1DWIDGET_H
