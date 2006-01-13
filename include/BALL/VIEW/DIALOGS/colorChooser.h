// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: colorChooser.h,v 1.3.2.1 2006/01/13 15:35:23 amoll Exp $
//

#ifndef BALL_VIEW_DIALOGS_COLORCHOOSER_H 
#define BALL_VIEW_DIALOGS_COLORCHOOSER_H 

#ifndef BALL_VIEW_DATATYPE_COLORRGBA_H
  #include <BALL/VIEW/DATATYPE/colorRGBA.h>
#endif

#include <qwidget.h>
 #include <qwidgetplugin.h>
//Added by qt3to4:
#include <QLabel>

class QLabel;
class QSlider;
class QPushButton;

using namespace BALL;
using namespace BALL::VIEW;

class QT_WIDGET_PLUGIN_EXPORT ColorChooser : public QWidget
{
		Q_OBJECT

public:
		ColorChooser( QWidget *parent = 0, const char *name = 0);
		ColorRGBA getColor() const;
		void setColor(const ColorRGBA& color);

public slots:
		void chooseColor();

signals:
		void colorChanged();

public:
		QLabel* 			label;
		QLabel* 			color_label;
		QPushButton* 	button;
};

#endif
