// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: colorChooser.h,v 1.1 2004/09/28 15:26:36 amoll Exp $
//

#ifndef BALL_VIEW_DIALOGS_COLORCHOOSER_H 
#define BALL_VIEW_DIALOGS_COLORCHOOSER_H 

#ifndef BALL_VIEW_DATATYPE_COLORRGBA_H
//  #include <BALL/VIEW/DATATYPE/colorRGBA.h>
#endif

#include <qwidget.h>
#include <qwidgetplugin.h>

class QLabel;
class QSlider;
class QPushButton;

/*
namespace BALL 
{
	namespace VIEW 
	{
	*/
		class QT_WIDGET_PLUGIN_EXPORT ColorChooser : public QWidget
		{
				Q_OBJECT

		public:
				ColorChooser( QWidget *parent = 0, const char *name = 0);
// 				ColorRGBA getColor() const;
// 				void setColor(const ColorRGBA& color);
				void setFactor(float);
				float getFactor() const;

		public slots:
				void chooseColor();

		signals:
				void colorChanged();

		public:
				QLabel* 			label;
				QLabel* 			color_label;
				QPushButton* 	button;

				float 				factor_;

		};

/*
	}
}
*/
#endif
