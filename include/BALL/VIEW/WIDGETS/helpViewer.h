// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: helpViewer.h,v 1.1.2.10 2005/11/17 14:13:13 amoll Exp $
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

		class BALL_VIEW_EXPORT MyTextBrowser
			: public QTextBrowser
		{
			Q_OBJECT

			public:

			MyTextBrowser(QWidget* parent, const char* name = 0);

			public slots:

			void setBackwardAvailable(bool b);

			void setForwardAvailable(bool b);

			protected:

			virtual QPopupMenu* createPopupMenu(const QPoint& pos);

			bool forward_, backward_;
		};

		/** DockWidget to show online help texts e.g. the BALLView documentation
		 		To show a help page, just call ModularWidget::showHelp(String) or send
				a ShowHelpMessage.
				Per default the HelpViewer looks for index.html in $BALL_DATA_PATH/../doc/BALLView .
				You can change this behavior by using setDefaultDir() and setDefaultPage().
				Links to pages on the www wont work!
				The HelpViewer also servers as registration server for the online documentation.
				QWidgets and menu entries can be registered with a link into the HTML documentation.
				See registerWidgetForHelpSystem.
				The implemention for opening the documentation per "Whats this?" menu entry and
				the hotkey "Shift-F1" is also done here.
		*/
		class BALL_VIEW_EXPORT HelpViewer
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

			/// Register a widget for showing its documentation
			virtual void registerWidgetForHelpSystem(const QWidget* widget, const String& docu_entry);

			/// Unregister a widget for showing its documentation
			void unregisterWidgetForHelpSystem(const QWidget* widget);

			/// Register a menu entry for showing its documentation
			virtual void registerMenuEntryForHelpSystem(Index entry, const String& docu_entry);
			
			/// Unregister a menu entry for showing its documentation
			void unregisterMenuEntryForHelpSystem(Index id);
			
			/// Test if we have a documentation entry for the menu entry id
			bool hasHelpFor(Index id) const;

			/// Show the documentation entry for a given widget
			bool showHelpFor(const QWidget* widget);

			/// Show documentation for object under cursor
			bool showDocumentationForObject();

			/// Check wheter we have a documentation entry for a given widget
			bool hasHelpFor(const QWidget* widget) const;

			/// Get help entry for menu entry
			String getHelpEntryFor(Index id) const;

			/// Get help entry for widget
			String getHelpEntryFor(const QWidget* widget) const;
					
			public slots:

			/// Show default page
			virtual void showHelp();

			///
			void enterWhatsThisMode();

			///
			void exitWhatsThisMode();

			/// Event filter for the whats this mode
			bool eventFilter(QObject*, QEvent*);
		
			protected:

			String 					default_page_;
			String 					base_dir_;
			MyTextBrowser* 	browser_;
			bool 						whats_this_mode_;
			bool 						ignore_event_;

			HashMap<const QWidget*, String> docu_for_widget_;

			HashMap<Index, String> docu_for_menu_entry_;

		};
  	
} } // namespaces

#endif // BALL_VIEW_WIDGETS_HELPVIEWER_H
