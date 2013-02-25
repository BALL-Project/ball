// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:

#include <BALL/VIEW/DIALOGS/downloadElectronDensity.h>
#include <BALL/VIEW/DIALOGS/downloadPDBFile.h>

#include <BALL/KERNEL/system.h>
#include <BALL/FORMAT/lineBasedFile.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/WIDGETS/datasetControl.h>
#include <BALL/VIEW/DATATYPE/standardDatasets.h>
#include <BALL/FORMAT/DSN6File.h>
#include <BALL/FORMAT/CCP4File.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/KERNEL/threads.h>

#include <fstream>
#include <iostream>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>

#include <QtGui/QLineEdit> 
#include <QtGui/QRadioButton>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QImage>
#include <QtGui/QPushButton>
#include <QtGui/QApplication>
#include <QtGui/QProgressBar>

#include <QtCore/QUrl>
#include <QtCore/QFile>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

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
			: QDialog(parent, fl),
				Ui_DownloadElectronDensityData(),
				ModularWidget(name),
				aborted_(false),
				error_(false),
				eds_prefix_("http://eds.bmc.uu.se/eds/dfs/"),
				eds_infix_("/"),
				eds_suffix_(".omap"),
				emdb_prefix_("ftp://emdb.rutgers.edu/structures/EMD-"),
				emdb_infix_("/map/"),
				emdb_suffix_(".map.gz"),
				current_reply_(0),
				progress_bar_(0),
				network_manager_(0)
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
		}

		DownloadElectronDensity::~DownloadElectronDensity()
		{
#ifdef BALL_VIEW_DEBUG
			Log.info() << "Destructing object " << this << " of class DownloadElectronDensity" << std::endl; 
#endif 

			if (current_reply_)
			{
				current_reply_->abort();
				delete(current_reply_);
			}

			if (progress_bar_)
				delete progress_bar_;
		}

		void DownloadElectronDensity::initializeWidget(MainControl&)
		{
			String description = "Shortcut|File|Open|Download_Electron_Density";
			menu_id_ = insertMenuEntry(MainControl::FILE_OPEN, tr("Download Electron Density"), this,
																	 SLOT(show()), description, QKeySequence("Ctrl+D"), 
																	 tr("Download an electron density map from EDS or EMDB"),
																	 UIOperationMode::MODE_ADVANCED);
			setMenuHelp(menu_id_, "tips.html#download_edmap");
			setIcon(menu_id_, "actions/download-electrondensity", true);
		}

		void DownloadElectronDensity::slotDownload()
		{
				String id = ascii(entryId->currentText());
				id.toLower();
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
				//thread_->setFilename(temp_filename);
				//bool ok = threadedDownload_(url);
				//downloadEnded_();
			
				if (progress_bar_)
					delete(progress_bar_);

				progress_bar_ = new QProgressBar(progress_label);
				progress_bar_->resize(progress_label->size());
				progress_bar_->show();

				network_manager_ = new QNetworkAccessManager(this);
				current_reply_ = network_manager_->get(QNetworkRequest(QUrl(url.c_str())));
				connect(current_reply_, SIGNAL(finished()), this, SLOT(downloadFinished()));
				connect(current_reply_, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(downloadProgress(qint64, qint64)));
		}

		void DownloadElectronDensity::downloadFinished()
		{
			String id = ascii(entryId->currentText());
			if(eds_maptype->currentIndex() == 1)//diff map
			{
				id += "_diff";
			}
			
			if (current_reply_->error() != QNetworkReply::NoError)
			{
				Log.error() << "Could not download Electron Density map! Reason given was \"" + (String)(current_reply_->errorString()) + "\""<< std::endl;
				setStatusbarText(tr("Could not download electron Density map! Reason given was") + " \"" + current_reply_->errorString() + "\"");

				error_ = true;
			}
			else
			{
				RegularData3D* d3 = new RegularData3D();
				RegularData3DDataset* set = new RegularData3DDataset();


				try {
					String temp_filename = VIEW::createTemporaryFilename();

					QFile outfile(temp_filename.c_str());
					outfile.open(QIODevice::ReadWrite);

					outfile.write(current_reply_->readAll());
					outfile.close();
				
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

					setStatusbarText(tr("read ") + id.c_str() + tr(" electron density map"), true);

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
					if((server->currentIndex() == 0) && eds_downloadpdb->isChecked()) pdbDownloadChecked();

					close();
					entryId->clearEditText();

					download->setDefault(true);
					entryId->setFocus();
				}
				catch(...)
				{
					setStatusbarText(tr("download Electron Density map failed"), true);
					delete d3;
					delete set;
				}
			}
				
			current_reply_->deleteLater();
			current_reply_ = 0;
			
			network_manager_->deleteLater();
			network_manager_ = 0;

			progress_bar_->deleteLater();
			progress_bar_ = 0;

			downloadEnded_();
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
			DownloadPDBFile* dpf = DownloadPDBFile::getInstance(0);
			dpf->pdbId->setEditText(entryId->currentText());	
			dpf->slotDownload();
		}

		void DownloadElectronDensity::downloadProgress(qint64 received, qint64 total)
		{
			progress_bar_->setRange(0, total);
			progress_bar_->setValue(received);

			setStatusbarText(tr("received") + " " + QString::number(received) + " / " + QString::number(total) + " " + tr("bytes"), true);
		}

		void DownloadElectronDensity::abort()
		{
			if (current_reply_)
				current_reply_->abort();

			downloadEnded_();
		}

		void DownloadElectronDensity::downloadStarted_()
		{
			aborted_ = false;
			error_   = false;
			setStatusbarText(tr("Started download, please wait..."), true);
			button_abort->setEnabled(true);
			download->setEnabled(false);
			entryId->setEnabled(false);
			buttonClose->setEnabled(false);
		}

		void DownloadElectronDensity::downloadEnded_()
		{
			if (!aborted_ && !error_)
			{
				setStatusbarText(tr("Finished downloading, please wait..."), true);
			}
			button_abort->setEnabled(false);
			download->setEnabled(true);
			entryId->setEnabled(true);
			buttonClose->setEnabled(true);
			idChanged();
			qApp->processEvents();
			entryId->setFocus();

		}

		void DownloadElectronDensity::removeFile_(const String& filename)
		{
			try
			{
				File::remove(filename);
			}
			catch(...) {}
		}

		void DownloadElectronDensity::fetchPreferences(INIFile& inifile)
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
		{
			if (menu_id_)
				menu_id_->setEnabled(!mc.compositesAreLocked());
		}

	} // namespace VIEW
} // namespace BALL
