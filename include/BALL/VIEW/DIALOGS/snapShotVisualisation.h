#ifndef BALL_VIEW_DIALOG_SNAPSHOTVISUALISATIONDIALOG_H
#define BALL_VIEW_DIALOG_SNAPSHOTVISUALISATIONDIALOG_H

#ifndef BALL_VIEW_KERNEL_MODULARWIDGET_H
#include <BALL/VIEW/KERNEL/modularWidget.h>
#endif

#include <BALL/VIEW/UIC/snapShotVisualisationData.h>
#include <qstring.h>

namespace BALL
{
	class SnapShotManager;

	namespace VIEW
	{

/** Dialog for visualising SnapShot instances
		\ingroup ViewDialogs
*/
class SnapshotVisualisationDialog 
	: public SnapshotVisualisationDialogData, 
		public ModularWidget
{

	Q_OBJECT

	public:	

	/// Constructor
	SnapshotVisualisationDialog
		(QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );

	/// Destructor
	~SnapshotVisualisationDialog() throw();
	
	///This function sets a SnapShotManager
	void setSnapShotManager(SnapShotManager* snapshot_manager);
  
	///This function gets a SnapShotManager
	const SnapShotManager* getSnapShotManager() 
		{return (snap_shot_manager_);}

	public slots:

	///Function to select the analogous Snapshot
	virtual void firstSnapshotClicked();
	
	///Function to select the analogous Snapshot
	virtual void oneForwardClicked();
	
	///Function to select the analogous Snapshot
	virtual void oneBackwardClicked();
	
	///Function to select the analogous Snapshot
	virtual void tenForwardClicked();
	
	///Function to select the analogous Snapshot
	virtual void tenBackwardClicked();
	
	///Function to select the analogous Snapshot
	virtual void hundredForwardClicked();
	
	///Function to select the analogous Snapshot
	virtual void hundredBackwardClicked();
	
	///Function to select the analogous Snapshot
	virtual void lastSnapshotClicked();
	
	///Function to select the analogous Snapshot
	virtual void animateClicked();

	///
	virtual void close();
	
	///
	Size getStartSnapshot() const;

	///
	Size getEndSnapshot() const;
	

	protected:
  
	void update_();
	void forward(Size nr);
  void backward(Size nr);

	QString 					tmp_;
  SnapShotManager* 	snap_shot_manager_;
	Size 							number_of_snapshots_;
};

} } // namespaces

#endif //BALL_VIEW_DIALOG_SNAPSHOTVISUALISATIONDIALOG_H
