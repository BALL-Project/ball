// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include <BALL/VIEW/DIALOGS/downloadPDBFile.h>

#include <BALL/FORMAT/lineBasedFile.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/KERNEL/system.h>
#include <BALL/VIEW/KERNEL/threads.h>

#include <qcombobox.h> 
#include <qlineedit.h> 
#include <qtextedit.h> 
#include <qfile.h>
#include <qurl.h>
#include <qradiobutton.h>
#include <qcheckbox.h>
#include <qimage.h>
#include <qpushbutton.h>
#include <qgroupbox.h>
#include <qapplication.h>

namespace BALL
{
	namespace VIEW
	{

		DownloadPDBFile::DownloadPDBFile( QWidget* parent, const char* name, bool modal, WFlags fl )
			throw()
			: DownloadPDBFileData(parent, name, modal, fl),
				ModularWidget(name),
				thread_(0),
				aborted_(false)
		{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "new DownloadPDBFile" << this << std::endl;
#endif
			// register the widget with the MainControl
			registerWidget(this);
			hide();
			connect(results, SIGNAL(activated(const QString&)), this, SLOT(slotNewId(const QString&)));

#ifdef BALL_QT_HAS_THREADS
			thread_ = new FetchHTMLThread();
#endif
		}

		DownloadPDBFile::~DownloadPDBFile()
			throw()
		{
#ifdef BALL_VIEW_DEBUG
			Log.info() << "Destructing object " << (void *)this 
								 << " of class DownloadPDBFile" << std::endl; 
#endif 

#ifdef BALL_QT_HAS_THREADS
			if (thread_ != 0) delete thread_;
#endif
		}

		void DownloadPDBFile::initializeWidget(MainControl& main_control)
			throw()
		{
			String hint("Download a PDB file from www.rcsb.org");
			main_control.insertMenuEntry(MainControl::FILE_OPEN, "&Download Structure", (QObject *)this,
																	 SLOT(show()), CTRL+Key_D, -1, hint);
		}

		void DownloadPDBFile::finalizeWidget(MainControl& main_control)
			throw()
		{
			main_control.removeMenuEntry(MainControl::FILE_OPEN, "&Download Structure", (QObject *)this, 
																	 SLOT(show()), CTRL+Key_R);
		}

		void DownloadPDBFile::checkMenuEntries()
			throw()
		{
		}


		void DownloadPDBFile::slotSearch()
		{
			results->clear();

			QString filename = "http://www.rcsb.org/pdb/cgi/resultBrowser.cgi?Lucene::keyword=";
			QString search_contents = searchField->text();
			QUrl::encode(search_contents);
			
			filename+=search_contents;
			filename+="&Lucene::keyword_op=";

			filename+=(fullText->isOn()) ? "fullText" : "name";
			
			if (exactMatch->isOn())
				filename+="&exact=1";
			else
				filename+="&exact=0";

			if (removeSimilar->isOn())
				filename+="&SelectorRedundFilt::on=1";
			

			try {
				LineBasedFile search_result;
#ifndef BALL_QT_HAS_THREADS
				search_result = LineBasedFile(filename.latin1());
#else   // =============================
				downloadStarted_();
				thread_->setURL(filename.latin1());
				thread_->start();
				while (thread_->running())
				{
					qApp->wakeUpGuiThread();
					qApp->processEvents(500);
					thread_->wait(50);
				}

				search_result = LineBasedFile(thread_->getFilename());
#endif

				vector<String> result;
				String tmp;
				while (search_result.readLine())
				{
					tmp = search_result.getLine();
					Size pos = tmp.find("name=\"PDBID_");

					if (pos != string::npos)
					{
						Size end_pos = tmp.find("\"", pos+12);
						result.push_back(tmp.substr(pos+12,end_pos-(pos+12)));
					}
				}

				for (Size i=0; i<result.size(); i++)
				{
					results->insertItem(result[i].c_str());
				}
				if (result.size() != 0)
					pdbId->setText(result[0].c_str());

			}
			catch (...) { }

			downloadEnded_();
		}

		void DownloadPDBFile::threadedDownload_(const String& url)
		{
#ifdef BALL_QT_HAS_THREADS
			downloadStarted_();
			thread_->setURL(url);
			thread_->start();
			while (thread_->running())
			{
				qApp->processEvents(100);
				try
				{
					setStatusbarText("Downloaded: " + String(File::getSize(thread_->getFilename())));
				}
				catch(...)
				{}
				thread_->wait(10);
			}

#endif
		}

