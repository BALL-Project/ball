// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include "../../BALL/include/BALL/VIEW/DIALOGS/colorChooser.h"

#include <QtGui/qcolordialog.h>
#include <QtGui/qslider.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlayout.h>
#include <QtGui/qpushbutton.h>
//Added by qt3to4:
#include <Q3HBoxLayout>

#define BALLVIEW_BUTTON_WIDTH 70
#define BALLVIEW_BUTTON_HEIGHT 30

ColorChooser::ColorChooser(QWidget *parent, const char *name)
	: QWidget(parent, name)
{
	Q3HBoxLayout *layout = new Q3HBoxLayout(this);
	layout->setMargin(0);

	label = new QLabel(this, "Name");
	label->setText("name");
	label->setGeometry(5,0, 100, BALLVIEW_BUTTON_HEIGHT);
	label->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
	layout->addWidget(label);

	color_label = new QLabel(this, "Color");
	color_label->setFixedWidth(60);
	color_label->setFixedHeight(BALLVIEW_BUTTON_HEIGHT);
	color_label->setFrameShape(QLabel::WinPanel);
	layout->addWidget(color_label);

	layout->addSpacing(5);

	button = new QPushButton("Edit", this, "edit_button");
	button->setFixedWidth(BALLVIEW_BUTTON_WIDTH);
	button->setFixedHeight(BALLVIEW_BUTTON_HEIGHT);
	layout->addWidget(button);

	connect(button, SIGNAL(clicked()), this, SLOT(chooseColor()));
	setFocusProxy(button);
}

void ColorChooser::chooseColor()
{
	QColor qcolor = QColorDialog::getColor(label->backgroundColor());
	if (!qcolor.isValid()) return;

	color_label->setBackgroundColor(qcolor);
}

ColorRGBA ColorChooser::getColor() const
{
	ColorRGBA color;
	color.set(label->backgroundColor());
	return color;
}

void ColorChooser::setColor(const ColorRGBA& color)
{
	QColor qcolor;
	color.get(qcolor);
	label->setBackgroundColor(qcolor);
}

