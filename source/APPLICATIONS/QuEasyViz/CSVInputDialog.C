#include <CSVInputDialog.h>
#include <BALL/QSAR/exception.h>
#include <QtGui/QMessageBox>
#include <QtGui/QPushButton>

using namespace BALL::QSAR;

namespace BALL
{
	namespace VIEW
	{

		CSVInputDialog::CSVInputDialog(CSVInputDataItem* item):
			input_ok_(false),
			input_item_(item)
		{
			layout_ = new QGridLayout(this);
			QString tmp; tmp.setNum(1);
			activity_edit_ = new QLineEdit(tmp);
			
			separator_box_ = new QComboBox;
			separator_box_->addItem("comma",0);
			separator_box_->addItem("tabulator",1);
			separator_box_->addItem("semicolon",2);
			separator_box_->addItem("space",3);
			
			x_labels_ = new QCheckBox("File contains descriptor names", this);
			y_labels_ = new QCheckBox("File contains compound names", this);
			class_names_checkbox_ = new QCheckBox("non-numeric class names",this);
			center_descriptor_values_ = new QCheckBox("Center descriptor values", this);
			center_descriptor_values_->setChecked(true);
			center_response_values_ = new QCheckBox("Center response values",this);
			center_response_values_->setChecked(true);
			QDialogButtonBox* inputDialogButtons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,Qt::Horizontal, this);

			QString message_string = "Number of response variables:<br>(assumed to be located in last columns)";
			QString sep_string = "Character used as seperator<br>within the csv-file:";

			alabel_ = new QLabel(message_string,this);
			blabel_ = new QLabel(sep_string,this);
			layout_->addWidget(alabel_,1,1,3,3); layout_->addWidget(activity_edit_,3,4); 
			layout_->addWidget(blabel_,4,1,2,3); layout_->addWidget(separator_box_,4,4);
			layout_->addWidget(x_labels_,6,1,Qt::AlignLeft);
			layout_->addWidget(y_labels_,7,1,Qt::AlignLeft);
			layout_->addWidget(class_names_checkbox_,8,1,Qt::AlignLeft);
			layout_->addWidget(center_descriptor_values_,9,1,Qt::AlignLeft);
			layout_->addWidget(center_response_values_,10,1,Qt::AlignLeft);;
			layout_->addWidget(inputDialogButtons,11,1,1,2, Qt::AlignHCenter);
			
			setLayout(layout_);
			setWindowTitle("Preferences for " + input_item_->name());

			connect(inputDialogButtons, SIGNAL(accepted()), this, SLOT(accept()));
			connect(inputDialogButtons, SIGNAL(rejected()), this, SLOT(reject()));
			connect(class_names_checkbox_,SIGNAL(clicked()),this,SLOT(classNamesChange()));
		}

		CSVInputDialog::~CSVInputDialog()
		{
			delete separator_box_;
			delete activity_edit_;
			delete x_labels_;
			delete y_labels_;
			delete center_descriptor_values_;
			delete center_response_values_;
			delete layout_;
			delete alabel_;
			delete blabel_;
		}


		// SLOT
		void CSVInputDialog::classNamesChange()
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


		void CSVInputDialog::readNumY()
		{
			QString input = activity_edit_->text().trimmed();
			QString sep = separator_box_->currentText().trimmed();
			if(sep=="tabulator") sep="	";
			else if(sep=="comma") sep=",";
			else if(sep=="semicolon") sep=";";
			else if(sep=="space") sep=" ";

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
			input_item_->setSeperator(sep.toStdString());
			input_item_->setNonNumericClassNames(class_names_checkbox_->isChecked());
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
	}
}
