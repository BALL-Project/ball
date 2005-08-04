// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: helpViewer.h,v 1.1.2.1 2005/08/04 14:46:53 amoll Exp $
//

#ifndef BALL_VIEW_WIDGETS_HELPVIEWER_H
#define BALL_VIEW_WIDGETS_HELPVIEWER_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_WIDGETS_DOCKWIDGET_H
#	include <BALL/VIEW/WIDGETS/dockWidget.h>
#endif

#include <qtextbrowser.h>

namespace BALL
{
	namespace VIEW
	{
		BALL_EXPORT class HelpViewer
			: public DockWidget
		{
			Q_OBJECT

			public:

			BALL_EMBEDDABLE(HelpViewer, DockWidget)

			///
			HelpViewer(QWidget* parent, const char* name = 0)
				throw();

			///
			virtual ~HelpViewer()
				throw();

			/**	Setup the menu entry
			*/
			virtual void initializeWidget(MainControl& main_control)
				throw();

			/** Handles messages sent by other registered ConnectionObject objects.
					Filters for SceneMessage and sets the Camera appropriate or updates the visualization. 
					\param message the pointer to the message that should be processed
					\see   ConnectionObject
					\see   Message
		  */
			virtual void onNotify(Message *message)
				throw();

			///
			virtual void showHelp(const String& URL);

			///
			void setDefaultPage(const String& url);
		
			///
			const String& getDefaultPage() const;
			
			///
			void setBaseDirectory(const String& dir);
	
			///
			const String& getBaseDirectory() const;

			public slots:

			///
			virtual void showHelp();

			protected:

			String default_page_;
			String base_dir_;
			QTextBrowser* browser_;
		};
  	
} } // namespaces

#endif // BALL_VIEW_WIDGETS_HELPVIEWER_H
