// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include <BALL/VIEW/DIALOGS/contourSurfaceDialog.h>
#include <BALL/VIEW/WIDGETS/datasetControl.h>

#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qcombobox.h>

namespace BALL
{
 namespace VIEW
 {

ContourSurfaceDialog::ContourSurfaceDialog( QWidget* parent,  const char* name )
    : ContourSurfaceDialogData( parent, name, TRUE, 0 )
{
}

ContourSurfaceDialog::~ContourSurfaceDialog()
{
}

double ContourSurfaceDialog::getThreshold() const	
{
	if (threshold->text().isEmpty()) return DBL_MAX;
	try
	{
		return (double)String(threshold->text().ascii()).toFloat();
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
	buttonOk->setEnabled((grids->currentItem() != -1) && 
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
			grids->insertItem((*it).second.c_str());
		}
	}

	valuesChanged();
	if (!ContourSurfaceDialogData::exec()) return false;
	
	if (grids->currentItem() != -1)
	{
		it = grid_pair_list.begin();
		for (Index i = 0; i < grids->currentItem(); i++)
		{
			it++;
		}
		grid_ = (*it).first;
	}

	return true;
}
		

}} //namespaces
