// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: downloadPDBFile.h,v 1.14.16.1 2007/03/25 21:25:47 oliver Exp $
//

#ifndef BALL_VIEW_DIALOGS_DOWNLOADPDBFILE_H
#define BALL_VIEW_DIALOGS_DOWNLOADPDBFILE_H

#include <BALL/VIEW/UIC/downloadPDBFileData.h>

#ifndef BALL_VIEW_KERNEL_MODULARWIDGET_H
# include <BALL/VIEW/KERNEL/modularWidget.h>
#endif

#ifndef BALL_DATATYPE_HASHSET_H
# include <BALL/DATATYPE/hashSet.h>
#endif

namespace BALL
{
	class TCPTransfer;

	namespace VIEW
	{

 		class FetchHTMLThread;

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
												bool modal = FALSE, Qt::WFlags fl = 0 )
					throw();

				///
				~DownloadPDBFile()
					throw();

				///
				virtual void initializeWidget(VIEW::MainControl& main_control)
					throw();

				///
				virtual void fetchPreferences(INIFile& inifile)
					throw();

				///
				virtual void writePreferences(INIFile& inifile)
					throw();

				/** Set the prefix for the PDB.org url<br>
						Standard: http://www.rcsb.org/pdb/files/
				*/
				void setPrefix(String s) { prefix_ = s;}

				/** Set the suffix for the PDB.org url<br>
						Standard: (.pdb)
				*/
				void setSuffix(String s) { suffix_ = s;}

				///
				void checkMenu(MainControl& mc)
					throw();

			public slots:

				///
				void slotDownload();

				///
				void idChanged();

				/// 
				virtual void abort();

			protected:

				//_
				void downloadStarted_()
					throw();

				//_  
				void downloadEnded_()
					throw();

				bool threadedDownload_(const String& url);
				void removeFile_(const String& filename);

				void setProxyAndTransfer_(TCPTransfer& tcp);
				
				FetchHTMLThread 				*thread_;
				bool 										aborted_;
				bool 										error_;

				HashMap<String, QImage> image_cache_;

				// e.g. gif images if not supported
				HashSet<String> 				unsupported_images_;

				QAction* menu_id_;
				String   prefix_, suffix_;
		};

	} 
} //namespaces
#endif
