#include <BALL/VIEW/GUI/DIALOGS/transformationDialog.h>
#include <BALL/VIEW/GUI/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/GUI/WIDGETS/scene.h>
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
	ModularWidget::registerWidget(this);
}

TransformationDialog::~TransformationDialog() throw()
{
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
	if (rotateZ(-(getMaxRotation()),false)) update_();
}

void TransformationDialog::xNegRotationClicked()
{
	if (rotateX(-(getMaxRotation()),false)) update_();
}

void TransformationDialog::xNegTranslationClicked()
{
	if (translate(-(getMaxTrans()),0,0)) update_();
}

void TransformationDialog::xRotationClicked()
{
	if (rotateX(getMaxRotation(),false)) update_();
}

void TransformationDialog::xTranslationClicked()
{
	if (translate(getMaxTrans(),0,0)) update_();
}

void TransformationDialog::yNegRotationClicked()
{
	if (rotateY(-(getMaxRotation()),false)) update_();
}

void TransformationDialog::yNegTranslationClicked()
{
	if (translate(0,-(getMaxTrans()),0)) update_();
}

void TransformationDialog::yRotationClicked()
{
	if (rotateY(getMaxRotation(),false)) update_();
}

void TransformationDialog::yTranslationClicked()
{
	if (translate(0,getMaxTrans(),0)) update_();
}

void TransformationDialog::zNegTranslationClicked()
{
	if (translate(0,0,-(getMaxTrans()))) update_();
}

void TransformationDialog::zRotationClicked()
{
	if (rotateZ(getMaxRotation(),false)) update_();
}

void TransformationDialog::zTranslationClicked()
{
	if (translate(0,0,getMaxTrans())) update_();
}


float TransformationDialog::getMaxTrans() const
{
	float max_translation_value_ = 
		String(defaultTranslationValue->text().ascii()).toFloat();
	
	if ((max_translation_value_ > 0.0) &&
			(max_translation_value_ < 1000.0))  
	{
		return max_translation_value_;
	}
	else
	{					
		Log.error() << "Invalid Translation Value -> set default value" << std::endl;
		return 1.0;
	}
}
	

float TransformationDialog::getMaxRotation() const
{	
	float max_rotation_value_ = 
		String(defaultRotationValue->text().ascii()).toFloat();
		
	if ((max_rotation_value_ <= 360.0) &&
			(max_rotation_value_ > 0.0))
	{
		return max_rotation_value_;
	}
	else
	{
		Log.error() << "Invalid Rotation Value -> set default value" << std::endl;
		return 10.0;
	}
}

void TransformationDialog::update_()
{
	
	Scene* scene= (Scene*) Scene::getInstance(0);
	scene->update(true);
	return;
	
	SceneMessage* new_message = new SceneMessage;
	new_message->setType(SceneMessage::REBUILD_DISPLAY_LISTS);
	notify_(new_message);
}

void TransformationDialog::onNotify(Message *message)
	throw()
{
	if (RTTI::isKindOf<ControlSelectionMessage>(*message))
  {
		if (getMainControl()->getControlSelection().size() > 0)
		{
			composite_ = *getMainControl()->getControlSelection().begin();
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
