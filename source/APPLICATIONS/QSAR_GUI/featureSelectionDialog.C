#include <BALL/APPLICATIONS/QSAR_GUI/featureSelectionDialog.h>
#include <iostream>
#include <BALL/QSAR/exception.h>
#include <BALL/APPLICATIONS/QSAR_GUI/exception.h>
#include <QtGui/QMessageBox>
#include <QtGui/QPushButton>



using namespace BALL::QSAR::Exception;
using namespace BALL::VIEW;

FeatureSelectionDialog::FeatureSelectionDialog(FeatureSelectionItem* fsitem, ModelItem* model):
	fs_item_(fsitem)
{
	QVBoxLayout* main_layout = new QVBoxLayout(this);
	QHBoxLayout* layout1 = new QHBoxLayout(this);
	k_edit_ = new QLineEdit(this);
	optimize_parameters_ = new QCheckBox("optimize model parameters", this);
	QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::Cancel);
	QPushButton* applyButton = new QPushButton("OK");
	buttons->addButton(applyButton, QDialogButtonBox::ApplyRole);

	QLabel* klabel = new QLabel("k for k-fold cross validation");

	layout1->addWidget(klabel);
	layout1->addWidget(k_edit_);

	QHBoxLayout* layout2 = new QHBoxLayout();
	layout2->addWidget(optimize_parameters_);
	
	main_layout->addLayout(layout1);
	statistic_box_ = NULL;
	
	// let user select validation statistic in case of classification model
	if(!model->getRegistryEntry()->regression)
	{
		QHBoxLayout* layout3 = new QHBoxLayout();
		QLabel* label3 = new QLabel("classification statistic");
		statistic_box_ = new QComboBox;
		
		const vector<String>* statistics = model->getRegistryEntry()->getStatistics();
		for(uint i=0;i<statistics->size();i++)
		{
			statistic_box_->addItem((*statistics)[i].c_str(),i);
		}
			
		layout3->addWidget(label3);layout3->addWidget(statistic_box_);
		main_layout->addLayout(layout3);
		
		q_objects_.push_back(layout3);
		q_objects_.push_back(label3);
		q_objects_.push_back(statistic_box_);
	}
	
	main_layout->addLayout(layout2);
	main_layout->addWidget(buttons);
	this->setLayout(main_layout);
	this->setWindowTitle("Feature Selection:" + fs_item_->name());

	connect(applyButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(buttons, SIGNAL(rejected()), this, SLOT(reject()));
}

FeatureSelectionDialog::FeatureSelectionDialog():
	k_edit_(NULL),
	optimize_parameters_(NULL)
{
}

FeatureSelectionDialog::~FeatureSelectionDialog()
{
	delete k_edit_;
	delete optimize_parameters_;
	
	for(list<QObject*>::iterator it=q_objects_.begin(); it!=q_objects_.end();it++)
	{
		delete *it;
	}
}	

void FeatureSelectionDialog::applyInput()
{
	bool ok;
	int num = k_edit_->text().toInt(&ok);
	if (ok)
	{	
		k_ = num;
	}
	else
	{
		throw BALL::VIEW::Exception::InvalidK(__FILE__, __LINE__);
	}

	optimize_ = optimize_parameters_->isChecked();
	fs_item_->setK(k_);
	fs_item_->setOpt(optimize_);
	
	statistic_ = -1;
	if(statistic_box_!=NULL)
	{
		statistic_ = statistic_box_->currentIndex();
	}
}

int FeatureSelectionDialog::k()
{
	return k_;
}

bool FeatureSelectionDialog::optimize()
{
	return optimize_;
}
