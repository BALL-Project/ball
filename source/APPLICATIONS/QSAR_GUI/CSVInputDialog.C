#include <BALL/APPLICATIONS/QSAR_GUI/CSVInputDialog.h>
#include <BALL/QSAR/exception.h>
#include <QtGui/QMessageBox>
#include <QtGui/QPushButton>

using namespace BALL::QSAR::Exception;
using namespace BALL::VIEW;

CSVInputDialog::CSVInputDialog(CSVInputDataItem* item):
	input_ok_(false),
	input_item_(item)
{
	QGridLayout* layout = new QGridLayout(this);
	QString tmp;
	tmp.setNum(0);
	activity_edit_ = new QLineEdit(tmp);
	x_labels_ = new QCheckBox("File contains descriptor names", this);
	y_labels_ = new QCheckBox("File contains compound names", this);
	center_descriptor_values_ = new QCheckBox("Center descriptor values", this);
	center_descriptor_values_->setChecked(true);
	center_response_values_ = new QCheckBox("Center response values",this);
	center_response_values_->setChecked(true);
	QDialogButtonBox* inputDialogButtons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,Qt::Horizontal, this);

	QString message_string = "Number of properties that are to be used as activities:<br>(activity values are assumed to be located in last columns of the table)";


	QLabel* alabel = new QLabel(message_string,this);
	layout->addWidget(alabel,1,1,1,2);
	layout->addWidget(activity_edit_,2,1,1,2);
	layout->addWidget(x_labels_,3,1,Qt::AlignLeft);
	layout->addWidget(y_labels_,4,1,Qt::AlignLeft);
	layout->addWidget(center_descriptor_values_,5,1,Qt::AlignLeft);
	layout->addWidget(center_response_values_,6,1,Qt::AlignLeft);;
	layout->addWidget(inputDialogButtons,7,1,1,2, Qt::AlignHCenter);
	
	setLayout(layout);
	setWindowTitle("Preferences for " + input_item_->name());


	connect(inputDialogButtons, SIGNAL(accepted()), this, SLOT(accept()));
	connect(inputDialogButtons, SIGNAL(rejected()), this, SLOT(reject()));
}

CSVInputDialog::~CSVInputDialog()
{
	delete activity_edit_;
	delete x_labels_;
	delete y_labels_;
}

void CSVInputDialog::readNumY()
{
	QString input = activity_edit_->text().trimmed();

	bool ok;
	int num = 0;
	num = input.toInt(&ok);
	if (ok && num >= 0)
	{	
		no_y_ = num;
		input_ok_ = true;
	}
	else 
	{	
		input_ok_ = false;
		throw BALL::QSAR::Exception::InvalidActivityID(__FILE__,__LINE__);
	}

	input_item_->setCenterDataFlag(center_descriptor_values_->isChecked());
	input_item_->setCenterResponseFlag(center_response_values_->isChecked());
	input_item_->setXLabelFlag(x_labels_->isChecked());
	input_item_->setYLabelFlag(y_labels_->isChecked());
	input_item_->setNumOfActivities(no_y_);
}

bool CSVInputDialog::xLabels()
{
	return x_labels_;
}

bool CSVInputDialog::yLabels()
{
	return y_labels_;
}

bool CSVInputDialog::inputOk()
{
	return input_ok_;
}

int CSVInputDialog::numberOfActivities()
{
	return no_y_;
}
