#ifndef BALL_VIEW_DIALOG_SNAPSHOTVISUALISATIONDIALOG_H
#define BALL_VIEW_DIALOG_SNAPSHOTVISUALISATIONDIALOG_H

#ifndef BALL_VIEW_KERNEL_MODULARWIDGET_H
#include <BALL/VIEW/KERNEL/modularWidget.h>
#endif

#include <BALL/VIEW/UIC/snapShotVisualisationData.h>

namespace BALL
{
	class SnapShotManager;

	namespace VIEW
	{

/** Dialog to visualise SnapShot instances
 		@see SnapShotManager
		@see SnapShot
		\ingroup ViewDialogs
*/
class BALL_EXPORT SnapshotVisualisationDialog 
	: public SnapshotVisualisationDialogData, 
		public ModularWidget
{
	Q_OBJECT

	public:	

	BALL_EMBEDDABLE(SnapshotVisualisationDialog, ModularWidget)

	/// Constructor
	SnapshotVisualisationDialog
		(QWidget* parent = 0, const char* name = 0);//, bool modal = FALSE, WFlags fl = 0);

	/// Destructor
	~SnapshotVisualisationDialog() throw();
	
	///This function sets the SnapShotManager
	void setSnapShotManager(SnapShotManager* snapshot_manager);
  
	///This function gets the SnapShotManager
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
	
	///test wether input in line edits startSnapshot or endSnapshot are valid
	virtual void snapShotInputTest();
	
	///
	Size getStartSnapshot() const;

	///
	Size getEndSnapshot() const;
	
	///
	virtual void sliderMovedToPos();
	
	///
	virtual void animationSpeedChanged();
	
	///
	virtual void checkNoLoop();
	
	///
	virtual void checkLoop();
	
	///
	virtual void checkRock();
	
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
