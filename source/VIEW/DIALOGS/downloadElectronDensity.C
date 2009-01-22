// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:

#include <BALL/VIEW/DIALOGS/downloadElectronDensity.h>

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
#include <QtGui/qcombobox.h>
#include <QtGui/qimage.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qapplication.h>

#ifdef BALL_HAS_SSTREAM
# include <sstream>
#else
# include <strstream>
#endif

using namespace std;

namespace BALL
{
	namespace VIEW
	{

		DownloadElectronDensity::DownloadElectronDensity(QWidget* parent, const char* name, bool, Qt::WFlags fl)
			throw()
			: QDialog(parent, fl),
			Ui_DownloadElectronDensityData(),
			ModularWidget(name),
			thread_(0),
			aborted_(false),
			error_(false),
			eds_prefix_("http://eds.bmc.uu.se/"),
			eds_suffix_(".omap"),
			emdb_prefix_("ftp://ftp.ebi.ac.uk/pub/databases/emdb/structures/"),
			emdb_suffix_(".map.gz")
		{
#ifdef BALL_VIEW_DEBUG
			Log.error() << "new DownloadElectronDensity" << this << std::endl;
#endif
			setupUi(this);
			setObjectName(name);

			// signals and slots connections
			connect( buttonClose, SIGNAL( clicked() ), this, SLOT( close() ) );
			connect( download, SIGNAL( clicked() ), this, SLOT( slotDownload() ) );
			connect( button_abort, SIGNAL( clicked() ), this, SLOT( abort() ) );
			connect( entryId, SIGNAL( editTextChanged(const QString&) ), this, SLOT( idChanged() ) );

			// register the widget with the MainControl
			registerWidget(this);
			hide();
			entryId->setFocus();

			thread_ = new FetchHTMLThread();
			thread_->setMainControl(getMainControl());
		}

		DownloadElectronDensity::~DownloadElectronDensity()
			throw()
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

		void DownloadElectronDensity::initializeWidget(MainControl&)
			throw()
		{
			menu_id_ = insertMenuEntry(MainControl::FILE_OPEN, "Download PDB", this,
					SLOT(show()), Qt::CTRL+Qt::Key_T);
			setMenuHint("Download an electron density map from EDS or EMDB");
			setMenuHelp("tips.html#download_edmap");
			setIcon("download.png", true);
		}

		bool DownloadElectronDensity::threadedDownload_(const String& url)
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

			if (thread_->getTCPTransfer().getReceivedBytes() == 0 ||
					thread_->getTCPTransfer().getStatusCode() != TCPTransfer::OK) 
			{
				if (thread_->getTCPTransfer().getReceivedBytes() == 0)
				{
					setStatusbarText(String("Could not download the given file. Maybe it does not exist? ") +
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
	}
}

