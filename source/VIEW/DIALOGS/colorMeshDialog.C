// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: colorMeshDialog.C,v 1.23.2.3 2004/05/14 12:27:29 amoll Exp $
//

#include <BALL/VIEW/DIALOGS/colorMeshDialog.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/DATATYPE/colorTable.h>

#include <BALL/SYSTEM/path.h>
#include <BALL/SYSTEM/file.h>
#include <BALL/DATATYPE/regularData3D.h>
#include <BALL/COMMON/limits.h>
#include <BALL/KERNEL/system.h>
#include <BALL/STRUCTURE/geometricProperties.h>

#include <qlineedit.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qcolordialog.h>
#include <qtabwidget.h>
#include <qbuttongroup.h>
#include <qlabel.h>
#include <qradiobutton.h>
#include <qcombobox.h>

namespace BALL
{
	 namespace VIEW
	 {

ColorMeshDialog::ColorMeshDialog( QWidget* parent,  const char* name, bool modal, WFlags fl )
	: ColorMeshDialogData(parent, name, modal, fl),
		ModularWidget(name),
		grid_(0),
		mesh_(0),
		rep_(0)
{
	registerWidget(this);
}

ColorMeshDialog::~ColorMeshDialog()
	throw()
{
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
	MainControl::getMainControl(this)->redrawAllRepresentations(true);
}


void ColorMeshDialog::cancelPressed()
{
	hide();
}


void ColorMeshDialog::choosePressed()
{
	QColor qcolor = setColor(choose_button);
	selected_color.set(qcolor);
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
	max_color.set(setColor(max_button));
}

void ColorMeshDialog::midPressed()
{
	mid_color.set(setColor(mid_button));
}

void ColorMeshDialog::minPressed()
{
	min_color.set(setColor(min_button));
}

void ColorMeshDialog::minMinPressed()
{
	min_min_color.set(setColor(min_min_button));
}

void ColorMeshDialog::maxMaxPressed()
{
	max_max_color.set(setColor(max_max_button));
}


void ColorMeshDialog::tabChanged()
{
	if (!mesh_ || !rep_)
	{
		autoscale->setEnabled(false);
		apply_button->setEnabled(false);
		return;
	}

	if (surface_tab->currentPage() == by_grid)
	{
		if (!grid_)
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


void ColorMeshDialog::autoScalePressed()
{
	min_box->setText(String(min_value_).c_str());
	mid_box->setText(String(mid_value_).c_str());
	max_box->setText(String(max_value_).c_str());
}


//--------------------- Helper functions ----------------------------------
bool ColorMeshDialog::insertGrid_(RegularData3D& grid, const String& name)
{
	grid_list_.push_back(&grid);
	grids->insertItem(name.c_str());
	if (grid_ == 0) grid_ = &grid;
	if (mesh_ == 0 || !mesh_->vertex.size()) return false;

 	gridSelected();
	return true;
}

void ColorMeshDialog::removeGrid_(RegularData3D& grid)
{
	list<RegularData3D*>::iterator it = grid_list_.begin();
	Position pos = 0;
	for (; it != grid_list_.end(); it++)
	{
		if (*it == &grid)
		{
			if ((Index)pos == grids->currentItem()) 
			{
				grids->setCurrentItem(-1);
				invalidateGrid_();
			}
			grid_list_.erase(it);
			grids->removeItem(pos);
			return;
		}
		pos++;
	}
}


void ColorMeshDialog::gridSelected()
{
	if (mesh_ == 0 || rep_ == 0) return;

	// prevent freezing, if clicking on representation, while
	// an other is still rendering
	if (!getMainControl()->compositesAreMuteable() ||
	     rep_->updateRunning())
	{
		return;
	}

	if (grids->count() == 0 || grids->currentItem() == -1)
	{
		invalidateGrid_();
		return;
	}

	Index pos = grids->currentItem();
	list<RegularData3D*>::iterator it = grid_list_.begin();
	for (Position p = 0; p < (Position)pos; p++)
	{
		it++;
	}
	
	grid_ = *it;

	if (grid_ == 0) return;

	min_value_  = Limits<float>::max();
	max_value_  = Limits<float>::min(); 
	mid_value_  = 0;

	try
	{
		for(Position p = 0; p < mesh_->vertex.size(); p++)
		{
			float value = grid_->getInterpolatedValue(mesh_->vertex[p]);

			mid_value_ += value;
			if (value < min_value_) min_value_ = value;
			if (value > max_value_) max_value_ = value;
		}
	}
	catch(Exception::OutOfGrid)
	{
		setStatusbarText("Can not color this surface with this grid, the mesh has points outside the grid!");
		return;
	}

	mid_value_ /= mesh_->vertex.size();

	apply_button->setEnabled(true);
	autoscale->setEnabled(true);
	min_box->setText(String(min_value_).c_str());
	mid_box->setText(String(mid_value_).c_str());
	max_box->setText(String(max_value_).c_str());
}


void ColorMeshDialog::setColor_(ColorRGBA& color, const QPushButton* button, const QSpinBox* box)
{
	QPalette p = button->palette();
	color.set(p.color(QPalette::Active, QColorGroup::Button));
	color.setAlpha(box->value());
}

void ColorMeshDialog::getColor_(const ColorRGBA& color, QPushButton* button, QSpinBox* box)
{
	QPalette p = button->palette();
	p.setColor(QColorGroup::Button, color.getQColor());
	button->setPalette(p);
	box->setValue(color.getAlpha());
	button->update();
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

void ColorMeshDialog::colorByCustomColor_()
{
	ColorRGBA col(red_box->value(), green_box->value(), blue_box->value(), alpha_box->value());
	mesh_->colorList.resize(1);
	mesh_->colorList[0] = col;

	if (transparency_group_custom->selected() == none_button_custom)
	{
		rep_->setTransparency(0);
	}
	else if (transparency_group_custom->selected() == alpha_button_custom)
	{
		rep_->setTransparency(min_min_color.getAlpha());
	}
}


void ColorMeshDialog::colorByGrid_()
{
	if (grid_ == 0 ||
			mesh_ == 0 ||
			!getMainControl()->compositesAreMuteable() ||
			rep_ == 0 ||
			rep_->updateRunning())
	{
		setStatusbarText("Could not color surface, maybe because an other thread is still running?");
		return;
	}

	try
	{
		String((mid_box->text().ascii())).toFloat();
		String((min_box->text().ascii())).toFloat();
		String((max_box->text().ascii())).toFloat();
	}
	catch(...)
	{
		setStatusbarText("Invalid value for min, mid or max value!");
	}

	setColor_(min_min_color, min_min_button, min_min_alpha);
	setColor_(min_color, min_button, min_alpha);
	setColor_(mid_color, mid_button, mid_alpha);
	setColor_(max_color, max_button, max_alpha);
	setColor_(max_max_color, max_max_button, max_max_alpha);

	// now do the colorizing stuff...
	mesh_->colorList.resize(mesh_->vertex.size());
 	ColorRGBA list[2];

 	list[0] = min_color;
 	list[1] = mid_color;

	ColorTable lower_table(list, 2);
	lower_table.setMinMaxColors(min_min_color, max_max_color);
	lower_table.setAlphaBlending(true);
	lower_table.setNumberOfColors(levels_box->value()/2);
	lower_table.setRange(String((min_box->text().ascii())).toFloat(), String((mid_box->text().ascii())).toFloat());
	lower_table.createTable();

 	list[0] = mid_color;
 	list[1] = max_color;

	ColorTable upper_table(list, 2);
	upper_table.setMinMaxColors(min_min_color, max_max_color);
	upper_table.setAlphaBlending(true);
	upper_table.setNumberOfColors(levels_box->value()/2);
	upper_table.setRange(String((mid_box->text().ascii())).toFloat(), String((max_box->text().ascii())).toFloat());
	upper_table.createTable();

	try 
	{
		for (Position i=0; i<mesh_->colorList.size(); i++)
		{
			float grid_value = grid_->getInterpolatedValue(mesh_->vertex[i]);
			if (grid_value <= String(mid_box->text().ascii()).toFloat())
			{
				mesh_->colorList[i] = lower_table.map(grid_value);
			}
			else
			{
				mesh_->colorList[i] = upper_table.map(grid_value);
			}
		}
	}	
	catch (Exception::OutOfGrid)
	{
		Log.error() << "Error! There is a point contained in the surface that is not "
								<< "inside the grid! Aborting the coloring..." << std::endl;
		setStatusbarText("Aborted calculation because a point of the surface is out of the grid!");
		return;
	}

	if (transparency_group_grid->selected() == none_button_grid)
	{
		rep_->setTransparency(0);
	}
	else if (transparency_group_grid->selected() == alpha_button_grid)
	{
		rep_->setTransparency(min_min_color.getAlpha());
	}
}


void ColorMeshDialog::saveSettings_()
{
	if (!configs_.has(rep_))
	{
		configs_[rep_] = ColoringConfig();
	}
	ColoringConfig& config = configs_[rep_];

	setColor_(config.min_min_color, min_min_button, min_min_alpha);
	setColor_(config.min_color, min_button, min_alpha);
	setColor_(config.mid_color, mid_button, mid_alpha);
	setColor_(config.max_color, max_button, max_alpha);
	setColor_(config.max_max_color, max_max_button, max_max_alpha);

	config.min_value = String(min_box->text().ascii()).toFloat();
	config.mid_value = String(mid_box->text().ascii()).toFloat();
	config.max_value = String(max_box->text().ascii()).toFloat();

	config.number_of_levels = levels_box->value();

	config.transparency = 0;
	if (surface_tab->currentPage() == by_grid)
	{
		config.tab = 0;
		if (transparency_group_grid->selected() == none_button_grid)
		{
			config.transparency = 0;
		}
		else
		{
			config.transparency = 2;
		}
	}
	else if (surface_tab->currentPage() == by_color)
	{
		config.tab = 1;

		if (transparency_group_custom->selected() == none_button_custom)
		{
			config.transparency = 0;
		}
		else
		{
			config.transparency = 2;
		}
	}
	else
	{
		config.tab = 2;
	}
}


void ColorMeshDialog::loadSettings_()
{
	if (!configs_.has(rep_))
	{
		return;
	}
	ColoringConfig& config = configs_[rep_];

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

	if (config.tab == 0)
	{
		if (config.transparency == 0)
		{
			none_button_grid->setEnabled(true);
		}
		else if (config.transparency == 1)
		{
			alpha_button_grid->setEnabled(true);
		}
	}
	else if (config.tab == 1)
	{
		if (config.transparency == 0)
		{
			none_button_custom->setEnabled(true);
		}
		else if (config.transparency == 1)
		{
			alpha_button_custom->setEnabled(true);
		}
	}
			
	QWidget::update();
}

void ColorMeshDialog::onNotify(Message *message)
	throw()
{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "ColorMeshDialog " << this << " onNotify " << message << std::endl;
#endif
	if (RTTI::isKindOf<RepresentationMessage>(*message))
	{
		RepresentationMessage *rm = RTTI::castTo<RepresentationMessage>(*message);
		Representation* rep = rm->getRepresentation();
		if (rep != rep_) return;

		if (rm->getType() == RepresentationMessage::REMOVE)
		{
			invalidateMesh_();
			return;
		}

		if (rm->getType() == RepresentationMessage::UPDATE)
		{
			// if current Representation changed from Surface to something else, invalidate
			if (!isSurfaceModel(rep->getModelType()) ||
					rep->getGeometricObjects().size() == 0)
			{
				invalidateMesh_();
				return;
			}

			// new Surface in Representation
			mesh_ = (Mesh*) *rep->getGeometricObjects().begin();
		}
		return;
	}

	if (!RTTI::isKindOf<RegularData3DMessage>(*message)) return;

	RegularData3DMessage *rm = RTTI::castTo<RegularData3DMessage>(*message);
	switch (rm->getType())
	{
		case RegularData3DMessage::NEW:
			insertGrid_(*rm->getRegularData3D(), rm->getCompositeName());
			return;

		case RegularData3DMessage::REMOVE:
			removeGrid_(*rm->getRegularData3D());
			return;
		
		default:
			return;
	}
}

void ColorMeshDialog::invalidateGrid_()
	throw()
{
	grid_ = 0;
	grids->setCurrentItem(-1);
	autoscale->setEnabled(false);
	if (surface_tab->currentPage() == by_grid)
	{
		apply_button->setEnabled(false);
	}
}

void ColorMeshDialog::invalidateMesh_()
	throw()
{
	mesh_ = 0;
	rep_ = 0;
	apply_button->setEnabled(false);
	autoscale->setEnabled(false);
}


void ColorMeshDialog::setMesh(Mesh* mesh, Representation* rep)
	throw()
{
	mesh_ = mesh;
	rep_ = rep;
	if (mesh == 0 || rep == 0)
	{
		apply_button->setEnabled(false);
		return;
	}

	if (grids->currentItem() == -1 && 
			grids->count() != 0)
	{
		grids->setCurrentItem(grids->count()-1);
	}
 	gridSelected() ;
 	apply_button->setEnabled(grid_ != 0);
}

void ColorMeshDialog::show()
{
	ColorMeshDialogData::show();
	raise();
}


} } // namespaces
