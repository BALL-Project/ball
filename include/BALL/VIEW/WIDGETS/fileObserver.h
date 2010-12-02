// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_WIDGETS_FILEOBSERVER_H
#define BALL_VIEW_WIDGETS_FILEOBSERVER_H

#ifndef BALL_VIEW_WIDGETS_DOCKWIDGET_H
#	include <BALL/VIEW/WIDGETS/dockWidget.h>
#endif

#include <QtCore/QTimer>
#include <QtCore/QDateTime>

class QAction;

namespace BALL
{
	namespace VIEW
	{
		/** Class for observing changes in a molecular file.
		 		When the file changes, all current Systems are deleted and
				the new content of the observed molecular file is shown.
		 		Supported are all files from the MolecularFileDialog.
				A MolecularFileDialog must have been inserted into the MainControl.<br>
				Usage: (in from MainControl derived class)<br>
				\code
				FileObserver* fo = new FileObserver(this, "FileObserver");
				fo->setFileName("foo.pdb");
				fo->start();
				\endcode
				\ingroup ViewWidgets
		*/
		class BALL_VIEW_EXPORT FileObserver
			: public DockWidget
		{
			Q_OBJECT

			public:

			BALL_EMBEDDABLE(FileObserver,DockWidget)
		
			///
			FileObserver(QWidget *parent = 0, const char *name = 0);

			///
			virtual ~FileObserver();

			//
			virtual void initializeWidget(MainControl& main_control);
			
			//
			virtual void checkMenu(MainControl& main_control);

			/// Set the name of the molecular file to be observed
			void setFileName(String filename);

			///
			String getFileName() const;

			/// Set the update interval in milli seconds
			void setUpdateInterval(Size msec);

			public slots:

			/// Look for updates in the file
			void updateFile();

			/// Start the observation
			void start();

			/// Stop the observation
			void stop();

			/// Choose a file and start the observer
			void chooseFile();

			protected:

			QAction* start_action_, *stop_action_;

			QTimer 			timer_;
			String 			file_name_;
			QDateTime 	last_vis_time_;
		};
  	
} } // namespaces

#endif // BALL_VIEW_WIDGETS_FILEOBSERVER_H
