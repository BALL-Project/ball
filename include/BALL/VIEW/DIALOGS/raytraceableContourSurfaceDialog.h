// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_DIALOGS_RAYTRACEABLECONTOURSURFACEDIALOG_H
#define BALL_VIEW_DIALOGS_RAYTRACEABLECONTOURSURFACEDIALOG_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifndef BALL_COMMON_EXCEPTION_H
#	include <BALL/COMMON/exception.h>
#endif

#include <BALL/VIEW/UIC/raytraceableContourSurfaceDialogData.h>

#ifndef BALL_VIEW_KERNEL_MODULARWIDGET_H
# include <BALL/VIEW/KERNEL/modularWidget.h>
#endif

#ifndef BALL_VIEW_DATATYP_RAYTRACEABLEGRID_H
# include <BALL/VIEW/DATATYPE/raytraceableGrid.h>
#endif

#include <QtGui/QDialog>

namespace BALL
{
	namespace VIEW
	{
		/** Dialog for selecting contour urface thresholds for the raytracing mode of scene.
		\ingroups ViewDialogs
*/
class BALL_VIEW_EXPORT RaytraceableContourSurfaceDialog
	: public QDialog,
		public Ui_RaytraceableContourSurfaceDialogData,
		public ModularWidget

{
	Q_OBJECT

	public:

	BALL_EMBEDDABLE(RaytraceableContourSurfaceDialog, ModularWidget)

	///
	RaytraceableContourSurfaceDialog(RaytraceableGrid* grid, QWidget* parent = 0, const char* name = "RaytraceableContourSurfaceDialog", bool modal = FALSE, Qt::WFlags fl = 0);

	///
	~RaytraceableContourSurfaceDialog()
		throw();

	void setGrid(RaytraceableGrid* grid) {grid_ = grid;};
	RaytraceableGrid* getGrid() {return grid_;};

	public slots:
	
	///
	void add();
	
	///
	void accept();
	
	///
	void reject();
	
	///
	void changedSliderThreshold(int a);	
	
	///
	void changedEditThreshold();

	//  
	void changedRangeMin();

	//  
	void changedRangeMax();
	
	//  
	void changedRangeSteps();


	//void setDefaultRangeValues_();
	
	protected slots:

	void setDefaultRangeValues_();


	protected:
	
	void addModel_(float threshold) throw();
	
	private:
	
	float getValue_(const QAbstractSlider* slider) const
		throw();
	float getValue_(const QLineEdit* edit) const
		throw(Exception::InvalidFormat);
	

	// the sliders min and max
	float min_;
	float max_;

	int steps_;
	float current_threshold_;

	RaytraceableGrid* grid_;

};

	
	} // namespaces
}

#endif //BALL_VIEW_DIALOGS_RAYTRACEABLECONTOURSURFACEDIALOG_H

