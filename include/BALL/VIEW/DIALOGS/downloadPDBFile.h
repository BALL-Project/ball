// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: downloadPDBFile.h,v 1.11 2004/12/10 19:08:46 amoll Exp $
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

#include <qtextbrowser.h>
#include <qimage.h>

namespace BALL
{
	namespace VIEW
	{

 		class FetchHTMLThread;

		/** Dialog to search for and download structure files from the <a href="http://www.rcsb.org/">PDB data bank</a>.
				\ingroup ViewDialogs
		*/
		class BALL_EXPORT DownloadPDBFile 
			: public DownloadPDBFileData,
				public ModularWidget
		{ 
				Q_OBJECT
			public:
				BALL_EMBEDDABLE(DownloadPDBFile, ModularWidget)

				///
				DownloadPDBFile( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 )
					throw();

				///
				~DownloadPDBFile()
					throw();

				///
				virtual void initializeWidget(VIEW::MainControl& main_control)
					throw();

			/** Removes the widget.
					Reverses all actions performed in initializeWidget()
					This method will be called by MainControl::aboutToExit()
					\see			removeMenuEntry
			*/
			virtual void finalizeWidget(VIEW::MainControl& main_control)
				throw();

			public slots:

				///
				void slotSearch();

				///
				void slotDownload();

				///
				void slotShowDetail();

				///
				void slotNewId(const QString& new_id);

				///
				void displayHTML(const QString& url);

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
				
				QTextBrowser 						*qb_;
				FetchHTMLThread 				*thread_;
				bool 										aborted_;
				bool 										error_;

				HashMap<String, QImage> image_cache_;

				// e.g. gif images if not supported
				HashSet<String> 				unsupported_images_;
		};

	} 
} //namespaces
#endif
