
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
	//return if there's no parent
	if (item == NULL)
	{
		return;
	}
	pred_item_ = item;

	QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::Ok,Qt::Horizontal, this);
	QPushButton* print_button = new QPushButton("Save to File", buttons);

	QVBoxLayout* mainLayout = new QVBoxLayout();
	QVBoxLayout* resultGroupLayout = new QVBoxLayout();

	QGroupBox* resultGroup = new QGroupBox(tr("Predicted Activity Values"),this);

	file_name_ = item->name();
	results_ = item->results();
	compound_names_ = item->inputDataItem()->data()->getSubstanceNames();

	QStringList labels;
	labels << "Compound" << "Prediction";
	
	bool show_expected=0;
	const QSARData* test_data = 0;
	if(item->getTestData()->getNoResponseVariables()>0)
	{
		show_expected=1;
		labels<<"Expected";
		test_data = item->getTestData();
	}
	
	QTableWidget* table = new QTableWidget(results_->size(), 2+show_expected, this);	
	table->verticalHeader()->hide();
	table->setHorizontalHeaderLabels (labels);
	table->setAlternatingRowColors(true);					
	table->setDragDropMode(QAbstractItemView::NoDragDrop);		
	table->setEditTriggers(QAbstractItemView::NoEditTriggers);
	table->horizontalHeader()->setResizeMode(QHeaderView::Stretch);		

	if(((uint)results_->size()) == compound_names_->size())
	{	
		int i = 0;
		for (QList<Vector<double> >::ConstIterator it = results_->begin(); it != results_->end(); it++)
		{
			QTableWidgetItem* name = new QTableWidgetItem(QString(compound_names_->at(i).c_str()));
    			table->setItem(i, 0, name);
			QTableWidgetItem* pred = new QTableWidgetItem(QString((((String)(*it)(1)).c_str())));
    			table->setItem(i, 1, pred);
			if(show_expected)
			{
				vector<double>* e = test_data->getActivity(i);
				QTableWidgetItem* expected = new QTableWidgetItem(QString(((String((*e)[0])).c_str())));
				table->setItem(i, 2, expected);
				delete e;
			}
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
	setLayout(mainLayout);	
	setWindowTitle("Predicted Activity Values for " + item->name());
	
	resize(330,450);

	connect(buttons, SIGNAL(accepted()), this, SLOT(accept()));
	connect(print_button, SIGNAL(clicked()), this, SLOT(saveToFile()));
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

	ofstream out(file.fileName().toStdString().c_str());
	
	bool print_expected=0;
	const QSARData* test_data = 0;
	if(pred_item_->getTestData()->getNoResponseVariables()>0)
	{
		print_expected=1;
		test_data = pred_item_->getTestData();
	}

	if(((uint)results_->size()) == compound_names_->size())
	{	
		int i = 0;
		for (QList<Vector<double> >::ConstIterator it = results_->begin(); it != results_->end(); it++)
		{
			out << compound_names_->at(i) << "\t" << (*it)(1);
			if(print_expected) 
			{
				vector<double>* e = test_data->getActivity(i);
				out<<"\t"<<(*e)[0];
				delete e;
			}
			out<< "\n";
			i++;
		}
	}
}
