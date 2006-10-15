// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: fileObserver.h,v 1.1.2.1 2006/10/15 17:19:30 amoll Exp $
//

#ifndef BALL_VIEW_WIDGETS_FILEOBSERVER_H
#define BALL_VIEW_WIDGETS_FILEOBSERVER_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_WIDGETS_DOCKWIDGET_H
#	include <BALL/VIEW/WIDGETS/dockWidget.h>
#endif

#include <QtCore/qtimer.h>
#include <QtCore/QFSFileEngine>
#include <QtCore/QDateTime>

namespace BALL
{
	namespace VIEW
	{
		/** Class for observing changes in a file.
		*/
		class BALL_VIEW_EXPORT FileObserver
			: public DockWidget
		{
			Q_OBJECT

			public:

			BALL_EMBEDDABLE(FileObserver,DockWidget)
		
			///
			FileObserver(QWidget *parent = 0, const char *name = 0)
				throw();

			///
			virtual ~FileObserver()
				throw();

			///
			void setFileName(String filename);

			///
			String getFileName() const;

			/// Set the update interval in milli seconds
			void setUpdateInterval(Size msec);

			public slots:

			/// Look for updates in the file
			void updateFile();

			///
			void start();

			///
			void stop();

			protected:

			QTimer 			timer_;
			String 			file_name_;
			QDateTime 	last_vis_time_;
		};
  	
} } // namespaces

#endif // BALL_VIEW_WIDGETS_FAH_H
