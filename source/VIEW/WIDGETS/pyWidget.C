// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

// This include has to be first in order to avoid collisions.
#include <Python.h>

#include <BALL/VIEW/WIDGETS/pyWidget.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/DIALOGS/pythonSettings.h>
#include <BALL/PYTHON/pyInterpreter.h>
#include <BALL/VIEW/DIALOGS/preferences.h>
#include <BALL/FORMAT/lineBasedFile.h>
#include <BALL/SYSTEM/directory.h>

#include <QtGui/QFileDialog>
#include <QtGui/QApplication>
#include <QtGui/QKeyEvent>
#include <QtGui/QDropEvent>
#include <QtGui/QTextCursor>
#include <QtGui/QSplitter>
#include <QtGui/QPushButton>

// currently doesnt work right
#undef BALL_PYTHON_USE_THREADS

namespace BALL
{
	namespace VIEW
	{

		PythonHighlighter::PythonHighlighter()
			:QSyntaxHighlighter((QTextDocument*)0)
		{
			python_keywords 	<< "and" << "assert" << "break" << "class" << "continue" << "def"
				<< "del" << "elif" << "else" << "except" << "exec" << "finally"
				<< "for" << "from" << "global" << "if" << "import" << "in"
				<< "is" << "lambda" << "not" << "or" << "pass" << "print" << "raise"
				<< "return" << "try" << "while" << "yield";

			my_class_format.setForeground(QColor("royalblue"));
			my_class_format.setFontWeight(QFont::Bold);

			python_format.setForeground(QColor("coral"));
			python_format.setFontWeight(QFont::Bold);

			string_format.setForeground(QColor(255,127,80));
			comment_format.setForeground(QColor("mediumaquamarine"));
		}

		void PythonHighlighter::compilePattern()
		{
			python_patterns.clear();
			QString delim("\\b");
			QStringList::iterator lit = python_keywords.begin();
			for (; lit != python_keywords.end(); lit++)
			{
				python_patterns.push_back(QRegExp(delim + *lit + delim));
			}

			BALL_patterns.clear();
			lit = BALL_keywords.begin();
			for (; lit != BALL_keywords.end(); lit++)
			{
				BALL_patterns.push_back(QRegExp(delim + *lit + delim));
			}

			string_pattern  = QRegExp("\".*\"");
			comment_pattern = QRegExp("#.*");
		}

		void PythonHighlighter::highlightBlock(const QString& text)
		{
			if (text.startsWith(">")) return;

			for (Position p = 0; p < python_patterns.size(); p++)
			{
				const QRegExp& expression = python_patterns[p];
				Index index = expression.indexIn(text);
				while (index >= 0) 
				{
					int length = expression.matchedLength();
					setFormat(index, length, python_format);
					index = text.indexOf(expression, index + length);
				}
			}

			for (Position p = 0; p < BALL_patterns.size(); p++)
			{
				const QRegExp& expression = BALL_patterns[p];
				Index index = expression.indexIn(text);
				while (index >= 0) 
				{
					int length = expression.matchedLength();
					setFormat(index, length, my_class_format);
					index = text.indexOf(expression, index + length);
				}
			}

			Index index = string_pattern.indexIn(text);
			while (index >= 0) 
			{
				int length = string_pattern.matchedLength();
				setFormat(index, length, string_format);
				index = text.indexOf(string_pattern, index + length);
			}

			index = comment_pattern.indexIn(text);
			while (index >= 0) 
			{
				int length = comment_pattern.matchedLength();
				setFormat(index, length, comment_format);
				index = text.indexOf(comment_pattern, index + length);
			}
		}

		PythonValidator::PythonValidator(QObject* parent)
			: QValidator(parent)
		{
		}

		PythonValidator::~PythonValidator()
		{
		}

		QValidator::State PythonValidator::validate(QString& input, int& pos) const
		{
			// the following is just meant as an instructive example...
			// input.replace(QRegExp("i"), "o");

			return Acceptable;
		}

#ifdef BALL_PYTHON_USE_THREADS
		RunPythonThread::RunPythonThread()
			: QThread(),
			input(),
			output()
		{}

		void RunPythonThread::run()
		{
			output = PyInterpreter::run(input, state);
		}
#endif

		Hotkey Hotkey::createHotkey(String modifier, String key, String command, 
		                            bool& ok, String comm)
		{
			ok = false;

			Hotkey hotkey;
			hotkey.action = command;
			if      (modifier == "None" || modifier == "")  hotkey.button_state = Qt::NoModifier;
			else if (modifier == "Shift") hotkey.button_state = Qt::ShiftModifier;
			else if (modifier == "Ctrl")  hotkey.button_state = Qt::ControlModifier;
			else 
			{
				return hotkey;
			}

			if (key.size() != 2 && key.size() != 3)
			{
				return hotkey;
			}

			String skey = key.getSubstring(1, key.size() - 1);
			Index p = 99;
			try
			{
				p = skey.toUnsignedInt();
			}
			catch(...)
			{
			}

			if (p < 2 || p > 12) return hotkey;
			p--;

			hotkey.key = (Qt::Key) (Qt::Key_F1 + p);
			hotkey.comment = comm;

			ok = true;
			return hotkey;
		}


		const Hotkey& Hotkey::operator = (const Hotkey& hotkey)
		{
			comment 			= hotkey.comment;
			action 				= hotkey.action;
			key 	 				= hotkey.key;
			button_state 	= hotkey.button_state;
			return *this;
		}


		bool Hotkey::operator == (const Hotkey& hotkey) const
		{
			return (action 				== hotkey.action &&
			        key 	 				== hotkey.key    &&
			        comment 			== hotkey.comment &&
			        button_state 	== hotkey.button_state);
		}

		bool Hotkey::operator == (const QKeyEvent& e) const
		{

			return (key 	 				== e.key() 		&&
			        button_state 	== e.modifiers()	);
		}

		bool Hotkey::set(const String& data) 
		{
			vector<String> fields;
			Size nr = data.split(fields, String('#').c_str());
			if (nr < 3)
			{
				Log.error() << (String)(qApp->translate("BALL::VIEW::Hotkey", "Could not parse Hotkey ")) << data << std::endl;
				return false;
			}

			try
			{
				key = (Qt::Key) fields[0].toUnsignedInt();
				button_state = (Qt::KeyboardModifiers) fields[1].toUnsignedInt();
				action = fields[2];
				if (nr > 3)
				{
					comment = fields[3];
				}
			}
			catch(...)
			{
				Log.error() << (String)qApp->translate("BALL::VIEW::Hotkey", "Could not parse Hotkey ") << data << std::endl;
				return false;
			}

			return true;
		}


