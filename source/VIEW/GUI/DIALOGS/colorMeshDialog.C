// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: colorMeshDialog.C,v 1.18 2003/06/30 19:14:44 oliver Exp $
//

#include <BALL/VIEW/GUI/DIALOGS/colorMeshDialog.h>
#include <BALL/VIEW/GUI/DIALOGS/FDPBDialog.h>
#include <BALL/VIEW/GUI/WIDGETS/scene.h>
#include <BALL/MOLVIEW/FUNCTOR/molecularInformation.h>

#include <BALL/SYSTEM/path.h>
#include <BALL/SYSTEM/file.h>
#include <BALL/VIEW/DATATYPE/colorTable.h>
#include <BALL/DATATYPE/regularData3D.h>

#include <BALL/VIEW/GUI/KERNEL/mainControl.h>
#include <BALL/COMMON/limits.h>

#include <BALL/KERNEL/system.h>
#include <BALL/STRUCTURE/geometricProperties.h>

#include <qlineedit.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qfiledialog.h>
#include <qcolordialog.h>
#include <qtabwidget.h>
#include <qbuttongroup.h>
#include <qlistbox.h>
#include <qcombobox.h>
#include <qradiobutton.h>
#include <vector>


namespace BALL
{
	 namespace VIEW
	 {
			using BALL::MOLVIEW::MolecularInformation;

ColorMeshDialog::ColorMeshDialog( QWidget* parent,  const char* name, bool modal, WFlags fl )
	throw()
	: ColorMeshDialogData(parent, name, modal, fl),
		ModularWidget("ColorMeshDialog"),
		mesh_(0)
{
	compute_method->insertItem("from distance");

	if (FDPBDialog::getInstance(0) != 0)
	{
		compute_method->insertItem("from Finite Difference Poisson Boltzmann");
	}

	registerWidget(this);
}

ColorMeshDialog::~ColorMeshDialog()
	throw()
{
	// no need to delete child widgets, Qt does it all for us
	std::vector<BALL::RegularData3D*>::iterator it(grids_.begin());
	for (; it != grids_.end(); ++it)
	{
		delete *it;
	}
}

// ------------------------- SLOTS ------------------------------------------------
// --------------------------------------------------------------------------------
void ColorMeshDialog::applyPressed() 
{
	if (surface_tab->currentPage() == by_grid)
	{
		colorByGrid_();
	}
	else if (surface_tab->currentPage() == by_color)
	{
		colorByCustomColor_();
	}
		
	// repaint of the scene and the composites needed
	for (Position p = 0; ; p++)
	{
		Scene* scene= (Scene*) Scene::getInstance(p);
		if (scene == 0) return;
		scene->update(true);
	}
}


void ColorMeshDialog::loadPressed()
{
	QString result = QFileDialog::getOpenFileName("", "*", 0, "Select a RegularData file");
	if (!result.isEmpty())
	{
		loadGrid_(result.ascii());
	}
}


void ColorMeshDialog::cancelPressed()
{
	hide();
}


void ColorMeshDialog::choosePressed()
{
	QColor qcolor = setColor(choose_button);
	selected_color.set(qcolor.red(), qcolor.green(), qcolor.blue());
	red_box->setValue(qcolor.red());
	blue_box->setValue(qcolor.blue());
	green_box->setValue(qcolor.green());
}


void ColorMeshDialog::colorBoxesChanged()
{
	selected_color.set(red_box->value(),
										 green_box->value(),
										 blue_box->value(),
										 alpha_box->value());
	QColor qcolor(red_box->value(), green_box->value(), blue_box->value());
	QPalette p = choose_button->palette();
	p.setColor(QColorGroup::Button, qcolor);
	p.setColor(QColorGroup::Base, qcolor);
	p.setColor(QColorGroup::Light, qcolor);
	p.setColor(QColorGroup::Mid, qcolor);
	p.setColor(QColorGroup::Midlight, qcolor);
	p.setColor(QColorGroup::Shadow, qcolor);
	choose_button->setPalette(p);
}


void ColorMeshDialog::maxPressed()
{
	QColor qcolor = setColor(max_button);
	max_color.set(qcolor.red(), qcolor.green(), qcolor.blue());;
}

void ColorMeshDialog::midPressed()
{
	QColor qcolor = setColor(mid_button);
	mid_color.set(qcolor.red(), qcolor.green(), qcolor.blue());;
}

void ColorMeshDialog::minPressed()
{
	QColor qcolor = setColor(min_button);
	min_color.set(qcolor.red(), qcolor.green(), qcolor.blue());;
}

void ColorMeshDialog::minMinPressed()
{
	QColor qcolor = setColor(min_min_button);
	min_min_color.set(qcolor.red(), qcolor.green(), qcolor.blue());;
}

void ColorMeshDialog::maxMaxPressed()
{
	QColor qcolor = setColor(max_max_button);
	max_max_color.set(qcolor.red(), qcolor.green(), qcolor.blue());;
}


void ColorMeshDialog::tabChanged()
{
	if (surface_tab->currentPage() == grids)
	{
		apply_button->setEnabled(false);
		return;
	}

	if (surface_tab->currentPage() == by_grid)
	{
		if (grid_combobox->currentText() == "")
		{
			apply_button->setEnabled(false);
		}
		else
		{
			apply_button->setEnabled(true);
			autoscale->setEnabled(true);
		}
	
		return;
	}

	if (surface_tab->currentPage() == by_color)
	{
		// if coloring by selected color, always enabled
		apply_button->setEnabled(true);
	}
}


void ColorMeshDialog::computePressed()
{
	String name;
	RegularData3D* dat = 0;
	switch(compute_method->currentItem())
	{
		case GRID_FROM_DISTANCE:
			name = "distance";
			dat = createGridFromDistance_();
			break;

		case GRID_FROM_FDPD:
			name = "electrostatic potential";
			dat = createGridFromFPDB_();
			break;

		default:
			Log.error() << "Unknown coloring method in ColorMeshDialog." << std::endl;
			return;
	}

	if (dat == 0)
	{
		Log.error() << "RegularData3D was incorrect, aborting..." << std::endl;
		return;
	}

	// get information about the composite
	MolecularInformation 	information;
	name += " of " + information.getName();

	if (!insertGrid_(*dat, name))
	{
		Log.error() << "RegularData3D was incorrect, aborting..." << std::endl;
		delete dat;
		return;
	}
}


void ColorMeshDialog::deletePressed()
{
	Position p = grid_list->currentItem();
	grid_combobox->removeItem(p);
	grid_list->removeItem(p);
	if (grid_list->currentItem() == -1)
	{
		delete_button->setEnabled(false);
		save_button->setEnabled(false);
	}

	if (grid_combobox->count() == 0)
	{
		grid_combobox->setEnabled(false);
	}

	vector<RegularData3D*>::iterator it = grids_.begin();
	for (Position x = 0; x < p; x++) it++;
	grids_.erase(it);
}


void ColorMeshDialog::gridChoosen()
{
	apply_button->setEnabled(true);
	min_box->setText(String(min_values_[grid_combobox->currentItem()]).c_str());
	mid_box->setText(String(mid_values_[grid_combobox->currentItem()]).c_str());
	max_box->setText(String(max_values_[grid_combobox->currentItem()]).c_str());
}


void ColorMeshDialog::gridSelected()
{
	save_button->setEnabled(true);
	delete_button->setEnabled(true);
}


void ColorMeshDialog::savePressed()
{
	QString result = QFileDialog::getSaveFileName("", "*", 0, "Select a RegularData file");
	if (!result.isEmpty())
	{
		saveGrid_(result.ascii());
	}
}


void ColorMeshDialog::autoScalePressed()
{
	Position p = grid_combobox->currentItem();
	min_box->setText(String(min_values_[p]).c_str());
	mid_box->setText(String(mid_values_[p]).c_str());
	max_box->setText(String(max_values_[p]).c_str());
}


//--------------------- Helper functions ----------------------------------
//-------------------------------------------------------------------------
bool ColorMeshDialog::insertGrid_(RegularData3D& grid, const String& name)
{
	if (mesh_->vertex.size() == 0) return false;

	float min_value  = Limits<float>::max();
	float max_value  = Limits<float>::min(); 
	float mean_value = 0;

	try
	{
		for(Position p = 0; p < mesh_->vertex.size(); p++)
		{
			mesh_->vertex[p];
			float value = grid(mesh_->vertex[p]);
			mean_value += value;
			if (value < min_value) min_value = value;
			if (value > max_value) max_value = value;
		}
	}
	catch(Exception::OutOfGrid)
	{
		return false;
	}

	mean_value /= mesh_->vertex.size();
	mid_values_.push_back(mean_value);
	min_values_.push_back(min_value);
	max_values_.push_back(max_value);
	grids_.push_back(&grid);
	grid_combobox->setEnabled(true);	
	grid_list->insertItem(name.c_str());
	grid_combobox->insertItem(name.c_str());
	return true;
}


void ColorMeshDialog::setColor_(ColorRGBA& color, const QPushButton* button, const QSpinBox* box)
{
	QPalette p = button->palette();
	QColor qcolor = p.color(QPalette::Active, QColorGroup::Button);
	color.set(qcolor.red(), 
						qcolor.green(), 
						qcolor.blue(),
						box->value());
}

void ColorMeshDialog::getColor_(const ColorRGBA& color, QPushButton* button, QSpinBox* box)
{
	QColor qcolor(color.getRed(), color.getGreen(), color.getBlue());
	QPalette p = button->palette();
	p.setColor(QColorGroup::Button, qcolor);
	button->setPalette(p);
	box->setValue(color.getAlpha());
	button->update();
}



RegularData3D* ColorMeshDialog::createGridFromDistance_()
{
	System& S = *(System*) &composite_->getRoot();
	
	Vector3 v(0,0,0);
	AtomIterator atit = S.beginAtom();

	for (; +atit; ++atit)
	{
		v+=atit->getPosition();
	}

	v /= S.countAtoms();

	BoundingBoxProcessor bs;

	S.apply(bs);

	RegularData3D* regdat = new RegularData3D(RegularData3D::IndexType(50), 
																						bs.getLower()-Vector3(2,2,2), 
																						bs.getUpper()-bs.getLower()+Vector3(4,4,4));

	for (Size i=0; i < regdat->size(); i++)
	{
		float distance = (regdat->getCoordinates(i) - v).getLength();
		(*regdat)[i] = distance;
	}

	return regdat;
}


QColor ColorMeshDialog::setColor(QPushButton* button)
{
	QPalette p = button->palette();
	QColor qcolor = QColorDialog::getColor(button->backgroundColor());
	p.setColor(QColorGroup::Button, qcolor);
	p.setColor(QColorGroup::Base, qcolor);
	p.setColor(QColorGroup::Light, qcolor);
	p.setColor(QColorGroup::Mid, qcolor);
	p.setColor(QColorGroup::Midlight, qcolor);
	p.setColor(QColorGroup::Shadow, qcolor);
	button->setPalette(p);
	return qcolor;
}


void ColorMeshDialog::loadGrid_(const String& filename)
{
	// coloring by file
	RegularData3D* dat = new RegularData3D;
	File infile;
	
	try
	{
		infile.open(filename, std::ios::in);
	}
	catch(Exception::FileNotFound)
	{
		Log.error() << "File could not be found!" << std::endl;
		return;
	}

	infile >> *dat;
	infile.close();

	if (!insertGrid_(*dat, filename))
	{
		Log.error() << "RegularData3D file was invalid, aborting..." << std::endl;
		delete dat;
		return;
	}
}


void ColorMeshDialog::saveGrid_(const String& filename)
{
	File outfile;
	
	try
	{
		outfile.open(filename, std::ios::out);
	}
	catch(Exception::GeneralException)
	{
		Log.error() << "File could not be written!" << std::endl;
		return;
	}

	outfile << *grids_[grid_list->currentItem()];
	outfile.close();
}


void ColorMeshDialog::colorByCustomColor_()
{
	ColorRGBA col(red_box->value(), green_box->value(), blue_box->value(), alpha_box->value());
	mesh_->colorList.resize(1);
	mesh_->colorList[0] = col;
}


void ColorMeshDialog::colorByGrid_()
{
	setColor_(min_min_color, min_min_button, min_min_alpha);
	setColor_(min_color, min_button, min_alpha);
	setColor_(mid_color, mid_button, mid_alpha);
	setColor_(max_color, max_button, max_alpha);
	setColor_(max_max_color, max_max_button, max_max_alpha);

	// now do the colorizing stuff...
	mesh_->colorList.resize(mesh_->vertex.size());
	ColorRGBA list[3];

	list[0] = min_color;
	list[1] = mid_color;
	list[2] = max_color;

	ColorTable table(list, 3);
	table.setMinMaxColors(min_min_color, max_max_color);
	table.setAlphaBlending(true);
	table.setNumberOfColors(levels_box->value());
	table.setRange(String((min_box->text().ascii())).toFloat(), String((max_box->text().ascii())).toFloat());
	table.createTable();
	try 
	{
		for (Position i=0; i<mesh_->colorList.size(); i++)
		{
			mesh_->colorList[i] = table.map((*grids_[grid_combobox->currentItem()])(mesh_->vertex[i]));
		}
	}	
	catch (Exception::OutOfGrid)
	{
		Log.error() << "Error! There is a point contained in the surface that is not "
								<< "inside the grid! Aborting the coloring..." << std::endl;
		return;
	}
}


RegularData3D* ColorMeshDialog::createGridFromFPDB_()
{
	System& system = *(System*) &composite_->getRoot();
	FDPBDialog* dialog = (FDPBDialog*) FDPBDialog::getInstance(0);
	if (dialog == 0)
	{
		Log.error() << "No FDPBDialog instance exists. Aborting..." << std::endl;
		return 0;
	}
	dialog->setSystem(&system);
	dialog->exec();

	FDPB& solver = dialog->getFDPBSolver();
	if (solver.phi_grid == 0) return 0;
	RegularData3D* data = new RegularData3D(*solver.phi_grid);
	return data;
}

void ColorMeshDialog::saveSettings_()
{
	if (!configs_.has(mesh_))
	{
		configs_[mesh_] = ColoringConfig();
	}
	ColoringConfig& config = configs_[mesh_];

	setColor_(config.min_min_color, min_min_button, min_min_alpha);
	setColor_(config.min_color, min_button, min_alpha);
	setColor_(config.mid_color, mid_button, mid_alpha);
	setColor_(config.max_color, max_button, max_alpha);
	setColor_(config.max_max_color, max_max_button, max_max_alpha);

	config.min_value = String(min_box->text().ascii()).toFloat();
	config.mid_value = String(mid_box->text().ascii()).toFloat();
	config.max_value = String(max_box->text().ascii()).toFloat();

	config.number_of_levels = levels_box->value();
}


void ColorMeshDialog::loadSettings_()
{
	if (!configs_.has(mesh_))
	{
		return;
	}
	ColoringConfig& config = configs_[mesh_];

	getColor_(config.min_min_color, min_min_button, min_min_alpha);
	getColor_(config.min_color, min_button, min_alpha);
	getColor_(config.mid_color, mid_button, mid_alpha);
	getColor_(config.max_color, max_button, max_alpha);
	getColor_(config.max_max_color, max_max_button, max_max_alpha);

	min_box->setText(String(config.min_value).c_str());
	mid_box->setText(String(config.mid_value).c_str());
	max_box->setText(String(config.max_value).c_str());

	levels_box->setValue(config.number_of_levels);


	surface_tab->setCurrentPage(config.tab);

	for (Position p = 0; p < (Position)grid_combobox->count(); p++)
	{
		if (grid_combobox->text(p) != "" &&
				grid_combobox->text(p).ascii() == config.selected_grid)
		{
			grid_combobox->setCurrentItem(p);
			break;
		}
	}

	QWidget::update();
}
	
void ColorMeshDialog::setMesh(Mesh& mesh)
	throw()
{
	if (!mesh_ == 0) saveSettings_();
	mesh_ = &mesh;
	loadSettings_();
}

}  // namespace VIEW
} // namespace BALL