// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include <BALL/VIEW/DIALOGS/downloadPDBFile.h>

#include <BALL/FORMAT/lineBasedFile.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/KERNEL/system.h>


#include <qcombobox.h> 
#include <qlineedit.h> 
#include <qtextedit.h> 
#include <qfile.h>
#include <qurl.h>
#include <qradiobutton.h>
#include <qcheckbox.h>
#include <qimage.h>
#include <qpushbutton.h>

namespace BALL
{
	namespace VIEW
	{

		DownloadPDBFile::DownloadPDBFile( QWidget* parent, const char* name, bool modal, WFlags fl )
			throw()
			: DownloadPDBFileData(parent, name, modal, fl),
				ModularWidget(name)
		{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "new DownloadPDBFile" << this << std::endl;
#endif
			// register the widget with the MainControl
			registerWidget(this);
			hide();
			connect(results, SIGNAL(activated(const QString&)), this, SLOT(slotNewId(const QString&)));
		}

		DownloadPDBFile::~DownloadPDBFile()
			throw()
		{
#ifdef BALL_VIEW_DEBUG
			Log.info() << "Destructing object " << (void *)this 
								 << " of class DownloadPDBFile" << std::endl; 
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
				LineBasedFile search_result(filename.latin1());

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
			catch (...)
			{ }
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

				Log.info() << filename << std::endl;
				PDBFile pdb_file(filename);
				pdb_file >> *system;
				pdb_file.close();

				Log.info() << "> read " << system->countAtoms() << " atoms from URL \"" << filename<< "\"" << std::endl;

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
				qb_ = new QTextBrowser();

				QString filename;

				if (url.find("http://") == -1)
					filename = "http://www.rcsb.org/"+url;	
				else
					filename = url;

				Log.info() << "Reading " << filename << std::endl;

				LineBasedFile search_result(filename.latin1());

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
							LineBasedFile img("http://www.rcsb.org/"+img_url);
							String tmp_filename;

							File::createTemporaryFilename(tmp_filename);
							img.copyTo(tmp_filename);

							QImage qi;
							qi.load(tmp_filename);

							File temp(tmp_filename);
							temp.remove();

							hm[img_url] = qi;
						}
					}
				}

				HashMap<String, QImage>::Iterator hi;
				for (hi = hm.begin(); hi!=hm.end(); hi++)
				{
					qb_->mimeSourceFactory()->setImage(hi->first, hi->second);
				}

				qb_->setText(QString(result.c_str()));

				connect(qb_, SIGNAL(linkClicked(const QString&)), this, SLOT(displayHTML(const QString&)));

				qb_->show();
			}
			catch (...)
			{ }
		} 


		void DownloadPDBFile::idChanged()
		{
			download->setEnabled(pdbId->text() != "");
		}

	}
}
