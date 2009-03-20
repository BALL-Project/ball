// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: helpViewer.h,v 1.2.18.1 2007/03/25 21:26:21 oliver Exp $
//

#ifndef BALL_VIEW_WIDGETS_HELPVIEWER_H
#define BALL_VIEW_WIDGETS_HELPVIEWER_H

#ifndef BALL_VIEW_WIDGETS_DOCKWIDGET_H
#	include <BALL/VIEW/WIDGETS/dockWidget.h>
#endif

#include <QtGui/QTextBrowser>

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

//   			void setBackwardAvailable(bool b);

//   			void setForwardAvailable(bool b);

			protected:

//   			virtual QMenu* createPopupMenu(const QPoint& pos);

			bool forward_, backward_;
		};

		/** DockWidget to show online help texts e.g. the BALLView documentation
				The documentation for BALLView was written in the HTML format, such that it either be
				used online from the project's website or inside the program itself.
				To obtain help for individual elements in the graphical user interface a special mode
				was added. In the "Help" menu the entry "Whats this?" will switch into this mode and 
				transform the mouse cursor into a question mark. While the "Whats this?" mode is
				active a left mouse click on any widget will open the corresponding help entry in the
				documentation. (If no help entry for that widget exists nothing will happen.)
				To leave the "Whats this?" mode just press the right mouse button or press the "Escape" key.
				As an alternative for this approach the "F1" key will popup the documentation for
				the current widget under the mouse cursor.
				<br><br>
		 		To show a specific help page, just call ModularWidget::showHelp(String) or send
				a ShowHelpMessage.
				Per default the HelpViewer looks for index.html in $BALL_DATA_PATH/../doc/BALLView .
				You can change this behavior by using setDefaultDir() and setDefaultPage().
				Links to pages on the WWW wont work!
				QWidgets and menu entries can be registered with a link into the HTML documentation.
				See registerWidgetForHelpSystem.
				The implemention for opening the documentation per "Whats this?" menu entry and
				the hotkey "F1" is also done here.
				\ingroup ViewWidgets
		*/
		class BALL_VIEW_EXPORT HelpViewer
			: public DockWidget
		{
			Q_OBJECT

			public:

			BALL_EMBEDDABLE(HelpViewer, DockWidget)

			///
			HelpViewer(QWidget* parent, const char* name = 0);

			///
			virtual ~HelpViewer();

			/**	Setup the menu entry
			*/
			virtual void initializeWidget(MainControl& main_control);

			/** Handles messages sent by other registered ConnectionObject objects.
					Filters for SceneMessage and sets the Camera appropriate or updates the visualization. 
					\param message the pointer to the message that should be processed
					\see   ConnectionObject
					\see   Message
		  */
			virtual void onNotify(Message *message);

			///
			virtual void showHelp(const String& URL);

			///
			virtual void showHelp(const String& URL, String entry);

			///
			void setDefaultPage(const String& url);
		
			///
			const String& getDefaultPage() const;

			///
			void setProject(const String& project) { project_ = project;}

			///
			String getProject() const { return project_;}
			
			///
			void setBaseDirectory(const String& dir);
	
			///
			const String& getBaseDirectory() const;

			/// Register a widget for showing its documentation
			virtual void registerForHelpSystem(const QObject* object, const String& docu_entry);

			/// Unregister a widget for showing its documentation
			void unregisterForHelpSystem(const QObject* object);

			/// Show the documentation entry for a given widget
			bool showHelpFor(const QObject* object);

			/// Show documentation for object under cursor
			bool showDocumentationForObject();

			/// Check wheter we have a documentation entry for a given object
			bool hasHelpFor(const QObject* object) const;

			/// Get help entry for widget
			String getHelpEntryFor(const QObject* object) const;

			///
			void setWhatsThisEnabled(bool state) { whats_this_ = state;}

			///
			bool isWhatsThisEnabled() const {return whats_this_;}

			///
			void showDocumentationFor(const String& classname, const String& member);
					
			public slots:

			/// Show default page
			virtual void showHelp();

			///
			void enterWhatsThisMode();

			///
			void exitWhatsThisMode();

			/// Event filter for the whats this mode
			bool eventFilter(QObject* obj, QEvent* e);
		
			protected:

			void collectClasses_();

			String 					project_;
			String 					default_page_;
			String 					base_dir_;
			MyTextBrowser* 	browser_;
			bool 						whats_this_mode_;
			bool 						ignore_event_;
			bool 						whats_this_;
			QAction* 				whats_action_;

			HashMap<const QObject*, String> docu_entries_;
			StringHashMap<String> classes_to_files_;
		};
  	
} } // namespaces

#endif // BALL_VIEW_WIDGETS_HELPVIEWER_H