		void Hotkey::get(String& data) const 
		{
			data = String(key) + "#" + String(button_state) + "#" + action + "#" + comment;
		}

		// ==========================================================

		void PyWidget::MyLineEdit::keyPressEvent(QKeyEvent* e) 
		{ 
			if (pw_->keyPressed(e)) return;

			QLineEdit::keyPressEvent(e);
		}

		void PyWidget::MyTextEdit::keyPressEvent(QKeyEvent* e) 
		{ 
			setUndoRedoEnabled(true);

			if (e->key() == Qt::Key_Right&&
			    e->modifiers() == Qt::ShiftModifier)
			{
				pw_->showCompletion();
				return;
			}

			// if return is pressed: add whitespaces 
			if (e->key() != Qt::Key_Return)
			{
				TextEditorWidget::keyPressEvent(e);
				return;
			}

			QTextCursor ct = textCursor();
			QTextBlock block = ct.block();
			String text = ascii(block.text());

			String prefix;

			// collect whitespaces in last line
			for (Position p = 0; p < text.size(); p++)
			{
				if (String::isWhitespace(text[p]))
				{
					prefix += text[p];
				}
				else
				{
					break;
				}
			}


			if (text.hasSuffix(":"))
			{
				prefix += '\t';
			}

			TextEditorWidget::keyPressEvent(e);
			insertPlainText(prefix.c_str());
		}


		//
		// ######################################################################################################

		PyWidget::PyWidget(QWidget *parent, const char *name)
			: DockWidget(parent, name),
			valid_(false),
			started_startup_script_(false),
			current_line_(0),
			multi_line_mode_(false),
			multi_lines_(0),
			history_position_(0),
			thread_(0),
			stop_script_(false),
			intend_(0),
			running_(false),
			silent_(false),
			full_silent_(false),
			script_mode_(false)
		{
#ifdef BALL_VIEW_DEBUG
			Log.error() << "new PyWidget " << this << std::endl;
#endif

			tab_widget_ = new QTabWidget(this);
			BALL_ASSIGN_NAME(tab_widget_)
				setGuest(*tab_widget_);

			///////////////////////////////////////////////////////
			// widgets for instant mode:
			QWidget* widget = new QWidget(this);
			BALL_ASSIGN_NAME(widget)
			text_edit_ = new TextEditorWidget(widget);
			
			BALL_ASSIGN_NAME(text_edit_)
			text_edit_->setLineWrapMode(TextEditorWidget::WidgetWidth);
			text_edit_->setReadOnly(true);
			text_edit_->setTabStopWidth((Position)(text_edit_->tabStopWidth() / 3.0));
			text_edit_->setContextMenuPolicy(Qt::CustomContextMenu);
			text_edit_->setToolTip(tr("Output for single Python commands"));
			QPalette pal = text_edit_->palette();
			QColor color = pal.color(QPalette::Window);
			pal.setColor(QPalette::Base, color.dark(100));
			text_edit_->setPalette(pal);

			line_edit_ = new MyLineEdit(widget);
			BALL_ASSIGN_NAME(line_edit_)
			line_edit_->setPyWidget(this);
			line_edit_->setToolTip(tr("Enter a Python command and press return for execution. Cursor right at end of line for completion, Enter or Shift-F1 for documentation."));
			line_edit_->setValidator(&validator_);
			combo_box_ = new QComboBox(widget);
			BALL_ASSIGN_NAME(combo_box_)
			combo_box_->setMaxVisibleItems(30);

			QGridLayout* lay = new QGridLayout(widget);
			lay->setMargin(0);
			lay->addWidget(text_edit_,0, 0, 1, 2);
			lay->addWidget(line_edit_,1, 0, 1, 1);
			lay->addWidget(combo_box_,1, 1, 1, 1);
			connect(line_edit_, SIGNAL(returnPressed()), this, SLOT(returnPressed()));
			connect(text_edit_, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)));
			connect(combo_box_, SIGNAL(activated(int)), this, SLOT(completionSelected_()));

			combo_box_->hide();
			tab_widget_->addTab(widget, tr("Instant Mode"));

			editor_combo_box_ = new QComboBox(0);
			BALL_ASSIGN_NAME(editor_combo_box_)
			editor_combo_box_->setMaxVisibleItems(30);
			connect(editor_combo_box_, SIGNAL(activated(int)), this, SLOT(completionSelected_()));
			editor_combo_box_->hide();


			///////////////////////////////////////////////////////
			// widgets for edit mode:
			QSplitter *splitter = new QSplitter();
			BALL_ASSIGN_NAME(splitter)
			splitter->setOrientation(Qt::Vertical);
			tab_widget_->addTab(splitter, tr("Scripting Mode"));

