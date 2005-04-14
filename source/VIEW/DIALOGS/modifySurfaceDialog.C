// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: modifySurfaceDialog.C,v 1.1.2.1 2005/04/14 13:02:29 amoll Exp $

#include <BALL/VIEW/DIALOGS/modifySurfaceDialog.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/DATATYPE/colorTable.h>
#include <BALL/VIEW/MODELS/standardColorProcessor.h>

#include <BALL/SYSTEM/path.h>
#include <BALL/SYSTEM/file.h>
#include <BALL/DATATYPE/regularData3D.h>
#include <BALL/COMMON/limits.h>
#include <BALL/KERNEL/system.h>
#include <BALL/STRUCTURE/geometricProperties.h>

#include <qlineedit.h>
#include <qspinbox.h>
#include <qtabwidget.h>
#include <qbuttongroup.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qradiobutton.h>
#include <qcombobox.h>
#include <qslider.h>

namespace BALL
{
	 namespace VIEW
	 {

ModifySurfaceDialog::ModifySurfaceDialog( QWidget* parent,  const char* name, bool modal, WFlags fl )
	: ModifySurfaceDialogData(parent, name, modal, fl),
		ModularWidget(name),
		grid_(0),
		rep_(0)
{
	registerWidget(this);
}

ModifySurfaceDialog::~ModifySurfaceDialog()
	throw()
{
}

// ------------------------- SLOTS ------------------------------------------------
void ModifySurfaceDialog::applyPressed() 
{
	if (surface_tab->currentPage() == by_grid)
	{
		if (!colorByGrid_()) return;
	}
	else if (surface_tab->currentPage() == by_color)
	{
		colorByCustomColor_();
	}
	else if (surface_tab->currentPage() == split)
	{
		split_();
		return;
	}
		
	rep_->setModelProcessor(0);
	rep_->setColorProcessor(0);
	if (rep_->isHidden())
	{
		rep_->setNeedsUpdate();
		return;
	}
	RepresentationMessage* msg = new RepresentationMessage(*rep_, RepresentationMessage::UPDATE);
	notify_(*msg);
}


void ModifySurfaceDialog::cancelPressed()
{
	hide();
}


void ModifySurfaceDialog::choosePressed()
{
	QColor qcolor = chooseColor(custom_color_label);
	selected_color.set(qcolor);
	red_box->setValue(qcolor.red());
	blue_box->setValue(qcolor.blue());
	green_box->setValue(qcolor.green());
}


void ModifySurfaceDialog::colorBoxesChanged()
{
	selected_color.set(red_box->value(),
										 green_box->value(),
										 blue_box->value(),
										 alpha_box->value());
	QColor qcolor(red_box->value(), green_box->value(), blue_box->value());
	custom_color_label->setBackgroundColor(qcolor);
}


void ModifySurfaceDialog::maxPressed()
{
	max_color.set(chooseColor(max_label));
}

void ModifySurfaceDialog::midPressed()
{
	mid_color.set(chooseColor(mid_label));
}

void ModifySurfaceDialog::minPressed()
{
	min_color.set(chooseColor(min_label));
}

void ModifySurfaceDialog::minMinPressed()
{
	min_min_color.set(chooseColor(min_min_label));
}

void ModifySurfaceDialog::maxMaxPressed()
{
	max_max_color.set(chooseColor(max_max_label));
}


void ModifySurfaceDialog::tabChanged()
{
	checkApplyButton_();
}


void ModifySurfaceDialog::autoScalePressed()
{
	min_box->setText(String(min_value_).c_str());
	mid_box->setText(String(mid_value_).c_str());
	max_box->setText(String(max_value_).c_str());
}


//--------------------- Helper functions ----------------------------------
bool ModifySurfaceDialog::insertGrid_(RegularData3D& grid, const String& name)
{
	grid_list_.push_back(&grid);
	grids->insertItem(name.c_str());
	if (grid_ == 0) grid_ = &grid;
	if (rep_ == 0) return false;

 	gridSelected();
	return true;
}

void ModifySurfaceDialog::removeGrid_(RegularData3D& grid)
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


void ModifySurfaceDialog::gridSelected()
{
	if (rep_ == 0) return;

	// prevent freezing, if clicking on representation, while
	// an other is still rendering
	if (getMainControl()->compositesAreLocked() || 
			getMainControl()->updateOfRepresentationRunning())
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
	calculateValues_();
}

void ModifySurfaceDialog::setGrid(RegularData3D* grid)
	throw()
{
	grid_ = grid;
	if (grid_ != 0 && rep_ != 0)
	{
		calculateValues_();
	}
}

void ModifySurfaceDialog::calculateValues_()
{
	if (grid_ == 0) return;

	min_value_  = Limits<float>::max();
	max_value_  = Limits<float>::min(); 
	mid_value_  = 0;

	try
	{
		for(Position p = 0; p < mesh_->vertex.size(); p++)
		{
			const float& value = grid_->getInterpolatedValue(mesh_->vertex[p]);

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


void ModifySurfaceDialog::setColor_(ColorRGBA& color, const QLabel* label, const QSpinBox* box, const QRadioButton* rbutton)
{
	color.set(label->backgroundColor());
	if (rbutton->isChecked())
	{
		color.setAlpha(box->value());
	}
	else
	{
		color.setAlpha(255);
	}
}

void ModifySurfaceDialog::getColor_(const ColorRGBA& color, QLabel* label, QSpinBox* box)
{
	label->setBackgroundColor(color.getQColor());
	box->setValue(color.getAlpha());
}

void ModifySurfaceDialog::colorByCustomColor_()
{
	ColorRGBA col(red_box->value(), green_box->value(), blue_box->value(), alpha_box->value());

	if (transparency_group_custom->selected() == none_button_custom)
	{
		col.setAlpha(255);
		rep_->setTransparency(0);
	}
	else if (transparency_group_custom->selected() == alpha_button_custom)
	{
		rep_->setTransparency(255 - (int) col.getAlpha());
	}

	mesh_->colorList.resize(1);
	mesh_->colorList[0] = col;
}


bool ModifySurfaceDialog::colorByGrid_()
{
	if (grid_ == 0 ||
			getMainControl()->compositesAreLocked() ||
			rep_ == 0 ||
			getMainControl()->updateOfRepresentationRunning())
	{
		setStatusbarText("Could not color surface, maybe because an other thread is still running?", true);
		return false;
	}

	try
	{
		String((mid_box->text().ascii())).toFloat();
		String((min_box->text().ascii())).toFloat();
		String((max_box->text().ascii())).toFloat();
	}
	catch(...)
	{
		setStatusbarText("Invalid value for min, mid or max value!", true);
		return false;
	}

	setColor_(min_min_color, min_min_label, min_min_alpha, alpha_button_grid);
	setColor_(min_color, min_label, min_alpha, alpha_button_grid);
	setColor_(mid_color, mid_label, mid_alpha, alpha_button_grid);
	setColor_(max_color, max_label, max_alpha, alpha_button_grid);
	setColor_(max_max_color, max_max_label, max_max_alpha, alpha_button_grid);

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
		const float mid_value = String(mid_box->text().ascii()).toFloat();
		for (Position i = 0; i < mesh_->colorList.size(); i++)
		{
			const float grid_value = grid_->getInterpolatedValue(mesh_->vertex[i]);
			if (grid_value <= mid_value)
			{
				mesh_->colorList[i].set(lower_table.map(grid_value));
			}
			else
			{
				mesh_->colorList[i].set(upper_table.map(grid_value));
			}
		}
	}	
	catch (Exception::OutOfGrid)
	{
		setStatusbarText("Error! There is a point contained in the surface that is not inside the grid!", true);
		return false;
	}

	if (transparency_group_grid->selected() == none_button_grid)
	{
		rep_->setTransparency(0);
	}
	else if (transparency_group_grid->selected() == alpha_button_grid)
	{
		rep_->setTransparency(255 - (int) min_min_color.getAlpha());
	}

	return true;
}


void ModifySurfaceDialog::saveSettings_()
{
	if (!configs_.has(rep_))
	{
		configs_[rep_] = ColoringConfig();
	}
	ColoringConfig& config = configs_[rep_];

	setColor_(config.min_min_color, min_min_label, min_min_alpha, alpha_button_grid);
	setColor_(config.min_color, min_label, min_alpha, alpha_button_grid);
	setColor_(config.mid_color, mid_label, mid_alpha, alpha_button_grid);
	setColor_(config.max_color, max_label, max_alpha, alpha_button_grid);
	setColor_(config.max_max_color, max_max_label, max_max_alpha, alpha_button_grid);

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


void ModifySurfaceDialog::loadSettings_()
{
	if (!configs_.has(rep_))
	{
		return;
	}
	ColoringConfig& config = configs_[rep_];

	getColor_(config.min_min_color, min_min_label, min_min_alpha);
	getColor_(config.min_color, min_label, min_alpha);
	getColor_(config.mid_color, mid_label, mid_alpha);
	getColor_(config.max_color, max_label, max_alpha);
	getColor_(config.max_max_color, max_max_label, max_max_alpha);

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

void ModifySurfaceDialog::checkApplyButton_()
{
	if (!rep_ || !mesh_)
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

	if (surface_tab->currentPage() == by_color ||
			surface_tab->currentPage() == split)
	{
		// if coloring by selected color, always enabled
		apply_button->setEnabled(true);
	}
}

void ModifySurfaceDialog::onNotify(Message *message)
	throw()
{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "ModifySurfaceDialog " << this << " onNotify " << message << std::endl;
#endif
	if (RTTI::isKindOf<RepresentationMessage>(*message))
	{
		RepresentationMessage *rm = RTTI::castTo<RepresentationMessage>(*message);
		Representation* rep = rm->getRepresentation();

		/// we are only interested into the Representation we currently have
		if (rep != rep_) return;

		if (rm->getType() == RepresentationMessage::REMOVE)
		{
			invalidateMesh_();
		}

		else if (rm->getType() == RepresentationMessage::UPDATE)
		{
			// if current Representation changed from Surface to something else, invalidate
			setRepresentation(rep);
		}

		return;
	}

	if (!RTTI::isKindOf<RegularData3DMessage>(*message)) return;

	RegularData3DMessage *rm = RTTI::castTo<RegularData3DMessage>(*message);
	switch (rm->getType())
	{
		case (CompositeMessage::CompositeMessageType) RegularData3DMessage::NEW:
			insertGrid_(*rm->getData(), rm->getCompositeName());
			return;

		case (CompositeMessage::CompositeMessageType) RegularData3DMessage::REMOVE:
			removeGrid_(*rm->getData());
			return;
		
		default:
			return;
	}
}

void ModifySurfaceDialog::invalidateGrid_()
	throw()
{
	grid_ = 0;
	grids->setCurrentItem(-1);
	checkApplyButton_();
}

void ModifySurfaceDialog::invalidateMesh_()
	throw()
{
	mesh_ = 0;
	rep_ = 0;
	checkApplyButton_();
}


void ModifySurfaceDialog::setRepresentation(Representation* rep)
	throw()
{
	if (rep == 0 ||
	    !isSurfaceModel(rep->getModelType()) ||
			rep->getGeometricObjects().size() == 0)
	{
		apply_button->setEnabled(false);
		return;
	}
	else
	{
		rep_ = rep;
		mesh_ = (Mesh*) *rep->getGeometricObjects().begin();
	}

	if (grids->currentItem() == -1 && 
			grids->count() != 0)
	{
		grids->setCurrentItem(grids->count()-1);
	}
 	gridSelected() ;

	checkApplyButton_();
}

void ModifySurfaceDialog::show()
{
	ModifySurfaceDialogData::show();
	raise();
}

void ModifySurfaceDialog::gridTransparencyChanged()
{
	min_min_alpha->setEnabled(alpha_button_grid->isChecked());
			min_alpha->setEnabled(alpha_button_grid->isChecked());
			mid_alpha->setEnabled(alpha_button_grid->isChecked());
	    max_alpha->setEnabled(alpha_button_grid->isChecked());
	max_max_alpha->setEnabled(alpha_button_grid->isChecked());
}

void ModifySurfaceDialog::customColorTransparencyChanged()
{
	alpha_box->setEnabled(alpha_button_custom->isChecked());
}

void ModifySurfaceDialog::setMinValue(float value)
{
	min_label->setText(String(value).c_str());
}

void ModifySurfaceDialog::setMaxValue(float value)
{
	max_label->setText(String(value).c_str());
}

void ModifySurfaceDialog::setMidValue(float value)
{
	mid_label->setText(String(value).c_str());
}

void ModifySurfaceDialog::splitDistanceChanged()
{
	split_distance_label->setText(String(((float)split_distance_slider->value()) / 10.0).c_str());
}

void ModifySurfaceDialog::splitMethodChanged()
{
	split_distance_slider->setEnabled(split_by_distance->isChecked());
}

void ModifySurfaceDialog::split_()
{
	// make sure we have a colorProcessor
	if (rep_->getColorProcessor() == 0) 
	{
		rep_->setColorProcessor(new CustomColorProcessor);
		rep_->getColorProcessor()->createAtomGrid();
	}
	ColorProcessor* cp = rep_->getColorProcessor();

	rep_->setModelProcessor(0);

	// create a new representation with the subset of the original mesh
	Representation* new_rep = new Representation;
	new_rep->getComposites() = rep_->getComposites();
	new_rep->setColorProcessor(new ColorProcessor(*rep_->getColorProcessor()));
	new_rep->setModelType(rep_->getModelType());
	new_rep->setColoringMethod(rep_->getColoringMethod());
	new_rep->setModelProcessor(0);

	Representation::GeometricObjectList::iterator git = rep_->getGeometricObjects().begin();
	for (; git != rep_->getGeometricObjects().end(); ++git)
	{
		// get the original mesh
		if (!RTTI::isKindOf<Mesh>(**git)) continue;
		Mesh& org_mesh = *dynamic_cast<Mesh*>(*git);

		// copy all vertices and colors into a new mesh
		Mesh* new_mesh = new Mesh;
		new_mesh->vertex = org_mesh.vertex;
		new_mesh->colorList = org_mesh.colorList;
		new_mesh->normal = org_mesh.normal;
		new_mesh->setComposite(org_mesh.getComposite());

		// collect informations which vertices are to be included
		vector<bool> include_vertex;
		include_vertex.resize(org_mesh.vertex.size());
		
		for (Position p = 0; p < org_mesh.vertex.size(); p++)
		{
			// make sure we found an atom
			const Atom* atom = cp->getClosestItem(org_mesh.vertex[p]);

			include_vertex[p] = (atom != 0 && atom->isSelected());
		}

		vector<Surface::Triangle> tri_temp = org_mesh.triangle;
		org_mesh.triangle.clear();
		
		vector<Surface::Triangle>::iterator tit = tri_temp.begin();
		for (; tit != tri_temp.end(); tit++) 
		{
			Surface::Triangle& tri = *tit;
			if (include_vertex[tri.v1] &&
					include_vertex[tri.v2] &&
					include_vertex[tri.v3])
			{
				new_mesh->triangle.push_back(tri);
			}
			else
			{
				org_mesh.triangle.push_back(tri);
			}
		}

		if (new_mesh->triangle.size() == 0)
		{
			delete new_mesh;
		}
		else
		{
			new_rep->getGeometricObjects().push_back(new_mesh);
		}
	}

	if (new_rep->getGeometricObjects().size() == 0) 
	{
		delete new_rep;
		return;
	}

	getMainControl()->insert(*new_rep);
	getMainControl()->update(*rep_);
}



} } // namespaces
