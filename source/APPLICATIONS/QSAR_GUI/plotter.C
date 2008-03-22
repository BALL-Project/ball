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
	
// 	okButton_ = new QPushButton("Ok", this);
// 	buttons_ = new QDialogButtonBox(Qt::Horizontal, this);
// 	buttons_->addButton(okButton_, QDialogButtonBox::AcceptRole);
// 	buttonsLayout_ = new QHBoxLayout();
// 	buttonsLayout_->addWidget(buttons_);
// 	buttonsLayout_->setAlignment(Qt::AlignBottom);
	

	
}



Plotter::~Plotter()
{
	std::cout<<"Plotter::~Plotter()"<<std::endl;
}


