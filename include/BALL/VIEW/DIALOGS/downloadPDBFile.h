// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: downloadPDBFile.h,v 1.1 2004/02/02 18:58:23 anhi Exp $
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
		/** Dialog to search for and download structure files from the PDB data bank
				\ingroup ViewDialogs
		*/
		class DownloadPDBFile 
			: public DownloadPDBFileData,
				public ModularWidget
		{ 
				Q_OBJECT
			public:
				BALL_EMBEDDABLE(DownloadPDBFile, ModularWidget)

				DownloadPDBFile( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 )
					throw();

				~DownloadPDBFile()
					throw();

				virtual void initializeWidget(VIEW::MainControl& main_control)
					throw();

	/** Removes the widget.
			Reverses all actions performed in initializeWidget
			(removes menu entries of this MolecularFileDialog).
			This method will be called by aboutToExit from the
			MainControl object.
			\param		main_control the MainControl object to be finalized with
								this MolecularFileDialog
			\see			initializeWidget
			\see			removeMenuEntry
			\see			aboutToExit
			*/
				virtual void finalizeWidget(VIEW::MainControl& main_control)
					throw();

			public slots:
				void showDialog();
				void slotSearch();
				void slotDownload();
				void slotShowDetail();
				void slotNewId(const QString& new_id);
				void displayHTML(const QString& url);

				virtual void checkMenuEntries()
					throw();

			protected:
				QTextBrowser *qb_;
		};


	} 
} //namespaces
#endif
