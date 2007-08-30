// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/DIALOGS/raytraceableContourSurfaceDialog.h>

#include <QtGui/QPushButton>
#include <QtGui/qlineedit.h>
//#include <QtGui/qpushbutton.h>
#include <QtGui/QSlider>
#include <iostream>

namespace BALL
{
	namespace VIEW
	{
	
RaytraceableContourSurfaceDialog::RaytraceableContourSurfaceDialog(QWidget* parent, const char* name, bool, Qt::WFlags fl )
	: QDialog(parent, fl),
		Ui_RaytraceableContourSurfaceDialogData(),
		ModularWidget(name)
{
	std::cout << "Constructor RT_CS_Dialog " << std::endl;
	
	setupUi(this);
	//setINIFileSectionName("Raytracing");
	setObjectName(name);
	registerWidget(this);
	setObjectName(name);

	// signals and slots connections
	connect( OK_button, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect( cancel_button , SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect( add_button, SIGNAL( clicked() ), this, SLOT( add() ) );
	//connect(ContourSurfaceThresholdSlider, SIGNAL( sliderMoved(int) ), this, SLOT( changedSliderThreshold() ) );
	//connect(ContourSurfaceThresholdLineEdit, SIGNAL( editingFinished() ), this, SLOT( changedEditThreshold() ) );

	OK_button->setEnabled(true);
	add_button->setEnabled(true);
	cancel_button->setEnabled(true);
	
	// initialize slider and editLine
	min_= 0;
	max_ = 50;
	current_threshold_ = 1.4;
	max_as_angstroem_ = 10;
	QString str;
	str.arg(current_threshold_, 0,'g',2);
	ContourSurfaceThresholdLineEdit->setText(str);
	ContourSurfaceThresholdSlider->setRange(min_,max_);
	ContourSurfaceThresholdSlider->setSliderPosition((int) ((max_-min_)/max_as_angstroem_ * current_threshold_));
	std::cout << "Finished RT_CS_Dialog " << std::endl;
}

		
RaytraceableContourSurfaceDialog::~RaytraceableContourSurfaceDialog()
	throw()
{
	// TODO
}

void RaytraceableContourSurfaceDialog::add()
{
	// TODO:
	double new_threshold = 3.;
	addModel_(new_threshold);
	std::cout << " model reated- close()" << std::endl;

}

void RaytraceableContourSurfaceDialog::accept()
{
	//TODO:
	// get the new threshold
	double new_threshold = 3.;
	addModel_(new_threshold);
	std::cout << "new model created - waiting()" << std::endl;
	QDialog::accept();
}

void RaytraceableContourSurfaceDialog::reject()
{	
	std::cout << "no model reated- close()" << std::endl;
	QDialog::reject();
}

void RaytraceableContourSurfaceDialog::changedSliderThreshold()
{
	current_threshold_ = getValue_(ContourSurfaceThresholdSlider);
	QString str;
	str.arg(current_threshold_);
	ContourSurfaceThresholdLineEdit->setText(str);

}


void RaytraceableContourSurfaceDialog::changedEditThreshold()
{
	//TODO: abgleichen von Slider und LineEdit
	//evtl range ändern 
}

void RaytraceableContourSurfaceDialog::addModel_(float threshold)
	throw()
{
	// TODO: create a new model and add it to the representationlist
}
	

float RaytraceableContourSurfaceDialog::getValue_(const QAbstractSlider* slider) const
	throw()
{
	return(slider->value());
}


float RaytraceableContourSurfaceDialog::getValue_(const QLineEdit* edit) const
	throw(Exception::InvalidFormat)
{
	return (edit->text()).toFloat();
}


	} // VIEW and BALL namespace:
}
