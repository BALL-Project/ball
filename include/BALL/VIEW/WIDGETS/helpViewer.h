// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: helpViewer.h,v 1.1.2.4 2005/08/07 19:01:28 amoll Exp $
//

#ifndef BALL_VIEW_WIDGETS_HELPVIEWER_H
#define BALL_VIEW_WIDGETS_HELPVIEWER_H

#ifndef BALL_VIEW_WIDGETS_DOCKWIDGET_H
#	include <BALL/VIEW/WIDGETS/dockWidget.h>
#endif

#include <qtextbrowser.h>

namespace BALL
{
	namespace VIEW
	{

		class MyTextBrowser
			: public QTextBrowser
		{
			Q_OBJECT

			public:

			MyTextBrowser(QWidget* parent, const char* name = 0);

			public slots:

			void setBackwardAvailable(bool b);

			void setForwardAvailable(bool b);

			protected:

			QPopupMenu* createPopupMenu(const QPoint& pos);

			bool forward_, backward_;
		};

		/** DockWidget to show online help texts e.g. the BALLView documentation
		 		To show a help page, just call ModularWidget::showHelp(String) or send
				a ShowHelpMessage.
				Per default the HelpViewer looks for index.html in $BALL_DATA_PATH/../doc/BALLView .
				You can change this behavior by using setDefaultDir() and setDefaultPage().
				Links to pages on the www wont work!
		*/
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

			/// Show default page
			virtual void showHelp();

			protected:

			String 				default_page_;
			String 				base_dir_;
			MyTextBrowser* browser_;
		};
  	
} } // namespaces

#endif // BALL_VIEW_WIDGETS_HELPVIEWER_H
