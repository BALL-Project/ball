// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: gridVisualizationDialog.C,v 1.1.2.1 2006/06/27 20:37:21 amoll Exp $
//

#include <BALL/VIEW/DIALOGS/gridVisualizationDialog.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/DATATYPE/colorMap.h>
#include <BALL/VIEW/MODELS/standardColorProcessor.h>
#include <BALL/VIEW/PRIMITIVES/illuminatedLine.h>
#include <BALL/VIEW/WIDGETS/datasetControl.h>

#include <BALL/SYSTEM/path.h>
#include <BALL/SYSTEM/file.h>
#include <BALL/DATATYPE/regularData3D.h>
#include <BALL/DATATYPE/hashGrid.h>
#include <BALL/COMMON/limits.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/STRUCTURE/geometricProperties.h>
#include <BALL/SYSTEM/sysinfo.h>

#include <QtGui/qlineedit.h>
#include <QtGui/qspinbox.h>
#include <QtGui/qtabwidget.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qlabel.h>
#include <QtGui/qradiobutton.h>
#include <QtGui/qcheckbox.h>
#include <QtGui/QComboBox>
#include <QtGui/qslider.h>
#include <QtGui/qmessagebox.h>

namespace BALL
{
	 namespace VIEW
	 {

		 GridVisualizationDialog::GridVisualizationDialog(const GridVisualizationDialog&)
			: QDialog(0)
		{
		}

		GridVisualizationDialog::GridVisualizationDialog( QWidget* parent,  const char* name, bool, Qt::WFlags fl )
			: QDialog(parent, fl),
				Ui_GridVisualizationDialogData(),
				grid_(0)
		{
			setupUi(this);

			// signals and slots connections
			connect( apply_button, SIGNAL( clicked() ), this, SLOT( applyPressed() ) );
			connect( cancel_button, SIGNAL( clicked() ), this, SLOT( cancelPressed() ) );
			connect( autoscale, SIGNAL( clicked() ), this, SLOT( autoScale() ) );
			connect( grids, SIGNAL( activated(int) ), this, SLOT( gridSelected() ) );
			connect( transparency, SIGNAL( stateChanged(int) ), this, SLOT(gridTransparencyChanged()));
			connect( normalization, SIGNAL( clicked() ), this, SLOT(normalizationChanged()));
			connect( min_min_button, SIGNAL( clicked() ), this, SLOT( minMinPressed() ) );
			connect( min_button, SIGNAL( clicked() ), this, SLOT( minPressed() ) );
			connect( mid_button, SIGNAL( clicked() ), this, SLOT( midPressed() ) );
			connect( max_button, SIGNAL( clicked() ), this, SLOT( maxPressed() ) );
			connect( max_max_button, SIGNAL( clicked() ), this, SLOT( maxMaxPressed() ) );

			setObjectName(name);
		}

		void GridVisualizationDialog::normalizationChanged()
		{
			if (!normalization->checkState() == Qt::Checked) return;

			min_box->setText("0.0");
			mid_box->setText("0.5");
			max_box->setText("1.0");
		}

		GridVisualizationDialog::~GridVisualizationDialog()
			throw()
		{
		}

		// ------------------------- SLOTS ------------------------------------------------

		void GridVisualizationDialog::cancelPressed()
		{
			hide();
		}


		void GridVisualizationDialog::maxPressed()
		{
			max_color.set(chooseColor(max_label));
		}

		void GridVisualizationDialog::midPressed()
		{
			mid_color.set(chooseColor(mid_label));
		}

		void GridVisualizationDialog::minPressed()
		{
			min_color.set(chooseColor(min_label));
		}

		void GridVisualizationDialog::minMinPressed()
		{
			min_min_color.set(chooseColor(min_min_label));
		}

		void GridVisualizationDialog::maxMaxPressed()
		{
			max_max_color.set(chooseColor(max_max_label));
		}


		//--------------------- Helper functions ----------------------------------
		bool GridVisualizationDialog::insertGrid_(RegularData3D& grid, const String& name)
		{
			grid_list_.push_back(&grid);
			grids->addItem(name.c_str());
			if (grid_ == 0) grid_ = &grid;

			gridSelected();
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
		}

		void GridVisualizationDialog::setGrid(RegularData3D* grid)
			throw()
		{
			grid_ = grid;
		}

