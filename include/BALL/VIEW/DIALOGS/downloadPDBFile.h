// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: downloadPDBFile.h,v 1.5 2004/04/28 15:33:02 amoll Exp $
//

#ifndef BALL_VIEW_DIALOGS_DOWNLOADPDBFILE_H
#define BALL_VIEW_DIALOGS_DOWNLOADPDBFILE_H

#include <BALL/VIEW/UIC/downloadPDBFileData.h>

#ifndef BALL_VIEW_KERNEL_MODULARWIDGET_H
# include <BALL/VIEW/KERNEL/modularWidget.h>
#endif

#include <qtextbrowser.h>

namespace BALL
{
	namespace VIEW
	{

 		class FetchHTMLThread;

		/** Dialog to search for and download structure files from the PDB data bank
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
					\param		main_control the MainControl object to be finalized with
										this MolecularFileDialog
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

				///
				virtual void checkMenuEntries()
					throw();

			protected:

				void threadedDownload_(const String& url);
				
				QTextBrowser *qb_;
				FetchHTMLThread * thread_;
				bool aborted_;
		};

	} 
} //namespaces
#endif
