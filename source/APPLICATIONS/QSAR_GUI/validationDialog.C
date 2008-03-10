#include <BALL/APPLICATIONS/QSAR_GUI/validationDialog.h>
#include <BALL/APPLICATIONS/QSAR_GUI/exception.h>
#include <BALL/QSAR/exception.h>
#include <QtGui/QMessageBox>
#include <QtGui/QPushButton>

using namespace BALL::QSAR::Exception;
using namespace BALL::VIEW;

ValidationDialog::ValidationDialog(ValidationItem* val_item, ModelItem* model):
	val_item_(val_item)
{
	QVBoxLayout* main_layout = new QVBoxLayout(this);
	QGridLayout* layout1 = new QGridLayout();
	QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::Cancel,Qt::Horizontal, this);
	QPushButton* applyButton = new QPushButton("OK", this);
	buttons->addButton(applyButton, QDialogButtonBox::ApplyRole);
	
// 	q_objects_.push_back(main_layout);
 	q_objects_.push_back(layout1); 
// 	q_objects_.push_back(buttons);
// 	q_objects_.push_back(applyButton);
	

	if (val_item->getValidationType() == 2)
	{
		QLabel* klabel = new QLabel("k for k-fold cross validation");
		k_edit_ = new QLineEdit(this);
		layout1->addWidget(klabel,1,1);
		layout1->addWidget(k_edit_,1,2);
		
		q_objects_.push_back(klabel); 
		q_objects_.push_back(k_edit_);
	}

	else if (val_item->getValidationType() == 3)
	{
		QLabel* label = new QLabel("number of samples for bootstrapping");
		n_of_samples_edit_ = new QLineEdit(this);
		layout1->addWidget(label,1,1);
		layout1->addWidget(n_of_samples_edit_,1,2);
		
		q_objects_.push_back(label);
		q_objects_.push_back(n_of_samples_edit_);
	}
		
	else if (val_item->getValidationType() == 4)
	{
		QLabel* klabel = new QLabel("k for k-fold cross validation");
		QLabel* label = new QLabel("number of runs for response permutation test");
		k_edit_ = new QLineEdit(this);
		n_of_runs_edit_ = new QLineEdit(this);
		layout1->addWidget(label,1,1);
		layout1->addWidget(n_of_runs_edit_,1,2);
		layout1->addWidget(klabel,2,1);
		layout1->addWidget(k_edit_,2,2);
		
		q_objects_.push_back(k_edit_);
		q_objects_.push_back(n_of_runs_edit_);
		q_objects_.push_back(klabel);
		q_objects_.push_back(label);
	}
	
	main_layout->addLayout(layout1);
	
	statistic_box_ = NULL;
	available_statistics_= NULL;
	if(!model->getRegistryEntry()->regression)
	{
		QHBoxLayout* layout3 = new QHBoxLayout();
		QLabel* label3 = new QLabel("classification statistic");
		statistic_box_ = new QComboBox;
		
		available_statistics_ = model->getRegistryEntry()->getStatistics();
		for(uint i=0;i<available_statistics_->size();i++)
		{
			statistic_box_->addItem((*available_statistics_)[i].first.c_str(),i);
		}
			
		layout3->addWidget(label3);layout3->addWidget(statistic_box_);
		main_layout->addLayout(layout3);
		
		q_objects_.push_back(layout3);
		q_objects_.push_back(label3);
		q_objects_.push_back(statistic_box_);
	}

	main_layout->addWidget(buttons);

	this->setLayout(main_layout);
	this->setWindowTitle("Model Validation");

	connect(applyButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(buttons, SIGNAL(rejected()), this, SLOT(reject()));
}

ValidationDialog::ValidationDialog():
	val_item_(NULL)
{
}

ValidationDialog::~ValidationDialog()
{
	for(list<QObject*>::iterator it=q_objects_.begin(); it!=q_objects_.end();it++)
	{
		delete *it;
	}
}	

void ValidationDialog::applyInput()
{
	bool ok;
	int num = 0;
	if (ok)
	{	
		k_ = num;
	}

	switch(val_item_->getValidationType())
	{
		case 2:
			ok = false;
			num = k_edit_->text().toInt(&ok);
			if (ok)
			{	
				k_ = num;
				val_item_->setK(k_);
			}
			else
			{
				throw BALL::VIEW::Exception::InvalidK(__FILE__, __LINE__);
			}
			break;
		case 3:	
			ok = false;
			num = n_of_samples_edit_->text().toInt(&ok);
			if (ok)
			{	
				n_of_samples_ = num;
				val_item_->setNumOfSamples(n_of_samples_);
			}	
			else
			{
				throw BALL::VIEW::Exception::InvalidK(__FILE__, __LINE__);
			}
			break;
		case 4:	
			ok = false;
			num = k_edit_->text().toInt(&ok);
			if (ok)
			{	
				k_ = num;
				val_item_->setK(k_);
			}
			else
			{
				throw BALL::VIEW::Exception::InvalidK(__FILE__, __LINE__);
			}
			
			ok = false;
			num = n_of_runs_edit_ ->text().toInt(&ok);
			if (ok)
			{	
				n_of_runs_ = num;
				val_item_->setNumOfRuns(n_of_runs_);
			}
			else
			{
				throw BALL::VIEW::Exception::InvalidK(__FILE__, __LINE__);
			}
			break;
	}
	statistic_ = 0;
	if(statistic_box_!=NULL)
	{
		statistic_ = (*available_statistics_)[statistic_box_->currentIndex()].second;
	}
}

int ValidationDialog::k()
{
	return k_;
}