			script_edit_ = new MyTextEdit(this);
			BALL_ASSIGN_NAME(script_edit_)
			script_edit_->setLineWrapMode(TextEditorWidget::WidgetWidth);
			script_edit_->setContextMenuPolicy(Qt::CustomContextMenu);
			script_edit_->setTabStopWidth((Position)(script_edit_->tabStopWidth() / 3.0));
			script_edit_->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum));
			script_edit_->setPyWidget(this);
			connect(script_edit_, SIGNAL(cursorPositionChanged()), this, SLOT(printCursorPosition_()));
			//   	script_edit_->setToolTip("Editor for Python scripts, for blue printed capital words, a BALL documentation entry is available. Press right mouse button for a context menu.");
			splitter->addWidget(script_edit_);

			widget = new QWidget(this);
			widget->setObjectName("widget2");
			QPushButton* run_button = new QPushButton(widget);
			BALL_ASSIGN_NAME(run_button)
			run_button->setAutoDefault(true);
			run_button->setDefault(true);
			run_button->setText(tr("Run"));
			connect(run_button, SIGNAL(clicked()), this, SLOT(runCurrentScript()));

			QPushButton* save_button = new QPushButton(widget);
			BALL_ASSIGN_NAME(save_button)
			save_button->setText(tr("Save as"));
			connect(save_button, SIGNAL(clicked()), this, SLOT(saveScript()));

			QPushButton* load_button = new QPushButton(widget);
			BALL_ASSIGN_NAME(load_button)
			load_button->setText(tr("Load"));
			connect(load_button, SIGNAL(clicked()), this, SLOT(loadScript()));

			QPushButton* clear_button = new QPushButton(widget);
			BALL_ASSIGN_NAME(clear_button)
			clear_button->setText(tr("Clear"));
			connect(clear_button, SIGNAL(clicked()), this, SLOT(clearScript()));

			QHBoxLayout *hlayout = new QHBoxLayout;
			hlayout->setMargin(1);
			hlayout->addWidget(run_button);
			hlayout->addWidget(save_button);
			hlayout->addWidget(load_button);
			hlayout->addWidget(clear_button);
			widget->setLayout(hlayout);
			widget->setMaximumSize(QSize(22222, save_button->height() + 2)); 
			widget->setMinimumSize(QSize(22, save_button->height() + 2)); 

			splitter->addWidget(widget);

			script_output_ = new TextEditorWidget();
			BALL_ASSIGN_NAME(script_output_)
			script_output_->setLineWrapMode(TextEditorWidget::WidgetWidth);
			script_output_->setTabStopWidth((Position)(script_output_->tabStopWidth() / 3.0));
			script_output_->setReadOnly(true);
			script_output_->setPalette(pal);
			script_output_->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum));
			script_output_->setToolTip(tr("Output of Python scripts"));
			connect(script_edit_, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showEditContextMenu(const QPoint&)));
			splitter->addWidget(script_output_);

			default_visible_ = false;
			QFont font = text_edit_->document()->defaultFont();
			font.setPointSize(12);
			text_edit_->document()->setDefaultFont(font);
			script_edit_->document()->setDefaultFont(font);
			script_output_->document()->setDefaultFont(font);
			registerWidget(this);

			current_script_ = Directory::getUserHomeDir() + FileSystem::PATH_SEPARATOR + "last_BALL_script.py";
		}

		PyWidget::~PyWidget()
		{
			delete editor_combo_box_;
		}

		void PyWidget::initializeWidget(MainControl& main_control)
		{
			DockWidget::initializeWidget(main_control);
			registerForHelpSystem(this, "pythonInterpreter.html");

	insertMenuEntry(MainControl::TOOLS_PYTHON, tr("Load Python Script"), this,
									SLOT(loadScript()), "Shortcut|Tools|Python|Load_script",
									QKeySequence(), tr(""), UIOperationMode::MODE_ADVANCED);

	insertMenuEntry(MainControl::TOOLS_PYTHON, tr("Save Python Script"), this,
									SLOT(saveScript()), "Shortcut|Tools|Python|Save_script",
									QKeySequence(), tr(""), UIOperationMode::MODE_ADVANCED);

	insertMenuEntry(MainControl::TOOLS_PYTHON, tr("Exec Python Script"), this,
									SLOT(execScript()), "Shortcut|Tools|Python|Exec_script",
									QKeySequence(), tr(""), UIOperationMode::MODE_ADVANCED);

	insertMenuEntry(MainControl::TOOLS_PYTHON, tr("Run Current Script"), this,
									SLOT(runCurrentScript()), "Shortcut|Tools|Python|Run_current_script",
									QKeySequence(), tr(""), UIOperationMode::MODE_ADVANCED);

	insertMenuEntry(MainControl::TOOLS_PYTHON, tr("Abort Python Script"), this,
									SLOT(abortScript()), "Shortcut|Tools|Python|Abort_script",
									QKeySequence(), tr(""), UIOperationMode::MODE_ADVANCED);

	insertMenuEntry(MainControl::TOOLS_PYTHON, tr("Export History"), this,
									SLOT(exportHistory()), "Shortcut|Tools|Python|Export_history",
									QKeySequence(), tr(""), UIOperationMode::MODE_ADVANCED);

	insertMenuEntry(MainControl::TOOLS_PYTHON, tr("Clear Editor"), this,
									SLOT(clearScript()), "Shortcut|Tools|Python|Clear_editor",
									QKeySequence(), tr(""), UIOperationMode::MODE_ADVANCED);
			startInterpreter();

			valid_ = PyInterpreter::isValid();

			if (!valid_)
			{
		QMenu* menu = getMainControl()->initPopupMenu(MainControl::TOOLS_PYTHON, UIOperationMode::MODE_ADVANCED);
		if (menu)
			menu->setEnabled(false);
				appendText("> "+ (String)tr("No Python support available:"), true);
				runString("import BALL");
				setEnabled(false);
				setStatusbarText((String)tr("No Python support available! (See PyWidget)"), true);
				return;
			}
		}


		void PyWidget::finalizeWidget(MainControl& main_control)
		{
			abortScript();
			storeScript_();
			PyInterpreter::finalize();
			DockWidget::finalizeWidget(main_control);
		}


		void PyWidget::initializePreferencesTab(Preferences &preferences)
		{
			python_settings_= new PythonSettings();
			preferences.insertEntry(python_settings_);
			preferences_ = &preferences;
		}

		void PyWidget::finalizePreferencesTab(Preferences &preferences)
		{
			if (python_settings_ != 0)
			{
				preferences.removeEntry(python_settings_);
				python_settings_ = 0;
			}
		}

		list<Hotkey>::iterator PyWidget::findKey_(Hotkey& hotkey)
		{
			list<Hotkey>::iterator it = hotkeys_.begin();
			for (; it != hotkeys_.end(); it++)
			{
				Hotkey& this_key = *it;
				if (this_key.button_state == hotkey.button_state &&
				    this_key.key == hotkey.key)
				{
					return it;
				}
			}

			return it;
		}

		void PyWidget::map(String modifier, String key, String command, String comment)
		{
			bool ok;
			Hotkey hotkey = Hotkey::createHotkey(modifier, key, command, ok, comment);

			if (!ok)
			{
				setStatusbarText((String)tr("Problem mapping key"), true);
				return;
			}

			list<Hotkey>::iterator it = findKey_(hotkey);
			if (it != hotkeys_.end())  *it = hotkey; 
			else 											 hotkeys_.push_back(hotkey);

			python_settings_->setContent(hotkeys_);
			applyPreferences();
		}

		void PyWidget::unmap(String modifier, String key)
		{
			bool ok;
			Hotkey hotkey = Hotkey::createHotkey(modifier, key, "", ok);

			if (!ok)
			{
				setStatusbarText((String)tr("Problem unmapping key"), true);
				return;
			}

			list<Hotkey>::iterator it = findKey_(hotkey);
			if (it != hotkeys_.end()) hotkeys_.erase(it);

			python_settings_->setContent(hotkeys_);
			applyPreferences();
		}


		void PyWidget::applyPreferences()
		{
			DockWidget::applyPreferences();

			if (python_settings_ == 0) return;	

			////////////////////////////////////////////////
			QFont font = python_settings_->getEditorFont();
			text_edit_->document()->setDefaultFont(font);
			script_edit_->document()->setDefaultFont(font);
			script_output_->document()->setDefaultFont(font);

			////////////////////////////////////////////////
			hotkeys_ = (python_settings_->getContent());

	QMenu* menu = getMainControl()->initPopupMenu(MainControl::USER, UIOperationMode::MODE_ADVANCED);
	if (menu)
	{
		menu->clear();

		list<Hotkey>::iterator it = hotkeys_.begin();
		for (; it != hotkeys_.end(); it++)
		{
			String entry = (*it).action;
			if (entry.size() > 35)
			{ 
				entry.truncate(32);
				entry += "...";
			}

			QKeySequence seq((*it).key);
			if ((*it).button_state != Qt::NoModifier)
			{
				String prefix;
				if      ((*it).button_state == Qt::SHIFT) prefix = "Shift";
				else if ((*it).button_state == Qt::CTRL)  prefix = "Ctrl";
				else BALLVIEW_DEBUG

					seq = QKeySequence(((prefix + "+F") + String((*it).key - Qt::Key_F1 + 1)).c_str());
			}

			QString comment = QString((*it).comment.c_str());
			if (comment == "")
			{
				comment = tr("Run a Python script");
			}

			QAction* action = insertMenuEntry(MainControl::USER, entry.c_str(), this, 
			                                  SLOT(hotkeyItem()), "", seq, comment, UIOperationMode::MODE_ADVANCED);
			if (action) action->setData((*it).action.c_str());
			setMenuHelp(action, "pythonInterpreter.html#create_hotkeys");
		}

		getMainControl()->insertPopupMenuSeparator(MainControl::USER, UIOperationMode::MODE_ADVANCED);
		QAction* action = insertMenuEntry(MainControl::USER, tr("Modify"), this, SLOT(modifyHotkeys()), "", QKeySequence(),
		                                  tr("Manage user defined Python commands"), UIOperationMode::MODE_ADVANCED);
		setMenuHelp(action, "pythonInterpreter.html#create_hotkeys");
	}

			/////////////////////////////////////////
			// startup script
			/////////////////////////////////////////
			if (started_startup_script_ || !isValid())
			{
				return;
			}

			String temp = current_script_;

			started_startup_script_ = true;

			Path p;

			String startup = p.find("startup.py");
			if (!openFile(startup, true))
			{
				Log.error() << (String)tr("Could not find startup script. Please set the correct path to the data path!") << std::endl;
				Log.error() << (String)tr("To do so set the environment variable BALL_DATA_PATH or BALLVIEW_DATA_PATH.") << std::endl;
			}

			String user_startup = python_settings_->getFilename();
			if (user_startup != "") 
			{
				openFile(user_startup, true);
			}

			script_edit_->clear();
			openFile(current_script_, false);
			appendText("> " + (String)tr("Ready"), false, true);
		}

		void PyWidget::modifyHotkeys()
		{
			preferences_->showEntry(python_settings_->widget_stack->widget(1));
			preferences_->show();
		}

		void PyWidget::hotkeyItem()
		{
			if (sender() == 0) return;

			QAction* action = dynamic_cast<QAction*>(sender());

			if (action == 0) return;

			runString(ascii(action->data().toString()));
		}

		void PyWidget::abortScript()
		{
			setStatusbarText((String)tr("Aborting Python script"));
			stop_script_ = true;
		}

		PyWidget::PyWidget(const PyWidget& p)
			: DockWidget(p)
		{}

		bool PyWidget::toAbortScript() 
		{
			return stop_script_;
		}

		void PyWidget::insertHotkey(const Hotkey& hotkey) 
		{
			list<Hotkey>::iterator it = hotkeys_.begin();
			for (; it != hotkeys_.end(); it++)
			{
				if ((*it) == hotkey) return;
			}

			hotkeys_.push_back(hotkey);
		}

		void PyWidget::removeHotkey(const Hotkey& hotkey) 
		{
			list<Hotkey>::iterator it = hotkeys_.begin();
			for (; it != hotkeys_.end(); it++)
			{
				if ((*it) == hotkey) return;
			}

			hotkeys_.erase(it);
		}

		void PyWidget::reactTo(const QKeyEvent& e) 
		{
			if (e.key() == Qt::Key_F1 &&
			    e.modifiers() == Qt::ShiftModifier)
			{
				showDocumentation();
				return;
			}

			list<Hotkey>::iterator it = hotkeys_.begin();
			for (; it != hotkeys_.end(); it++)
			{
				if ((*it) == e) 
				{
					runString((*it).action);
					return;
				}
			}
		}

		bool PyWidget::runCurrentScript()
		{
			tab_widget_->setCurrentIndex(1);
			script_output_->clear();
			bool state = storeScript_() && openFile(current_script_, true, true);
			String result;
			if (state) result = (String)tr("Finished script sucessfully");
			else 			 result = (String)tr("Error occured in Script");
			setStatusbarText(result, true);
			return state;
		}

		void PyWidget::setError_(bool state)
		{
			QColor color;
			if (state) color = Qt::red;
			else 			 color = QColor("forestgreen");

			TextEditorWidget* text_edit = text_edit_;
			if (script_mode_)
			{
				text_edit = script_output_;
			}

			QTextCharFormat tf;
			tf = text_edit->currentCharFormat();
			tf.setForeground(QBrush(Qt::black));
		}

		void PyWidget::appendText(const String& t, bool output, bool state_message)
		{
			if (full_silent_) return;

			String text = t; 
			if (!text.size() || text[text.size() - 1] != '\n')
			{
				text += '\n';
			}

			TextEditorWidget* text_edit = text_edit_;
			if (script_mode_)
			{
				text_edit = script_output_;
			}

			QTextCharFormat tf;
			tf = text_edit->currentCharFormat();

			if (!output && !state_message) tf.setForeground(QBrush(Qt::black));
			if (state_message) tf.setForeground(QBrush(Qt::blue));

			text_edit->setCurrentCharFormat(tf);
			appendText_(text_edit, text);
		}

		void PyWidget::appendText_(TextEditorWidget* text_edit, String text)
		{
			text_edit->setUpdatesEnabled(false);

			QTextCursor ct = text_edit->textCursor();
			if (!ct.atEnd()) 
			{
				ct.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
				text_edit->setTextCursor(ct);
			}

			text_edit->insertPlainText(text.c_str());
			ct.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
			text_edit->setTextCursor(ct);
			text_edit->ensureCursorVisible();
			text_edit->setUpdatesEnabled(true);
		}

		void PyWidget::contentsDragEnterEvent(QDragEnterEvent* )
		{
			//   			e->accept(Q3TextDrag::canDecode(e));
		}

		void PyWidget::contentsDropEvent(QDropEvent*)
		{
			//   			VIEW::processDropEvent(e);
		}

		void PyWidget::exportHistory()
		{
			QString s = QFileDialog::getSaveFileName(
			                                         0, tr("Export History"),
			                                         getWorkingDir().c_str(), "");

			if (s == QString::null) return;
			String filename(ascii(s));
			setWorkingDirFromFilename_(filename);

			File file(filename, std::ios::out);
			if (!file.isOpen()) 
			{
				appendText(("> " + (String)tr("Could not export history to file ") + filename + "\n").c_str(), true);
				newPrompt_();
				return;
			}

			for (Position p = 0; p < history_.size(); p++)
			{
				if (results_[p]) file << history_[p] << std::endl;
			}

			file.close();
		}

		void PyWidget::saveScript()
		{
			QString s = QFileDialog::getSaveFileName(
			                                         0, tr("Save script"),
			                                         getWorkingDir().c_str(), "*.py");

			if (s == QString::null) return;
			String filename(ascii(s));
			setWorkingDirFromFilename_(filename);

			File file(filename, std::ios::out);
			if (!file.isOpen()) 
			{
				appendText(((String)"> " + (String)tr("Could not save script to ") + filename + "\n").c_str(), true);
				newPrompt_();
				return;
			}

			file.close();
			String temp = current_script_;
			current_script_ = filename;
			storeScript_();
			current_script_ = temp;
		}


		void PyWidget::clear()
		{
			text_edit_->clear();
			newPrompt_();
		}

		void PyWidget::paste()
		{
			line_edit_->paste();
		}

		void PyWidget::scriptDialog(bool run)
		{
			if (working_dir_ == "") working_dir_ = getWorkingDir();

			tab_widget_->setCurrentIndex(1);

			QString s = QFileDialog::getOpenFileName(
			                                         0, tr("Choose a Python script"),
			                                         working_dir_.c_str(),
			                                         tr("Python Scripts") + "(*.py)");

			if (s == QString::null) return;
			setWorkingDirFromFilename_(ascii(s));
			working_dir_ = getWorkingDir();

			openFile(ascii(s), run);
		}

		void PyWidget::execScript()
		{
			scriptDialog(true);
		}

		void PyWidget::loadScript()
		{
			scriptDialog(false);
		}

		String PyWidget::getCurrentLine() const
		{
			if (isInDirectMode())
			{
				return String(ascii(line_edit_->text()));
			}

			return "";
		}

		bool PyWidget::openFile(const String& filename, bool run, bool is_current)
		{
			bool dummy;
			PyInterpreter::run("\n", dummy);
			PyInterpreter::run("\n", dummy);

			multi_line_mode_ = false;
			multi_lines_ = 0;
			multi_line_text_ = "";

			script_output_->clear();
			if (!is_current) script_edit_->clear();
			script_mode_ = true;
			stop_script_ = false;
			full_silent_ = filename.hasSuffix("startup.py");

			if (run) appendText(((String)"> " + (String)tr("executing script from ") + filename + "\n").c_str(), false, true);
			else     appendText(((String)"> " + (String)tr("loading script from ") + filename + "\n").c_str(), false, true);
			LineBasedFile file;
			try
			{
				file.open(filename);
			}
			catch	(...)
			{
				appendText(("> " + (String)tr("Could not find file ") + filename + "\n").c_str(), true);
				newPrompt_();
				script_mode_ = false;
				return false;
			}

			// no prompts!
			silent_ = true;

			vector<String> lines;
			while (file.readLine())
			{
				lines.push_back(file.getLine());
			}

			if (!is_current)
			{
				for (Position i = 0; i < lines.size(); i++)
				{
					appendText_(script_edit_, lines[i] + "\n");
				}

				script_edit_->setUndoRedoEnabled(false);
			}

			if (!run) 
			{
				script_mode_ = false;
				return true;
			}

			bool aborted = false;
			for (Position i = 0; i < lines.size() && !aborted; i++)
			{
				current_line_ = i;
				if (!parseLine_(lines[i]))
				{
					String result_string = "> " + (String)tr("Error in line ") + String(i + 1) + (String)tr(" of file ") + filename + "\n";
					appendText(result_string.c_str(), true);
					aborted = true;
				}

				if (stop_script_)
				{
					setStatusbarText((String)tr("Aborted script"), true);
					appendText("> " + (String)tr("aborted..."), true);
					aborted = true;
				}
			}

			silent_ = false;
			full_silent_ = false;
			if (!aborted)
			{
				appendText("> " + (String)tr("Finished."), false, true);
				setStatusbarText((String)tr("Finished script."));
			}
			newPrompt_();
			return !aborted;
		}

		void PyWidget::dump(std::ostream& s, Size depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);

			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_DEPTH(s, depth);
			s << "multiline_mode : " << multi_line_mode_<< std::endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "multi_line_text  : " << multi_line_text_<< std::endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "history : "<< std::endl;

			for (Position i = 0; i < history_.size(); i++)
			{
				BALL_DUMP_DEPTH(s, depth);
				s << history_[i]<< std::endl;
			}

			BALL_DUMP_DEPTH(s, depth);
			s << "history_position : " << history_position_ << std::endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "current_line : " << getCurrentLine() << std::endl;

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void PyWidget::newPrompt_()
		{
			if (silent_) return;

			line_edit_->setText("");

			// intend
			if (multi_line_mode_)
			{
				String s;
				for (Position p = 0; p < intend_; p++) s += "    ";
				line_edit_->setText(s.c_str());
			}
		}

		bool PyWidget::keyPressed(QKeyEvent* e)
		{
			Index hp = history_position_;

			if 			(e->key() == Qt::Key_Up)   			hp--;
			else if (e->key() == Qt::Key_Down) 			hp++;
			else if (e->key() == Qt::Key_PageUp) 		hp = 0;
			else if (e->key() == Qt::Key_PageDown)	hp = history_.size() - 1;
			else if (e->key() == Qt::Key_Right)	
			{
				// show completion
				if (isInDirectMode())
				{
					if (line_edit_->cursorPosition() != (Index) getCurrentLine().size()) return false;
					showCompletion();
					return true;
				}
				else
				{
					if (e->modifiers() == Qt::ShiftModifier)
					{
						showCompletion();
						return true;
					}

					return false;
				}
			}
			else if (e->key() == Qt::Key_Enter)	
			{
				// show docu
				showDocumentation();
				return true;
			}
			else 
			{
				// clear history position
				history_position_ = history_.size();
				return false;
			}

			// show history entry
			if (hp < 0 || hp >= (Index) history_.size()) 
			{
				return true;
			}

			retrieveHistoryLine_((Position)hp);
			return true;
		} 

		bool PyWidget::testMultilineStart_(const String& line)
		{
			if (line.hasSuffix(":")&&
			    (line.hasPrefix("for ") 		|| 
			     line.hasPrefix("def ") 		||
			     line.hasPrefix("class ") 	||
			     line.hasPrefix("while ")	  ||
			     line.hasPrefix("if ")))
			{
				multi_line_mode_ = true;
				multi_line_text_ = line;
				multi_line_text_.append("\n");
				multi_lines_ = 1;

				appendToHistory_(line);
				intend_ = 1;
				newPrompt_();

				return true;
			}

			return false;
		}


		String PyWidget::runCommand_(const String& text, bool& state)
		{
			String result;
#ifndef BALL_PYTHON_USE_THREADS
			result = PyInterpreter::run(text, state);
#else
			thread_->input = text;
			thread_->start();

			while (thread_->running()) 
			{
				qApp->wakeUpGuiThread();
				qApp->processEvents(500);
				thread_->wait(50);
			}

			result = thread_->output;
			state = thread_->state;
#endif

			return result;
		}


		bool PyWidget::parseLine_(String line)
		{
			if (!Py_IsInitialized())
			{
				appendText("> " + (String)tr("ERROR: no interpreter running!\n"), true);
				return false;
			}

			line.trimRight();

			// check for comments
			String temp(line);
			temp.trim();
			if (temp.hasPrefix("#")) 
			{
				appendText(line);
				return true;
			}

			// singe <-> multi line mode
			if (!multi_line_mode_)
			{
				if (line.isEmpty()) return true;

				appendText(line);
				if (testMultilineStart_(line)) return true;

				multi_lines_ = 0;
				appendToHistory_(line);
			}
			else // Multiline mode
			{
				multi_lines_ += 1;

				appendText(line);
				if (!line.isEmpty())
				{
					if (line[line.size() - 1] == ':')
					{
						intend_ ++;
					}

					multi_line_text_ += line + "\n";
					appendToHistory_(line);
					newPrompt_();	
					return true;
				}

				line = multi_line_text_ + "\n";
				intend_ = 0;
			}

			// ok, now run the commad !
			bool state;
			String result = runCommand_(line, state);

			// prevent double output, when running a script:
			if (line == "runCurrentScript()")  result = "";

			// print results:
			if (result != "") 
			{
				// did an error occur?
				setError_(!state);
				if (!state)
				{
					appendText(result, true);
					findError_(result);
				}
				else
				{
					result = String("> ") + result;
					appendText(result, true);
				}

				QTextCharFormat tf;
				tf = text_edit_->currentCharFormat();
				tf.setForeground(QBrush(Qt::black));
				
				result = "";
			}

			multi_line_mode_ = false;

			if (silent_) 
			{
				return state;
			}

			if (!multi_line_mode_)
			{
				results_.push_back(state);
			}
			else
			{
				for (Position p = 0; p <= multi_lines_ -1; p++)
				{
					results_.push_back(state);
				}
			}

			newPrompt_();
			return state;
		}

		bool PyWidget::runString(String command)
		{
			if (getMainControl()->isBusy()) return false;

			if (running_) return false;

			running_ = true;
			bool result(false);

			if (!command.has('\n'))
			{
				result = parseLine_(command);
			}
			else
			{
				vector<String> lines;
				Size nr = command.split(lines, String('\n').c_str());
				for (Position p = 0; p < nr; p++)
				{
					if (!parseLine_(lines[p])) result = false;
					break;
				}
			}

			running_ = false;

			return result;
		}


		void PyWidget::appendToHistory_(const String& line)
		{
			if (silent_) return;
			history_.push_back(line);
			history_position_ = history_.size();
		}

		const char* PyWidget::getPrompt_() const
		{
			return (multi_line_mode_ ? ". " : "");
		}

		void PyWidget::retrieveHistoryLine_(Position index)
		{
			if (index > history_.size()) 
			{
				history_position_ = history_.size() - 1;
				return;
			}

			line_edit_->setText(history_[index].c_str());
			history_position_ = index;
		}

		bool PyWidget::returnPressed()
		{
			String line = getCurrentLine();

			script_mode_ = false;
			silent_ = false;
			parseLine_(line);
			newPrompt_();
			history_position_ = history_.size();
			script_mode_ = true;

			return true;
		}

		void PyWidget::startInterpreter()
		{
			stop_script_ = false;
			// initialize the interpreter
			PyInterpreter::initialize();

			if (!PyInterpreter::isValid())
			{
				appendText(PyInterpreter::getStartupLog(), false, true);
				return;
			}

			// print the BALL version and clear
			// the widget's contents in case of a restart
			text_edit_->clear();
			appendText(("> "+(String)tr("BALL")+ " " + VersionInfo::getVersion()).c_str(), false, true);

			// print the first prompt 
			multi_line_mode_ = false;
			newPrompt_();
			history_position_ = 0;

			bool state;
			runCommand_("import BALL", state);
			String result = runCommand_("dir(BALL)", state);
			vector<String> sv;
			result.split(sv, "[,\'] ");

			for (Position p = 0; p < sv.size(); p++)
			{
				// no Python special commands and only classes (begin with capital letter)
				if (sv[p][0] < 65 || sv[p][0] > 90) continue;

				highlighter_1_.BALL_keywords << sv[p].c_str();
				highlighter_2_.BALL_keywords << sv[p].c_str();
			}

			highlighter_1_.compilePattern();
			highlighter_2_.compilePattern();
			highlighter_1_.setDocument(script_edit_->document());
			highlighter_2_.setDocument(text_edit_->document());
		}

		void PyWidget::showCompletion()
		{
			String string;
			if (isInDirectMode()) 
			{
				string = getCurrentLine();
			}
			else 
			{
				QTextCursor cursor_pos = script_edit_->textCursor();
				string = getCurrentWord_(cursor_pos);
			}

			QStringList sl;

			if (string.hasSuffix(")") ||
			    string.hasSuffix("(") ||
			    !getClassAndMember_(string) ||
			    !getMembers(class_, sl, member_) ||
			    sl.size() == 0) 
			{
				setStatusbarText((String)tr("No completion found"), true);
				return;
			}

			// only one possibility: add it!
			if (sl.size() == 1)
			{
				String result = ascii(*sl.begin());
				String cl = getCurrentLine();

				if (result.size() <= member_.size()) 
				{
					setStatusbarText((String)tr("No completion found"), true);
					return;
				}

				String ss = result.getSubstring(complete_prefix_);

				if (!isInDirectMode())
				{
					script_edit_->insertPlainText(ss.c_str());
					return;
				}

				cl += ss;
				line_edit_->setText(cl.c_str());
				if (cl[cl.size() - 1] == ')')
				{
					line_edit_->setCursorPosition(cl.size() - 1);
				}
				return;
			}

			QComboBox* box = getCompletionBox_();

			// more then one: show combobox
			box->clear();
			box->addItems(sl);
			if (isInDirectMode())
			{
				box->show();
				box->showPopup();
				box->resize(width() / 2, line_edit_->size().height());
			}
			else
			{
				QRect rect = script_edit_->cursorRect();
				QPoint point(rect.x(), rect.y());
				point = script_edit_->mapToGlobal(point);
				box->move(point + QPoint(20,-10));
				box->showPopup();
				box->raise();
			}
		}

		bool PyWidget::getMembers(const String& classname, QStringList& sl, const String& prefix)
		{
			bool state;
			String cmd = String("dir(") + classname + ")";
			String result = runCommand_(cmd , state);
			if (!state) return false;

			vector<String> sv;
			result.split(sv, "[,\'] ");

			bool has_prefix = prefix != "";
			bool has_class  = classname != "";
			for (Position p = 0; p < sv.size(); p++)
			{
				// remove Python special commands
				if (sv[p].hasPrefix("__")) continue;

				// do we have a prefix for the member to find?
				if (has_prefix && !sv[p].hasPrefix(prefix)) continue;

				String res = sv[p];

				// add brackets to methods
				if (has_class)
				{
					char c = res[0];
					if (c >= 97 && c <= 122)
					{
						res += "()";
					}
				}

				String test = res;
				res.trim();
				if (res == "") continue;

				sl << res.c_str();
			}

			complete_prefix_ = prefix.size();

			return true;
		}

		QComboBox* PyWidget::getCompletionBox_()
		{
			if (isInDirectMode()) return combo_box_;

			return editor_combo_box_;
		}

		bool PyWidget::completionSelected_()
		{
			QComboBox* box = getCompletionBox_();
			String s = ascii(box->currentText());
			if (s == "") return false;

			String cl = getCurrentLine();

			cl += s.getSubstring(complete_prefix_);

			if (isInDirectMode())
			{
				line_edit_->setText(cl.c_str());
				if (cl[cl.size() - 1] == ')')
				{
					line_edit_->setCursorPosition(cl.size() - 1);
				}
			}
			else
			{
				script_edit_->insertPlainText(s.getSubstring(complete_prefix_).c_str());
			}

			box->hide();
			return true;
		}

		void PyWidget::showClassDocu(String classname, String member)
		{
			if (classname == "")
			{
				classname = member;
				member = "";
			}

			notify_(new ShowHelpMessage(classname, "BALL class", member));
		}

		void PyWidget::showDocumentation()
		{
			getClassAndMember_(getCurrentLine());
			showClassDocu(class_, member_);
		}

		bool PyWidget::getClassAndMember_(String toc)
		{
			if (toc == "") return false;

			vector<String> sv;
			// find last parameter or command
			//
			// ??? count opening and closing brackets, if opening > closing...
			if (toc.split(sv, ", +-=():#") < 1) return false;

			toc = sv[sv.size() - 1];

			bool global = !toc.has('.');
			String complete_prefix;

			if (!global)
			{
				// we do a member completion here...
				toc.split(sv, ".");
				// begin of command: all after the dot
				if (sv.size() > 1)
				{
					complete_prefix = sv[sv.size() - 1];
				}
				else
				{
					complete_prefix = "";
				}

				if (sv.size() == 0) return false;
				String cmd = sv[0];
				cmd += ".__class__";

				bool state;
				String result = runCommand_(cmd, state);
				if (!state) return false;

				vector<String> sv;
				result.split(sv, "[,\'] <>.");
				// e.g. class BALL Atom
				if (sv.size() < 3) return false;

				class_ = sv[2];
				member_ = complete_prefix;
			}
			else
			{
				// global search: not much to be done
				complete_prefix = toc;
				class_ = "";
			}

			member_ = complete_prefix;

			return true;
		}

		void PyWidget::showContextMenu(const QPoint& point)
		{
			TextEditorWidget* text_edit = text_edit_;
			if (!isInDirectMode()) text_edit = script_output_;

			QMenu menu;

			// "copy" action
			QAction* action1 = menu.addAction("Copy", text_edit, SLOT(copy()));
			action1->setEnabled(text_edit->textCursor().hasSelection());

			createMenuHelpEntry_(&menu, text_edit, point);

			menu.addSeparator();

			QAction* action2 = menu.addAction(tr("Create script"), this, SLOT(createScript_()));
			if (history_.size() == 0) action2->setEnabled(false);

			QAction* action3 = menu.addAction(tr("Clear history"), this, SLOT(clearHistory_()));
			if (history_.size() == 0) action3->setEnabled(false);

			menu.exec(mapToGlobal(point));
		}

		void PyWidget::showEditContextMenu(const QPoint& point)
		{
			QMenu* menu = script_edit_->createStandardContextMenu();
			menu->addSeparator();

			createMenuHelpEntry_(menu, script_edit_, point);
			menu->exec(mapToGlobal(point));
			delete menu;
		}

		String PyWidget::getCurrentWord_(QTextCursor& cursor_pos)
		{
			// "Help for" action
			const Position pos = cursor_pos.position() - cursor_pos.block().position();
			String text = ascii(cursor_pos.block().text());
			String string = text.getSubstring(0, pos); 
			String delim(". ()=:,+-*\"|&"); 
			for (Position i = pos; i < text.size(); i++)
			{
				if (delim.has(text[i])) break;
				string += text[i];
			}

			return string;
		}

		void PyWidget::createMenuHelpEntry_(QMenu* menu, TextEditorWidget* text_edit, const QPoint& point)
		{
			QTextCursor cursor_pos = text_edit->cursorForPosition(point);
			String string = getCurrentWord_(cursor_pos);
			bool found = getClassAndMember_(string);
			String entry(tr("Help for: "));

			if (found)
			{
				String key;

				if (class_ != "") key = class_+ "." + member_;
				else key = member_;
				key.trim();
				entry += key;
			}

			QAction* action = menu->addAction(entry.c_str(), this, SLOT(showHelp_()));
			if (!found) action->setEnabled(false);
		}

		void PyWidget::showHelp_()
		{
			showClassDocu(class_, member_);
		}

		void PyWidget::writePreferences(INIFile& inifile)
		{
			ModularWidget::writePreferences(inifile);
			inifile.appendSection("PYTHON");
			inifile.insertValue("PYTHON", "current_script", current_script_);
		}

		void PyWidget::fetchPreferences(INIFile& inifile)
		{
			ModularWidget::fetchPreferences(inifile);
			if (inifile.hasEntry("PYTHON", "current_script"))
			{
				current_script_ = inifile.getValue("PYTHON", "current_script");
			}
		}

		QString PyWidget::getCurrentScript()
		{
			QString result = script_edit_->document()->toPlainText();

			int pos = 0;
			validator_.validate(result, pos);

			return result;
		}

		bool PyWidget::storeScript_()
		{
			if (current_script_ == "")
			{
				String filename = VIEW::createTemporaryFilename();
				if (filename.size() == 0)
				{
					setStatusbarText((String)tr("Could not create temp file in user home dir! Please save script manualy once."), true);
					return false;
				}
				current_script_ = filename;
			}

			QString qtext = getCurrentScript();
			if (qtext.size() == 0) 
			{
				setStatusbarText((String)tr("Editor is empty, aborting..."), false);
				return false;
			}

			String text = ascii(qtext);
			if (text[text.size() - 1] != '\n')
			{
				text += '\n';
			}

			try
			{
				File file(current_script_, std::ios::out);
				file << text;
				file.close();
			}
			catch(...)
			{
				setStatusbarText((String)tr("Could not write to file! Please save script manualy once."), true);
				current_script_ = "";
				return false;
			}

			return true;
		}

		void PyWidget::clearScript()
		{
			script_edit_->clear();
			script_output_->clear();
		}

		// mark the line in a script where an error occured:
		void PyWidget::findError_(String result)
		{
			if (!script_mode_) return;

			vector<String> lines;
			result.split(lines, String('\n').c_str());

			String error;

			for (Position p = 0; p < lines.size(); p++)
			{
				// for syntax errors:
				if (lines[p].hasSuffix("^") && p > 0)
				{
					error = lines[p - 1];
					error.trim();
					break;
				}

				// for exceptions:
				if (lines[p].hasSubstring("Error: "))
				{
					vector<String> fields;
					Size nr = lines[p].split(fields, "'");
					if (nr == 3 || nr == 2)
					{
						error = fields[1];
						break;
					}
				}
			}

			QTextDocument* docu = script_edit_->document();
			QTextCursor cursor(docu->begin());

			if (error != "") 
			{
				QTextCursor end_pos(docu->begin());
				end_pos.movePosition(QTextCursor::NextBlock, QTextCursor::MoveAnchor, current_line_);
				end_pos.movePosition(QTextCursor::EndOfLine);

				while (!cursor.isNull())
				{
					cursor = docu->find(error.c_str(), cursor);
					if (cursor.isNull() ||
					    cursor > end_pos)
					{
						break;
					}

					script_edit_->setTextCursor(cursor);
				}

				if (!cursor.isNull()) return;
			}

			cursor = QTextCursor(docu->begin());
			cursor.movePosition(QTextCursor::NextBlock, QTextCursor::MoveAnchor, current_line_);
			cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
			script_edit_->setTextCursor(cursor);
		}

		void PyWidget::printCursorPosition_()
		{
			if (script_edit_ == 0 || !script_edit_->isVisible()) return;

			QTextDocument* docu = script_edit_->document();
			QTextCursor cursor(script_edit_->textCursor());
			QTextBlock tb = cursor.block();

			Size line_count = 1;
			for (QTextBlock block = docu->begin(); block < tb; block = block.next(), ++line_count);


			Size x = cursor.position() - tb.position();
			setStatusbarText(String("Line ") + String(line_count) + ", Col " + String(x));
		}

		bool PyWidget::canHandle(const String& fileformat) const
		{
			return fileformat == "py" || fileformat == "PY";
		}

		bool PyWidget::openFile(const String& filename)
		{
			return openFile(filename, true);
		}

		void PyWidget::createScript_()
		{
			script_edit_->clear();
			for (Position i = 0; i < history_.size(); i++)
			{
				if (results_[i]) appendText_(script_edit_, history_[i] + "\n");
			}

			tab_widget_->setCurrentIndex(1);
		}

		void PyWidget::clearHistory_()
		{
			text_edit_->clear();
			history_.clear();
			results_.clear();
		}

		void PyWidget::checkMenu(MainControl& main_control)
		{
	QMenu* menu = main_control.initPopupMenu(MainControl::TOOLS_PYTHON, UIOperationMode::MODE_ADVANCED);
	if (menu)
		menu->setEnabled(!main_control.isBusy());
		}

		bool PyWidget::isInDirectMode() const
		{
			return (tab_widget_->currentIndex() == 0);
		}

	} // namespace VIEW
} // namespace BALL
