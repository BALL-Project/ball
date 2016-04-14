// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#ifndef BALL_VIEW_DIALOGS_CONTOURSURFACEDIALOG_H
#define BALL_VIEW_DIALOGS_CONTOURSURFACEDIALOG_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifndef BALL_VIEW_DATATYPE_COLORRGBA_H
# include <BALL/VIEW/DATATYPE/colorRGBA.h>
#endif

#include <BALL/VIEW/UIC/ui_contourSurfaceDialog.h>

namespace BALL
{
 namespace VIEW
 {
	 class DatasetController;
	 class Dataset;

/** Dialog for creating contour surfaces from RegularData3D
    \ingroup  ViewDialogs
 */
class BALL_VIEW_EXPORT ContourSurfaceDialog 
	: public QDialog,
		public Ui_ContourSurfaceDialogData
{ 
    Q_OBJECT

	public:
    ContourSurfaceDialog( QWidget* parent = 0, const char* name = "ContourSurfaceDialog");
    ~ContourSurfaceDialog();

	void setGrid(Dataset* grid);
    Dataset* getGrid();
    double getThreshold() const;
		void setController(DatasetController* controller) {controller_ = controller;}
		ColorRGBA getColor();

	public slots:
		virtual int exec();

	protected slots: 
		virtual void valuesChanged();
		virtual void chooseColor();
	
	private:
		Dataset* grid_;
		DatasetController* controller_;
};

} } // namespaces
#endif // CONTOURSURFACEDIALOG_H
