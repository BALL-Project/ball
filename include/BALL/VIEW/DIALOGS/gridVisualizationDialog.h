// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: gridVisualizationDialog.h,v 1.1.6.1 2007/03/25 21:25:49 oliver Exp $
//

#ifndef BALL_VIEW_DIALOGS_GRIDVISUALIZATIONDIALOG_H
#define BALL_VIEW_DIALOGS_GRIDVISUALIZATIONDIALOG_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifdef BALL_COMPILER_MSVC
# include <BALL/VIEW/UIC/ui_gridVisualizationDialog.h>
#else
# include <BALL/VIEW/UIC/gridVisualizationDialogData.h>
#endif


#ifndef BALL_VIEW_DATATYPE_COLORRGBA_H
# include <BALL/VIEW/DATATYPE/colorRGBA.h>
#endif

#ifndef BALL_DATATYPE_REGULARDATA3D
# include <BALL/DATATYPE/regularData3D.h>
#endif 

#ifndef BALL_VIEW_DATATYPE_STANDARDDATASETS_H
# include <BALL/VIEW/DATATYPE/standardDatasets.h>
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
			GridVisualizationDialog(QWidget* parent = 0, const char* name = "GridVisualizationDialog", 
															bool modal = FALSE, Qt::WFlags fl = 0);

			///
			~GridVisualizationDialog();
					
			///
			void setGrid(RegularData3D* grid);
			
			///
			void setController(RegularData3DController* controller);
			
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
			bool exec();

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
			RegularData3DController* controller_;

			ColorRGBA	 	min_min_color, min_color, mid_color, max_color, max_max_color;	

			std::list<RegularData3D*> grid_list_;
		};

	} // namespace VIEW
} // namespace BALL

#endif
