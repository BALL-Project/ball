#include <BALL/VIEW/DIALOGS/snapShotVisualisation.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/KERNEL/system.h>
#include <BALL/MOLMEC/COMMON/snapShotManager.h>
#include <BALL/FORMAT/trajectoryFile.h>
#include <BALL/SYSTEM/systemCalls.h>

#include <QtWidgets/QLineEdit>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QSlider>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QProgressDialog>

namespace BALL
{
	namespace VIEW
	{

SnapshotVisualisationDialog::SnapshotVisualisationDialog(QWidget* parent, const char* name)
	: QDialog(parent),
		Ui_SnapshotVisualisationDialogData(),
		ModularWidget(name),
		snap_shot_manager_(0),
		drag_(false)
{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "new SnapshotVisualisationDialog" << this << std::endl;
#endif
	setupUi(this);

	// signals and slots connections
	connect( animateButton, SIGNAL( clicked() ), this, SLOT( animateClicked() ) );
	connect( cancelButton, SIGNAL( clicked() ), this, SLOT( cancelPressed() ) );
	connect( firstSnapshotButton, SIGNAL( clicked() ), this, SLOT( firstSnapshotClicked() ) );
	connect( tenBackwardButton, SIGNAL( clicked() ), this, SLOT( tenBackwardClicked() ) );
	connect( oneBackwardButton, SIGNAL( clicked() ), this, SLOT( oneBackwardClicked() ) );
	connect( oneForwardButton, SIGNAL( clicked() ), this, SLOT( oneForwardClicked() ) );
	connect( tenForwardButton, SIGNAL( clicked() ), this, SLOT( tenForwardClicked() ) );
	connect( lastSnapshotButton, SIGNAL( clicked() ), this, SLOT( lastSnapshotClicked() ) );
	connect( snapShotSlider, SIGNAL( valueChanged(int) ), this, SLOT( sliderMovedToPos() ) );
	connect( snapShotSlider, SIGNAL( sliderPressed() ), this, SLOT( sliderDragStarted_() ) );
	connect( snapShotSlider, SIGNAL( sliderReleased() ), this, SLOT( sliderDragEnded_() ) );
	connect( startSnapshot, SIGNAL( textChanged(const QString&) ), this, SLOT( snapShotInputTest() ) );
	connect( endSnapshot, SIGNAL( textChanged(const QString&) ), this, SLOT( snapShotInputTest() ) );
	connect( animationSpeedSlider, SIGNAL( valueChanged(int) ), this, SLOT( animationSpeedChanged() ) );
	connect( forwardLoopButton, SIGNAL( clicked() ), this, SLOT( checkLoop() ) );
	connect( rockLoopButton, SIGNAL( clicked() ), this, SLOT( checkRock() ) );
	connect( noLoopButton, SIGNAL( clicked() ), this, SLOT( checkNoLoop() ) );

	animateButton->setEnabled(true);
	cancelButton->setText("Close");

	setObjectName(name);
	tmp_.setNum(1);
	ModularWidget::registerWidget(this);
}

SnapshotVisualisationDialog::~SnapshotVisualisationDialog()
{
}

void SnapshotVisualisationDialog::cancelPressed()
{
	stop_();

	if (animation_running_) return;

	close();
}

void SnapshotVisualisationDialog::firstSnapshotClicked()
{
	if (main_control_->getRepresentationManager().updateRunning()) return;

  if (snap_shot_manager_->applyFirstSnapShot())
	{
		tmp_.setNum(1, 10);
		snapShotSlider->setValue(1);
		update_();
	}
  else
	{
		Log.error() << (String)tr("Unable to apply first snapshot") <<std::endl;
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
	if (main_control_->getRepresentationManager().updateRunning()) return;

	if (snap_shot_manager_->applySnapShot(
				snap_shot_manager_->getTrajectoryFile()->getNumberOfSnapShots()))
	{
		snapShotSlider->setValue(snap_shot_manager_->getTrajectoryFile()->getNumberOfSnapShots());
		update_();
	}
  else
	{
		Log.error() << (String)tr("Unable to apply first snapshot") << std::endl;
	}
}

void SnapshotVisualisationDialog::animateClicked()
{
	if (main_control_->getRepresentationManager().updateRunning()) return;

	cancel_ = false;
	error_ = false;
	Size tempo = getEndSnapshot();
	Size speed = animationSpeedSlider->value();
	bool forward = true;
	animateButton->setEnabled(false);
	sliderBox->setEnabled(false);
	cancelButton->setText("Cancel");
	animation_running_ = true;
	
	for (Size i = getStartSnapshot(); 
			 i < tempo && !error_ && !cancel_; )
	{
		setWindowTitle(((String)tr("CurrentSnapshot") + (String)(": ") + String(i)).c_str());
		snapShotSlider->setValue(i);
		update_();

		if (export_PNG->isChecked())
		{
			notify_(new SceneMessage(SceneMessage::EXPORT_PNG));
		}

		if (export_POV->isChecked())
		{
			notify_(new SceneMessage(SceneMessage::EXPORT_POVRAY));
		}

		tmp_.setNum(i, 10);
		bool ok = true;

		if (i <= 1) 
		{
			ok = snap_shot_manager_->applyFirstSnapShot();
		}
		else if (snap_shot_manager_->getCurrentSnapshotNumber() + 1 == i)
		{
			ok = snap_shot_manager_->applyNextSnapShot();
		}
		else 
		{
			ok = snap_shot_manager_->applySnapShot(i);
		}
		if (!ok) break;

		if (forward)
		{
			if (speed >= tempo - i)
			{
				i = tempo;
				setWindowTitle(((String)tr("CurrentSnapshot") + (String)(": ") + String(i)).c_str());
				snapShotSlider->setValue(i);
				update_();

				if (export_PNG->isChecked())
				{
					notify_(new SceneMessage(SceneMessage::EXPORT_PNG));
				}
				if (export_POV->isChecked())
				{
					notify_(new SceneMessage(SceneMessage::EXPORT_POVRAY));
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
	
	setWindowTitle(tr("Snapshot Visualisation"));
	animateButton->setEnabled(true);
	cancelButton->setText("Close");
	animation_running_ = false;
	sliderBox->setEnabled(true);
}


void SnapshotVisualisationDialog::backward(Size nr)
{
	if (main_control_->getRepresentationManager().updateRunning()) return;

  Index tmpnr = (currentSnapshot->text().toInt()) - (Index) nr;
	if (tmpnr <= 0)
	{
		firstSnapshotClicked();
		tmpnr = 1;
	}
	else
	{
		if (!snap_shot_manager_->applySnapShot(tmpnr)) 
		{
			Log.error() << (String)tr("Could not apply snapshot") << std::endl;
			error_ = true;
		}
	}

	snapShotSlider->setValue(tmpnr);
	tmp_.setNum(tmpnr);
	update_();
}

void SnapshotVisualisationDialog::forward(Size nr)
{
	if (main_control_->getRepresentationManager().updateRunning()) return;

	Size tmpnr = (currentSnapshot->text().toInt()) + nr;
  if (tmpnr >= snap_shot_manager_->getTrajectoryFile()->getNumberOfSnapShots())
  {
  	lastSnapshotClicked();
		tmpnr = snap_shot_manager_->getTrajectoryFile()->getNumberOfSnapShots();
  }
	else
	{
		bool ok = true;
		if (tmpnr == snap_shot_manager_->getCurrentSnapshotNumber() + 1)
		{
			ok = snap_shot_manager_->applyNextSnapShot();
		}
		else
		{
			ok = snap_shot_manager_->applySnapShot(tmpnr);
		}

		if (!ok)
		{
			Log.error() << (String)tr("Could not apply snapshot") << std::endl;
			error_ = true;
		}
	}

	snapShotSlider->setValue(tmpnr);
	tmp_.setNum(tmpnr);
	update_();
}


Size SnapshotVisualisationDialog::getStartSnapshot() const
{
	try
	{
		Size s = (Size)ascii(startSnapshot->text()).toUnsignedInt();
		s = BALL_MAX(s, 1);
		return s;
	}
	catch(...)
	{
		Log.error() << (String)tr("Invalid Start-Snapshot") << std::endl;
		return 1;
	}
}


Size SnapshotVisualisationDialog::getEndSnapshot() const
{
	try 
	{
		return (Size)ascii(endSnapshot->text()).toUnsignedInt();
	}
	catch(...)
	{
		Log.error() << (String)tr("Invalid End-Snapshot") << std::endl;
		return 0;
	}
}

void SnapshotVisualisationDialog::sliderMovedToPos()
{
	if (snap_shot_manager_ == 0 || !drag_) return;
	if (main_control_->getRepresentationManager().updateRunning()) return;

	currentSnapshot->setText(String(snapShotSlider->value()).c_str());
	Position tmpnr = (currentSnapshot->text().toInt());	
	
	bool ok = true;

	if (tmpnr == 1) ok = snap_shot_manager_->applyFirstSnapShot();
	else 						
	{
		if (tmpnr == snap_shot_manager_->getCurrentSnapshotNumber() + 1)
		{
			ok = snap_shot_manager_->applyNextSnapShot();
		}
		else
		{
			ok = snap_shot_manager_->applySnapShot(tmpnr);
		}
	}

	if (ok)
	{
		tmp_.setNum(tmpnr);
		update_();
	}
	else
	{
		Log.error() << (String)tr("Could not apply snapshot") << std::endl;
		error_ = true;
	} 
}

void SnapshotVisualisationDialog::update_()
{
  currentSnapshot->setText(tmp_);
	update();
	notify_(new CompositeMessage(*snap_shot_manager_->getSystem(), CompositeMessage::CHANGED_COMPOSITE));

	while (main_control_->getRepresentationManager().updateRunning())
	{
		QApplication::processEvents();
		sleepFor(10);
	}
}

void SnapshotVisualisationDialog::setSnapShotManager(SnapShotManager* snapshot_manager)  
{
	snap_shot_manager_ = snapshot_manager;
	if (snapshot_manager == 0) return;

	main_control_ = getMainControl();
	if (main_control_ == 0)
	{
		Log.error() << (String)tr("No MainControl available for SnapshotVisualisationDialog!") << std::endl;
		return;
	}

//	snap_shot_manager_->clearBuffer();
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
	String startSnap = ascii(startSnapshot->text());
	String endSnap = ascii(endSnapshot->text());
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
	String num_of_shots = ascii(numberOfSnapshots->text());
	String num_of_startsnap = ascii(startSnapshot->text());
	String num_of_endsnap = ascii(endSnapshot->text());
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

void SnapshotVisualisationDialog::stop_()
{
	cancel_ = true;
	if (!unlockComposites())
	{
		BALLVIEW_DEBUG
	}
}

void SnapshotVisualisationDialog::show()
{
	main_control_ = getMainControl();
	if (main_control_ == 0)
	{
		Log.error() << (String)tr("No MainControl available for SnapshotVisualisationDialog!") << std::endl;
		return;
	}

	if (!lockComposites()) 
	{
		BALLVIEW_DEBUG
		return;
	}
	QDialog::show();
	snap_shot_manager_->applyFirstSnapShot();
	unlockComposites();
}

void SnapshotVisualisationDialog::closeEvent(QCloseEvent*)
{
	stop_();
}

void SnapshotVisualisationDialog::sliderDragStarted_()
{
	drag_ = true;
}

void SnapshotVisualisationDialog::sliderDragEnded_()
{
	drag_ = false;
}

} } // namespace