		void GridVisualizationDialog::autoScale()
		{
			if (grid_ == 0) return;

			const vector<float>& values = grid_->getData();

			mid_value_ = 0;
			min_value_ = values[0];
			max_value_ = values[0];

			for (Position p = 1; p < values.size(); p++)
			{
				min_value_ = std::min(min_value_, values[p]);
				max_value_ = std::max(max_value_, values[p]);
			}
		
			mid_value_ = (max_value_ - min_value_) * 0.5 + min_value_;

			apply_button->setEnabled(true);
			autoscale->setEnabled(true);
			min_box->setText(String(min_value_).c_str());
			mid_box->setText(String(mid_value_).c_str());
			max_box->setText(String(max_value_).c_str());
		}


		void GridVisualizationDialog::setColor_(ColorRGBA& color, const QLabel* label, const QSpinBox* box)
		{
			color = VIEW::getColor(label);

			if (plane->isChecked() &&
					transparency->checkState() != Qt::Checked)
			{
				color.setAlpha(255);
			}
			else
			{
				color.setAlpha(box->value());
			}
		}

		void GridVisualizationDialog::getColor_(const ColorRGBA& color, QLabel* label, QSpinBox* box)
		{
			VIEW::setColor(label, color);
			box->setValue(color.getAlpha());
		}

		bool GridVisualizationDialog::colorByGrid_()
		{
			try
			{
				ascii(mid_box->text()).toFloat();
				ascii(min_box->text()).toFloat();
				ascii(max_box->text()).toFloat();
			}
			catch(...)
			{
				getMainControl()->setStatusbarText("Invalid value for min, mid or max value!", true);
				return false;
			}

			setColor_(min_min_color, min_min_label, min_min_alpha);
			setColor_(min_color, min_label, min_alpha);
			setColor_(mid_color, mid_label, mid_alpha);
			setColor_(max_color, max_label, max_alpha);
			setColor_(max_max_color, max_max_label, max_max_alpha);

			// now do the colorizing stuff...
			float min_value = ascii(min_box->text()).toFloat();
			float mid_value = ascii(mid_box->text()).toFloat();
			float max_value = ascii(max_box->text()).toFloat();

			ColorRGBA list[3];
			list[0] = min_color;
			list[1] = mid_color;
			list[2] = max_color;
	
			ColorMap cm(list, 3);
			cm.setMinMaxColors(min_min_color, max_max_color);
			cm.setAlphaBlending(true);
			cm.setNumberOfColors(levels_box->value());
			cm.setRange(min_value, max_value);

			std::vector<Vector4> interpolation_points(3);
			interpolation_points[0] = Vector4(0.);
			interpolation_points[1] = Vector4( (mid_value - min_value) / (max_value - min_value));
			interpolation_points[2] = Vector4(1.);
			
			cm.setInterpolationBoundaries(interpolation_points);
			cm.createMap();

			vector<float> values;

			if (normalization->checkState() == Qt::Checked)
			{
				grid_ = DatasetControl::getInstance(0)->createHistogramGrid(*grid_);
			}

			Size trans = 0;
			
			if ((Size)min_min_color.getAlpha() 	!= 255 ||
					(Size)min_color.getAlpha() 			!= 255 ||
					(Size)mid_color.getAlpha() 			!= 255 ||
					(Size)max_color.getAlpha() 			!= 255 ||
					(Size)max_max_color.getAlpha() 	!= 255)
			{
				trans = 80;
			}

			return true;
		}


		void GridVisualizationDialog::checkApplyButton_()
		{
			apply_button->setEnabled(grid_ != 0);
			autoscale->setEnabled(grid_ != 0);
		}

		void GridVisualizationDialog::show()
		{
			gridSelected();
			QDialog::show();
			raise();
		}

		void GridVisualizationDialog::gridTransparencyChanged()
		{
 			bool enabled = !plane->isChecked() || transparency->checkState() == Qt::Checked;
			min_min_alpha->setEnabled(enabled);
					min_alpha->setEnabled(enabled);
					mid_alpha->setEnabled(enabled);
					max_alpha->setEnabled(enabled);
			max_max_alpha->setEnabled(enabled);
		}

		void GridVisualizationDialog::setMinValue(float value)
		{
			min_label->setText(String(value).c_str());
		}

		void GridVisualizationDialog::setMaxValue(float value)
		{
			max_label->setText(String(value).c_str());
		}

		void GridVisualizationDialog::setMidValue(float value)
		{
			mid_label->setText(String(value).c_str());
		}

		void GridVisualizationDialog::setGrids(List<std::pair<RegularData3D*, String> >& grid_list)
			throw()
		{
			grids->clear();
			grid_list_.clear();
			List<std::pair<RegularData3D*, String> >::Iterator it = grid_list.begin();
			for (; it != grid_list.end(); it++)
			{
				insertGrid_(*it->first, it->second);
			}
		}
		

	} // namespace VIEW
} // namespace BALL
