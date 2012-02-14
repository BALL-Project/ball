// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/DIALOGS/modifyRepresentationDialog.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/DATATYPE/standardDatasets.h>
#include <BALL/VIEW/WIDGETS/geometricControl.h>
#include <BALL/VIEW/MODELS/standardColorProcessor.h>
#include <BALL/VIEW/MODELS/colorByGridProcessor.h>
#include <BALL/VIEW/PRIMITIVES/multiLine.h>
#include <BALL/VIEW/PRIMITIVES/quadMesh.h>

#include <BALL/SYSTEM/path.h>
#include <BALL/SYSTEM/file.h>
#include <BALL/DATATYPE/regularData3D.h>
#include <BALL/DATATYPE/hashGrid.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/STRUCTURE/geometricProperties.h>
#include <BALL/SYSTEM/sysinfo.h>

#include <QtGui/QLineEdit>
#include <QtGui/QSpinBox>
#include <QtGui/QTabWidget>
#include <QtGui/QPushButton>
#include <QtGui/QLabel>
#include <QtGui/QRadioButton>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QSlider>
#include <QtGui/QMessageBox>

namespace BALL
{
	 namespace VIEW
	 {

		 ModifyRepresentationDialog::ModifyRepresentationDialog(const ModifyRepresentationDialog& mrd)
			: QDialog(0),
				ModularWidget()
		{
			material_settings_ = mrd.material_settings_;
		}

		ModifyRepresentationDialog::ModifyRepresentationDialog( QWidget* parent,  const char* name, bool, Qt::WFlags fl )
			: QDialog(parent, fl),
				Ui_ModifyRepresentationDialogData(),
				ModularWidget(name),
				grid_(0),
				rep_(0),
				material_settings_(NULL)
		{
			setupUi(this);

			// NOTE: we *rely* on the object name "MaterialSettingsForRepresentation" to be able to
			//       distinguish between the dialog used here and the one used in the preferences
			material_settings_ = new MaterialSettings(material_settings_, "MaterialSettingsForRepresentation");
			material_setting->layout()->addWidget(material_settings_);

			// signals and slots connections
			connect( surface_tab, SIGNAL( currentChanged(int) ), this, SLOT( tabChanged() ) );
			connect( gridColorWidget, SIGNAL( autoScaleRequested() ), this, SLOT( autoScale() ) );
			connect( grids, SIGNAL( activated(int) ), this, SLOT( gridSelected() ) );
			connect( choose_button, SIGNAL( clicked() ), this, SLOT( choosePressed() ) );
			connect( split_by_selection, SIGNAL( clicked() ), this, SLOT( splitMethodChanged() ) );
			connect( split_by_distance, SIGNAL( clicked() ), this, SLOT( splitMethodChanged() ) );
			connect( transparency_slider, SIGNAL( valueChanged(int) ), this, SLOT( customColorTransparencyChanged() ) );
			connect( transparency_slider_2, SIGNAL( valueChanged(int) ), this, SLOT( changeDrawingModeTransparencyChanged() ) );

			registerWidget(this);
			setObjectName(name);
		}

		ModifyRepresentationDialog::~ModifyRepresentationDialog()
		{
		}

		// ------------------------- SLOTS ------------------------------------------------
		void ModifyRepresentationDialog::accept()
		{
			if (surface_tab->currentWidget() == by_grid)
			{
				if (!colorByGrid_()) return;
			}
			else if (surface_tab->currentWidget() == by_color)
			{
				colorByCustomColor_();
			}
			else if (surface_tab->currentWidget() == drawing_mode)
			{
				changeDrawingMode_();
			}
			else if (surface_tab->currentWidget() == split)
			{
				if (split_by_distance->isChecked())
				{
					try
					{
						ascii(distance_edit->text()).toFloat();
					}
					catch(...)
					{
						QMessageBox::critical(this, tr("BALLView"), tr("Please enter a correct numerical value for the split distance!"), 
															 QMessageBox::Ok, Qt::NoButton);
						return;
					}
				}
				applySplit();
				return;
			}
			else if (surface_tab->currentWidget() == material_setting)	
			{
				if (material_settings_)
				{
					material_settings_->apply();
				}
			}

			if (rep_->isHidden())
			{
				rep_->setNeedsUpdate();
				return;
			}

			getMainControl()->update(*rep_);

			QDialog::accept();
		}

