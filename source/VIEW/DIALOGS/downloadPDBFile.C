// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: downloadPDBFile.C,v 1.34.16.1 2007/03/25 22:01:56 oliver Exp $
//

#include <BALL/VIEW/DIALOGS/downloadPDBFile.h>

#include <BALL/SYSTEM/TCPTransfer.h>
#include <BALL/KERNEL/system.h>
#include <BALL/FORMAT/lineBasedFile.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/KERNEL/threads.h>

#include <QtGui/qlineedit.h> 
#include <QtGui/qradiobutton.h>
#include <QtGui/qcheckbox.h>
#include <QtGui/qimage.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qapplication.h>

#ifdef BALL_HAS_SSTREAM
# include <sstream>
#else
# include <strstream>
#endif

namespace BALL
{
	namespace VIEW
	{

DownloadPDBFile::DownloadPDBFile(QWidget* parent, const char* name, bool, Qt::WFlags fl)
	: QDialog(parent, fl),
		Ui_DownloadPDBFileData(),
		ModularWidget(name),
		thread_(0),
		aborted_(false),
		error_(false),
		prefix_("http://www.rcsb.org/pdb/files/"),
		suffix_(".pdb")
{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "new DownloadPDBFile" << this << std::endl;
#endif
	setupUi(this);
	setObjectName(name);
	
	// signals and slots connections
	connect( buttonClose, SIGNAL( clicked() ), this, SLOT( close() ) );
	connect( download, SIGNAL( clicked() ), this, SLOT( slotDownload() ) );
	connect( button_abort, SIGNAL( clicked() ), this, SLOT( abort() ) );
	connect( pdbId, SIGNAL( editTextChanged(const QString&) ), this, SLOT( idChanged() ) );

	// register the widget with the MainControl
	registerWidget(this);
	hide();
	pdbId->setFocus();

	thread_ = new FetchHTMLThread();
	thread_->setMainControl(getMainControl());
}

DownloadPDBFile::~DownloadPDBFile()
{
#ifdef BALL_VIEW_DEBUG
	Log.info() << "Destructing object " << this << " of class DownloadPDBFile" << std::endl; 
#endif 

	if (thread_ != 0) 
	{
		if (thread_->isRunning())
		{
			thread_->terminate();
			thread_->wait();
		}
		delete thread_;
	}
}

void DownloadPDBFile::initializeWidget(MainControl&)
{
	String description = "Shortcut|File|Open|Download_PDB";
	menu_id_ = insertMenuEntry(MainControl::FILE_OPEN, "Download PDB", this,
															 SLOT(show()), description,
															 QKeySequence("Ctrl+T"));

	setMenuHint("Download a PDB file from www.rcsb.org");
	setMenuHelp("tips.html#download_pdb");
	setIcon("actions/download-pdb", true);
}

bool DownloadPDBFile::threadedDownload_(const String& url)
{
	error_ = false;
	downloadStarted_();
	thread_->setURL(url);
	thread_->start();
	Size last_bytes = 0;
	while (thread_->isRunning())
	{
		qApp->processEvents();
		
		Size bytes = thread_->getTCPTransfer().getReceivedBytes();
		if (bytes != last_bytes)
		{
			setStatusbarText("received " + String(bytes) + " bytes", true);
			last_bytes = bytes;
		}
		
		thread_->wait(200);
	}

	if (aborted_) return false;

	TCPTransfer::Status status = thread_->getTCPTransfer().getStatusCode();

	if (thread_->getTCPTransfer().getReceivedBytes() == 0 ||
			status != TCPTransfer::OK)
	{
		if (status!=TCPTransfer::CONNECT__ERROR && status!=TCPTransfer::PROXY__ERROR)
		{
			setStatusbarText(String("Could not download the given file. Maybe it does not exist on pdb.org? ") +
											 thread_->getTCPTransfer().getErrorCode() + " occured.", true);
		}
		else
		{
			setStatusbarText(String("Failed to download file, an ") + 
											 thread_->getTCPTransfer().getErrorCode() + " occured. " +
											 "Maybe you need a proxy?" , true);
		}
		error_ = true;
		return false;
	}

	return true;
}

void DownloadPDBFile::slotDownload()
{
	System *system = new System();

	try
	{
		String id = ascii(pdbId->currentText());
		String url = prefix_;
		url += id;
		url += suffix_;
		String temp_filename = VIEW::createTemporaryFilename();
		thread_->setFilename(temp_filename);
		bool ok = threadedDownload_(url);
		downloadEnded_();

		if (!ok) 
		{
			delete system;
			return;
		}

		PDBFile pdb_file(temp_filename);

		pdb_file >> *system;
		pdb_file.close();

 		removeFile_(temp_filename);

		if (system->countAtoms() == 0)
		{
			delete system;
			show();
			setStatusbarText("Could not fetch the given PDBFile", true);
			return;
		}
		else
		{
			setStatusbarText(String("read ") + String(system->countAtoms()) + " atoms for:  " + id, true);
		}

		if (system->getName() == "")
		{
			system->setName(ascii(pdbId->currentText()));
		}

		system->setProperty("FROM_FILE", url);
		close();
		pdbId->clearEditText();
		getMainControl()->insert(*system, ascii(pdbId->currentText()));
		
		notify_(new CompositeMessage(*system, CompositeMessage::CENTER_CAMERA));
		
		download->setDefault(true);
		pdbId->setFocus();
	}
	catch(...)
	{
		setStatusbarText("download PDB file failed", true);
		delete system;
	}
}

void DownloadPDBFile::idChanged()
{
	download->setEnabled(pdbId->currentText() != "");
}

void DownloadPDBFile::abort()
{
	if (thread_ == 0) return;
	aborted_ = true;

	thread_->abort();
	thread_->wait(5500);

	if (thread_->isRunning())
	{
		thread_->terminate();
		thread_->wait();
	}
	removeFile_(thread_->getFilename());
	
	downloadEnded_();
}

void DownloadPDBFile::downloadStarted_()
{
	aborted_ = false;
	error_   = false;
	setStatusbarText("Started download, please wait...", true);
	button_abort->setEnabled(true);
	download->setEnabled(false);
	pdbId->setEnabled(false);
	buttonClose->setEnabled(false);
}

void DownloadPDBFile::downloadEnded_()
{
	if (!aborted_ && !error_)
	{
		setStatusbarText("Finished downloading, please wait...", true);
	}
	button_abort->setEnabled(false);
	download->setEnabled(true);
	pdbId->setEnabled(true);
	buttonClose->setEnabled(true);
	idChanged();
	qApp->processEvents();
	pdbId->setFocus();

	if (error_)
	{
		removeFile_(thread_->getFilename());
	}
}

void DownloadPDBFile::removeFile_(const String& filename)
{
	try
	{
 		File::remove(filename);
	}
	catch(...) {}
}

void DownloadPDBFile::setProxyAndTransfer_(TCPTransfer& tcp)
{
	MainControl* mc = getMainControl();
 	tcp.setProxy(mc->getProxy(), mc->getProxyPort());
	tcp.transfer();
}

void DownloadPDBFile::fetchPreferences(INIFile& inifile)
{
	if (!inifile.hasSection("PDBFiles") ||
			!inifile.hasEntry("PDBFiles", "History"))
	{
		return;
	}

	String files = inifile.getValue("PDBFiles", "History");
	vector<String> fields;
	files.split(fields, "# ");
	for (Position p = 0; p < fields.size(); p++)
	{
		QString file(fields[p].c_str());
		if (pdbId->findText(file) == -1)
		{
			pdbId->addItem(file);
		}
	}
}

void DownloadPDBFile::writePreferences(INIFile& inifile)
{
	String files;
	for (Index p = 0; p < pdbId->count(); p++)
	{
		String s = ascii(pdbId->itemText(p));
		if (s == "") continue;
		files += s + "#";
	}

	inifile.appendSection("PDBFiles");
	inifile.insertValue("PDBFiles", "History", files);
}

void DownloadPDBFile::checkMenu(MainControl& mc)
{
	menu_id_->setEnabled(!mc.compositesAreLocked());
}

	} // namespace VIEW
} // namespace BALL
