// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_DIALOGS_DOWNLOADPDBFILE_H
#define BALL_VIEW_DIALOGS_DOWNLOADPDBFILE_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifndef BALL_VIEW_KERNEL_MODULARWIDGET_H
# include <BALL/VIEW/KERNEL/modularWidget.h>
#endif

#ifndef BALL_DATATYPE_HASHSET_H
# include <BALL/DATATYPE/hashSet.h>
#endif

#include <BALL/VIEW/UIC/ui_downloadPDBFile.h>

#include <QtNetwork/QNetworkReply>

class QProgressBar;
class QNetworkAccessManager;

namespace BALL
{
	namespace VIEW
	{
		/** Dialog to search for and download structure files from the <a href="http://www.rcsb.org/">PDB data bank</a>.
				\ingroup ViewDialogs
		*/
		class BALL_VIEW_EXPORT DownloadPDBFile 
			: public QDialog,
				public Ui_DownloadPDBFileData,
				public ModularWidget
		{ 
				Q_OBJECT
			public:
				BALL_EMBEDDABLE(DownloadPDBFile, ModularWidget)

				///
				DownloadPDBFile(QWidget* parent = 0, const char* name = "DownloadPDBFileDialog", 
												bool modal = false, Qt::WindowFlags fl = 0 );

				///
				~DownloadPDBFile();

				///
				virtual void initializeWidget(VIEW::MainControl& main_control);

				///
				virtual void fetchPreferences(INIFile& inifile);

				///
				virtual void writePreferences(INIFile& inifile);

				/** Set the prefix for the PDB.org url<br>
						Standard: http://files.rcsb.org/download/
				*/
				void setPrefix(String s) { prefix_ = s;}

				/** Set the suffix for the PDB.org url<br>
						Standard: (.pdb)
				*/
				void setSuffix(String s) { suffix_ = s;}

				///
				void checkMenu(MainControl& mc);

			public slots:

				///
				void slotDownload();

				///
				void idChanged();

				/// 
				virtual void abort();

				///
				void downloadFinished();

				///
				void downloadProgress(qint64 received, qint64 total);

			protected:

				//_
				void downloadStarted_();

				//_  
				void downloadEnded_();

				bool threadedDownload_(const String& url);
				void removeFile_(const String& filename);

				bool 										aborted_;
				bool 										error_;

				HashMap<String, QImage> image_cache_;

				// e.g. gif images if not supported
				HashSet<String> 				unsupported_images_;

				QAction* menu_id_;
				String   prefix_, suffix_;
				String   biounit_suffix_;

				// the current network reply
				QNetworkReply* current_reply_;

				// the current progress bar
				QProgressBar*  progress_bar_;
				QNetworkAccessManager* network_manager_;
		};

	} 
} //namespaces
#endif