		void ModifyRepresentationDialog::choosePressed()
		{
			QColor qcolor = chooseColor(custom_color_label);
			selected_color.set(qcolor);
		}

		void ModifyRepresentationDialog::tabChanged()
		{
			checkApplyButton_();
		}



		//--------------------- Helper functions ----------------------------------
		bool ModifyRepresentationDialog::insertGrid_(RegularData3D& grid, const String& name)
		{
			grid_list_.push_back(&grid);
			grids->addItem(name.c_str());
			if (grid_ == 0) grid_ = &grid;
			if (rep_ == 0) return false;

			gridSelected();
			return true;
		}

		void ModifyRepresentationDialog::removeGrid_(RegularData3D& grid)
		{
			list<RegularData3D*>::iterator it = grid_list_.begin();
			Position pos = 0;
			for (; it != grid_list_.end(); it++)
			{
				if (*it == &grid)
				{
					if ((Index)pos == grids->currentIndex()) 
					{
						grids->setCurrentIndex(-1);
						invalidateGrid_();
					}
					grid_list_.erase(it);
					grids->removeItem(pos);
					return;
				}
				pos++;
			}
		}


		void ModifyRepresentationDialog::gridSelected()
		{
			if (rep_ == 0) return;

			// prevent freezing, if clicking on representation, while
			// an other is still rendering
			if (getMainControl()->isBusy()) return;

			if (grids->count() == 0 || grids->currentIndex() == -1)
			{
				invalidateGrid_();
				return;
			}

			Index pos = grids->currentIndex();
			list<RegularData3D*>::iterator it = grid_list_.begin();
			for (Position p = 0; p < (Position)pos; p++)
			{
				it++;
			}
			
			grid_ = *it;
		}

		void ModifyRepresentationDialog::setGrid(RegularData3D* grid)
		{
			grid_ = grid;
		}

		void ModifyRepresentationDialog::autoScale()
		{
			if (grid_ == 0 || rep_ == 0) return;

			if (gridColorWidget->isNormalizationEnabled())
			{
				gridColorWidget->setMinValue(0.0f);
				gridColorWidget->setMidValue(0.5f);
				gridColorWidget->setMaxValue(1.0f);
				return;
			}

			if (vertices_.size() == 0)
			{
				const GeometricObjectList& ls = rep_->getGeometricObjects();

				GeometricObjectList::const_iterator it = ls.begin();
				for (; it != ls.end(); it++)
				{
					(**it).getVertices(vertices_);
				}
			}

			float min_value = 0.0f;
			float mid_value = 0.0f;
			float max_value = 0.0f;

			if (vertices_.size() == 0)
			{
				mid_value = 0;
				min_value = 0;
				max_value = 0;
			}
			else
			{
				bool error = false;

				try
				{
					min_value  = grid_->getInterpolatedValue(vertices_[0]);
					max_value  = grid_->getInterpolatedValue(vertices_[0]);
				}
				catch(...)
				{
					min_value = FLT_MAX;
					max_value = FLT_MIN;
					error = true;
				}

				mid_value  = 0;
				float value;

				for(Position p = 1; p < vertices_.size(); p++)
				{
					try
					{
						value = grid_->getInterpolatedValue(vertices_[p]);
						min_value = std::min(min_value, value);
						max_value = std::max(max_value, value);
					}
					catch(...)
					{
						error = true;
					}
				}

				if (error)
				{
					setStatusbarText((String)tr("Warning the geometric object has points outside the grid!"), true);
				}

				mid_value = (max_value - min_value) * 0.5 + min_value;
			}

			buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
			gridColorWidget->setEnabled(true);
			gridColorWidget->setMinValue(min_value);
			gridColorWidget->setMidValue(mid_value);
			gridColorWidget->setMaxValue(max_value);
		}

