// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: colorMeshDialog.C,v 1.5 2003/09/19 18:17:57 amoll Exp $
//

#include <BALL/VIEW/DIALOGS/colorMeshDialog.h>
#include <BALL/VIEW/KERNEL/message.h>

#include <BALL/SYSTEM/path.h>
#include <BALL/SYSTEM/file.h>
#include <BALL/VIEW/DATATYPE/colorTable.h>
#include <BALL/DATATYPE/regularData3D.h>

#include <BALL/VIEW/KERNEL/mainControl.h>
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
	MainControl::getMainControl(this)->updateAll(true);
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
	if (surface_tab->currentPage() == by_grid)
	{
		if (grid_label->text() == "")
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
	autoscale->setEnabled(true);
	if (!mesh_->vertex.size()) return false;
	grid_ = &grid;
	grid_label->setText(name.c_str());

	min_value_  = Limits<float>::max();
	max_value_  = Limits<float>::min(); 
	mid_value_  = 0;

	try
	{
		for(Position p = 0; p < mesh_->vertex.size(); p++)
		{
			mesh_->vertex[p];
			float value = grid(mesh_->vertex[p]);
			mid_value_ += value;
			if (value < min_value_) min_value_ = value;
			if (value > max_value_) max_value_ = value;
		}
	}
	catch(Exception::OutOfGrid)
	{
		return false;
	}

	mid_value_ /= mesh_->vertex.size();

	apply_button->setEnabled(true);
	min_box->setText(String(min_value_).c_str());
	mid_box->setText(String(mid_value_).c_str());
	max_box->setText(String(max_value_).c_str());
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

	if (transparency_group_custom->selected() == (QButton*) none_button_custom)
	{
		rep_->clearProperty(Representation::PROPERTY__TRANSPARENT_FULL);
		rep_->clearProperty(Representation::PROPERTY__TRANSPARENT_BLENDING);
	}
	else if (transparency_group_custom->selected() == (QButton*) full_button_custom)
	{
		rep_->clearProperty(Representation::PROPERTY__TRANSPARENT_BLENDING);
		rep_->setProperty(Representation::PROPERTY__TRANSPARENT_FULL);
	}			
	else if (transparency_group_custom->selected() == (QButton*) alpha_button_custom)
	{
		rep_->clearProperty(Representation::PROPERTY__TRANSPARENT_FULL);
		rep_->setProperty(Representation::PROPERTY__TRANSPARENT_BLENDING);
	}
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
			mesh_->colorList[i] = table.map((*grid_)(mesh_->vertex[i]));
		}
	}	
	catch (Exception::OutOfGrid)
	{
		Log.error() << "Error! There is a point contained in the surface that is not "
								<< "inside the grid! Aborting the coloring..." << std::endl;
		return;
	}

	if (transparency_group_grid->selected() == (QButton*) none_button_grid)
	{
		rep_->clearProperty(Representation::PROPERTY__TRANSPARENT_FULL);
		rep_->clearProperty(Representation::PROPERTY__TRANSPARENT_BLENDING);
	}
	else if (transparency_group_grid->selected() == (QButton*) full_button_grid)
	{
		rep_->clearProperty(Representation::PROPERTY__TRANSPARENT_BLENDING);
		rep_->setProperty(Representation::PROPERTY__TRANSPARENT_FULL);
	}			
	else if (transparency_group_grid->selected() == (QButton*) alpha_button_grid)
	{
		rep_->clearProperty(Representation::PROPERTY__TRANSPARENT_FULL);
		rep_->setProperty(Representation::PROPERTY__TRANSPARENT_BLENDING);
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
		if (transparency_group_grid->selected() == (QButton*) none_button_grid)
		{
			config.transparency = 0;
		}
		else if (transparency_group_grid->selected() == (QButton*) full_button_grid)
		{
			config.transparency = 1;
		}			
		else
		{
			config.transparency = 2;
		}
	}
	else if (surface_tab->currentPage() == by_color)
	{
		config.tab = 1;

		if (transparency_group_custom->selected() == (QButton*) none_button_custom)
		{
			config.transparency = 0;
		}
		else if (transparency_group_custom->selected() == (QButton*) full_button_custom)
		{
			config.transparency = 1;
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

	if (grid_label->text() != "")
	{
		config.selected_grid = grid_label->text().ascii();
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
		else
		{
			full_button_grid->setEnabled(true);
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
		else
		{
			full_button_custom->setEnabled(true);
		}
	}
			
	QWidget::update();
}
	
void ColorMeshDialog::setRepresentation(Representation& rep)
	throw()
{
	if (!rep_ == 0) saveSettings_();
	rep_ = &rep;
	loadSettings_();
}

void ColorMeshDialog::onNotify(Message *message)
	throw()
{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "ColorMeshDialog " << this << " onNotify " << message << std::endl;
#endif
	if (!RTTI::isKindOf<RegularData3DMessage>(*message)) return;

	RegularData3DMessage *rm = RTTI::castTo<RegularData3DMessage>(*message);
	switch (rm->getType())
	{
		case RegularData3DMessage::SELECTED:
			if (!rm->getRegularData3D())
			{
				invalidateGrid_();
				return;
			}
			
			insertGrid_(*rm->getRegularData3D(), rm->getCompositeName());
			return;

		case RegularData3DMessage::REMOVE:
			if (rm->getRegularData3D() == grid_)
			{
				invalidateGrid_();
			}
			return;
	}
}

void ColorMeshDialog::invalidateGrid_()
	throw()
{
	grid_ = 0;
	grid_label->setText("");
	autoscale->setEnabled(false);
	if (surface_tab->currentPage() == by_grid)
	{
		apply_button->setEnabled(false);
	}
}

} } // namespaces
