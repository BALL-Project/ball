// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/DIALOGS/downloadPDBFile.h>

#include <BALL/KERNEL/system.h>
#include <BALL/FORMAT/lineBasedFile.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/message.h>

#include <fstream>
#include <iostream>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>

#include <QtGui/QLineEdit> 
#include <QtGui/QRadioButton>
#include <QtGui/QCheckBox>
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

namespace BALL
{
	namespace VIEW
	{

		DownloadPDBFile::DownloadPDBFile(QWidget* parent, const char* name, bool, Qt::WFlags fl)
			: QDialog(parent, fl),
				Ui_DownloadPDBFileData(),
				ModularWidget(name),
				aborted_(false),
				error_(false),
				prefix_("http://www.rcsb.org/pdb/files/"),
				suffix_(".pdb"),
				biounit_suffix_(".pdb1.gz"),
				current_reply_(0),
				progress_bar_(0),
				network_manager_(0)
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
		}

		DownloadPDBFile::~DownloadPDBFile()
		{
#ifdef BALL_VIEW_DEBUG
			Log.info() << "Destructing object " << this << " of class DownloadPDBFile" << std::endl; 
#endif 

			if (current_reply_)
			{
				current_reply_->abort();
				delete(current_reply_);
			}

			if (progress_bar_)
				delete progress_bar_;
		}

		void DownloadPDBFile::initializeWidget(MainControl&)
		{
			String description = "Shortcut|File|Open|Download_PDB";
			menu_id_ = insertMenuEntry(MainControl::FILE_OPEN, tr("Download PDB"), this,
																 SLOT(show()), description, QKeySequence("Ctrl+T"),
																 tr("Download a PDB file from www.rcsb.org"),
																 UIOperationMode::MODE_ADVANCED);
			setMenuHelp(menu_id_, "tips.html#download_pdb");
			setIcon(menu_id_, "actions/download-pdb", true);
		}

		void DownloadPDBFile::slotDownload()
		{
			String id = ascii(pdbId->currentText());
			String url = prefix_;
			url += id;
			url += (checkbox_biounit->isChecked()) ? biounit_suffix_ : suffix_;

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

		void DownloadPDBFile::downloadFinished()
		{
			String id = ascii(pdbId->currentText());
			String url = prefix_;
			url += id;
			url += (checkbox_biounit->isChecked()) ? biounit_suffix_ : suffix_;
			Log.info() << "Url: " << url << std::endl; 

			if (current_reply_->error() != QNetworkReply::NoError)
			{
				Log.error() << "Could not download PDBFile! Reason given was \"" + (String)(current_reply_->errorString()) + "\""<< std::endl;
				setStatusbarText("Could not download PDBFile! Reason given was \"" + (String)(current_reply_->errorString()) + "\"");

				error_ = true;
			}
			else
			{
				System *system = new System();

				try {
					String temp_filename = VIEW::createTemporaryFilename();
					
					QFile outfile(temp_filename.c_str());
					outfile.open(QIODevice::ReadWrite);

					outfile.write(current_reply_->readAll());
					outfile.close();
					
					// if we download the biological unit, we have to decompress the pdb.gz file
					if (checkbox_biounit->isChecked())
					{
						String unzipped_filename = VIEW::createTemporaryFilename();	
						std::ifstream file(temp_filename.c_str(), std::ios_base::in | std::ios_base::binary);
						boost::iostreams::filtering_streambuf<boost::iostreams::input> in;
						in.push(boost::iostreams::gzip_decompressor());
						in.push(file);
					
						PDBFile pdb_file(unzipped_filename, std::ios::out | std::ios::binary);
						boost::iostreams::copy(in, pdb_file);
						pdb_file.reopen(std::ios::in);
						
						// the individual units are organized as MODELs
						// by default PDBFile only reads the first MODEL
						pdb_file.selectAllModels();
						pdb_file >> *system;
						pdb_file.close();
						
						removeFile_(unzipped_filename);
					}
					else
					{
						PDBFile pdb_file(temp_filename);
						pdb_file >> *system;
						pdb_file.close();
					}

					removeFile_(temp_filename);

					if (system->countAtoms() == 0)
					{
						delete system;
						show();
						setStatusbarText(tr("Could not fetch the given PDBFile"), true);
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
					setStatusbarText(tr("download PDB file failed"), true);
					delete system;
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

		void DownloadPDBFile::downloadProgress(qint64 received, qint64 total)
		{
			progress_bar_->setRange(0, total);
			progress_bar_->setValue(received);

			setStatusbarText("received " + String((unsigned int)received) + " / " + String((unsigned int)total) + " bytes", true);
		}

		void DownloadPDBFile::idChanged()
		{
			download->setEnabled(pdbId->currentText() != "");
		}

		void DownloadPDBFile::abort()
		{
			if (current_reply_)
				current_reply_->abort();

			downloadEnded_();
		}

		void DownloadPDBFile::downloadStarted_()
		{
			aborted_ = false;
			error_   = false;
			setStatusbarText(tr("Started download, please wait..."), true);
			button_abort->setEnabled(true);
			download->setEnabled(false);
			pdbId->setEnabled(false);
			buttonClose->setEnabled(false);
		}

		void DownloadPDBFile::downloadEnded_()
		{
			if (!aborted_ && !error_)
			{
				setStatusbarText(tr("Finished downloading, please wait..."), true);
			}
			button_abort->setEnabled(false);
			download->setEnabled(true);
			pdbId->setEnabled(true);
			buttonClose->setEnabled(true);
			idChanged();
			qApp->processEvents();
			pdbId->setFocus();
		}

		void DownloadPDBFile::removeFile_(const String& filename)
		{
			try
			{
				File::remove(filename);
			}
			catch(...) {}
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
			if (menu_id_)
				menu_id_->setEnabled(!mc.compositesAreLocked());
		}

	} // namespace VIEW
} // namespace BALL
