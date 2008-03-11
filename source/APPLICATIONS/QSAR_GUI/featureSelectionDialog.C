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
	QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::Cancel);
	QPushButton* applyButton = new QPushButton("OK");
	buttons->addButton(applyButton, QDialogButtonBox::ApplyRole);
	optimize_parameters_ = 0;
	
	if(fsitem->getType()>0) // no validation statistics for removal of colineal features
	{
		optimize_parameters_ = new QCheckBox("optimize model parameters", this);
	
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
	}
	else
	{
		QLabel* label = new QLabel("correlation threshold");
		layout1->addWidget(label);
		layout1->addWidget(k_edit_);
		main_layout->addLayout(layout1);
		
		//q_objects_.push_back(label);
	}
	
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
	bool ok = 0;
	
	if(fs_item_->getType()>0) // no validation statistics for removal of colineal features
	{
		int num = k_edit_->text().toInt(&ok);
		k_ = num;
		optimize_ = optimize_parameters_->isChecked();
		fs_item_->setK(k_);
		fs_item_->setOpt(optimize_);
		
		statistic_ = -1;
		if(statistic_box_!=NULL)
		{
			statistic_ = statistic_box_->currentIndex();
		}
	}
	else
	{
		fs_item_->cor_threshold_=k_edit_->text().toDouble(&ok);
		statistic_ = -1;
		k_ = 0;
		optimize_ = 0;
	}
	
//	if(!ok) throw BALL::VIEW::Exception::InvalidK(__FILE__, __LINE__);
}

int FeatureSelectionDialog::k()
{
	return k_;
}

bool FeatureSelectionDialog::optimize()
{
	return optimize_;
}
