// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: transformationDialog.C
//

#include <BALL/VIEW/DIALOGS/transformationDialog.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <qlineedit.h>

namespace BALL
{
	namespace VIEW
	{

TransformationDialog::TransformationDialog(QWidget* parent,  const char* name, bool modal, WFlags fl) 
	: TransformationDialogData(parent, name, modal, fl), 
		ModularWidget(name),
		composite_(0)
{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "new TransformationDialog " << this << std::endl;
#endif
	ModularWidget::registerWidget(this);
}

TransformationDialog::~TransformationDialog() throw()
{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "deleting TransformationDialog " << this << std::endl;
#endif
}


bool TransformationDialog::translate(float x, float y, float z)
{
  if (!composite_) return false;

  matrix_.setTranslation(x,y,z);	
  my_processor_.setTransformation(matrix_);
  return (composite_->apply(my_processor_));
}

bool TransformationDialog::rotateX(float angle, bool radian)
{
  if (!composite_) return false;
  
	composite_->apply(center_processor_);
	composite_center_ = center_processor_.getCenter();
	
	matrix_.setTranslation(composite_center_ * -1);
	my_processor_.setTransformation(matrix_);
	composite_->apply(my_processor_);
	
	angle_.set(angle, radian);
	matrix_.setRotationX(angle_);
	my_processor_.setTransformation(matrix_);
	composite_->apply(my_processor_);
	
	matrix_.setTranslation(composite_center_);
	my_processor_.setTransformation(matrix_);

	return (composite_->apply(my_processor_));
}

bool TransformationDialog::rotateY(float angle, bool radian)
{
  if (!composite_) return false;

	composite_->apply(center_processor_);
	composite_center_ = center_processor_.getCenter();
	
	matrix_.setTranslation(composite_center_ * -1);
	my_processor_.setTransformation(matrix_);
	composite_->apply(my_processor_);
	
	angle_.set(angle, radian);
	matrix_.setRotationY(angle_);
	my_processor_.setTransformation(matrix_);
	composite_->apply(my_processor_);
	
	matrix_.setTranslation(composite_center_);
	my_processor_.setTransformation(matrix_);

	return (composite_->apply(my_processor_));
}

bool TransformationDialog::rotateZ(float angle, bool radian)
{
  if (!composite_) return false;

	composite_->apply(center_processor_);
	composite_center_ = center_processor_.getCenter();
	
	matrix_.setTranslation(composite_center_ * -1);
	my_processor_.setTransformation(matrix_);
	composite_->apply(my_processor_);
	
	angle_.set(angle, radian);
	matrix_.setRotationZ(angle_);
	my_processor_.setTransformation(matrix_);
	composite_->apply(my_processor_);
	
	matrix_.setTranslation(composite_center_);
	my_processor_.setTransformation(matrix_);

	return (composite_->apply(my_processor_));
}


void TransformationDialog::zNegRotationClicked()
{
	if (rotateZ(-(getRotation()),false)) update_();
}

void TransformationDialog::xNegRotationClicked()
{
	if (rotateX(-(getRotation()),false)) update_();
}

void TransformationDialog::xNegTranslationClicked()
{
	if (translate(-(getTranslation()),0,0)) update_();
}

void TransformationDialog::xRotationClicked()
{
	if (rotateX(getRotation(),false)) update_();
}

void TransformationDialog::xTranslationClicked()
{
	if (translate(getTranslation(),0,0)) update_();
}

void TransformationDialog::yNegRotationClicked()
{
	if (rotateY(-(getRotation()),false)) update_();
}

void TransformationDialog::yNegTranslationClicked()
{
	if (translate(0,-(getTranslation()),0)) update_();
}

void TransformationDialog::yRotationClicked()
{
	if (rotateY(getRotation(),false)) update_();
}

void TransformationDialog::yTranslationClicked()
{
	if (translate(0,getTranslation(),0)) update_();
}

void TransformationDialog::zNegTranslationClicked()
{
	if (translate(0,0,-(getTranslation()))) update_();
}

void TransformationDialog::zRotationClicked()
{
	if (rotateZ(getRotation(),false)) update_();
}

void TransformationDialog::zTranslationClicked()
{
	if (translate(0,0,getTranslation())) update_();
}


float TransformationDialog::getTranslation() const
{
	try
	{
		float translation_value = 
			String(defaultTranslationValue->text().ascii()).toFloat();
		
		if ((translation_value > 0.0) &&
				(translation_value < 1000.0))  
		{
			return translation_value;
		}
	}
	catch(...)
	{					
	}

	Log.error() << "Invalid Translation Value -> set default value" << std::endl;
	defaultTranslationValue->setText("1.0");
	return 1.0;
}
	

float TransformationDialog::getRotation() const
{	
	try
	{
		float rotation_value = 
			String(defaultRotationValue->text().ascii()).toFloat();
			
		if ((rotation_value <= 360.0) &&
				(rotation_value > 0.0))
		{
			return rotation_value;
		}
	}
	catch(...)
	{
	}
	Log.error() << "Invalid Rotation Value -> set default value" << std::endl;
	defaultRotationValue->setText("10");
	return 10.0;
}

void TransformationDialog::update_()
{
	CompositeMessage* message = new CompositeMessage(*composite_, 
													CompositeMessage::CHANGED_COMPOSITE);
	notify_(message);
}

void TransformationDialog::onNotify(Message *message)
	throw()
{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "TransformationDialog " << this << " onNotify " << message << std::endl;
#endif
	if (RTTI::isKindOf<ControlSelectionMessage>(*message))
  {
		if (getMainControl()->getMolecularControlSelection().size() != 0)
		{
			composite_ = *getMainControl()->getMolecularControlSelection().begin();
		}
		else
		{
			composite_ = 0;
		}
	}
}

void TransformationDialog::setComposite(Composite* composite) 
{
	composite_ = composite;
}
  
} } // namespaces
