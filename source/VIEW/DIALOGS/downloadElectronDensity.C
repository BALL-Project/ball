// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:

#include <BALL/VIEW/DIALOGS/downloadElectronDensity.h>

#include <BALL/SYSTEM/TCPTransfer.h>
#include <BALL/KERNEL/system.h>
#include <BALL/FORMAT/lineBasedFile.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/WIDGETS/datasetControl.h>
#include <BALL/VIEW/DATATYPE/standardDatasets.h>
#include <BALL/FORMAT/DSN6File.h>
#include <BALL/FORMAT/CCP4File.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/KERNEL/threads.h>

#include <fstream>
#include <iostream>
//#include "/usr/include/boost/iostreams/filtering_streambuf.hpp"
//#include "/usr/include/boost/iostreams/copy.hpp"
//#include "/usr/include/boost/iostreams/filter/gzip.hpp"
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>

#include <QtGui/qlineedit.h> 
#include <QtGui/qradiobutton.h>
#include <QtGui/qcheckbox.h>
#include <QtGui/qcombobox.h>
#include <QtGui/qimage.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/qapplication.h>

#include <QtNetwork/qhttp.h>
#include <QtNetwork/qftp.h>

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
			eds_prefix_("http://eds.bmc.uu.se/eds/dfs/"),
			eds_infix_("/"),
			eds_suffix_(".omap"),
			emdb_prefix_("ftp://ftp1.ebi.ac.uk/pub/databases/emdb/structures/EMD-"),
			emdb_infix_("/map/"),
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
			connect( server, SIGNAL( currentIndexChanged(int) ), this, SLOT( serverChanged() ) );

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

		void DownloadElectronDensity::slotDownload()
		{
			RegularData3D* d3 = new RegularData3D();
			RegularData3DDataset* set = new RegularData3DDataset();

			//try
			//{
				String id = ascii(entryId->currentText());
				String url = "";
				String filename_onserver = ""; 
				String temp_filename = VIEW::createTemporaryFilename(); 
				if(server->currentIndex() == 0) //if EDS
				{
					url += eds_prefix_;
					url += id(1,2);
					url += eds_infix_;
					url += id;
					url += eds_infix_;
					if(eds_maptype->currentIndex() == 1)//diff map
					{
						id += "_diff";
					}
					filename_onserver += id;
					filename_onserver += eds_suffix_;
					url += filename_onserver;
				}
				else
				{
					url += emdb_prefix_; 
					url += id;
					url += emdb_infix_;
					id = "emd_" + id;
					filename_onserver += id;
					filename_onserver += emdb_suffix_;
					url += filename_onserver;
				}
				thread_->setFilename(temp_filename);
				cout << url << endl;
				bool ok = threadedDownload_(url);
				downloadEnded_();
			

				if (!ok) 
				{
					delete d3;
					delete set;
					cout << "Mist" << endl;
					return;
				}

				if(server->currentIndex() == 0)
				{
					DSN6File map_file(temp_filename);
					map_file.read(*d3);
					map_file.close();
				}
				else
				{
					//String unzipped_filename = filename.before(".gz");
					String unzipped_filename = VIEW::createTemporaryFilename();
					ifstream file(temp_filename.c_str(), ios_base::in | ios_base::binary);
					//cout << "ifstream build" << endl;
					boost::iostreams::filtering_streambuf<boost::iostreams::input> in;
					//cout << "filtering_streambuf build" << endl;
					in.push(boost::iostreams::gzip_decompressor());
					//cout << "unzipper set up" << endl;
					in.push(file);
					//cout << "handed file to unzipper" << endl;
					CCP4File map_file(unzipped_filename, ios::out | ios::binary);
					boost::iostreams::copy(in, map_file);
					map_file.reopen(ios::in);
					//cout << "copied unzipped stream to map_file" << endl;
					//CCP4File map_file(filename.c_str(), ios_base::in | ios_base::binary);
					//boost::iostreams::filtering_streambuf<boost::iostreams::input> in;
					//in.push(boost::iostreams::gzip_decompressor());
					//map_file.reopen(ios::out | ios::binary);
					//boost::iostreams::copy(in, map_file);
					//map_file.reopen(ios::in);

					map_file.read(*d3);
					map_file.close();
					removeFile_(unzipped_filename);
				}

				set->setData(d3);
				set->setName(id);
				set->setType(RegularData3DController::type);

				notify_(new DatasetMessage(set, DatasetMessage::ADD));
				removeFile_(temp_filename);

				setStatusbarText(String("read ") + id + " electron density map", true);

				DatasetController* dc = DatasetControl::getInstance(0)->getController(RegularData3DController::type);
				RegularData3DController& rcon = *(RegularData3DController*) dc;

				if((eds_maptype->currentIndex() == 1) && (server->currentIndex() == 0))//diff map
				{
					rcon.computeIsoContourSurface(*set, ColorRGBA(0,255,0), 3*d3->calculateSD());
					rcon.computeIsoContourSurface(*set, ColorRGBA(255,0,0), -3*d3->calculateSD());
				}
				else
				{
					rcon.computeIsoContourSurface(*set, ColorRGBA(0,100,255), d3->calculateSD());
				}

				close();
				entryId->clearEditText();

				download->setDefault(true);
				entryId->setFocus();
			//catch(...)
			//{
			//	setStatusbarText("download PDB file failed", true);
			//	//delete system;
			//}
		}

		void DownloadElectronDensity::idChanged()
		{
			download->setEnabled(entryId->currentText() != "");
		}

		void DownloadElectronDensity::typeChanged()
		{
		}

		void DownloadElectronDensity::serverChanged()
		{
			//disable eds options if anything other than EDS is selected
			eds_options->setDisabled(server->currentIndex());
		}

		void DownloadElectronDensity::pdbDownloadChecked()
		{
		}


		void DownloadElectronDensity::abort()
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

		void DownloadElectronDensity::downloadStarted_()
			throw()
		{
			aborted_ = false;
			error_   = false;
			setStatusbarText("Started download, please wait...", true);
			button_abort->setEnabled(true);
			download->setEnabled(false);
			entryId->setEnabled(false);
			buttonClose->setEnabled(false);
		}

		void DownloadElectronDensity::downloadEnded_()
			throw()
		{
			if (!aborted_ && !error_)
			{
				setStatusbarText("Finished downloading, please wait...", true);
			}
			button_abort->setEnabled(false);
			download->setEnabled(true);
			entryId->setEnabled(true);
			buttonClose->setEnabled(true);
			idChanged();
			qApp->processEvents();
			entryId->setFocus();

			if (error_)
			{
				removeFile_(thread_->getFilename());
			}
		}

		void DownloadElectronDensity::removeFile_(const String& filename)
		{
			try
			{
				File::remove(filename);
			}
			catch(...) {}
		}

		void DownloadElectronDensity::setProxyAndTransfer_(TCPTransfer& tcp)
		{
			MainControl* mc = getMainControl();
			tcp.setProxy(mc->getProxy(), mc->getProxyPort());
			tcp.transfer();
		}

		void DownloadElectronDensity::fetchPreferences(INIFile& inifile)
			throw()
			{
				if (!inifile.hasSection("ElectronDensityFiles") ||
						!inifile.hasEntry("ElectronDensityFiles", "History"))
				{
					return;
				}

				String files = inifile.getValue("ElectronDensityFiles", "History");
				vector<String> fields;
				files.split(fields, "# ");
				for (Position p = 0; p < fields.size(); p++)
				{
					QString file(fields[p].c_str());
					if (entryId->findText(file) == -1)
					{
						entryId->addItem(file);
					}
				}
			}

		void DownloadElectronDensity::writePreferences(INIFile& inifile)
			throw()
			{
				String files;
				for (Index p = 0; p < entryId->count(); p++)
				{
					String s = ascii(entryId->itemText(p));
					if (s == "") continue;
					files += s + "#";
				}

				inifile.appendSection("ElectronDensityFiles");
				inifile.insertValue("ElectronDensityFiles", "History", files);
			}

		void DownloadElectronDensity::checkMenu(MainControl& mc)
			throw()
		{
			menu_id_->setEnabled(!mc.compositesAreLocked());
		}

	} // namespace VIEW
} // namespace BALL