		void DownloadPDBFile::slotDownload()
		{
			System *system = new System();

			try
			{
				String filename = "http://www.rcsb.org/pdb/cgi/export.cgi/";
				filename += pdbId->text().latin1();
				filename += ".pdb?format=PDB&pdbId=";
				filename += pdbId->text().latin1();
				filename +="&compression=None"; 
				PDBFile* pdb_file = 0;
#ifndef BALL_QT_HAS_THREADS
				pdb_file = new PDBFile(filename);
#else   // =============================
				downloadStarted_();
				threadedDownload_(filename);
				downloadEnded_();

				if (aborted_) 
				{
					delete system;
					return;
				}

				pdb_file = new PDBFile(thread_->getFilename());
#endif

				(*pdb_file) >> *system;
				(*pdb_file).close();
				delete pdb_file;

#ifdef BALL_QT_HAS_THREADS
				try
				{
					File::remove(thread_->getFilename());
				}
				catch(...) {}
#endif

				Log.info() << "> read " << system->countAtoms() << " atoms from URL \"" << filename<< "\"" << std::endl;
				if (system->countAtoms() == 0)
				{
					delete system;
					setStatusbarText("Could not fetch the given PDBFile");
					return;
				}

				if (system->getName() == "")
				{
					system->setName(pdbId->text().latin1());
				}

				system->setProperty("FROM_FILE", filename);
				getMainControl()->insert(*system, pdbId->text().latin1());
			}
			catch(...)
			{
				Log.info() << "> download PDB file failed." << std::endl;
				delete system;
			}
		}

		void DownloadPDBFile::slotShowDetail()
		{
			setStatusbarText("Downloading information, please wait...");
			QString filename = "http://www.rcsb.org/pdb/cgi/explore.cgi?job=summary&pdbId=";
			filename += pdbId->text();
			filename += "&page=";

			displayHTML(filename);
		}

		void DownloadPDBFile::slotNewId(const QString& new_id)
		{
			pdbId->setText(new_id);
		}

		void DownloadPDBFile::displayHTML(const QString& url)
		{
			try
 			{
				QString filename;

				if (url.find("http://") == -1)
					filename = "http://www.rcsb.org/"+url;	
				else
					filename = url;

				Log.info() << "Reading " << filename << std::endl;

				LineBasedFile search_result;
#ifndef BALL_QT_HAS_THREADS
				search_result = LineBasedFile(filename.latin1());
#else
				threadedDownload_(filename.ascii());
				if (aborted_)
				{
					return;
				}

				search_result = LineBasedFile(thread_->getFilename());
#endif

				setStatusbarText("Please wait, while loading images...");

				if (qb_ != 0) delete qb_;
				qb_ = new QTextBrowser();
				String result;

				HashMap<String, QImage> hm;
				String current_line;

				while (search_result.readLine())
				{
					current_line = search_result.getLine();
					result += current_line + "\n";

					// find out all the images
					String tmp = current_line;
					tmp.toUpper();
					Size pos_1 = 0;
					Size pos_2 = 0;

					while ( (pos_1 = tmp.find("<IMG", pos_2)) != string::npos )
					{
						pos_1 = tmp.find("SRC=\"", pos_1);
						pos_2 = current_line.find("\"", pos_1+9);

						String img_url = current_line.substr(pos_1+5, pos_2 - (pos_1+5));
						if (!hm.has(img_url))
						{
							File img("http://www.rcsb.org/"+img_url);
							String tmp_filename;

							File::createTemporaryFilename(tmp_filename);
							img.copyTo(tmp_filename);

							QImage qi;
							qi.load(tmp_filename.c_str());

							File::remove(tmp_filename);

							hm[img_url] = qi;
						}
					}
				}

				HashMap<String, QImage>::Iterator hi;
				for (hi = hm.begin(); hi!=hm.end(); hi++)
				{
					qb_->mimeSourceFactory()->setImage(hi->first.c_str(), hi->second);
				}

				qb_->setText(QString(result.c_str()));

				connect(qb_, SIGNAL(linkClicked(const QString&)), this, SLOT(displayHTML(const QString&)));

				Log.info() << "Finished download of HTML page" << std::endl;

				qb_->showMaximized();
				qb_->show();
	 		}
 			catch (...)
 			{ 
			 	setStatusbarText("Failed to download HTML page.");
			}

#ifdef BALL_QT_HAS_THREADS
			try
			{
				File::remove(thread_->getFilename());
			}
			catch(...){}
#endif
		} 


		void DownloadPDBFile::idChanged()
		{
			download->setEnabled(pdbId->text() != "");
		}

		void DownloadPDBFile::abort()
		{
#ifdef BALL_QT_HAS_THREADS
			if (thread_ != 0)
			{
				thread_->terminate();
			}
			aborted_ = true;

			try
			{
				File::remove(thread_->getFilename());
			}
			catch(...){}
#endif
		}

		void DownloadPDBFile::downloadStarted_()
			throw()
		{
			aborted_ = false;
			setStatusbarText("Started download, please wait...");
			button_abort->setEnabled(true);
			download->setEnabled(false);
			pdbId->setEnabled(false);
			search_box->setEnabled(false);			
			showDetails->setEnabled(false);
			buttonClose->setEnabled(false);
		}

		void DownloadPDBFile::downloadEnded_()
			throw()
		{
			if (!aborted_)
			{
				setStatusbarText("Finished downloading, please wait...");
			}
			else
			{
				setStatusbarText("Aborted download");
			}
			button_abort->setEnabled(false);
			download->setEnabled(true);
			pdbId->setEnabled(true);
			search_box->setEnabled(true);			
			showDetails->setEnabled(true);
			buttonClose->setEnabled(true);
			idChanged();
		}

	}
}