		void ModifyRepresentationDialog::colorByCustomColor_()
		{
			QPalette pal(custom_color_label->palette());
			QColor qcolor = pal.color(custom_color_label->backgroundRole());
			ColorRGBA col(qcolor);

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

			// if the whole representation needs recoloring, things are pretty simple
			if (!color_only_selection->isChecked())
			{
				// if the current representation is not already custom colored,
				// change its coloring processor
				if (   (rep_->getColorProcessor() == 0) 
						|| (!RTTI::isKindOf<CustomColorProcessor>(*(rep_->getColorProcessor()))))
				{
					rep_->setColorProcessor(new CustomColorProcessor);
					rep_->setColoringMethod(COLORING_CUSTOM);
					rep_->getColorProcessor()->createAtomGrid();
				}
				ColorProcessor* cp = rep_->getColorProcessor();
				cp->setDefaultColor(col);
			}
			else
			{
				// color only vertices belonging to selected atoms

				// if the current representation already has a coloring processor we won't 
				// change it; we only want to color a certain selected patch
				if (rep_->getColorProcessor() == 0) 
				{
					rep_->setColorProcessor(new CustomColorProcessor);
					rep_->setColoringMethod(COLORING_CUSTOM);
					rep_->getColorProcessor()->createAtomGrid();
				}
				BALL_SELECTED_COLOR = col;
			}
		}

		bool ModifyRepresentationDialog::colorByGrid_()
		{
			if (grid_ == 0 || rep_ == 0 || getMainControl()->isBusy())
			{
				setStatusbarText((String)tr("Could not color surface, maybe because an other thread is still running?"), true);
				return false;
			}

			ColorByGridProcessor* grid_processor = new ColorByGridProcessor(grid_, gridColorWidget);
			rep_->setColorProcessor(grid_processor);

			return true;
		}


		void ModifyRepresentationDialog::checkApplyButton_()
		{
			if (!rep_)
			{
				gridColorWidget->setEnabled(false);
				buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
				return;
			}

			if (surface_tab->currentWidget() == by_grid)
			{
				buttonBox->button(QDialogButtonBox::Ok)->setEnabled(grid_ != 0);
				gridColorWidget->setEnabled(grid_ != 0);
				return;
			}

			// if coloring by selected color, always enabled
			buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
		}

		void ModifyRepresentationDialog::onNotify(Message *message)
		{
		#ifdef BALL_VIEW_DEBUG
			Log.error() << "ModifyRepresentationDialog " << this << " onNotify " << message << std::endl;
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
					if (rep_ == rep) setRepresentation(rep);
				}
				else if (rm->getType() == RepresentationMessage::SELECTED ||
								 rm->getType() == RepresentationMessage::ADD)
				{
					setRepresentation(rep);
				}

				return;
			}

			if (!RTTI::isKindOf<DatasetMessage>(*message)) return;

			DatasetMessage& dmsg = *(DatasetMessage*)(message);
			if (!dmsg.isValid()) return;

			RegularData3DDataset* rset = dynamic_cast<RegularData3DDataset*>(dmsg.getDataset());
			if (rset == 0) return;

