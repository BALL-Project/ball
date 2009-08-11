#include <BALL/APPLICATIONS/QuEasyViz/inputDataDialog.h>

#include <QtGui/QDialogButtonBox>
#include <QtGui/QFileDialog>
#include <QtGui/QLabel>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QPushButton>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtGui/QScrollArea>
#include <QtGui/QTableWidget>
#include <QtGui/QHeaderView>

using namespace BALL::VIEW;

InputDataDialog::InputDataDialog(InputDataItem* item)	
{
	///return if there's no parent
	if (item == NULL)
	{
		return;
	}
	file_name_ = item->name();
	if (item->data())
	{
		compound_names_ = item->data()->getSubstanceNames();
	}
	QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::Ok,Qt::Horizontal, this);
	QPushButton* print_button = new QPushButton("Save to File", buttons);
	QVBoxLayout* mainLayout = new QVBoxLayout();
	QVBoxLayout* resultGroupLayout = new QVBoxLayout();
	QGroupBox* resultGroup = new QGroupBox(tr("Compounds"),this);

	if (compound_names_->size() > 0)
	{
		QStringList labels;
		labels << "Compound";
	
		QTableWidget* table = new QTableWidget(compound_names_->size(), 1, this);	
		table->verticalHeader()->hide();
		table->setHorizontalHeaderLabels (labels);
		table->setAlternatingRowColors(true);
		table->setDragDropMode(QAbstractItemView::NoDragDrop);
		table->setEditTriggers(QAbstractItemView::NoEditTriggers);
		table->horizontalHeader()->setResizeMode(QHeaderView::Stretch); 
	
		for (unsigned int i=0; i< compound_names_->size(); i++)
		{
			QTableWidgetItem* name = new QTableWidgetItem(QString(compound_names_->at(i).c_str()));
			table->setItem(i, 0, name);	
		}
	
		QScrollArea* scrollArea = new QScrollArea(this);
		scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
		scrollArea->setFrameShape(QFrame::NoFrame);
		scrollArea->setWidget(table);
		scrollArea->setWidgetResizable(true);
	
		resultGroupLayout->addWidget(scrollArea);
		resultGroup->setLayout(resultGroupLayout);
	}
	else
	{
		QLabel* label = new QLabel("No data available, execute pipeline first");
		resultGroupLayout->addWidget(label);
		resultGroup->setLayout(resultGroupLayout);
	}
	
	mainLayout->addWidget(resultGroup);
	mainLayout->addWidget(buttons);
	mainLayout->addStretch(1);
	setLayout(mainLayout);	
	setWindowTitle("Descriptors in " + item->name());

	connect(buttons, SIGNAL(accepted()), this, SLOT(accept()));
	connect(print_button, SIGNAL(clicked()), this, SLOT(saveToFile()));
}

InputDataDialog::~InputDataDialog()
{
}

void InputDataDialog::saveToFile()
{
	QString filename = QFileDialog::getSaveFileName(this, tr("Save File as"),file_name_ +"_compounds.txt",tr("text (*.txt)"));
	if (filename.isEmpty())
	{
		return;
	} 
	
	QFile file(filename);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		return;
	}

	QTextStream out(&file);

	for (unsigned int i=0; i< compound_names_->size(); i++)
	{
		out << QString(compound_names_->at(i).c_str()) << "\n";	
	}
}
