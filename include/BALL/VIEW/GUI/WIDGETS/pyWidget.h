// $Id: pyWidget.h,v 1.1 2000/09/23 12:50:28 hekl Exp $

#ifndef BALL_VIEW_OPENGL_WIDGETS_PYWIDGET_H
#define BALL_VIEW_OPENGL_WIDGETS_PYWIDGET_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
#	include <BALL/DATATYPE/string.h>
#endif

#ifndef BALL_VIEW_OPENGL_WIDGETS_MODULARWIDGET_H
#	include <BALL/VIEW/OPENGL/WIDGETS/modularWidget.h>
#endif

#include <qmultilineedit.h>

using namespace BALL;

/**
*/
class PyWidget
  : public QMultiLineEdit,
		public ModularWidget
{
	Q_OBJECT

  public:
	
	BALL_EMBEDDABLE(PyWidget)

	/**	@name	Constructors and Destructors
	*/
	//@{
	/** Standard constructor.
			Creates a new widget. If the widget is part of a BALL \ref{MainControl} widget, 
			it inserts a menu entry {\tt Tools|Restart Python} into the menu bar.
			@param parent the parent widget
			@param name the widget name
	*/
  PyWidget(QWidget* parent = 0, const char* name = 0);

	/** Copy constructor.
			Creates a new widget and inserts it in the same place as the original widget.
	*/
  PyWidget(const PyWidget& widget);

	/// Destructor
	virtual ~PyWidget();
	//@}

	public slots:

	/**	@name Running the interpreter
	*/
	//@{
	/** Start the interpreter.
			This method initializes the interpreter if it
			is not yet running. An already running interpreter 
			is reinitialized.
			This method calls {\tt PyInitialize()} to 
			create an interpreter.
			This is a QT {\bf SLOT}.
	*/
	virtual void startInterpreter();
	/**	Stop the interpreter.
			The interpreter is stoped by calling {\tt Py_Finish()}.
			This is a QT {\bf SLOT}.
	*/
	virtual void stopInterpreter();
	//@}

	/**	@name	ModularWidget related methods
	*/
	//@{
	/**	Setup the menu entries.
			PyWidget creates an entry in Tools|Restart Python and connects
			the entry to startInterpreter().
	*/
	virtual void initializeWidget(MainControl& main_control);

	/**	Remove menu entries.
	*/
	virtual void finalizeWidget(MainControl& main_control);
	//@}
	
	public:

	/** @name	Widget related methods
			These methods implement the basic behaviour of the edit window by 
			overwriting the corresponding methods of {\tt QMultiLineEdit}.
			You should not call them immediately, but you might want to
			overwrite them in derived classes.
	*/
	//@{
	///
  virtual void cursorUp(bool mark = FALSE);
	///
  virtual void cursorDown(bool mark = FALSE);
	///
	virtual void newLine();
	///
	virtual void cursorLeft(bool mark = FALSE, bool wrap = TRUE);
	///
	virtual void backspace();
	///
	virtual void cursorRight(bool mark = FALSE, bool wrap = TRUE);
	///
	virtual void mousePressEvent(QMouseEvent* m);
	///
	virtual void home(bool mark = FALSE);
	//@}

	protected:

	void parseLine_();

	void appendToHistory_(const char*);
	
	/**	Print prompt.
			Determine the correct type of prompt and append it 
			to the current text. The cursor is placed after
			the prompt and {\tt textChanged} is emitted.
	*/
	void newPrompt_();

	const char* getPrompt_() const;

	/**	Replace the line the cursor is in with a line from the history.
			Used to display text from the history (cursor down/up).
			The previous content of the line is stored in
			{\tt current_line_} if this is the first time the history
			function is used for this specific line.
	*/	
	void retrieveHistoryLine_(Position index);

	bool						multi_line_mode_;
	String					multi_line_text_;
	vector<String>	history_;
	Position				history_position_;
	String					current_line_;
}; 
#endif // BALL_VIEW_OPENGL_WIDGETS_PYWIDGET_H
