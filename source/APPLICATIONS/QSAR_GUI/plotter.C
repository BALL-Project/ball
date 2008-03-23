#include <BALL/APPLICATIONS/QSAR_GUI/plotter.h>
#include <iostream>
 
using namespace BALL::VIEW;
using namespace std;

Plotter::Plotter(DataItem* item)
{
	item_ = item;
	
	data_symbol.setStyle(QwtSymbol::Ellipse);
	data_symbol.setSize(5,5);
	data_label_font.setPixelSize(7);
	data_label_alignment=Qt::AlignRight;
	show_data_labels = 1;
	
	qwt_plot_ = new QwtPlot;
	
	okButton_ = new QPushButton("Ok", this);
	show_labels_ = new QCheckBox("show labels",this);
	show_labels_->setChecked(show_data_labels);
	
	buttonsLayout_ = new QHBoxLayout;
	buttonsLayout_->addWidget(okButton_);
	buttonsLayout_->addWidget(show_labels_);
	buttonsLayout_->setAlignment(Qt::AlignLeft);
	
	main_layout_ = new QVBoxLayout;
	main_layout_->addWidget(qwt_plot_);
	main_layout_->addLayout(buttonsLayout_);
	setLayout(main_layout_);
	
	connect(okButton_, SIGNAL(clicked()), this, SLOT(close()));
	connect(show_labels_, SIGNAL(clicked()), this, SLOT(labelsChangeState()));
	
	resize(600,400);
	qwt_plot_->resize(600,400);
	qwt_plot_->show();	
}



Plotter::~Plotter()
{
	std::cout<<"Plotter::~Plotter()"<<std::endl;
	
	delete qwt_plot_;
	delete okButton_;
	delete buttonsLayout_;
	delete main_layout_;
	delete show_labels_;
}


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


