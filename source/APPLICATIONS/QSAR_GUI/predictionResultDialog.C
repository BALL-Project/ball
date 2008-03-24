
#include <BALL/APPLICATIONS/QSAR_GUI/predictionResultDialog.h>

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

PredictionResultDialog::PredictionResultDialog(PredictionItem* item)	
{
	///return if there's no parent
	if (item == NULL)
	{
		return;
	}

	QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::Ok,Qt::Horizontal, this);
	QPushButton* print_button = new QPushButton("Save to File", buttons);

	QVBoxLayout* mainLayout = new QVBoxLayout();
	QVBoxLayout* resultGroupLayout = new QVBoxLayout();

	QGroupBox* resultGroup = new QGroupBox(tr("Predicted Activity Values"),this);

	file_name_ = item->name();
	results_ = item->results();
	compound_names_ = item->inputDataItem()->data()->getSubstanceNames();

	QStringList labels;
	labels << "Compound" << "Activity";

	QTableWidget* table = new QTableWidget(results_->size(), 2, this);	
	table->verticalHeader()->hide();
	table->setHorizontalHeaderLabels (labels);
	table->setAlternatingRowColors(true);					
	table->setDragDropMode(QAbstractItemView::NoDragDrop);		
	table->setEditTriggers(QAbstractItemView::NoEditTriggers);
	table->horizontalHeader()->setResizeMode(QHeaderView::Stretch);		

	if(results_->size() == compound_names_->size())
	{	
		int i = 0;
		for (QList<RowVector>::ConstIterator it = results_->begin(); it != results_->end(); it++)
		{
			QTableWidgetItem* name = new QTableWidgetItem(QString(compound_names_->at(i).c_str()));
    			table->setItem(i, 0, name);
			QTableWidgetItem* value = new QTableWidgetItem(QString((((String)(*it)(1)).c_str())));
    			table->setItem(i, 1, value);
			i++;
		}
	}

	QScrollArea* scrollArea = new QScrollArea(this);
	scrollArea->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
	scrollArea->setVerticalScrollBarPolicy ( Qt::ScrollBarAsNeeded );
	scrollArea->setFrameShape(QFrame::NoFrame);
	scrollArea->setWidget(table);
	scrollArea->setWidgetResizable(true);

	resultGroupLayout->addWidget(scrollArea);
	resultGroup->setLayout(resultGroupLayout);
	
	mainLayout->addWidget(resultGroup);
	mainLayout->addWidget(buttons);
	mainLayout->addStretch(1);
	setLayout(mainLayout);	
	setWindowTitle("Predicted Activity Values for " + item->name());

	connect(buttons, SIGNAL(accepted()), this, SLOT(accept()));
	connect(print_button, SIGNAL(clicked()), this, SLOT(saveToFile()));
}

PredictionResultDialog::PredictionResultDialog()
{
}

PredictionResultDialog::~PredictionResultDialog()
{
}

void PredictionResultDialog::saveToFile()
{
	QString filename = QFileDialog::getSaveFileName(this, tr("Save File as"),file_name_ +"_prediction_results.txt",tr("text (*.txt)"));

	QFile file(filename);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		return;
	}

	QTextStream out(&file);

	if(results_->size() == compound_names_->size())
	{	
		int i = 0;
		for (QList<RowVector>::ConstIterator it = results_->begin(); it != results_->end(); it++)
		{
			out << QString(compound_names_->at(i).c_str()) << "\t" << QString((((String)(*it)(1)).c_str())) << "\n";
			i++;
		}
	}
}
