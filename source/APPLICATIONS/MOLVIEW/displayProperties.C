#include "displayProperties.h"

using namespace std;

DisplayProperties::DisplayProperties
  (QWidget *parent__pQWidget,
	 const char *name__pc)
		:
		QWidget(parent__pQWidget, name__pc),
		__mpMoleculeObjectProcessor_(0),
	  apply__mQPushButton_(this),
		main__mQGroupBox_(this),
		model_radio__mQButtonGroup_(&main__mQGroupBox_),
		radio_buttons__mList_()
{
	setCaption("Display Properties");

	resize(200, 200);

	// APPLY-Button --------

	apply__mQPushButton_.move(20, 170);
	apply__mQPushButton_.setText("apply");

	// main group box -----

	main__mQGroupBox_.resize(200, 155);

	// model radio button  group ---

	model_radio__mQButtonGroup_.move(10, 10);
	model_radio__mQButtonGroup_.resize(155, 135);
	model_radio__mQButtonGroup_.setTitle("Model");

	// model radio buttons -

	QRadioButton *none__pQRadioButton = new QRadioButton(&model_radio__mQButtonGroup_);
	CHECK_PTR(none__pQRadioButton);
	radio_buttons__mList_.push_back(none__pQRadioButton);
	model_radio__mQButtonGroup_.insert(none__pQRadioButton , 1);

	none__pQRadioButton->setText("none");
	none__pQRadioButton->move(10, 20);
	
	QRadioButton *lines__pQRadioButton = new QRadioButton(&model_radio__mQButtonGroup_);
	CHECK_PTR(lines__pQRadioButton);
	radio_buttons__mList_.push_back(lines__pQRadioButton);
	model_radio__mQButtonGroup_.insert(lines__pQRadioButton, 2);

	lines__pQRadioButton->setText("Lines");
	lines__pQRadioButton->move(10, 40);
	
	QRadioButton *stick__pQRadioButton = new QRadioButton(&model_radio__mQButtonGroup_);
	CHECK_PTR(stick__pQRadioButton);
	radio_buttons__mList_.push_back(stick__pQRadioButton);
	model_radio__mQButtonGroup_.insert(stick__pQRadioButton , 3);

	stick__pQRadioButton->setText("Stick");
	stick__pQRadioButton->move(10, 60);
	
	QRadioButton *ball_and_stick__pQRadioButton = new QRadioButton(&model_radio__mQButtonGroup_);
	CHECK_PTR(ball_and_stick__pQRadioButton);
	radio_buttons__mList_.push_back(ball_and_stick__pQRadioButton);
	model_radio__mQButtonGroup_.insert(ball_and_stick__pQRadioButton, 4);

	ball_and_stick__pQRadioButton->setText("Ball and Stick");
	ball_and_stick__pQRadioButton->move(10, 80);
	
	QRadioButton *van_der_waals__pQRadioButton = new QRadioButton(&model_radio__mQButtonGroup_);
	CHECK_PTR(van_der_waals__pQRadioButton);
	radio_buttons__mList_.push_back(van_der_waals__pQRadioButton);
	model_radio__mQButtonGroup_.insert(van_der_waals__pQRadioButton, 5);

	van_der_waals__pQRadioButton->setText("Van Der Waals");
	van_der_waals__pQRadioButton->move(10, 100);
	

	// connection ----------

	connect(&apply__mQPushButton_,
					SIGNAL(pressed()),
					this,
					SIGNAL(apply()));

	connect(&model_radio__mQButtonGroup_,
					SIGNAL(pressed(int)),
					this,
					SLOT(modelSelected(int)));
}

DisplayProperties::~DisplayProperties
  (void)
{
	List<QRadioButton *>::Iterator iterator__List;

	for (iterator__List = radio_buttons__mList_.begin();
			 iterator__List != radio_buttons__mList_.end();
			 ++iterator__List)
	{
		delete *iterator__List;
	}

	radio_buttons__mList_.clear();	
}

void DisplayProperties::modelSelected(int model)
{
	switch (model)
	{
	  case 1:
			__mpMoleculeObjectProcessor_->setValue(ADDRESS__STATIC_MODEL, VALUE__MODEL_REMOVE);
			__mpMoleculeObjectProcessor_->setValue(ADDRESS__DYNAMIC_MODEL, VALUE__MODEL_REMOVE);
			break;

	  case 2:
			__mpMoleculeObjectProcessor_->setValue(ADDRESS__STATIC_MODEL, VALUE__MODEL_LINES);
			__mpMoleculeObjectProcessor_->setValue(ADDRESS__DYNAMIC_MODEL, VALUE__MODEL_LINES);
			break;

	  case 3:
			__mpMoleculeObjectProcessor_->setValue(ADDRESS__STATIC_MODEL, VALUE__MODEL_STICK);
			__mpMoleculeObjectProcessor_->setValue(ADDRESS__DYNAMIC_MODEL, VALUE__MODEL_LINES);
			break;

	  case 4:
			__mpMoleculeObjectProcessor_->setValue(ADDRESS__STATIC_MODEL, VALUE__MODEL_BALL_AND_STICK);
			__mpMoleculeObjectProcessor_->setValue(ADDRESS__DYNAMIC_MODEL, VALUE__MODEL_LINES);
			break;

	  case 5:
			__mpMoleculeObjectProcessor_->setValue(ADDRESS__STATIC_MODEL, VALUE__MODEL_VAN_DER_WAALS);
			__mpMoleculeObjectProcessor_->setValue(ADDRESS__DYNAMIC_MODEL, VALUE__MODEL_LINES);
			break;

	  default:
			break;
	}
}

void DisplayProperties::modelPrecisionSelected(int model)
{
}

void DisplayProperties::modelDrawingModeSelected(int model)
{
}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include "displayProperties.iC"
#		endif



