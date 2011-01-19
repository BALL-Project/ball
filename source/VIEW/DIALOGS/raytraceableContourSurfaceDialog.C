// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/DIALOGS/raytraceableContourSurfaceDialog.h>

#include <QtGui/QPushButton>
#include <QtGui/QLineEdit>
//#include <QtGui/qpushbutton.h>
#include <QtGui/QSlider>
#include <iostream>

namespace BALL
{
	namespace VIEW
	{
	
RaytraceableContourSurfaceDialog::RaytraceableContourSurfaceDialog(RaytraceableGrid* grid, QWidget* parent, const char* name, bool, Qt::WFlags fl )
	: QDialog(parent, fl),
		Ui_RaytraceableContourSurfaceDialogData(),
		ModularWidget(name),
		grid_(grid)
{
	std::cout << "Constructor RT_CS_Dialog " << std::endl;
	
	setupUi(this);
	//setINIFileSectionName("Raytracing");
	setObjectName(name);
	registerWidget(this);
	setObjectName(name);

	// signals and slots connections
	
	// Contour Threshold Range
	connect( OK_button, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect( cancel_button , SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect( add_button, SIGNAL( clicked() ), this, SLOT( add() ) );
	connect( ContourSurfaceThresholdSlider, SIGNAL( valueChanged(int) ), this, SLOT( changedSliderThreshold(int) ) );
	connect( ContourSurfaceThresholdLineEdit, SIGNAL( editingFinished() ), this, SLOT( changedEditThreshold() ) );
	
	// Contour Threshold Range
	connect( autoRangeButton, SIGNAL( clicked() ), this, SLOT ( setDefaultRangeValues_() ) );
	connect( RangeLineEdit_min, SIGNAL( editingFinished() ), this, SLOT(changedRangeMin()) );
	connect( RangeLineEdit_max, SIGNAL( editingFinished() ), this, SLOT(changedRangeMax()) );
	connect( RangeLineEdit_steps, SIGNAL( editingFinished() ), this, SLOT(changedRangeSteps()) );


	OK_button->setEnabled(true);
	add_button->setEnabled(true);
	cancel_button->setEnabled(true);
	autoRangeButton->setEnabled(true);

	// initialize slider and editLine
	if (grid_ != 0)
	{	
		setDefaultRangeValues_();
	}
	else
	{
		Log.error() << (String)tr("The grid is not valid") << std::endl;
	}
}

		
RaytraceableContourSurfaceDialog::~RaytraceableContourSurfaceDialog()
{
	// TODO
}

void RaytraceableContourSurfaceDialog::setDefaultRangeValues_()
{
	//Log.info() << " setDefaultValues" << std::endl;
	min_ = grid_->getGridMinValue();
	max_ = grid_->getGridMaxValue();
	current_threshold_ = (min_+ max_)/2.;
	steps_ = 50; 

	QString str = QString("%1").arg((double)current_threshold_, 0,'g',3);
	ContourSurfaceThresholdLineEdit->setText(str);
	
	ContourSurfaceThresholdSlider->setRange(0,steps_);
	ContourSurfaceThresholdSlider->setSliderPosition(steps_/2);
	str = QString("%1").arg((double)max_, 0, 'g', 3);
	RangeLineEdit_max->setText(str);
	str = QString("%1").arg((double)min_, 0, 'g', 3);
	RangeLineEdit_min->setText(str);
	str = QString("%1").arg((double)steps_, 0, 'g', 3);
	RangeLineEdit_steps->setText(str);
	
	str = QString("%1").arg((double)current_threshold_, 0, 'g', 3);
	ContourSurfaceThresholdLineEdit->setText(str);

}

void RaytraceableContourSurfaceDialog::add()
{
	//std::cout << " model reated- close()" << std::endl;
	//double new_threshold = 3.;
	//addModel_(new_threshold);
	addModel_(current_threshold_);
}

void RaytraceableContourSurfaceDialog::accept()
{
	//std::cout << "new model created - waiting()" << std::endl;
	// get the new threshold
	//double new_threshold = 3.;
	//addModel_(new_threshold);
	addModel_(current_threshold_);
	QDialog::accept();
}

void RaytraceableContourSurfaceDialog::reject()
{	
//	std::cout << "no model reated- close()" << std::endl;
	QDialog::reject();
}

void RaytraceableContourSurfaceDialog::changedSliderThreshold(int sliderpos)
{
	//std::cout << "call slider changed! " << std::endl;
	current_threshold_ =  min_ + (float)((max_ - min_)/steps_) * (float)sliderpos;
	QString str = QString("%1").arg((double)current_threshold_, 0, 'g', 3);
	ContourSurfaceThresholdLineEdit->setText(str);
}


void RaytraceableContourSurfaceDialog::changedEditThreshold()
{
//	std::cout << "call edit th changed !" << std::endl;
	float new_threshold = getValue_(ContourSurfaceThresholdLineEdit);
	if (new_threshold > max_) 	
	{
		// ToDo: are there other restrictions??
		// ToDo: appropriate error/info message
		current_threshold_ = max_;
		QString str = QString("%1").arg((double)current_threshold_, 0,'g',3);
		ContourSurfaceThresholdLineEdit->setText(str);
	}
	else if(new_threshold < min_ )
	{
		current_threshold_ = min_;
		QString str = QString("%1").arg((double)current_threshold_, 0,'g',3);
		ContourSurfaceThresholdLineEdit->setText(str);
	}
	else
	{
		current_threshold_ = new_threshold;
	}
		
	// update the slider
	ContourSurfaceThresholdSlider->setSliderPosition((int) ((current_threshold_-min_)/(max_ - min_) * steps_));

	
}

void RaytraceableContourSurfaceDialog::addModel_(float /*threshold*/)
{
	// TODO: create a new model and add it to the representationlist
}

void  RaytraceableContourSurfaceDialog::changedRangeMin()
{
	//std::cout << "call min changed"  << std::endl;

	min_= getValue_(RangeLineEdit_min);
	if (current_threshold_ < min_)
	{
		current_threshold_ = min_;
		// update the value LineEdit
		QString str = QString("%1").arg((double)current_threshold_, 0, 'g', 3);
		ContourSurfaceThresholdLineEdit->setText(str);
	}
	// update the values slider
	ContourSurfaceThresholdSlider->setSliderPosition((int) ((current_threshold_-min_)/(max_ - min_) * steps_));

}

void  RaytraceableContourSurfaceDialog::changedRangeMax()
{ 
	//std::cout << "call max changed"  << std::endl;
	max_= getValue_(RangeLineEdit_max);
	if (current_threshold_ > max_)
	{
		current_threshold_ = max_;
		// update the value LineEdit
		QString str = QString("%1").arg((double)current_threshold_, 0, 'g', 3);
		ContourSurfaceThresholdLineEdit->setText(str);
	}

	// update the value slider
	ContourSurfaceThresholdSlider->setSliderPosition((int) ((current_threshold_-min_)/(max_ - min_) * steps_));

}

void  RaytraceableContourSurfaceDialog::changedRangeSteps()
{
	//std::cout << "call steps changed"  << std::endl;
	int new_steps = (int)getValue_(RangeLineEdit_steps);
	if (new_steps < 1)
	{
		steps_ = 1; //(int)(max_ - min_);
		QString str = QString("%1").arg((double)steps_, 0, 'g', 3);
		RangeLineEdit_steps->setText(str);
		// TO DO: appropriate error/info message??
	}
	else
	{
		steps_= new_steps;
	}
	// update the range and value slider
	ContourSurfaceThresholdSlider->setRange(0,(int)steps_);
	ContourSurfaceThresholdSlider->setSliderPosition((int) ((current_threshold_-min_)/(max_ - min_) * steps_));

}

float RaytraceableContourSurfaceDialog::getValue_(const QAbstractSlider* slider) const
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
