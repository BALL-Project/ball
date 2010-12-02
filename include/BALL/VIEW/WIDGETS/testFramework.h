// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_WIDGETS_TESTFRAMEWORK_H
#define BALL_VIEW_WIDGETS_TESTFRAMEWORK_H

#ifndef BALL_VIEW_WIDGETS_DOCKWIDGET_H
#	include <BALL/VIEW/WIDGETS/dockWidget.h>
#endif

#ifndef BALL_SYSTEM_FILE_H
# include <BALL/SYSTEM/file.h>
#endif

#ifndef BALL_SYSTEM_TIMER_H
# include <BALL/SYSTEM/timer.h>
#endif

#include <QtCore/QThread>

class QAction;

namespace BALL
{
	namespace VIEW
	{
		/** Class for recording mouse and keyboard inputs, storing them in
		 		a line based file and replaying them.
		 		This can be used to 
				<ul>
				<li>create tests
				<li>record entire presentations in advance
				<li>for debugging (e.g. for recording the cause for a crash)
				<li>storing and playing tutorials
				</ul>
				This widget will create it's own menu with the name ''Macros''.
				The ''Pause'' key can be used to stop the recording or pause the replay
				of a macro. If a mouse key is pressed in replay mode this will also
				cause a pause.<br><br>
				The file format is line based and its fields are separated with the degree sign (°). 
				The first field consists of only one letter and defines the line's meaning: 
				<ul>
				<li>''C'' comment lines
				<li>''F'' for reading project files
				<li>''I'' for mouse and keyboard inputs 
				<li>''P'' for Python commands
				<li>''T'' for Python test lines.
				<li>''R'' set maximum runtime (in seconds) of a test, 
									kills the application after this time
				<li>''X'' exit the application and return the number of failed tests
				</ul>
				The last two lines are only processed if a test is opened through a 
				command line argument.
				Then comes the time in secs to wait before performing the action.
				\ingroup ViewWidgets
		*/
		class BALL_VIEW_EXPORT TestFramework
			: public QWidget,
				public ModularWidget
		{
			Q_OBJECT

			class MyMouseEvent
				: public QMouseEvent
			{
				public:

					MyMouseEvent(Type type, const QPoint & position, Qt::MouseButton button, 
											 Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers)
						: QMouseEvent(type, position, button, buttons, modifiers) {}
			};

			class MyEventThread
				: public QThread
			{
				public:

					MyEventThread();

					virtual void run();

					TestFramework* tf;
					MainControl* mc;
			};

			friend class MyEventThread;

			public:

			BALL_EMBEDDABLE(TestFramework,DockWidget)
		
			///
			TestFramework(QWidget *parent = 0, const char *name = 0);

			///
			virtual ~TestFramework();

			//
			virtual void initializeWidget(MainControl& main_control);

			///
			void setFilename(const String& filename);
			
			///
			void checkMenu(MainControl& main_control);

			///
			virtual bool canHandle(const String& fileformat) const;

			///
			virtual bool openFile(const String& filename);

			public slots:

			///
			void startTest();

			///
			void stopTest();

			///
			void runTest();

			///
			void abortTest();

			///
			void saveFile();

			///
			void loadFile();

			///
			virtual bool eventFilter(QObject*, QEvent* e);

			///
			virtual void timeOut();

			///
			void switchRecordMove();
			
			protected:

			virtual bool event(QEvent* e);
		
			void storeEvent(QEvent& e);

			void processEvent_();

			QAction* start_recording, *stop_recording, *run_test, *abort_test, 
							 *save_file, *load_file, *switch_move;
			QAction* action_;

			File 				outfile_;
			File 				infile_;
			String 			filename_;
			Timer 			timer_, runtime_;
			list<String>lines_;
			QWidget* 		widget_, *last_widget_;
			float 			time_;
			bool 				event_busy_;
			Index 			x_, y_;
			int 				modifiers_, type_, our_type_, button_, buttons_;
			bool 				test_running_, recording_;
			String 			last_event_string_, last_names_, shortcut_;
			String 			python_line_;
			String 			expected_result_;
			MyEventThread thread_;
			Size 				errors_;
			QEvent* 		last_event_;
			bool 				is_command_line_;
			float 			max_run_time_;
			Position    line_nr_;
		};
  	
} } // namespaces

#endif // BALL_VIEW_WIDGETS_TESTFRAMEWORK_H
