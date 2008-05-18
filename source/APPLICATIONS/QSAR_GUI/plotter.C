#include <BALL/APPLICATIONS/QSAR_GUI/plotter.h>
#include <iostream>
#include <QPrinter>
#include <QFileDialog>
#include <QPrintDialog>
#include <qwt_plot_zoomer.h>
 
using namespace BALL::VIEW;
using namespace std;

Plotter::Plotter(DataItem* item)
{
	item_ = item;
	
	data_symbol.setStyle(QwtSymbol::Ellipse);
	data_symbol.setSize(5,5);
	data_label_font.setPointSize(6);
	data_label_alignment=Qt::AlignRight;
	show_data_labels = 1;
	
	qwt_plot_ = new QwtPlot;
	
	okButton_ = new QPushButton("Ok", this);
	show_labels_ = new QCheckBox("show labels",this);
	show_labels_->setChecked(show_data_labels);
	saveButton_ = new QPushButton("save",this);
	printButton_ = new QPushButton("print",this);
	
	buttonsLayout_ = new QHBoxLayout;
	buttonsLayout_->addWidget(okButton_);
	buttonsLayout_->addWidget(show_labels_);
	buttonsLayout_->addWidget(saveButton_);
	buttonsLayout_->addWidget(printButton_);
	buttonsLayout_->setAlignment(Qt::AlignLeft);
	
	main_layout_ = new QVBoxLayout;
	main_layout_->addWidget(qwt_plot_);
	main_layout_->addLayout(buttonsLayout_);
	setLayout(main_layout_);
	
	connect(okButton_, SIGNAL(clicked()), this, SLOT(close()));
	connect(show_labels_, SIGNAL(clicked()), this, SLOT(labelsChangeState()));
	connect(saveButton_, SIGNAL(clicked()), this, SLOT(save()));
	connect(printButton_,SIGNAL(clicked()),this,SLOT(print()));
	
	resize(600,400);
	qwt_plot_->resize(600,400);
	qwt_plot_->show();	
}



Plotter::~Plotter()
{
	delete qwt_plot_;
 	delete okButton_;
 	delete show_labels_;
 	delete saveButton_;
	delete printButton_;
	delete buttonsLayout_;
	delete main_layout_;
}

// SLOT
void Plotter::labelsChangeState()
{
	
	int a = show_labels_->checkState();
	if(a==0) // unchecked
	{
		show_data_labels = 0;
		plot();
	}
	else if(a==2) // checked
	{
		show_data_labels = 1;
		plot();
	}
}

// SLOT
void Plotter::save()
{
	QString file = QFileDialog::getSaveFileName(this, tr("Save Plot"),
			QDir::homePath(), tr("Images (*.png *.xpm *.jpg *.pdf *.ps *.eps)"));
	
	if(file!="") printToFile(file);
}

// SLOT
void Plotter::print()
{
	QPrinter printer(QPrinter::HighResolution);
	QPrintDialog print_dialog(&printer,this);
	if (print_dialog.exec() == QDialog::Accepted) 
	{
		qwt_plot_->print(printer);
	}
}


void Plotter::printToFile(QString& file)
{
	QPrinter printer(QPrinter::HighResolution);
	printer.setOutputFileName(file);
	qwt_plot_->print(printer);
}




