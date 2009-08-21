#include <BALL/APPLICATIONS/QuEasyViz/plotter.h>
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
	selected_activity_ = 0;
	
	data_symbol.setStyle(QwtSymbol::Ellipse);
	data_symbol.setSize(5,5);
	data_label_font.setPointSize(6);
	data_label_alignment=Qt::AlignRight;
	show_data_labels = 1;
	print_data_symbol = data_symbol;
	
	zoomer_ = NULL;
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
	activity_combobox_ = new QComboBox;
	buttonsLayout_->addWidget(activity_combobox_);
	buttonsLayout_->setAlignment(Qt::AlignLeft);
	
	main_layout_ = new QVBoxLayout;
	main_layout_->addWidget(qwt_plot_);
	main_layout_->addLayout(buttonsLayout_);
	setLayout(main_layout_);
	
	connect(okButton_, SIGNAL(clicked()), this, SLOT(close()));
	connect(show_labels_, SIGNAL(clicked()), this, SLOT(labelsChangeState()));
	connect(saveButton_, SIGNAL(clicked()), this, SLOT(save()));
	connect(printButton_,SIGNAL(clicked()),this,SLOT(print()));
	connect(activity_combobox_,SIGNAL(currentIndexChanged(int)),this,SLOT(activityChange()));
	
	activity_combobox_->hide();
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
		plot(0);
		qwt_plot_->replot();
	}
	else if(a==2) // checked
	{
		show_data_labels = 1;
		plot(0);
		qwt_plot_->replot();
	}
}

// SLOT
void Plotter::save()
{
	QwtSymbol symbol_backup = data_symbol;
	if(data_symbol!=print_data_symbol)
	{
		data_symbol = print_data_symbol;
		plot(0);
	}
	
	QString selected_ext;
	QString file = QFileDialog::getSaveFileName(this, tr("Save Plot"),
		QDir::homePath(), tr("PNG (*.png);;Encapsulated PostScript (*.eps);;Portable Document Format (*.pdf);;PostScript (*.ps);;JPG (*.jpg);;GIF (*.gif);;Windows Bitmap (*.bmp);;Tagged Image File Format (*.tif)"),&selected_ext);
	if(file=="") return; 
	
	String f=file.toStdString();
	bool valid_extension=false;
	if(f.find_last_of(".")!=string::npos) // check whether user ommitted file-extension
	{
		String ext=f.substr(f.find_last_of(".")+1);
		if(ext=="png"||ext=="eps"||ext=="pdf"||ext=="ps"||ext=="jpg"||ext=="gif"||ext=="bmp"||ext=="tif")
		{
			valid_extension=true;
		}
	}
	if(!valid_extension) // set extension according to selected filter
	{
		String s = selected_ext.toStdString().substr(0,3);
		if(s=="PNG") file+=".png";
		else if(s=="Enc") file+=".eps";
		else if(s=="Por") file+=".pdf";
		else if(s=="Pos") file+=".ps";
		else if(s=="JPG") file+=".jpg";
		else if(s=="GIF") file+=".gif";
		else if(s=="Win") file+=".bmp";
		else if(s=="Tag") file+=".tif";		
	}
	
	printToFile(file);
	
	if(data_symbol!=symbol_backup)
	{
		data_symbol = symbol_backup;
		plot(0);
	}
}

// SLOT
void Plotter::print()
{
	QwtSymbol symbol_backup = data_symbol;
	if(data_symbol!=print_data_symbol)
	{
		data_symbol = print_data_symbol;
		plot(0);
	}
	
	QPrinter printer(QPrinter::HighResolution);
	QPrintDialog print_dialog(&printer,this);
	if (print_dialog.exec() == QDialog::Accepted) 
	{
		printer.setResolution(600);
		qwt_plot_->print(printer);
	}
	
	if(data_symbol!=symbol_backup)
	{
		data_symbol = symbol_backup;
		plot(0);
	}
}


// SLOT
void Plotter::activityChange()
{
	delete zoomer_;
	selected_activity_ = activity_combobox_->itemData(activity_combobox_->currentIndex()).toInt();
	plot(1);
	qwt_plot_->replot();
	zoomer_ = new QwtPlotZoomer(qwt_plot_->canvas(),this); // if not creating a new zoomer, zooming will not work correctly
}


void Plotter::printToFile(QString& file)
{
	QwtSymbol symbol_backup = data_symbol;
	if(data_symbol!=print_data_symbol)
	{
		data_symbol = print_data_symbol;
		plot(0);
	}
	
	String ext = file.toStdString();
	if(ext.find_last_of(".")!=string::npos)
	{
		ext=ext.substr(ext.find_last_of(".")+1);
	}
	if(ext=="eps"||ext=="pdf"||ext=="ps")
	{
		QPrinter printer(QPrinter::HighResolution);
		printer.setOutputFileName(file);
		printer.setResolution(600);
		printer.setOrientation(QPrinter::Landscape);
		qwt_plot_->print(printer);
	}
	else
	{
		QImage image(qwt_plot_->width(),qwt_plot_->height(),QImage::Format_RGB32);
		QColor color(255,255,255);
		image.fill(color.rgb());
		qwt_plot_->print(image);
		image.save(file);		
	}
	
	if(data_symbol!=symbol_backup)
	{
		data_symbol = symbol_backup;
		plot(0);
	}
}

QColor Plotter::generateColor(int no_colors, int current_no)
{
	int k = round(pow(no_colors,1/3.));
	int n = pow(3.,k)-1;
	int width = 255./k;
	
	int R = (current_no/3)*width;
	int G = ((current_no+2)/3)*width;
	int B = ((n-current_no-1)/3)*width;
	
	return QColor(R,G,B);
}




