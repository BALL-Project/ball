// $Id: displayProperties.h,v 1.1 2000/01/09 17:49:31 hekl Exp $

#ifndef BALL_APPLICATIONS_MOLVIEW_DISPLAYPROPERTIES_H
#define BALL_APPLICATIONS_MOLVIEW_DISPLAYPROPERTIES_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#include <qmetaobject.h>
#include <qwidget.h>
#include <qradiobutton.h>
#include <qbuttongroup.h>
#include <qpushbutton.h>
#include <qgroupbox.h>
#include <qvgroupbox.h>
#include <qhgroupbox.h>

#ifndef BALL_COMMON_LOGSTREAM_H
#	include <BALL/COMMON/logStream.h>
#endif

#ifndef BALL_MOLVIEW_OPENGL_FUNCTOR_MOLECULEOBJECTPROCESSOR_H
# include <BALL/MOLVIEW/OPENGL/FUNCTOR/moleculeObjectProcessor.h>
#endif


using namespace BALL;
using namespace BALL::VIEW;
using namespace BALL::MOLVIEW;


class DisplayProperties: public QWidget
{
	Q_OBJECT

  public:

	  DisplayProperties
			(QWidget *parent__pQWidget = 0,
			 const char *name__pc = 0);

  	~DisplayProperties
			(void);

		void setObjectProcessor
			(const MoleculeObjectProcessor &__rMoleculeObjectProcessor);

  public slots:

		void modelSelected(int model);
		void modelPrecisionSelected(int model);
		void modelDrawingModeSelected(int model);

  signals:

	  void apply();
    
  protected:
	  
  private:
	
    MoleculeObjectProcessor *__mpMoleculeObjectProcessor_;

	  QPushButton apply__mQPushButton_;
		QGroupBox main__mQGroupBox_;
		QButtonGroup model_radio__mQButtonGroup_;

		List<QRadioButton *> radio_buttons__mList_;
};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include "displayProperties.iC"
#		endif

#endif // BALL_APPLICATIONS_MOLVIEW_DISPLAYPROPERTIES_H
