#include <BALL/VIEW/DIALOGS/snapShotVisualisation.h>
#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/MOLMEC/COMMON/snapShotManager.h>
#include <BALL/FORMAT/trajectoryFile.h>
#include <qlineedit.h>
#include <qcheckbox.h>

namespace BALL
{
	namespace VIEW
	{

SnapshotVisualisationDialog::SnapshotVisualisationDialog
	(QWidget* parent,  const char* name)//, bool modal, WFlags fl)
	: SnapshotVisualisationDialogData(parent, name),//, modal, fl),
		ModularWidget(name)
{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "new SnapshotVisualisationDialog" << this << std::endl;
#endif
	tmp_.setNum(1);
	ModularWidget::registerWidget(this);
}

SnapshotVisualisationDialog::~SnapshotVisualisationDialog() throw()
{
}

void SnapshotVisualisationDialog::firstSnapshotClicked()
{
  if (snap_shot_manager_->applyFirstSnapShot())
	{
		tmp_.setNum(1, 10);
		update_();
	}
  else
	{
		Log.error() << "Unable to apply first snapshot" <<std::endl;
	}
}

void SnapshotVisualisationDialog::oneForwardClicked()
{
	forward(1);
}

void SnapshotVisualisationDialog::oneBackwardClicked()
{
  backward(1);
}

void SnapshotVisualisationDialog::tenForwardClicked()
{
  forward(10);
}

void SnapshotVisualisationDialog::tenBackwardClicked()
{
  backward(10);
}

void SnapshotVisualisationDialog::hundredForwardClicked()
{
  forward(100);
}

void SnapshotVisualisationDialog::hundredBackwardClicked()
{
	backward(100);
}

void SnapshotVisualisationDialog::lastSnapshotClicked()
{
	if (snap_shot_manager_->applySnapShot(
				snap_shot_manager_->getTrajectoryFile()->getNumberOfSnapShots()))
	{
		update_();
	}
  else
	{
		Log.error() << "Unable to apply first snapshot" <<std::endl;
	}
}

void SnapshotVisualisationDialog::animateClicked()
{
	Size tempo = getEndSnapshot();
	for( Size i=getStartSnapshot(); i <= tempo; i ++)
	{
  	tmp_.setNum(i, 10);
		// speed things up after first snapshot is read
		if (i == getStartSnapshot())
		{
			if (!snap_shot_manager_->applySnapShot(i)) return;
		}
		else
		{
			if (!snap_shot_manager_->applyNextSnapShot()) return;
		}

		setCaption("CurrentSnapshot: " + String(i));
		update_();
		if (export_PNG->isChecked())
		{
			SceneMessage* message = new SceneMessage(SceneMessage::EXPORT_PNG);
			notify_(message);
		}
	}
	setCaption("Snapshot Visualisation");
}

void SnapshotVisualisationDialog::close()
{
  lastSnapshotClicked();
	update_();
	if (snap_shot_manager_->getTrajectoryFile())
	{
		delete snap_shot_manager_->getTrajectoryFile();
	}
	delete snap_shot_manager_;
}

void SnapshotVisualisationDialog::backward(Size nr)
{
	if (nr > (Size)currentSnapshot->text().toInt()) 
	{
		firstSnapshotClicked();
		return;
	}

  Position tmpnr = (currentSnapshot->text().toInt()) - nr;
 
  if (snap_shot_manager_->applySnapShot(tmpnr))
	{
		tmp_.setNum(tmpnr);
		update_();
	}
  else
	{
		Log.error() << "Could not apply  snapshot" <<std::endl;
	} 
}

void SnapshotVisualisationDialog::forward(Size nr)
{
	Size tmpnr = (currentSnapshot->text().toInt()) + nr;
  if (tmpnr >= snap_shot_manager_->getTrajectoryFile()->getNumberOfSnapShots())
  {
    lastSnapshotClicked();
  }
  else
  {
    if (snap_shot_manager_->applySnapShot(tmpnr))
		{
	  	tmp_.setNum(tmpnr);
			update_();
		}
    else
		{
	  	Log.error() << "Could not apply  snapshot" <<std::endl;
		} 
  }
}


Size SnapshotVisualisationDialog::getStartSnapshot() const
{
	try
	{
		return (Size)String(startSnapshot->text().ascii()).toUnsignedInt();
	}
	catch(...)
	{
		Log.error() << "Invalid Start-Snapshot" << std::endl;
		return 1;
	}
}


Size SnapshotVisualisationDialog::getEndSnapshot() const
{
	try 
	{
		return (Size)String(endSnapshot->text().ascii()).toUnsignedInt();
	}
	catch(...)
	{
		Log.error() << "Invalid End-Snapshot" << std::endl;
		return number_of_snapshots_;
	}
}

void SnapshotVisualisationDialog::update_()
{
  currentSnapshot->setText(tmp_);
	
	SceneMessage* new_message = new SceneMessage;
	new_message->setType(SceneMessage::REBUILD_DISPLAY_LISTS);
	notify_(new_message);
}

void SnapshotVisualisationDialog::setSnapShotManager(SnapShotManager* snapshot_manager)  
{
	snap_shot_manager_ = snapshot_manager;
  tmp_.setNum(snap_shot_manager_->getTrajectoryFile()->getNumberOfSnapShots());
	numberOfSnapshots->setText(tmp_);
	endSnapshot->setText(tmp_);
	tmp_.setNum(1);
  currentSnapshot->setText(tmp_);
	startSnapshot->setText(tmp_);
}
  
} } // namespace

