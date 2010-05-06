// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:

#ifndef BALL_VIEW_DIALOGS_DOWNLOADELECTRONDENSITY_H
#define BALL_VIEW_DIALOGS_DOWNLOADELECTRONDENSITY_H

#include <BALL/VIEW/UIC/ui_downloadElectronDensity.h>

#ifndef BALL_VIEW_KERNEL_MODULARWIDGET_H
# include <BALL/VIEW/KERNEL/modularWidget.h>
#endif

#ifndef BALL_DATATYPE_HASHSET_H
# include <BALL/DATATYPE/hashSet.h>
#endif

#include <BALL/VIEW/UIC/ui_downloadElectronDensity.h>

#include <QtNetwork/QNetworkReply>

class QProgressBar;
class QNetworkAccessManager;

namespace BALL
{
	class TCPTransfer;

	namespace VIEW
	{

		/** Dialog to search for and download electron density maps from either the <a href="http://eds.bmc.uu.se/eds/">Uppsala Electron-Density Server</a>
				or the <a href="www.ebi.ac.uk/msd/emdb/">Electron Microscopy Data Bank</a>.
				\ingroup ViewDialogs
		*/
		class BALL_VIEW_EXPORT DownloadElectronDensity 
			: public QDialog,
				public Ui_DownloadElectronDensityData,
				public ModularWidget
		{ 
				Q_OBJECT
			public:
				BALL_EMBEDDABLE(DownloadElectronDensity, ModularWidget)
				
				///
				DownloadElectronDensity(QWidget* parent = 0, const char* name = "DownloadElectronDensityDialog", 
												bool modal = FALSE, Qt::WFlags fl = 0 );

				///
				~DownloadElectronDensity();

				///
				virtual void initializeWidget(VIEW::MainControl& main_control);

				///
				virtual void fetchPreferences(INIFile& inifile);

				///
				virtual void writePreferences(INIFile& inifile);
				
				/** Set the prefix for the Uppsala Electron Density Server url<br>
						Standard: http://www.eds.bmc.uu.se/eds/sfd/
				*/
				void setEDSPrefix(String s) { eds_prefix_ = s;}

				/** Set the suffix for the Uppsala Electron Density Server url<br>
						Standard: (.omap)
				*/
				void setEDSSuffix(String s) { eds_suffix_ = s;}

				/** Set the prefix for the Electron Microscopy Data Bank url<br>
						Standard: http://www.eds.bmc.uu.se/eds/sfd/
				*/
				void setEMDBPrefix(String s) { emdb_prefix_ = s;}

				/** Set the suffix for the Electron Microscopy Data Bank url<br>
						Standard: (.omap)
				*/
				void setEMDBSuffix(String s) { emdb_suffix_ = s;}
				
				///
				void checkMenu(MainControl& mc);
			
			public slots:

				///
				void slotDownload();

				///
				void idChanged();

				///
				void serverChanged();

				///
				void typeChanged();

				///
				void pdbDownloadChecked();

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
				String   eds_prefix_, eds_infix_, eds_suffix_; 
				String emdb_prefix_, emdb_infix_, emdb_suffix_;
				
				// the current network reply
				QNetworkReply* current_reply_;

				// the current progress bar
				QProgressBar*  progress_bar_;
				QNetworkAccessManager* network_manager_;
		};

	} 
} //namespaces
#endif
