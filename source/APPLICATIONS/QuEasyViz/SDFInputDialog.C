#include <BALL/APPLICATIONS/QuEasyViz/SDFInputDialog.h>
#include <BALL/QSAR/exception.h>
#include <QtGui/QMessageBox>
#include <QtGui/QPushButton>
#include <QtGui/QScrollArea>
#include <QtGui/QTableWidget>
#include <QtGui/QHeaderView>

using namespace BALL::QSAR::Exception;
using namespace BALL::VIEW;

SDFInputDialog::SDFInputDialog(SDFInputDataItem* item):
	input_ok_(false),
	input_item_(item)
{
	property_names_ = input_item_->data()->readPropertyNames(String(input_item_->filename().toStdString()));

	QGridLayout* layout = new QGridLayout(this);
	activity_edit_ = new QLineEdit();
	sd_descriptors_checkbox_ = new QCheckBox("use other properties as descriptors", this);
	class_names_checkbox_ = new QCheckBox("non-numeric class names",this);
	center_descriptor_values_ = new QCheckBox("center descriptor values", this);
	center_descriptor_values_->setChecked(true);
	center_response_values_ = new QCheckBox("center response values",this);
	center_response_values_->setChecked(true);
	QDialogButtonBox* SDFInputDialogButtons= new QDialogButtonBox(QDialogButtonBox::Ok |QDialogButtonBox::Cancel,Qt::Horizontal, this);

	QString tmp;
	QString message_string = QString("This input file contains "+ tmp.setNum(property_names_->size())+ " properties: <br>");

	QStringList labels;
	labels << "ID" <<"Property";

	QTableWidget* table = new QTableWidget(property_names_->size(), 2, this);	
	table->verticalHeader()->hide();
	table->setHorizontalHeaderLabels (labels);
	table->setAlternatingRowColors(true);
	table->setDragDropMode(QAbstractItemView::NoDragDrop);
	table->setEditTriggers(QAbstractItemView::NoEditTriggers);
	table->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
	table->horizontalHeader()->setResizeMode(1,QHeaderView::Stretch); 
	
	for (unsigned int i=0; i< property_names_->size(); i++)
	{
		QTableWidgetItem* id = new QTableWidgetItem("[" + tmp.setNum(i) + "]");
		table->setItem(i, 0, id);	
		QTableWidgetItem* name = new QTableWidgetItem(QString(property_names_->at(i).c_str()));
		table->setItem(i, 1, name);	
	}

	QScrollArea* scrollArea = new QScrollArea(this);
	scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	scrollArea->setFrameShape(QFrame::NoFrame);
	scrollArea->setWidget(table);
	scrollArea->setWidgetResizable(true);
	layout->addWidget(scrollArea, 1, 3, 5, 1,Qt::AlignHCenter);

	message_string += "<br> Numbers of properties that are to be used as activities: <br> (comma separated)";

	QLabel* alabel = new QLabel(message_string,this);

	layout->addWidget(alabel,1,1,1,2);
	layout->addWidget(activity_edit_,2,1,1,2);
	layout->addWidget(sd_descriptors_checkbox_,3,1,Qt::AlignLeft);
	layout->addWidget(class_names_checkbox_,4,1,Qt::AlignLeft);
	layout->addWidget(center_descriptor_values_,5,1,Qt::AlignLeft);
	layout->addWidget(center_response_values_,6,1,Qt::AlignLeft);;
	layout->addWidget(SDFInputDialogButtons,7,1,1,2, Qt::AlignHCenter);

	this->setLayout(layout);
	this->setWindowTitle("Preferences for " + input_item_->name());

	connect(SDFInputDialogButtons, SIGNAL(accepted()), this, SLOT(accept()));
	connect(SDFInputDialogButtons, SIGNAL(rejected()), this, SLOT(reject()));
	connect(class_names_checkbox_,SIGNAL(clicked()),this,SLOT(classNamesChange()));
}


// SLOT
void SDFInputDialog::classNamesChange()
{
	if(class_names_checkbox_->isChecked())
	{
		center_response_values_->setChecked(0);
		center_response_values_->setEnabled(0);
	}
	else
	{
		center_response_values_->setEnabled(1);
	}
}	


SDFInputDialog::SDFInputDialog():
	activity_edit_(NULL),
	center_descriptor_values_(NULL),
	center_response_values_(NULL)	
{
}

SDFInputDialog::~SDFInputDialog()
{
	delete activity_edit_;
	delete center_descriptor_values_;
	delete center_response_values_;
}

void SDFInputDialog::getNumbers()
{
	std::vector<QString> invalid_input;
	QString input = activity_edit_->text().trimmed();
	QList<QString> substrings = input.split(",");
	numbers_.clear();

	bool ok;
	int num = 0;
	for (int i = 0; i < substrings.size(); i++)
	{
		num = substrings[i].toInt(&ok);
		if (ok && num >= 0 && num < (int)property_names_->size())
		{	
			numbers_.insert(num);
		}
		else 
		{	
			if (substrings[i] != "")
			{
				invalid_input.push_back(substrings[i]);
			}
		}
	}

	if (invalid_input.empty())
	{
		input_ok_ = true;
	}
	else
	{	
		numbers_.clear();
		input_ok_ = false;
		throw BALL::QSAR::Exception::InvalidActivityID(__FILE__,__LINE__);
	}

	input_item_->useSDProperties(sd_descriptors_checkbox_->isChecked());
	input_item_->setCenterDataFlag(center_descriptor_values_->isChecked());
	input_item_->setCenterResponseFlag(center_response_values_->isChecked());
	input_item_->setActivityValues(numbers_);
	input_item_->setNonNumericClassNames(class_names_checkbox_->isChecked());
}

SortedList<int> SDFInputDialog::numbers()
{
	return numbers_;
}

bool SDFInputDialog::centerDescriptorValues()
{
	return center_descriptor_values_->isChecked();
}

bool SDFInputDialog::centerResponseValues()
{
	return center_response_values_->isChecked();
}

bool SDFInputDialog::inputOk()
{
	return input_ok_;
}
