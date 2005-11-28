// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyWidget.h,v 1.23.2.15 2005/11/28 15:10:17 amoll Exp $
//

#ifndef BALL_VIEW_WIDGETS_PYWIDGET_H
#define BALL_VIEW_WIDGETS_PYWIDGET_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
#	include <BALL/DATATYPE/string.h>
#endif

#ifndef BALL_VIEW_WIDGETS_DOCKWIDGET_H
#	include <BALL/VIEW/WIDGETS/dockWidget.h>
#endif

#include <qtextedit.h>

// currently doesnt work right
#undef BALL_QT_HAS_THREADS

#ifdef BALL_QT_HAS_THREADS
# include <qthread.h>
#endif

namespace BALL
{
	namespace VIEW
	{
		class PythonSettings;

		class RunPythonThread;

#ifdef BALL_QT_HAS_THREADS
		class BALL_VIEW_EXPORT RunPythonThread
			: public QThread
		{
			public:
				RunPythonThread()
					throw();

				///
				virtual void run();

				bool state;
				String input;
				String output;
		};
#endif

		///
		struct BALL_VIEW_EXPORT Hotkey
		{
			public:

			///
			bool operator == (const Hotkey& hotkey) const
				throw();

			///
			bool operator == (const QKeyEvent& e) const
				throw();

			/// 
			const Hotkey& operator = (const Hotkey& s)
				throw();

			/// Needed for MSVC
			bool operator < (const Hotkey& key) { return this < &key;}

			///
			bool set(const String& data) throw();

			///
			void get(String& data) const throw();

			String 					action;
			Qt::ButtonState button_state;
			Qt::Key 				key;
		};


		/** Python Widget base class.
		 		This class was added, because we had to overwrite some qt-methods.
				Use the PyWidget class for your application!!!
				\ingroup ViewWidgets
		*/
		class BALL_VIEW_EXPORT PyWidgetData
			: public QTextEdit
		{
			friend class PyWidget;

			Q_OBJECT

			public:
			
			/**	@name	Constructors and Destructors
			*/
			//@{

			/** Standard constructor.
			*/
			PyWidgetData(QWidget* parent = 0, const char* name = 0);

			/** Copy constructor.
			*/
			PyWidgetData(const PyWidgetData& widget);

			/// Destructor
			virtual ~PyWidgetData()
				throw();

			/** Internal state dump.
					Dump the current internal state of this to 
					the output ostream <b> s</b> with dumping depth <b> depth</b>.
					\param   s - output stream where to output the internal state of this
					\param   depth - the dumping depth
			*/
			void dump(std::ostream& s = std::cout, Size depth = 0) const
				throw();

			///
			bool runString(String command);

			/**	Run a Python program from a file.
					\param filename the name of the program file
			*/
			virtual bool runFile(const String& filename);
			
			public slots:

			//@}
			/**	@name QT Slots
			*/
			//@{
			
			///
			virtual void abortScript();

			///
			virtual void exportHistory();			

			//@}

			protected slots:

			virtual void contentsDragEnterEvent(QDragEnterEvent* e);

			virtual void contentsDropEvent(QDropEvent* e);

			protected:

			virtual bool returnPressed();

			/** Start the interpreter.
					This method initializes the interpreter if it is not yet running. 
					An already running interpreter is reinitialized.
					This method calls <tt>PyInitialize()</tt> to create an interpreter.
			*/
			virtual void startInterpreter();

			virtual void keyPressEvent(QKeyEvent* e);

			virtual void clear();
			
			virtual void paste();

			bool parseLine_();
			/// Parse a and execute a given string. If silent is set to true, no prompts are being printed.
			bool parseLine_(String line, bool silent = false);

			void appendToHistory_(const String& line);
			
			/**	Print prompt.
					Determine the correct type of prompt and append it 
					to the current text. The cursor is placed after
					the prompt and <tt>textChanged</tt> is emitted.
			*/
			void newPrompt_();

			//_
			const char* getPrompt_() const;

			/**	Replace the line the cursor is in with a line from the history.
					Used to display text from the history (cursor down/up).
					The previous content of the line is stored in
					<tt>current_line_</tt> if this is the first time the history
					function is used for this specific line.
			*/	
			void retrieveHistoryLine_(Position index);

			String getCurrentLine_();

			bool							multi_line_mode_;
			Size 							multi_lines_;
			String						multi_line_text_;
			vector<String>		history_;
			vector<bool> 			results_;
			Position					history_position_;
			String						current_line_;
			String 						startup_script_;
			PythonSettings* 	python_settings_;
			RunPythonThread* 	thread_;
			bool 							stop_script_;
		}; 


		/** Python Widget
		 		This class is a Window for a Python interpreter interface.
				So it is possible to access all data in the running application in realtime with the script language.
				PyWidget also has the capablities to run a Python script from a file at startup, or on demand from the user.
		*/
		class BALL_VIEW_EXPORT PyWidget
			: public DockWidget
		{
			Q_OBJECT

			public:
			
			BALL_EMBEDDABLE(PyWidget, Embeddable)

			/**	@name	Constructors and Destructors
			*/
			//@{

			/** Standard constructor.
					If the widget is part of a BALL \link MainControl MainControl \endlink widget, 
					it inserts a menu entry <tt>Tools|Restart Python</tt> into the menu bar.
					\param parent the parent widget
					\param name the widget name
			*/
			PyWidget(QWidget* parent = 0, const char* name = 0)
				throw();

			/// only needed for Pyhon Interface
			PyWidget(const PyWidget& p)
				throw();

			///
			~PyWidget()
				throw();
			
			/// Is full Python support available?
			bool isValid() const 
				throw() { return valid_;}

			/**	@name	ModularWidget related methods
			*/
			//@{

			/**	Setup the menu entries.
			*/
			virtual void initializeWidget(MainControl& main_control)
				throw();

			/**	Remove menu entries.
			*/
			virtual void finalizeWidget(MainControl& main_control)
				throw();
			
			///
			void initializePreferencesTab(Preferences &preferences)
				throw();
			
			///
			void finalizePreferencesTab(Preferences &preferences)
				throw();

			///
			virtual void applyPreferences()
				throw();

			///
			bool toAbortScript() throw();

			///
			void insertHotkey(const Hotkey& hotkey) throw();

			///
			void removeHotkey(const Hotkey& hotkey) throw();

			///
			void reactTo(const QKeyEvent& e) throw();

			/// run a Python script from a given file
			bool run(const String& filename) throw();

			// Rerun the last script again
			bool runAgain();

			public slots:

			/// Open a dialog to select a start up script
			virtual void scriptDialog();


			protected:

			PyWidgetData* 		text_edit_;
			List<Hotkey> 			hotkeys_;
			// 								we use an own working dir to find Python Scripts
			String 						working_dir_;
			String 						last_script_;
			bool 							valid_;
			bool 							started_startup_script_;
		};

	} // namespaces	
} // namespaces
	
#endif // BALL_VIEW_WIDGETS_PYWIDGET_H
