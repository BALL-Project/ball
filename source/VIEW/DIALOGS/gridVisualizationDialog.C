// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/DIALOGS/gridVisualizationDialog.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/DATATYPE/colorMap.h>
#include <BALL/VIEW/MODELS/standardColorProcessor.h>
#include <BALL/VIEW/WIDGETS/datasetControl.h>
#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/VIEW/RENDERING/RENDERERS/glRenderer.h>
#include <BALL/VIEW/PRIMITIVES/gridVisualisation.h>

#include <BALL/DATATYPE/regularData3D.h>
#include <BALL/KERNEL/forEach.h>
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
#include <QtGui/QTabWidget>

namespace BALL
{
	 namespace VIEW
	 {

		 GridVisualizationDialog::GridVisualizationDialog(const GridVisualizationDialog&)
			: QDialog(0)
		{
		}

		GridVisualizationDialog::GridVisualizationDialog(QWidget* parent,  const char* name, bool, Qt::WFlags fl)
			: QDialog(parent, fl),
				Ui_GridVisualizationDialogData(),
				grid_(0)
		{
			setupUi(this);

			// signals and slots connections
			connect( gridColorWidget, SIGNAL( autoScaleRequested() ), this, SLOT( autoScale() ) );
			connect( grids, SIGNAL( activated(int) ), this, SLOT( gridSelected() ) );

			setObjectName(name);

			RegisterHelpSystemMessage* msg = new RegisterHelpSystemMessage();
			msg->setObject(this);
			msg->setURL("datasetControl.html#volume");
			getMainControl()->sendMessage(*msg);
		}

		GridVisualizationDialog::~GridVisualizationDialog()
		{
		}

		//--------------------- Helper functions ----------------------------------
		bool GridVisualizationDialog::insertGrid_(RegularData3D& grid, const String& name)
		{
			grid_list_.push_back(&grid);
			grids->addItem(name.c_str());
			if (grid_ == 0) grid_ = &grid;

			return true;
		}

		void GridVisualizationDialog::gridSelected()
		{
			if (grids->count() == 0 || grids->currentIndex() == -1)
			{
				grid_ = 0;
				return;
			}

			Index pos = grids->currentIndex();
			list<RegularData3D*>::iterator it = grid_list_.begin();
			for (Position p = 0; p < (Position)pos; p++)
			{
				it++;
			}
			
			grid_ = *it;

			buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
		}

		void GridVisualizationDialog::setGrid(RegularData3D* grid)
		{
			if (grid_ == 0) return;
			grid_ = grid;
			if (controller_ == 0) return;
			resize_needed->setChecked(controller_->isGridSizePowerOfTwo(*grid_));

			Position p = 0;
			list<RegularData3D*>::iterator it = grid_list_.begin();
			for (; it != grid_list_.end(); it++)
			{
				if (*it == grid_) break;
				p++;
			}

			grids->setCurrentIndex(p);
			gridSelected();
		}

		void GridVisualizationDialog::autoScale()
		{
			if (grid_ == 0) return;

			if (gridColorWidget->isNormalizationEnabled())
			{
				gridColorWidget->setMinValue(0.0f);
				gridColorWidget->setMidValue(0.5f);
				gridColorWidget->setMaxValue(1.0f);
				return;
			}
			const vector<float>& values = grid_->getData();

			float mid_value = 0;
			float min_value = values[0];
			float max_value = values[0];

			for (Position p = 1; p < values.size(); p++)
			{
				min_value = std::min(min_value, values[p]);
				max_value = std::max(max_value, values[p]);
			}
		
			mid_value = (max_value - min_value) * 0.5 + min_value;

			buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
			gridColorWidget->setEnabled(true);
			gridColorWidget->setMinValue(min_value);
			gridColorWidget->setMidValue(mid_value);
			gridColorWidget->setMaxValue(max_value);
		}