			if (dmsg.getType() == DatasetMessage::ADD)
			{
				insertGrid_(*rset->getData(), rset->getName());
			}
			else if (dmsg.getType() == DatasetMessage::REMOVE)
			{
				removeGrid_(*rset->getData());
			}
		}

		void ModifyRepresentationDialog::invalidateGrid_()
		{
			grid_ = 0;
			grids->setCurrentIndex(-1);
			checkApplyButton_();
		}

		void ModifyRepresentationDialog::invalidateMesh_()
		{
			rep_ = 0;
			checkApplyButton_();
		}


		void ModifyRepresentationDialog::setRepresentation(Representation* rep)
		{
			if (rep == 0 ||
					rep->getGeometricObjects().size() == 0)
			{
				buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
				return;
			}
			else
			{
				rep_ = rep;
				vertices_.clear();
			}

			if (grids->currentIndex() == -1 && 
					grids->count() != 0)
			{
				grids->setCurrentIndex(grids->count()-1);
			}

			if (isVisible()) gridSelected() ;

			mode_combobox->setCurrentIndex(rep_->getDrawingMode());

			material_settings_->setCurrentRepresentation(rep_);

			checkApplyButton_();
		}

		void ModifyRepresentationDialog::show()
		{
			GeometricControl* go = GeometricControl::getInstance(0);
			if (go != 0) 
			{
				list<Representation*> reps = go->getHighlightedRepresentations();
				if (reps.size())
				{
					setRepresentation(*reps.begin());
				}
			}
			gridSelected();
			QDialog::show();
			raise();
		}

		void ModifyRepresentationDialog::setMinValue(float value)
		{
			gridColorWidget->setMinValue(value);
		}

		void ModifyRepresentationDialog::setMaxValue(float value)
		{
			gridColorWidget->setMaxValue(value);
		}

		void ModifyRepresentationDialog::setMidValue(float value)
		{
			gridColorWidget->setMidValue(value);
		}

		void ModifyRepresentationDialog::splitMethodChanged()
		{
			distance_edit->setEnabled(split_by_distance->isChecked());
		}

		void ModifyRepresentationDialog::applySplit()
		{
			if (!rep_) return;

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
			list<const Composite*>::const_iterator cit = rep_->getComposites().begin();
			for (; cit != rep_->getComposites().end(); ++cit)
			{
				roots.insert(&(*cit)->getRoot());
			}

			if (roots.size() == 0)
			{
				Composite* system = *getMainControl()->getCompositeManager().begin();
				roots.insert(system);
			}

			ColorProcessor cp;
			if (split_by_selection->isChecked())
			{
				HashSet<const Composite*>::iterator it = roots.begin();
				list<const Composite*> roots_list;
				for (; +it; ++it)
				{
					roots_list.push_back(*it);
				}

				cp.setComposites(&roots_list);
				cp.createAtomGrid();
			}

			GeometricObjectList::iterator git = rep_->getGeometricObjects().begin();
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

		void ModifyRepresentationDialog::calculateIncludedVertices_(vector<bool>& include_vertex, const Mesh& org_mesh, HashSet<const Composite*>& roots)
		{
			list<const Atom*> selected_atoms;
			list<const Atom*> all_atoms;

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

			float distance = ascii(distance_edit->text()).toFloat();

			BoundingBoxProcessor boxp;
			boxp.start();
			list<const Atom*>::iterator lit = all_atoms.begin();
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


		bool ModifyRepresentationDialog::checkInclude_(const AtomGrid& atom_grid, const Vector3& point) const
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

		void ModifyRepresentationDialog::customColorTransparencyChanged()
		{
			transparency_label->setText(String(transparency_slider->value()).c_str());
		}

		void ModifyRepresentationDialog::changeDrawingModeTransparencyChanged()
		{
			transparency_label_2->setText(String(transparency_slider_2->value()).c_str());
		}

		void ModifyRepresentationDialog::changeDrawingMode_()
		{
			if (rep_ == 0) return;

			rep_->setDrawingMode((DrawingMode)mode_combobox->currentIndex());

			Size transparency = (Size)((float)transparency_slider_2->value() * 2.55);

			rep_->setTransparency(transparency);

			/*
			GeometricObjectList::iterator it = rep_->getGeometricObjects().begin();
			for (; it != rep_->getGeometricObjects().end(); it++)
			{
				if (!RTTI::isKindOf<Mesh> (**it)) continue;

				Mesh* mesh = dynamic_cast<Mesh*> (*it);

				for (Position p = 0; p < mesh->colors.size(); p++)
				{
					mesh->colors[p].setAlpha(255 - transparency);
				}
			}
			*/
		}

		void ModifyRepresentationDialog::setSplitRadius(float distance)
		{
			if (distance == 0)
			{
				split_by_distance->setChecked(false);
				split_by_selection->setChecked(true);
			}
			else
			{
				split_by_distance->setChecked(true);
				split_by_selection->setChecked(false);
				distance_edit->setText(String(distance).c_str());
			}
		}

		void ModifyRepresentationDialog::setMode(Position pos)
		{
			surface_tab->setCurrentIndex(pos);
			tabChanged();
		}

	} // namespace VIEW
} // namespace BALL
