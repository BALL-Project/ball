#include <BALL/VIEW/DIALOGS/snapShotVisualisation.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/KERNEL/system.h>
#include <BALL/MOLMEC/COMMON/snapShotManager.h>
#include <BALL/FORMAT/trajectoryFile.h>

#include <qlineedit.h>
#include <qcheckbox.h>
#include <qslider.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qprogressdialog.h>

namespace BALL
{
	namespace VIEW
	{

SnapshotVisualisationDialog::SnapshotVisualisationDialog(QWidget* parent, const char* name)
	: SnapshotVisualisationDialogData(parent, name),
		ModularWidget(name),
		snap_shot_manager_(0)
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
		snapShotSlider->setValue(1);
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
		snapShotSlider->setValue(snap_shot_manager_->getTrajectoryFile()->getNumberOfSnapShots());
		update_();
	}
  else
	{
		Log.error() << "Unable to apply first snapshot" <<std::endl;
	}
}

void SnapshotVisualisationDialog::animateClicked()
{
	error_ = false;
	Size tempo = getEndSnapshot();
	Size speed = animationSpeedSlider->value();
	bool forward = true;
	QProgressDialog progress("SnapShot Visualisation", "Abort Animation", tempo, 0, "progress", true);
	
	for (Size i = getStartSnapshot(); 
			 i < tempo && !error_ && !progress.wasCancelled(); )
	{
		progress.setProgress(i);
			
		setCaption((String("CurrentSnapshot: ") + String(i)).c_str());
		snapShotSlider->setValue(i);
		update_();

		if (export_PNG->isChecked())
		{
			SceneMessage* message = new SceneMessage(SceneMessage::EXPORT_PNG);
			notify_(message);
		}

		if (export_POV->isChecked())
		{
			SceneMessage* message = new SceneMessage(SceneMessage::EXPORT_POVRAY);
			notify_(message);
		}

		tmp_.setNum(i, 10);
		// speed things up after first snapshot is read
		if (i == getStartSnapshot())
		{
			if (!snap_shot_manager_->applySnapShot(i)) break;
		}
		else
		{
			if (!snap_shot_manager_->applyNextSnapShot()) break;
		}

		if (forward)
		{
			if (speed >= tempo - i)
			{
				i = tempo;
				setCaption((String("CurrentSnapshot: ") + String(i)).c_str());
				snapShotSlider->setValue(i);
				update_();
				
				if (export_PNG->isChecked())
				{
					SceneMessage* message = new SceneMessage(SceneMessage::EXPORT_PNG);
					notify_(message);
				}
				if (export_POV->isChecked())
				{
					SceneMessage* message = new SceneMessage(SceneMessage::EXPORT_POVRAY);
					notify_(message);
				}
				
				if (forwardLoopButton->isChecked())
				{
					i = 1;
					forward = true;
				}
				if (rockLoopButton->isChecked())
				{
					i = tempo-1;
					forward = false;
				}
			}
			else
			{
				i += speed;
			}
		}
		else
		{
			if (speed<=i)
			{
				i -= speed;
			}
			else
			{
				i = 1;
				forward = true;
			}
		}
	}
	
	setCaption("Snapshot Visualisation");
}


void SnapshotVisualisationDialog::backward(Size nr)
{
	if (nr > (Size)currentSnapshot->text().toInt()) 
	{
		firstSnapshotClicked();
	}

  Position tmpnr = (currentSnapshot->text().toInt()) - nr;
 
  if (!snap_shot_manager_->applySnapShot(tmpnr)) 
	{
		Log.error() << "Could not apply  snapshot" <<std::endl;
		error_ = true;
	}

	snapShotSlider->setValue(tmpnr);
	tmp_.setNum(tmpnr);
	update_();
}

void SnapshotVisualisationDialog::forward(Size nr)
{
	Size tmpnr = (currentSnapshot->text().toInt()) + nr;
  if (tmpnr >= snap_shot_manager_->getTrajectoryFile()->getNumberOfSnapShots())
  {
  	lastSnapshotClicked();
  }
	
	if (!snap_shot_manager_->applySnapShot(tmpnr))
	{
	  Log.error() << "Could not apply  snapshot" << std::endl;
		error_ = true;
	}

	snapShotSlider->setValue(tmpnr);
	tmp_.setNum(tmpnr);
	update_();
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
		return 0;
	}
}

