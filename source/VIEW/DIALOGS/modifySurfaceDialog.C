// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: modifySurfaceDialog.C,v 1.1.2.31 2005/11/16 13:39:25 amoll Exp $
//

#include <BALL/VIEW/DIALOGS/modifySurfaceDialog.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/DATATYPE/colorMap.h>
#include <BALL/VIEW/MODELS/standardColorProcessor.h>

#include <BALL/SYSTEM/path.h>
#include <BALL/SYSTEM/file.h>
#include <BALL/DATATYPE/regularData3D.h>
#include <BALL/DATATYPE/hashGrid.h>
#include <BALL/COMMON/limits.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/STRUCTURE/geometricProperties.h>
#include <BALL/SYSTEM/sysinfo.h>

#include <qlineedit.h>
#include <qspinbox.h>
#include <qtabwidget.h>
#include <qbuttongroup.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qradiobutton.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qslider.h>
#include <qmessagebox.h>

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
			else if (surface_tab->currentPage() == drawing_mode)
			{
				changeDrawingMode_();
			}
			else if (surface_tab->currentPage() == split)
			{
				if (split_by_distance->isChecked())
				{
					try
					{
						String(distance_edit->text().ascii()).toFloat();
					}
					catch(...)
					{
						QMessageBox::critical(this, "BALLView", "Please enter a correct numerical value for the split distance!", 
															 QMessageBox::Ok, QMessageBox::NoButton);
						return;
					}
				}
				split_();
				return;
			}
				
			rep_->enableModelUpdate(false);
			rep_->enableColoringUpdate(false);
			if (rep_->isHidden())
			{
				rep_->setNeedsUpdate();
				return;
			}

			notify_(new RepresentationMessage(*rep_, RepresentationMessage::UPDATE));
		}


		void ModifySurfaceDialog::cancelPressed()
		{
			hide();
		}


		void ModifySurfaceDialog::choosePressed()
		{
			QColor qcolor = chooseColor(custom_color_label);
			selected_color.set(qcolor);
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

			min_value_  = 0;
			max_value_  = 0;
			mid_value_  = 0;

			float extr = 0;
			float value = 0;

			try
			{
				for(Position p = 0; p < mesh_->vertex.size(); p++)
				{
					value = fabs(grid_->getInterpolatedValue(mesh_->vertex[p]));
					if (value > extr) extr = value;
				}
			}
			catch(Exception::OutOfGrid)
			{
				setStatusbarText("Can not color this surface with this grid, the mesh has points outside the grid!");
				return;
			}

			min_value_  = -extr;
			max_value_  = +extr;

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
			ColorRGBA col(custom_color_label->backgroundColor());

			if (transparency_slider->value() == 0)
			{
				col.setAlpha(255);
				rep_->setTransparency(0);
			}
			else
			{
				rep_->setTransparency((Size)((float)transparency_slider->value() * 2.55));
				col.setAlpha(255 - rep_->getTransparency());
			}

			if (!color_only_selection->isChecked())
			{
				mesh_->colors.resize(1);
				mesh_->colors[0] = col;
				return;
			}

			if (mesh_->colors.size() != mesh_->vertex.size())
			{
				mesh_->colors.resize(mesh_->vertex.size());
			}

			// make sure we have a colorProcessor
			if (rep_->getColorProcessor() == 0)
			{
				rep_->setColorProcessor(new CustomColorProcessor);
				rep_->getColorProcessor()->createAtomGrid();
			}
			ColorProcessor* cp = rep_->getColorProcessor();

			// collect informations which vertices are to be colored
			for (Position p = 0; p < mesh_->vertex.size(); p++)
			{
				// make sure we found an atom
				const Atom* atom = cp->getClosestItem(mesh_->vertex[p]);

				if (atom == 0 || !atom->isSelected())
				{
					mesh_->colors[p].setAlpha(255 - rep_->getTransparency());
				}
				else
				{
					mesh_->colors[p] = col;
				}
			}
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
			mesh_->colors.resize(mesh_->vertex.size());
			ColorRGBA list[2];

			list[0] = min_color;
			list[1] = mid_color;

			ColorMap lower_table(list, 2);
			lower_table.setMinMaxColors(min_min_color, max_max_color);
			lower_table.setAlphaBlending(true);
			lower_table.setNumberOfColors(levels_box->value()/2);
			lower_table.setRange(String((min_box->text().ascii())).toFloat(), String((mid_box->text().ascii())).toFloat());
			lower_table.createMap();

			list[0] = mid_color;
			list[1] = max_color;

			ColorMap upper_table(list, 2);
			upper_table.setMinMaxColors(min_min_color, max_max_color);
			upper_table.setAlphaBlending(true);
			upper_table.setNumberOfColors(levels_box->value()/2);
			upper_table.setRange(String((mid_box->text().ascii())).toFloat(), String((max_box->text().ascii())).toFloat());
			upper_table.createMap();

			try 
			{
				const float mid_value = String(mid_box->text().ascii()).toFloat();
				for (Position i = 0; i < mesh_->colors.size(); i++)
				{
					const float grid_value = grid_->getInterpolatedValue(mesh_->vertex[i]);
					if (grid_value <= mid_value)
					{
						mesh_->colors[i].set(lower_table.map(grid_value));
					}
					else
					{
						mesh_->colors[i].set(upper_table.map(grid_value));
					}
				}
			}	
			catch (Exception::OutOfGrid)
			{
				setStatusbarText("Error! There is a point contained in the surface that is not inside the grid!", true);
				return false;
			}

			rep_->setTransparency(0);

			if (transparency_group_grid->selected() != none_button_grid)
			{
				if ((Size)min_min_color.getAlpha() 	!= 255 ||
						(Size)min_color.getAlpha() 			!= 255 ||
						(Size)mid_color.getAlpha() 			!= 255 ||
						(Size)max_color.getAlpha() 			!= 255 ||
						(Size)max_max_color.getAlpha() 	!= 255)
				{
					// if we use Transparency, just tell the Representation
					rep_->setTransparency(80);
				}
			}

			return true;
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
				apply_button->setEnabled(grid_ != 0);
				autoscale->setEnabled(grid_ != 0);
				return;
			}

			// if coloring by selected color, always enabled
			apply_button->setEnabled(true);
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

			if 			((Index)rm->getType() == (Index)RegularData3DMessage::NEW)
			{
				insertGrid_(*rm->getData(), rm->getCompositeName());
			}
			else if ((Index)rm->getType() == (Index)RegularData3DMessage::REMOVE)
			{
				removeGrid_(*rm->getData());
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

			mode_combobox->setCurrentItem(rep_->getDrawingMode());

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

		void ModifySurfaceDialog::splitMethodChanged()
		{
			distance_edit->setEnabled(split_by_distance->isChecked());
		}

		void ModifySurfaceDialog::split_()
		{
			rep_->enableModelUpdate(false);

			// create a new representation with the subset of the original mesh
			Representation* new_rep = (Representation*) rep_->create();
			new_rep->setComposites(rep_->getComposites());
			new_rep->setModelType(rep_->getModelType());
			new_rep->setColoringMethod(rep_->getColoringMethod());
			new_rep->enableModelUpdate(false);

			new_rep->clearGeometricObjects();

			// make sure we have a colorProcessor
			if (rep_->getColorProcessor() == 0)
			{
				new_rep->setColorProcessor(new ColorProcessor());
				new_rep->enableColoringUpdate(false);
				rep_->enableColoringUpdate(false);
			}

			HashSet<const Composite*> roots;
			List<const Composite*>::const_iterator cit = rep_->getComposites().begin();
			for (; cit != rep_->getComposites().end(); ++cit)
			{
				roots.insert(&(*cit)->getRoot());
			}

			ColorProcessor cp;
			if (split_by_selection->isChecked())
			{
				HashSet<const Composite*>::Iterator it = roots.begin();
				List<const Composite*> roots_list;
				for (; +it; ++it)
				{
					roots_list.push_back(*it);
				}

				cp.setComposites(&roots_list);
				cp.createAtomGrid();
			}

			Representation::GeometricObjectList::iterator git = rep_->getGeometricObjects().begin();
			for (; git != rep_->getGeometricObjects().end(); ++git)
			{
				// get the original mesh
				if (!RTTI::isKindOf<Mesh>(**git)) continue;
				Mesh& org_mesh = *dynamic_cast<Mesh*>(*git);

				// copy all vertices and colors into a new mesh
				Mesh* new_mesh = new Mesh;
				new_mesh->setComposite(org_mesh.getComposite());

				// collect informations which vertices are to be included into new mesh
				vector<bool> include_vertex;
				include_vertex.resize(org_mesh.vertex.size());
				
				if (split_by_selection->isChecked())
				{
					for (Position p = 0; p < org_mesh.vertex.size(); p++)
					{
						// make sure we found an atom
						const Atom* atom = cp.getClosestItem(org_mesh.vertex[p]);

						include_vertex[p] = (atom != 0 && atom->isSelected());
					}
				}
				else
				{
					calculateIncludedVertices_(include_vertex, org_mesh, roots);
				}

				// make a backup of the old meshs content and clear it
				vector<ColorRGBA> colors;

				bool multi_color = (org_mesh.colors.size() > 1);

				if (multi_color)
				{
					colors = org_mesh.colors;
					org_mesh.colors.clear();
				}
				else
				{ 
					new_mesh->colors = org_mesh.colors;
				}

				vector<Surface::Triangle> triangles = org_mesh.triangle;
				vector<Vector3> vertices = org_mesh.vertex;
				vector<Vector3> normals = org_mesh.normal;

				org_mesh.triangle.clear();
				org_mesh.vertex.clear();
				org_mesh.normal.clear();

				/// store a translation between the vertex position in the vector in the old and the new mesh
				vector<Position> new_vertex_pos;
				new_vertex_pos.resize(include_vertex.size());
				
				// copy the data to the new mesh
				for (Position pos = 0; pos < include_vertex.size(); pos++)
				{
					if (include_vertex[pos])
					{
						new_vertex_pos[pos] = new_mesh->vertex.size();

						new_mesh->vertex.push_back(vertices[pos]);
						new_mesh->normal.push_back(normals[pos]);
						if (multi_color)
						{
							new_mesh->colors.push_back(colors[pos]);
						}
					}
				}

				vector<Index> old_vertices_map;
				old_vertices_map.resize(include_vertex.size());
				for (Position pos = 0; pos < old_vertices_map.size(); pos++) old_vertices_map[pos] = -1;
				
				vector<Surface::Triangle>::iterator tit = triangles.begin();
				Position vpos = 0;
				for (; tit != triangles.end(); tit++) 
				{
					Surface::Triangle& tri = *tit;
					bool into_new = (include_vertex[tri.v1] && include_vertex[tri.v2] && include_vertex[tri.v3]);

					if (!into_new) 
					{
						for (Position i = 0; i < 3; i++)
						{
							if 			(i == 0) vpos = tri.v1;
							else if (i == 1) vpos = tri.v2;
							else if (i == 2) vpos = tri.v3;

							if (old_vertices_map[vpos] != -1) continue;

							old_vertices_map[vpos] = org_mesh.vertex.size();
							org_mesh.vertex.push_back(vertices[vpos]);
							org_mesh.normal.push_back(normals[vpos]);
							if (multi_color)
							{
								org_mesh.colors.push_back(colors[vpos]);
							}
						}

						org_mesh.triangle.push_back(tri);

						continue;
					}

					tri.v1 = new_vertex_pos[tri.v1];
					tri.v2 = new_vertex_pos[tri.v2];
					tri.v3 = new_vertex_pos[tri.v3];

					new_mesh->triangle.push_back(tri);
				}

				// correct the vertex numbers in the triangles
				for (Position pos = 0; pos < org_mesh.triangle.size(); pos++)
				{
					Surface::Triangle& tri = org_mesh.triangle[pos];
					tri.v1 = old_vertices_map[tri.v1];
					tri.v2 = old_vertices_map[tri.v2];
					tri.v3 = old_vertices_map[tri.v3];
				}


				// cleanup
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

		void ModifySurfaceDialog::calculateIncludedVertices_(vector<bool>& include_vertex, const Mesh& org_mesh, HashSet<const Composite*>& roots)
		{
			List<const Atom*> selected_atoms;
			List<const Atom*> all_atoms;

			HashSet<const Composite*>::ConstIterator it = roots.begin();
			for(; +it; it++)
			{
				if (RTTI::isKindOf<AtomContainer>(**it))
				{
					AtomConstIterator ait;
					const AtomContainer* const acont = dynamic_cast<const AtomContainer*>(*it);
					BALL_FOREACH_ATOM(*acont, ait)
					{
						all_atoms.push_back(&*ait);

						if ((*ait).isSelected()) selected_atoms.push_back(&*ait);
					}
				}
				else if (RTTI::isKindOf<Atom>(**it))
				{
					const Atom* atom = dynamic_cast<const Atom*> (*it);
					all_atoms.push_back(atom);

					if (atom->isSelected()) selected_atoms.push_back(atom);
				}
			}

			float distance = String(distance_edit->text().ascii()).toFloat();

			BoundingBoxProcessor boxp;
			boxp.start();
			List<const Atom*>::Iterator lit = all_atoms.begin();
			for(;lit != all_atoms.end(); lit++)
			{
				boxp.operator() (*(Atom*)*lit);
			}
			boxp.finish();

			Vector3 diagonal = boxp.getUpper() - boxp.getLower();
			
			// grid spacing, tradeoff between speed and memory consumption
			float grid_spacing = distance;

			float memory = SysInfo::getAvailableMemory();
			//
			// if we can not calculate the available memory, use around 6 MB for the grid
			if (memory == -1) memory = 10000000;
			memory *= (float) 0.6;

			Vector3 overhead(2.5 + distance);
			diagonal += overhead * 2.0;
			float min_spacing = HashGrid3<const Atom*>::calculateMinSpacing((LongIndex)memory, diagonal);
			if (min_spacing > grid_spacing) grid_spacing = min_spacing;
			
			AtomGrid atom_grid(boxp.getLower() - overhead, diagonal, grid_spacing); 
		 
			for (lit = selected_atoms.begin(); lit != selected_atoms.end(); lit++)
			{
				atom_grid.insert((*lit)->getPosition(), *lit);
			}

			square_distance_ = distance * distance;

			for (Position p = 0; p < org_mesh.vertex.size(); p++)
			{
				include_vertex[p] = checkInclude_(atom_grid, org_mesh.vertex[p]);
			}
		}


		bool ModifySurfaceDialog::checkInclude_(const AtomGrid& atom_grid, const Vector3& point) const
		{
			const AtomBox* box = atom_grid.getBox(point);

			if (!box) return false;

			Position x, y, z;
			atom_grid.getIndices(*box, x, y, z);

			Size dist = 1;
			// iterator over neighbour boxes
			for (Index xi = -(Index)dist; xi <= (Index)dist; xi++)
			{
				for (Index yi = -(Index)dist; yi <= (Index)dist; yi++)
				{
					for (Index zi = -(Index)dist; zi <= (Index)dist; zi++)
					{
						// iterate over all data items
						const AtomBox* box_ptr = atom_grid.getBox(x+xi, y+yi, z+zi);	
						if (box_ptr != 0 && !box_ptr->isEmpty())
						{
							AtomBox::ConstDataIterator hit = box_ptr->beginData();
							for (;+hit; hit++)
							{
								if (((*hit)->getPosition() - point).getSquareLength() <= square_distance_)
								{
									return true;
								}
							}
						}
					}
				}
			}

			return false;
		}

		void ModifySurfaceDialog::customColorTransparencyChanged()
		{
			transparency_label->setText(String(transparency_slider->value()).c_str());
		}

		void ModifySurfaceDialog::changeDrawingModeTransparencyChanged()
		{
			transparency_label_2->setText(String(transparency_slider_2->value()).c_str());
		}

		void ModifySurfaceDialog::changeDrawingMode_()
		{
			if (rep_ == 0) return;

			rep_->setDrawingMode((DrawingMode)mode_combobox->currentItem());

			Size transparency = (Size)((float)transparency_slider_2->value() * 2.55);

			rep_->setTransparency(transparency);

			Representation::GeometricObjectList::iterator it = rep_->getGeometricObjects().begin();
			for (; it != rep_->getGeometricObjects().end(); it++)
			{
				if (!RTTI::isKindOf<Mesh> (**it)) continue;

				Mesh* mesh = dynamic_cast<Mesh*> (*it);

				for (Position p = 0; p < mesh->colors.size(); p++)
				{
					mesh->colors[p].setAlpha(255 - transparency);
				}
			}
		}

	} // namespace VIEW
} // namespace BALL