		void GridVisualizationDialog::accept()
		{
			QDialog::accept();

			ColorRGBA min_min_color = gridColorWidget->getMinMinColor();
			ColorRGBA min_color = gridColorWidget->getMinColor();
			ColorRGBA mid_color = gridColorWidget->getMidColor();
			ColorRGBA max_color = gridColorWidget->getMaxColor();
			ColorRGBA max_max_color = gridColorWidget->getMaxMaxColor();

			// now do the colorizing stuff...
			float min_value = gridColorWidget->getMinValue();
			float mid_value = gridColorWidget->getMidValue();
			float max_value = gridColorWidget->getMaxValue();

			ColorRGBA list[3];
			list[0] = min_color;
			list[1] = mid_color;
			list[2] = max_color;
	
			ColorMap cm(list, 3);
			cm.setMinMaxColors(min_min_color, max_max_color);
			cm.setAlphaBlending(true);
			cm.setNumberOfColors(gridColorWidget->getNumLevels());
			cm.setRange(min_value, max_value);

			std::vector<Vector4> interpolation_points(3);
			interpolation_points[0] = Vector4(0.);
			interpolation_points[1] = Vector4( (mid_value - min_value) / (max_value - min_value));
			interpolation_points[2] = Vector4(1.);
			
			cm.setInterpolationBoundaries(interpolation_points);
			cm.createMap();

			vector<float> values;

			if (gridColorWidget->isNormalizationEnabled())
			{
				grid_ = controller_->createHistogramGrid(*grid_);
			}

			if (grid_ == 0) return;

			if (!controller_->isGridSizePowerOfTwo(*grid_))
			{
				grid_ = controller_->resizeGrid(*grid_);
			}
			
			if (grid_ == 0) return;

			Size trans = 0;
			
			if ((Size)min_min_color.getAlpha() 	!= 255 ||
					(Size)min_color.getAlpha() 			!= 255 ||
					(Size)mid_color.getAlpha() 			!= 255 ||
					(Size)max_color.getAlpha() 			!= 255 ||
					(Size)max_max_color.getAlpha() 	!= 255)
			{
				trans = 80;
			}

			const RegularData3D& grid = *grid_;

			Position texname = Scene::getInstance(0)->prepareGridTextures(grid, cm);

			if (texname == 0)
			{
				reject();
				getMainControl()->setStatusbarText(tr("Setting up of 3D textures failed!"));
				return;
			}

			if (texname == 0)
			{
				reject();
				getMainControl()->setStatusbarText(tr("Your computer does not support 3D textures, aborting..."), true);
				return;
			}
				

			GridVisualisation& vol = *new GridVisualisation;
			vol.setGrid(grid_);
			vol.setTexture(texname);

			Vector3 origin = grid.getOrigin();
			RegularData3D::IndexType s = grid.getSize();
			vol.x = grid.getCoordinates(RegularData3D::IndexType(s.x-1,0,0)) - origin;
			vol.y = grid.getCoordinates(RegularData3D::IndexType(0,s.y-1,0)) - origin;
			vol.z = grid.getCoordinates(RegularData3D::IndexType(0,0,s.z-1)) - origin;
			vol.origin = origin;
			vol.max_dim = BALL_MAX3(vol.x.getLength(), vol.y.getLength(), vol.z.getLength());
			vol.draw_box = render_box->isChecked();

			Representation* rep = new Representation;
			rep->insert(vol);
			rep->setTransparency(trans);
			rep->setModelType(MODEL_GRID_VOLUME);

			if (mode_box->currentIndex() == 0)
			{
				vol.type = GridVisualisation::PLANE; 
				Vector3 point = origin + (vol.x + vol.y + vol.z) / 2.0;
				vol.setPoint(point);
				rep->setModelType(MODEL_GRID_SLICE);
  			rep->setProperty("RENDER_DIRECT");
				Vector3 normal = Scene::getInstance(0)->getStage()->getCamera().getViewVector();
		    normal.normalize();
				vol.setNormal(normal);
			}
			else if (mode_box->currentIndex() == 1)
			{
				vol.setDotSize(dot_size->value());
				vol.type = GridVisualisation::DOTS;
				vector<Vector3>& points = vol.points;
				calculateRandomPoints(grid, number_dots->value(), points);
			}
			else 
			{
				vol.slices = slices_spin->value();
				rep->setProperty("DONT_CLIP");
  			rep->setProperty("RENDER_DIRECT");
				vol.type = GridVisualisation::SLICES;
			}

			getMainControl()->insert(*rep);
			getMainControl()->update(*rep);
		}


		void GridVisualizationDialog::checkApplyButton_()
		{
			buttonBox->button(QDialogButtonBox::Ok)->setEnabled(grid_ != 0);
			gridColorWidget->setEnabled(grid_ != 0);
		}

		bool GridVisualizationDialog::exec()
		{
			grids->clear();
			grid_list_.clear();
			if (controller_ == 0)
			{
				BALLVIEW_DEBUG
				return false;
			}

			RegularData3D* selected_grid = grid_;
			vector<Dataset*> sets = controller_->getDatasets();
			vector<Dataset*>::iterator it = sets.begin();
			for (; it != sets.end(); it++)
			{
				insertGrid_(*controller_->getData(*it), (**it).getName());
			}

			// make sure that previously selected grid (selected by use of setGrid()) stays selected!
			if(selected_grid) setGrid(selected_grid);
				
			gridSelected();
			return QDialog::exec();
		}

		void GridVisualizationDialog::setMinValue(float value)
		{
			gridColorWidget->setMinValue(value);
		}

		void GridVisualizationDialog::setMaxValue(float value)
		{
			gridColorWidget->setMaxValue(value);
		}

		void GridVisualizationDialog::setMidValue(float value)
		{
			gridColorWidget->setMidValue(value);
		}

		void GridVisualizationDialog::setController(RegularData3DController* controller)
		{
			controller_ = controller;
		}

	} // namespace VIEW
} // namespace BALL
