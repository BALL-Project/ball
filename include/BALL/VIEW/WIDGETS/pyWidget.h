// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyWidget.h,v 1.16 2004/04/19 22:12:42 amoll Exp $
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

#ifdef BALL_QT_HAS_THREADS
# include <qthread.h>
#endif

namespace BALL
{
	namespace VIEW
	{
		class PythonSettings;

		class RunPythonThread
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


		/** Python Widget base class.
		 		This class was added, because we had to overwrite some qt-methods.
				Use the derived PyWidget class for your application!
				\ingroup ViewWidgets
		*/
		class BALL_EXPORT PyWidgetData
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

			public slots:

			//@}
			/**	@name QT Slots
			*/
			//@{
			
			/** Start the interpreter.
					This method initializes the interpreter if it is not yet running. 
					An already running interpreter is reinitialized.
					This method calls <tt>PyInitialize()</tt> to create an interpreter.
			*/
			virtual void startInterpreter();

			/**	Stop the interpreter.
					The interpreter is stoped by calling <tt>Py_Finish()</tt>.
			*/
			virtual void stopInterpreter();

			/// Open a dialog to select a start up script
			virtual void scriptDialog();

			///
			virtual void abortScript();

			/**	Run a Python program from a file.
					\param filename the name of the program file
			*/
			virtual void runFile(const String& filename);

			///
			virtual void exportHistory();			

			public:

			//@}
			/** @name	Widget related methods
					These methods implement the basic behaviour of the edit window by 
					overwriting the corresponding methods of <tt>QTextEdit</tt>.
					You should not call them immediately, but you might want to
					overwrite them in derived classes.
			*/
			//@{

			///
			virtual bool returnPressed();

			/** Internal state dump.
					Dump the current internal state of this to 
					the output ostream <b> s</b> with dumping depth <b> depth</b>.
					\param   s - output stream where to output the internal state of this
					\param   depth - the dumping depth
			*/
			void dump(std::ostream& s = std::cout, Size depth = 0) const
				throw();

			//@}

			protected:

			virtual void keyPressEvent(QKeyEvent* e);

			virtual void clear();
			
			virtual void cut();
			
			virtual void paste();

			bool parseLine_();
			bool parseLine_(String line);

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
		class BALL_EXPORT PyWidget
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

			/**	@name	ModularWidget related methods
			*/
			//@{

			/**	Setup the menu entries.
					PyWidget creates an entry in Tools|Restart Python and connects
					the entry to startInterpreter().
			*/
			virtual void initializeWidget(MainControl& main_control)
				throw();

			/**	Remove menu entries.
			*/
			virtual void finalizeWidget(MainControl& main_control)
				throw();
			
			///
			virtual void fetchPreferences(INIFile& inifile)
				throw();
			
			///
			virtual void writePreferences(INIFile& inifile)
				throw();

			///
			void initializePreferencesTab(Preferences &preferences)
				throw();
			
			///
			void finalizePreferencesTab(Preferences &preferences)
				throw();

			///
			void applyPreferences(Preferences & /* preferences */)
				throw();

			///
			void cancelPreferences(Preferences&)
				throw();

			///
			virtual void startInterpreter();
			
			///
			virtual void stopInterpreter();
			
			///
			bool toAbortScript() throw();

			protected:

			PyWidgetData* 		text_edit_;
		};

	} // namespaces	
} // namespaces
	
#endif // BALL_VIEW_WIDGETS_PYWIDGET_H
