// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_DIALOGS_GRIDVISUALIZATIONDIALOG_H
#define BALL_VIEW_DIALOGS_GRIDVISUALIZATIONDIALOG_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
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

#include <BALL/VIEW/UIC/ui_gridVisualizationDialog.h>

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
			void autoScale();
			void gridSelected();
			bool exec();

			protected:

			GridVisualizationDialog(const GridVisualizationDialog& dialog);

			bool insertGrid_(RegularData3D& grid, const String& name);
			void checkApplyButton_();

			RegularData3D* grid_;
			RegularData3DController* controller_;

			std::list<RegularData3D*> grid_list_;
		};

	} // namespace VIEW
} // namespace BALL

#endif
