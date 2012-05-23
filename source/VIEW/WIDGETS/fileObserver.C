// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/WIDGETS/fileObserver.h>
#include <BALL/VIEW/DIALOGS/molecularFileDialog.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/KERNEL/system.h>

//#include <QtCore/QFSFileEngine>
#include <QtWidgets/QFileDialog>

namespace BALL
{
	namespace VIEW
	{

FileObserver::FileObserver(QWidget *parent, const char *name)
	: DockWidget(parent, name)
{
 	hide();
 	default_visible_ = false;
	registerWidget(this);
	connect(&timer_, SIGNAL(timeout()), SLOT(updateFile()));
	timer_.setInterval(1000);
}

FileObserver::~FileObserver()
{
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "Destructing object " << (void *)this 
								<< " of class FileObserver" << std::endl;
	#endif 
}

void FileObserver::updateFile()
{
	// aquire lock on composites:
	if (file_name_ == "" || !lockComposites()) return;

	// Compare time of file modification with the time of the last update
//	QDateTime l1 = QFSFileEngine(file_name_.c_str()).fileTime(QAbstractFileEngine::ModificationTime);
//	QDateTime l2 = QFSFileEngine(file_name_.c_str()).fileTime(QAbstractFileEngine::CreationTime);
	QDateTime last_mod; //= BALL_MAX(l1, l2);
	if (last_mod <= last_vis_time_) 
	{
		unlockComposites();
		return;
	}

	MolecularFileDialog* mf = MolecularFileDialog::getInstance(0);
	if (mf == 0)
	{
		setStatusbarText(tr("No MolecularFileDialog available, aborting..."), true);
		unlockComposites();
		return;
	}

	System* system = mf->openMolecularFile(file_name_);
	if (system == 0)
	{
		unlockComposites();
		return;
	}

	HashSet<Composite*> composites = getMainControl()->getCompositeManager().getComposites();
	HashSet<Composite*>::Iterator it = composites.begin();
	for (; +it; ++it)
	{
		if (*it != system) getMainControl()->remove(**it);
	}

	last_vis_time_ = last_mod;
	String time(ascii(last_vis_time_.toString("hh:mm:ss")));
	setStatusbarText(String("File observer update: ") + time, true);

	unlockComposites();
}

void FileObserver::start()
{
	if (file_name_ == "") return;
	last_vis_time_ = QDateTime();
	setStatusbarText(String("Starting observer for ") + file_name_, true);
	timer_.start();
}

void FileObserver::stop()
{
	timer_.stop();
}

void FileObserver::setFileName(String filename)
{
	file_name_ = filename;
}

String FileObserver::getFileName() const
{
	return file_name_;
}

void FileObserver::setUpdateInterval(Size msec)
{
	timer_.setInterval(msec);
}

void FileObserver::initializeWidget(MainControl&)
{
	start_action_ = 
		insertMenuEntry(MainControl::FILE_MONITOR, tr("Monitor Molecular File"), this, SLOT(chooseFile()),
		                "Shortcut|File|Monitor|Start", QKeySequence(), tr("Monitor a molecular file for changes"),
										UIOperationMode::MODE_ADVANCED);

	stop_action_ = 
		insertMenuEntry(MainControl::FILE_MONITOR, tr("Stop monitoring"), this, SLOT(stop()),
		                "Shortcut|File|Monitor|Stop", QKeySequence(), tr("Stop monitoring a molecular file"),
										UIOperationMode::MODE_ADVANCED);
}

// select a molecular file to monitor
void FileObserver::chooseFile()
{
	stop();

	MolecularFileDialog* mf = MolecularFileDialog::getInstance(0);
	if (mf == 0) return;

	QString file = QFileDialog::getOpenFileName(0,
										"Choose a molecular file to monitor for changes",
										getWorkingDir().c_str(),
										mf->getSupportedFileFormats().c_str());

	if (ascii(file) == "") return;

	file_name_ = ascii(file);
	start();
}

void FileObserver::checkMenu(MainControl& main_control)
{
	if (stop_action_)
		stop_action_->setEnabled(timer_.isActive());
	if (start_action_)
		start_action_->setEnabled(!main_control.isBusy());
}

	} // VIEW
} // namespace BALL
