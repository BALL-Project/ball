// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyWidget.h,v 1.7 2003/10/31 23:15:47 oliver Exp $
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

namespace BALL
{
	class LineBasedFile;

	namespace VIEW
	{
		class PythonSettings;

		/** Python Widget class.
				\ingroup ViewWidgets
		*/
		class PyWidgetData
			: public QTextEdit
		{
			friend class PyWidget;

			Q_OBJECT

			public:
			
			//BALL_EMBEDDABLE(PyWidgetData)

			/**	@name	Constructors and Destructors
			*/
			//@{

			/** Standard constructor.
					If the widget is part of a BALL \ref{MainControl} widget, 
					it inserts a menu entry <tt>Tools|Restart Python</tt> into the menu bar.
					\param parent the parent widget
					\param name the widget name
			*/
			PyWidgetData(QWidget* parent = 0, const char* name = 0);

			/** Copy constructor.
					Creates a new widget and inserts it in the same place as the original widget.
			*/
			PyWidgetData(const PyWidgetData& widget);

			/// Destructor
			virtual ~PyWidgetData()
				throw();

			public slots:

			//@}
			/**	@name Running the interpreter
			*/
			//@{
			
			/** Start the interpreter.
					This method initializes the interpreter if it is not yet running. 
					An already running interpreter is reinitialized.
					This method calls <tt>PyInitialize()</tt> to create an interpreter.
					This is a QT <b>SLOT</b>.
			*/
			virtual void startInterpreter();

			/**	Stop the interpreter.
					The interpreter is stoped by calling <tt>Py_Finish()</tt>.
					This is a QT <b>SLOT</b>.
			*/
			virtual void stopInterpreter();

			///
			virtual void scriptDialog();

			/**	Run a Python program from a file.
					\param filename the name of the program file
			*/
			virtual void runFile(const String& filename);

			///
			virtual void exportHistory();			
			//@}
			public:

			//@}

			/** @name	Widget related methods
					These methods implement the basic behaviour of the edit window by 
					overwriting the corresponding methods of <tt>QMultiLineEdit</tt>.
					You should not call them immediately, but you might want to
					overwrite them in derived classes.
			*/
			//@{

			///
			virtual bool returnPressed();

			///
			virtual void mousePressEvent(QMouseEvent* m);
			
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

			void keyPressEvent(QKeyEvent* e);

			void parseLine_();

			void appendToHistory_(const String& line);
			
			/**	Print prompt.
					Determine the correct type of prompt and append it 
					to the current text. The cursor is placed after
					the prompt and <tt>textChanged</tt> is emitted.
			*/
			void newPrompt_();

			const char* getPrompt_() const;

			/**	Replace the line the cursor is in with a line from the history.
					Used to display text from the history (cursor down/up).
					The previous content of the line is stored in
					<tt>current_line_</tt> if this is the first time the history
					function is used for this specific line.
			*/	
			void retrieveHistoryLine_(Position index);

			String getCurrentLine_();

			bool						multi_line_mode_;
			Size 						multi_lines_;
			String					multi_line_text_;
			vector<String>	history_;
			vector<bool> 		results_;
			Position				history_position_;
			String					current_line_;
			String 					startup_script_;
			PythonSettings* python_settings_;
		}; 


		///
		class PyWidget
			: public DockWidget
		{
			Q_OBJECT

			public:
			
			//BALL_EMBEDDABLE(PyWidget)

			/**	@name	Constructors and Destructors
			*/
			//@{

			/** Standard constructor.
					If the widget is part of a BALL \ref{MainControl} widget, 
					it inserts a menu entry <tt>Tools|Restart Python</tt> into the menu bar.
					\param parent the parent widget
					\param name the widget name
			*/
			PyWidget(QWidget* parent = 0, const char* name = 0)
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

			virtual void startInterpreter();
			virtual void stopInterpreter();

			protected:

			PyWidgetData* text_edit_;
		};

		
}	} // namespaces
	
#endif // BALL_VIEW_WIDGETS_PYWIDGET_H