void SnapshotVisualisationDialog::sliderMovedToPos()
{
	if (snap_shot_manager_ == 0) return;
	currentSnapshot->setText(String(snapShotSlider->value()).c_str());
	Position tmpnr = (currentSnapshot->text().toInt());	
	
	if (snap_shot_manager_->applySnapShot(tmpnr))
	{
		tmp_.setNum(tmpnr);
		update_();
	}
	else
	{
		Log.error() << "Could not apply  snapshot" <<std::endl;
		error_ = true;
	} 
}

void SnapshotVisualisationDialog::update_()
{
  currentSnapshot->setText(tmp_);
	update();
	const Composite* const composite = snap_shot_manager_ ->getSystem();
	CompositeMessage* msg = new CompositeMessage(*composite, CompositeMessage::CHANGED_COMPOSITE);
	notify_(msg);
}

void SnapshotVisualisationDialog::setSnapShotManager(SnapShotManager* snapshot_manager)  
{
	snap_shot_manager_ = snapshot_manager;
	if (snapshot_manager == 0) return;

  tmp_.setNum(snap_shot_manager_->getTrajectoryFile()->getNumberOfSnapShots());
	numberOfSnapshots->setText(tmp_);
	endSnapshot->setText(tmp_);
	snapShotSlider->setRange(1,snap_shot_manager_->getTrajectoryFile()->getNumberOfSnapShots());
	animationSpeedLineEdit->setText("1x");
	tmp_.setNum(1);
  currentSnapshot->setText(tmp_);
	startSnapshot->setText(tmp_);
}

void SnapshotVisualisationDialog::snapShotInputTest()
{
	String startSnap = startSnapshot->text().ascii();
	String endSnap = endSnapshot->text().ascii();
	String valid_char = "0123456789";
	//test if input is valid
	if (startSnap.size()!=0)
	{
		for (Size i = 0; i!=startSnap.size(); i++)
		{
			if (valid_char.find(startSnap.substr(i,1)) == string::npos)
			{
				//if written char is not a number, set string to old string
				startSnap = startSnap.substr(0,(startSnap.size()-1)); 
				startSnapshot->setText(startSnap.c_str());
				break;
			}
		}
	}
	if (endSnap.size()!=0)
	{
		for (Size i = 0; i!=endSnap.size(); i++)
		{
			if (valid_char.find(endSnap.substr(i,1)) == string::npos)
			{
				//if written char is not a number, set string to old string
				endSnap = endSnap.substr(0,(endSnap.size()-1)); 
				endSnapshot->setText(endSnap.c_str());
				break;
			}
		}
	}

	// set line edits to number of snapshots, if written number is bigger then number of snapshots
	String num_of_shots = numberOfSnapshots->text().ascii();
	String num_of_startsnap = startSnapshot->text().ascii();
	String num_of_endsnap = endSnapshot->text().ascii();
	Size num_shots = num_of_shots.toInt();
	Size num_startsnap = num_of_startsnap.toInt();
	Size num_endsnap = num_of_endsnap.toInt();
	if(num_startsnap > num_shots)
	{
		startSnapshot->setText(num_of_shots.c_str()); 
	}
	
	if(num_endsnap > num_shots) 
	{
		endSnapshot->setText(num_of_shots.c_str()); 
	}

	if (num_startsnap == 0) startSnapshot->setText(String(1).c_str());
}

void SnapshotVisualisationDialog::animationSpeedChanged()
{
	String animationSpeed = String(animationSpeedSlider->value()) + String("x");
	animationSpeedLineEdit->setText(animationSpeed.c_str());
}

void SnapshotVisualisationDialog::checkNoLoop()
{
	if(!noLoopButton->isChecked()) noLoopButton->setChecked(true);
	forwardLoopButton->setChecked(false);
	rockLoopButton->setChecked(false);
}
void SnapshotVisualisationDialog::checkLoop()
{
	if(!forwardLoopButton->isChecked()) forwardLoopButton->setChecked(true);
	noLoopButton->setChecked(false);
	rockLoopButton->setChecked(false);
}
void SnapshotVisualisationDialog::checkRock()
{
	if(!rockLoopButton->isChecked()) rockLoopButton->setChecked(true);
	noLoopButton->setChecked(false);
	forwardLoopButton->setChecked(false);
}

} } // namespace

