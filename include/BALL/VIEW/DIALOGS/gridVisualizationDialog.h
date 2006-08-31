// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: gridVisualizationDialog.h,v 1.1.4.1 2006/08/31 14:04:44 leonhardt Exp $
//

#ifndef BALL_VIEW_DIALOGS_GRIDVISUALIZATIONDIALOG_H
#define BALL_VIEW_DIALOGS_GRIDVISUALIZATIONDIALOG_H

#include <BALL/VIEW/UIC/gridVisualizationDialogData.h>

#ifndef BALL_VIEW_DATATYPE_COLORRGBA_H
# include <BALL/VIEW/DATATYPE/colorRGBA.h>
#endif

#ifndef BALL_DATATYPE_REGULARDATA3D
# include <BALL/DATATYPE/regularData3D.h>
#endif 

#ifndef BALL_DATATYPE_LIST_H
# include <BALL/DATATYPE/list.h>
#endif

namespace BALL
{
	namespace VIEW
	{
		/** Dialog for visualizing grids.
				\ingroup ViewDialogs
		*/
		class BALL_VIEW_EXPORT GridVisualizationDialog 
			: public QDialog,
				public Ui_GridVisualizationDialogData
		{ 
			Q_OBJECT

			public:

			///
			GridVisualizationDialog(QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WFlags fl = 0);

			///
			~GridVisualizationDialog()
				throw();
					
			///
			void setGrid(RegularData3D* grid)
				throw();
			
			///
			void setGrids(List<std::pair<RegularData3D*, String> > grids)
				throw();
			
			///
			void setMinValue(float value);
					
			///
			void setMaxValue(float value);
					
			///
			void setMidValue(float value);

			public slots:
			
			void accept();
			void maxPressed();
			void midPressed();
			void minPressed();
			void minMinPressed();
			void maxMaxPressed();
			void autoScale();
			void gridSelected();
			void show();

			protected slots:

			void gridTransparencyChanged();
			void normalizationChanged();

			protected:

			GridVisualizationDialog(const GridVisualizationDialog& dialog);

			bool insertGrid_(RegularData3D& grid, const String& name);
			void setColor_(ColorRGBA& color, const QLabel* label, const QSpinBox* box);
			void getColor_(const ColorRGBA& color, QLabel* label, QSpinBox* box);
			void checkApplyButton_();

			RegularData3D* grid_;

			ColorRGBA	 	min_min_color, min_color, mid_color, max_color, max_max_color;	

			List<RegularData3D*> grid_list_;
		};

	} // namespace VIEW
} // namespace BALL

#endif
