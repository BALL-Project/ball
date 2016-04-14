// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/DIALOGS/contourSurfaceDialog.h>
#include <BALL/VIEW/WIDGETS/datasetControl.h>
#include <BALL/VIEW/DATATYPE/standardDatasets.h>
#include <BALL/VIEW/KERNEL/common.h>

#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLayout>

namespace BALL
{
 namespace VIEW
 {

ContourSurfaceDialog::ContourSurfaceDialog( QWidget* parent,  const char* name )
 : QDialog(parent), 
 	 Ui_ContourSurfaceDialogData()
{
	setupUi(this);

	// signals and slots connections
	connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect( buttonOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect( threshold, SIGNAL( textChanged(const QString&) ), this, SLOT( valuesChanged() ) );
	connect( grids, SIGNAL( activated(int) ), this, SLOT( valuesChanged() ) );
	connect( color_button, SIGNAL( clicked() ), this, SLOT( chooseColor() ) );

	setObjectName(name);
}

ContourSurfaceDialog::~ContourSurfaceDialog()
{
}

double ContourSurfaceDialog::getThreshold() const	
{
	if (threshold->text().isEmpty()) return DBL_MAX;
	try
	{
		return (double)ascii(threshold->text()).toFloat();
	}
	catch(...)
	{
		return DBL_MAX;
	}
}

void ContourSurfaceDialog::setGrid(Dataset* grid)
{
	grid_ = grid;
}

Dataset* ContourSurfaceDialog::getGrid() 
{
	return grid_;
}

void ContourSurfaceDialog::valuesChanged()
{
	buttonOk->setEnabled((grids->currentIndex() != -1) && 
												!grids->currentText().isEmpty() &&
												(getThreshold() != DBL_MAX));
}

int ContourSurfaceDialog::exec()
{
	grids->clear();
	vector<Dataset*> sets = controller_->getDatasets();
	vector<Dataset*>::iterator it = sets.begin();
	unsigned int index_selected_grid=0;
	for (unsigned int i=0; it != sets.end(); it++, i++)
	{
		grids->addItem((**it).getName().c_str());
		if(*it==grid_) index_selected_grid=i;
	}
	grids->setCurrentIndex(index_selected_grid);

	valuesChanged();
	if (!QDialog::exec()) return QDialog::Rejected;
	
	if (grids->currentIndex() == -1) grid_ = 0;
	else grid_ = sets[grids->currentIndex()];

	return QDialog::Accepted;
}

void ContourSurfaceDialog::chooseColor()
{
	VIEW::chooseColor(color_label);
}

ColorRGBA ContourSurfaceDialog::getColor()
{
	return VIEW::getColor(color_label);
}
		

}} //namespaces
