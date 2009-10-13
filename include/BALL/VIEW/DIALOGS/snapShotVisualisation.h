#ifndef BALL_VIEW_DIALOG_SNAPSHOTVISUALISATIONDIALOG_H
#define BALL_VIEW_DIALOG_SNAPSHOTVISUALISATIONDIALOG_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifndef BALL_VIEW_KERNEL_MODULARWIDGET_H
#include <BALL/VIEW/KERNEL/modularWidget.h>
#endif

#include <BALL/VIEW/UIC/ui_snapShotVisualisation.h>

#include <QtGui/QDialog>

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
class BALL_VIEW_EXPORT SnapshotVisualisationDialog 
	: public QDialog,
		public Ui_SnapshotVisualisationDialogData, 
		public ModularWidget
{
	Q_OBJECT

	public:	

	BALL_EMBEDDABLE(SnapshotVisualisationDialog, ModularWidget)

	/// Constructor
	SnapshotVisualisationDialog(QWidget* parent = 0, const char* name = "SnapshotVisualisationDialog");

	/// Destructor
	~SnapshotVisualisationDialog();
	
	///This function sets the SnapShotManager
	void setSnapShotManager(SnapShotManager* snapshot_manager);
  
	///This function gets the SnapShotManager
	const SnapShotManager* getSnapShotManager() {return snap_shot_manager_;}
	
	///
	Size getStartSnapshot() const;

	///
	Size getEndSnapshot() const;
	
	public slots:

	///
	void show();

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

	///test wether input in line edits startSnapshot or endSnapshot are valid
	virtual void snapShotInputTest();

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

	///
	void cancelPressed();
	
	protected slots:

	void sliderDragStarted_();
	void sliderDragEnded_();

	protected:

	void stop_();
	virtual void closeEvent(QCloseEvent*);
	void update_();
	void forward(Size nr);
  void backward(Size nr);

	MainControl* 			main_control_;
	QString 					tmp_;
  SnapShotManager* 	snap_shot_manager_;
	bool 							error_;
	bool 							cancel_;
	bool 							animation_running_;
	bool 							drag_;
};

} } // namespaces

#endif //BALL_VIEW_DIALOG_SNAPSHOTVISUALISATIONDIALOG_H
