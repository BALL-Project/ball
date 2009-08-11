#include <BALL/APPLICATIONS/QuEasyViz/validationDialog.h>
#include <BALL/APPLICATIONS/QuEasyViz/exception.h>
#include <BALL/APPLICATIONS/QuEasyViz/mainWindow.h>
#include <BALL/QSAR/exception.h>
#include <QtGui/QMessageBox>
#include <QtGui/QPushButton>

using namespace BALL::QSAR::Exception;
using namespace BALL::VIEW;

ValidationDialog::ValidationDialog(ValidationItem* val_item, ModelItem* model):
	val_item_(val_item)
{
	QVBoxLayout* main_layout = new QVBoxLayout(this);
	QGridLayout* layout1 = new QGridLayout;
	QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::Cancel,Qt::Horizontal, this);
	QPushButton* applyButton = new QPushButton("OK", this);
	buttons->addButton(applyButton, QDialogButtonBox::ApplyRole);
	
	Registry* reg = model->view()->data_scene->main_window->registry();
	QString default_value;
			
	if (val_item->getValidationType() == 2)
	{
		QLabel* klabel = new QLabel("k for k-fold cross validation");
		k_edit_ = new QLineEdit(this);
		default_value.setNum(reg->default_k);
		k_edit_->setText(default_value);
		layout1->addWidget(klabel,1,1);
		layout1->addWidget(k_edit_,1,2);
	}

	else if (val_item->getValidationType() == 3 || val_item->getValidationType() == 6)
	{
		QLabel* label = new QLabel("number of samples for bootstrapping");
		n_of_samples_edit_ = new QLineEdit(this);
		default_value.setNum(reg->default_no_boostrap_samples);
		n_of_samples_edit_->setText(default_value);
		layout1->addWidget(label,1,1);
		layout1->addWidget(n_of_samples_edit_,1,2);
	}
		
	else if (val_item->getValidationType() == 4)
	{
		QLabel* klabel = new QLabel("k for k-fold cross validation");
		QLabel* label = new QLabel("number of runs for response permutation test");
		k_edit_ = new QLineEdit(this);
		default_value.setNum(reg->default_k);
		k_edit_->setText(default_value);
		n_of_runs_edit_ = new QLineEdit(this);
		default_value.setNum(reg->default_no_permutations);
		n_of_runs_edit_->setText(default_value);
		layout1->addWidget(label,1,1);
		layout1->addWidget(n_of_runs_edit_,1,2);
		layout1->addWidget(klabel,2,1);
		layout1->addWidget(k_edit_,2,2);
	}
	else if (val_item->getValidationType() == 5)
	{
		QLabel* label1 = new QLabel("fraction of data to be set aside for validation");
		val_fraction_edit_ = new QLineEdit(this);
		val_fraction_edit_->setText("0.25");
		QLabel* label2 = new QLabel("number of nested cross validation folds");
		n_of_ncv_folds_edit_ = new QLineEdit(this);
		n_of_ncv_folds_edit_->setText("4");

		layout1->addWidget(label1,1,1);
		layout1->addWidget(val_fraction_edit_,1,2);
		layout1->addWidget(label2,2,1);
		layout1->addWidget(n_of_ncv_folds_edit_,2,2);
	}
	
	main_layout->addLayout(layout1);
	
	statistic_box_ = NULL;
	if(val_item->getValidationType()!=6)
	{
		QHBoxLayout* layout3 = new QHBoxLayout;
		QLabel* label3 = new QLabel("quality statistic");
		statistic_box_ = new QComboBox;
		
		const map<uint,String>* statistics = model->getRegistryEntry()->getStatistics();
		for(map<uint,String>::const_iterator it=statistics->begin(); it!=statistics->end(); ++it)
		{
			statistic_box_->addItem(it->second.c_str(),it->first);
		}
			
		layout3->addWidget(label3);layout3->addWidget(statistic_box_);
		main_layout->addLayout(layout3);
	}

	main_layout->addWidget(buttons);

	this->setLayout(main_layout);
	this->setWindowTitle("Model Validation");

	connect(applyButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(buttons, SIGNAL(rejected()), this, SLOT(reject()));
}

ValidationDialog::~ValidationDialog()
{
}	

void ValidationDialog::applyInput()
{
	bool ok;
	int num = 0;
	if (ok)
	{	
		k_ = num;
	}

	if(val_item_->getValidationType()==2)
	{
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
	}
	else if(val_item_->getValidationType()==3 || val_item_->getValidationType()==6)
	{
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
	}
	else if(val_item_->getValidationType()==4)
	{	
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
	}
	else if(val_item_->getValidationType()==5)
	{
		ok = false;
		num = n_of_ncv_folds_edit_->text().toInt(&ok);
		val_fraction_ = val_fraction_edit_->text().toDouble(&ok);
		if (ok)
		{
			n_of_ncv_folds_ = num;
			val_item_->setValFraction(val_fraction_);
			val_item_->setNumOfNCVFolds(n_of_ncv_folds_);
		}
		else
		{
			throw BALL::VIEW::Exception::InvalidK(__FILE__, __LINE__);
		}
	}
	statistic_ = -1;
	if(statistic_box_!=NULL)
	{
		statistic_ = statistic_box_->currentIndex();
	}
}

int ValidationDialog::k()
{
	return k_;
}
