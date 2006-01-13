// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include <BALL/VIEW/DIALOGS/contourSurfaceDialog.h>
#include <BALL/VIEW/WIDGETS/datasetControl.h>
#include <BALL/VIEW/KERNEL/mainControl.h>

#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlayout.h>

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

RegularData3D* ContourSurfaceDialog::getGrid() 
{
	return grid_;
}

void ContourSurfaceDialog::valuesChanged()
{
	buttonOk->setEnabled((grids->currentIndex() != -1) && 
												!grids->currentText().isEmpty() &&
												(getThreshold() != DBL_MAX));
}

bool ContourSurfaceDialog::exec()
{
	grids->clear();
	List<std::pair<RegularData3D*, String> > grid_pair_list;
	List<std::pair<RegularData3D*, String> >::Iterator it;
	if (control_ != 0) 
	{
		grid_pair_list = control_->get3DGrids();
		it = grid_pair_list.begin(); 
		for (; it != grid_pair_list.end(); it++)
		{
			grids->addItem((*it).second.c_str());
		}
	}

	valuesChanged();
	if (!QDialog::exec()) return false;
	
	if (grids->currentIndex() != -1)
	{
		it = grid_pair_list.begin();
		for (Index i = 0; i < grids->currentIndex(); i++)
		{
			it++;
		}
		grid_ = (*it).first;
	}

	return true;
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
