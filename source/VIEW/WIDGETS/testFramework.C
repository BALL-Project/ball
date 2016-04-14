// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/WIDGETS/testFramework.h>
#include <BALL/VIEW/WIDGETS/dockWidget.h>
#include <BALL/VIEW/DIALOGS/molecularFileDialog.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/KERNEL/system.h>
#include <BALL/SYSTEM/path.h>

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QApplication>
#include <QtWidgets/QTreeView>
#include <QtTest/QTest>
#include <QtTest/QSpontaneKeyEvent>

#ifdef BALL_PYTHON_SUPPORT
 #include <BALL/VIEW/WIDGETS/pyWidget.h>
 #include <BALL/PYTHON/pyInterpreter.h>
#endif

namespace BALL
{
	namespace VIEW
	{

#define MOUSE 		1
#define KEY 			2
#define SHORTCUT 	3

TestFramework::MyEventThread::MyEventThread()
	:QThread(),
	 tf(0)
{
}

void TestFramework::MyEventThread::run()
{
	if (tf == 0) return;
	while (tf->test_running_ && !mc->isAboutToQuit())
	{
		msleep(20);
		QEvent* e = new QEvent((QEvent::Type)9999);
		qApp->postEvent(tf, e);
	}
}

TestFramework::TestFramework(QWidget *parent, const char *name)
	: QWidget(parent),
		ModularWidget(name),
		widget_(0),
		test_running_(false),
		recording_(false),
		errors_(0),
		last_event_(0),
		is_command_line_(false),
		max_run_time_(FLT_MAX)
{
 	hide();
 	default_visible_ = false;
	registerWidget(this);
	setObjectName(name);
	thread_.tf = this;
	thread_.mc = getMainControl();
}

TestFramework::~TestFramework()
{
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "Destructing TestFramework " << this << std::endl;
	#endif 
}


void TestFramework::initializeWidget(MainControl& mc)
{
	String description = "Shortcut|MainControl|Macro|Start";
	save_file = insertMenuEntry(MainControl::MACRO, tr("Start a new macro"), this,
				    SLOT(saveFile()), description, QKeySequence(),
				    tr("Select a file to start a new macro"),
				    UIOperationMode::MODE_ADVANCED);

	description = "Shortcut|MainControl|Macro|Record";
	start_recording = insertMenuEntry(MainControl::MACRO, tr("Record macro"), this,
					  SLOT(startTest()), description, QKeySequence(),
					  tr("Record a macro"),
					  UIOperationMode::MODE_ADVANCED);

	description = "Shortcut|MainControl|Macro|Stop";
	stop_recording = insertMenuEntry(MainControl::MACRO, tr("Stop recording"), this, 
					 SLOT(stopTest()), description, QKeySequence(),
					 tr("Stop the recording"), 
					 UIOperationMode::MODE_ADVANCED);
	
	description = "Shortcut|MainControl|Macro|Move";
	switch_move = insertMenuEntry(MainControl::MACRO, tr("Record mouse moves"), this, 
		      SLOT(switchRecordMove()), description, QKeySequence(),
		      tr("Enable or disable storing of events where the mouse only moved"), 
		      UIOperationMode::MODE_ADVANCED);
	
	if(switch_move)
	{
	  switch_move->setCheckable(true);
	  switch_move->setChecked(true);
	}

	mc.insertPopupMenuSeparator(MainControl::MACRO, UIOperationMode::MODE_ADVANCED);

	description = "Shortcut|MainControl|Macro|Load";
	load_file = insertMenuEntry(MainControl::MACRO, tr("Load macro"), this, 
				    SLOT(loadFile()), description, QKeySequence(),
				    tr("Load a macro"), 
				    UIOperationMode::MODE_ADVANCED);

	description = "Shortcut|MainControl|Macro|Run";
	run_test = insertMenuEntry(MainControl::MACRO, tr("Run macro"), this, 
				   SLOT(runTest()), description, QKeySequence(),
				   tr("Run a loaded macro"), 
				   UIOperationMode::MODE_ADVANCED);

	description = "Shortcut|MainControl|Macro|Abort";
	abort_test = insertMenuEntry(MainControl::MACRO, tr("Abort macro"), this, 
				     SLOT(abortTest()), description, QKeySequence(),
				     tr("Abort the current macro"),
				     UIOperationMode::MODE_ADVANCED);
}

void TestFramework::switchRecordMove()
{
}

void TestFramework::saveFile()
{
	QString file = QFileDialog::getSaveFileName(0,
										"Choose a test file",
										getWorkingDir().c_str(),
										"*.btf");

	if (ascii(file) == "") return;

	filename_ = ascii(file);
	setWorkingDirFromFilename_(filename_);
}

void TestFramework::loadFile()
{
	QString file = QFileDialog::getOpenFileName(0,
										"Choose a test file",
										getWorkingDir().c_str(),
										"*.btf");

	if (ascii(file) == "") return;

	filename_ = ascii(file);
	setWorkingDirFromFilename_(filename_);
}


void TestFramework::abortTest()
{
	type_ = 0;
	test_running_ = false;
	is_command_line_ = false;
	qApp->removeEventFilter(this);
}

void TestFramework::startTest()
{
	qApp->installEventFilter(this);
	outfile_.open(filename_, std::ios::out);
	recording_ = true;
	test_running_ = false;
	last_names_ = "";
	last_event_string_ = "";
 	last_widget_ = 0;
	if (!timer_.start()) timer_.reset();
}


void TestFramework::stopTest()
{
	qApp->removeEventFilter(this);
	recording_ = false;
	outfile_.close();
}

bool TestFramework::eventFilter(QObject* obj, QEvent* e) 
{
	if (test_running_)
	{
//   		if (e == last_event_) return false;
//   		last_event_ = e;

		bool stop = false;
		if (e->type() == QEvent::KeyPress)
		{
			QKeyEvent* ke = dynamic_cast<QKeyEvent*>(e);
			// pause macro if pause key is pressed
			if (ke->key() == Qt::Key_Pause) stop = true;
			else if (ke->key() == Qt::Key_X && ke->modifiers() == Qt::AltModifier)
			{
				// if a user presses Alt-X: quit immediately
				abortTest();
				getMainControl()->quit(0);
				return true;
			}
		}
		else if (e->type() == QEvent::MouseButtonPress ||
						 e->type() == QEvent::MouseButtonRelease)
		{
			// abort macro if user presses mouse button:
			if (!RTTI::isKindOf<MyMouseEvent>(e) && e->spontaneous())
			{
 				stop = true;
			}
		}
		else
		{
			return false;
		}

		if (stop)
		{
			abortTest();
			qApp->installEventFilter(this);
			return true;
		}

		return false;
	}
	
	// if test is paused and pause key is pressed->resume macro
	if (!recording_ && e->type() == QEvent::KeyPress && lines_.size() > 0)
	{
		QKeyEvent* ke = dynamic_cast<QKeyEvent*>(e);
		if (ke->key() == Qt::Key_Pause)
		{
			processEvent_();
			timer_.reset();
			timer_.start();
			test_running_ = true;
			thread_.start();
			return true;
		}

		return false;
	}

	if (!recording_) return false;

	if (!RTTI::isKindOf<QKeyEvent>(e) &&
			!RTTI::isKindOf<QMouseEvent>(e) &&
			!RTTI::isKindOf<QShortcutEvent>(e))
	{
		return false;
	}

	if (e->type() == QEvent::ShortcutOverride) return false;
 	if (e->type() == QEvent::KeyRelease) return false;
	QMouseEvent* 		me = dynamic_cast<QMouseEvent*>(e);
	QKeyEvent* 			ke = dynamic_cast<QKeyEvent*>(e);
	QShortcutEvent* se = dynamic_cast<QShortcutEvent*>(e);

	if (ke != 0 && 
			ke->type() == QEvent::KeyPress &&
			ke->key() == Qt::Key_Pause)
	{
		stopTest();
		return false;
	}

	///////////////////////////////////////////////////////
	// uniquely identify the active widget:
	// walk up the QObject tree and collect all names of QWidgets
	///////////////////////////////////////////////////////
	
	// take the sending object
 	QObject* o = obj;
	QObject* parent = 0;
	x_ = y_ = 0;

	// for mouse events: take widget under the mouse cursor
	if (me != 0) 
	{
		widget_ = qApp->widgetAt(me->globalPos());
		if (widget_ == 0) return false;
		if (widget_->objectName() == "" &&
				widget_->actions().size() == 0)
		{
			widget_ = dynamic_cast<QWidget*>(widget_->parent());
			if (widget_ == 0 || widget_->objectName() == "") return false;
		}
		o = widget_;

		QPoint global = me->globalPos();
		// if we can not get local coordinates: abort
 		QPoint local = widget_->mapFromGlobal(global);
		if (local.x() < 0 || local.y() < 0 ||
				local.x() >= widget_->width() || local.y() >= widget_->height()) 
		{
			return false;
		}

		// for menus: take the position of the action under the cursor
		QMenu* menu = dynamic_cast<QMenu*>(o);
		if (menu)
		{
			QAction* action = menu->actionAt(local);
			if (action != 0)
			{
				o = action;
				parent = menu;
				QRect rect = menu->actionGeometry(action);
				local.rx() -= rect.x();
				local.ry() -= rect.y();

				if (rect.width() == 0 || rect.height() == 0) return false;
				x_ = local.x();
				y_ = local.y();
			}
		}

		if (x_ == 0 && y_ == 0)
		{
			// take the position as percent of the widget's actual size
			if (widget_->width() == 0 || widget_->height() == 0) return false;
			x_ = local.x();
			y_ = local.y();
		}
	}

	String names;
	while (o != 0)
	{
		String name = ascii(o->objectName());
		if (name == "")
		{
			QWidget* widget = dynamic_cast<QWidget*>(o);
			if (widget != 0)
			{
				QList<QAction*> actions = widget->actions();
				if (actions.size() == 1)
				{
					name = ascii((**actions.begin()).objectName());
				}
			}
		}
		else
		{
			// if a parent has more childs with the same name: add a suffix with the number
			if (!parent) parent = o->parent();
			if (parent != 0)
			{
				QList<QWidget*> childs = parent->findChildren<QWidget*>(name.c_str());
				if (childs.size() > 1)
				{
					Position pos = 0;
					QList<QWidget*>::iterator wit = childs.begin();
					for (; wit != childs.end(); wit++)
					{
						if (*wit == o)
						{
							name += "#";
							name += String(pos);
							break;
						}

						pos++;
					}
				}
			}
		}

		if (name != "") names = name + "|" + names;
		o = o->parent();
	}


	String event_string;
	event_string += String((int)e->type()) + "|";
	event_string += String(getMainControl()->isBusy()) + "|";

	if (me != 0)
	{
		if (me->button() == Qt::NoButton &&
				!switch_move->isChecked() && 
				me->type() == QEvent::MouseMove &&	
				widget_ == last_widget_)
		{
			return false;
		}

 		last_widget_ = widget_;

		event_string += String((int)MOUSE) + "|";
		event_string += String((int) me->modifiers()) + "|";
		event_string += String((int) me->button()) + "|";
		event_string += String((int) me->buttons()) + "|";
		event_string += String(x_) + "|";
		event_string += String(y_) + "|";

		// prevent mouse move events with same position
		if (event_string == last_event_string_ &&
				names 			 == last_names_)
		{
			return false;
		}
	}
	else if (ke != 0)
	{
		// prevent accepting key events that are resend further up in the widget tree
		if (timer_.getClockTime() < 0.01) return false;

		int m = (int) ke->modifiers();
		// sometimes Qt sends nonsense Key messages
		if (m > (int)(Qt::AltModifier | Qt::ControlModifier | Qt::ShiftModifier)) return false;

		event_string += String((int)KEY) + "|";
		event_string += String(m);
		event_string += "|";
		event_string += String(ke->key()) + "|";
	}
	else if (se != 0)
	{
		event_string += String((int)SHORTCUT) + "|";
		event_string += String(se->shortcutId()) + "|";
		event_string += ascii(se->key().toString()) + "|";
	}

	float time = timer_.getClockTime();
	timer_.reset();

	outfile_ << "I°"
					 << time << "°"
					 << names << "°"
					 << event_string << std::endl;

	last_event_string_ = event_string;
	last_names_  			 = names;

	return false;
}

void TestFramework::runTest()
{
	line_nr_   = 0;
	errors_ 	 = 0;
	recording_ = false;
	infile_.open(filename_);
	lines_.clear();
	runtime_.stop();
	runtime_.start();

#define MAX_LINE_LENGTH 10000
	char buffer[MAX_LINE_LENGTH];
	while (!infile_.eof())
	{
		infile_.getline(buffer, BALL_MAX_LINE_LENGTH);
		lines_.push_back(buffer);
	}

	processEvent_();
	timer_.reset();
	timer_.start();
	test_running_ = true;
	thread_.start();
	qApp->installEventFilter(this);
}

void TestFramework::setFilename(const String& filename)
{
	filename_ = filename;
}

void TestFramework::timeOut()
{
	if (is_command_line_ && 
			max_run_time_ != FLT_MAX &&
			runtime_.getClockTime() > max_run_time_)
	{
		abortTest();
		getMainControl()->quit(-1);
		return;
	}

	// if MainControl is busy: send event only if MC was also busy when we recorded it
	if (getMainControl()->isBusy() && !event_busy_)
	{
		return;
	}

	if (!getMainControl()->isBusy())
	{
		getMainControl()->processEvents(1000);
	}

	if (timer_.getClockTime() < time_) return;

#ifdef BALL_PYTHON_SUPPORT		
	if (python_line_ != "")
	{
		PyWidget* pyw = 0;

		pyw = PyWidget::getInstance(0);
		if (pyw == 0) 
		{
			Log.error() << "Can not exec Python in macro since no Python support available" << std::endl;
		}

		else
		{
			if (expected_result_ == "")
			{
				pyw->runString(python_line_);
			}
			else
			{
				bool ok;
				String result = PyInterpreter::run(python_line_, ok);
				// trim newline
				if (result.size()) result.truncate(result.size()-1);
				if (!ok || result != expected_result_)
				{
					Log.error() << "------------------------------------------------" << std::endl;
					Log.error() << "Test failed in "
											<< filename_ << " " << line_nr_ << ":";
					if (!ok) Log.error() << "(call failed)";
					Log.error() << std::endl 
					            << python_line_ << std::endl 
					            << result << "!=" 
					            << expected_result_<< "!" << std::endl;
					errors_++;
				}
			}
		}

		processEvent_();
		return;
	}
#endif

	if (type_ == 0) 
	{
		test_running_ = false;
		return;
	}

	if (our_type_ != SHORTCUT && widget_ == 0)
	{
		BALLVIEW_DEBUG;
		return;
	}

	QEvent* e = 0;

	bool hide = false;

	if (our_type_ == MOUSE)
	{
		// circumvent problems with context menus:	
		if ((Qt::MouseButton) button_ == Qt::RightButton)
		{
			/*
			QWidget* w = (QWidget*)widget_->parent();
			while (w != 0)
			{
				DockWidget* dw = dynamic_cast<DockWidget*>(w);
				if (dw != 0)
				{
					QCursor::setPos(QPoint(x_,y_));
					dw->showGuestContextMenu(QPoint(x_,y_));
					processEvent_();
					return;
				}

				w = (QWidget*)w->parent();
			}
			*/
		}

		bool was_visible = widget_->isVisible();
		widget_->show();
		QPoint local = widget_->mapFromGlobal(QPoint(x_,y_));
		QPoint org_p = local;
		if (local.x() >= widget_->width())  local.setX((int)(widget_->width() * 0.9));
		if (local.y() >= widget_->height()) local.setY((int)(widget_->height() * 0.9));
		if (local.x() <= 0) local.setX((int)(widget_->width() * 0.1));
		if (local.y() <= 0) local.setY((int)(widget_->height() * 0.1));
#ifdef BALL_VIEW_DEBUG
		if (org_p != local)
		{
			Log.error() << ascii(widget_->objectName()) << " " << org_p.x() 
									<< " " << org_p.y() << std::endl;
		}
#endif
		e = new MyMouseEvent((QMouseEvent::Type)type_, 
												local,
												(Qt::MouseButton)button_, 
												(Qt::MouseButtons)buttons_, 
												(Qt::KeyboardModifier)modifiers_);
		
		if ((Qt::MouseButton) button_ == Qt::LeftButton &&
				RTTI::isKindOf<QMenu>(widget_) && 
				!was_visible)
		{
			hide = true;
		}
		
    QCursor::setPos(QPoint(x_,y_));
	}
	else if (our_type_ == KEY)
	{
		// workaround for capitalized keys:
		if (modifiers_ == Qt::ShiftModifier)
		{
			if (button_ != Qt::Key_Shift)
			{
				QKeySequence seq(button_);
				QTest::keyClicks(widget_, seq.toString());
			}
		}
		else
		{
			QTest::keyClick(widget_, (Qt::Key)button_, (Qt::KeyboardModifiers)modifiers_);
		}
	}
	else if (our_type_ == SHORTCUT)
	{
		if (action_ != 0)
		{
			action_->trigger();
		}
	}


	if (e != 0)
	{
		reinterpret_cast<QSpontaneKeyEvent*>(e)->setSpontaneous();
    if (!qApp->notify(widget_, e))
		{
 			BALLVIEW_DEBUG
		}
		delete e;
	}

	if (hide)
	{
		widget_->hide();
	}
 	processEvent_();
}

void TestFramework::processEvent_()
{
	widget_ = 0;
	action_ = 0;
	type_ = 0;
	python_line_ = "";

	if (lines_.size() == 0) return;

	String line = *lines_.begin();
#ifdef BALL_VIEW_DEBUG
	Log.info() << line  << std::endl;
#endif
	lines_.pop_front();
	line_nr_++;
	vector<String> fields;
	Size nr = line.split(fields, "°");
	if (nr < 1) return;

	vector<String> names;

	String line_type;

	try
	{
		line_type = fields[0];

		// comment lines:
		if (line_type == "C") 
		{
			processEvent_();
			return;
		}

		if (line_type == "X")
		{
			if (is_command_line_)
			{
				getMainControl()->quit(errors_);
			}
			else
			{
				processEvent_();
			}

			return;
		}

		if (nr < 2) 
		{
			BALLVIEW_DEBUG
			Log.error() << line << std::endl;
			return;
		}

		if (line_type == "R")
		{
			if (is_command_line_)
			{
				String time = fields[1];
				max_run_time_ = time.toFloat();
			}

			processEvent_();
			return;
		}

		if (line_type == "F") 
		{
			String file = fields[1];
			Path path;
			file = path.find(file);

			getMainControl()->clearData();
 			getMainControl()->loadBALLViewProjectFile(file);
			getMainControl()->wait();
			getMainControl()->processEvents(1000);
			processEvent_();
			return;
		}

		if (nr < 3) 
		{
			BALLVIEW_DEBUG
			Log.error() << line << std::endl;
			return;
		}

		time_ = fields[1].toFloat();
		if (line_type == "P" || line_type == "T")
		{
			python_line_ = fields[2];
			expected_result_ = "";

			if (line_type == "T")
			{
				if (nr < 4)
				{
					BALLVIEW_DEBUG
					return;
				}
				expected_result_ = fields[3];
			}
			return;
		}
		
		Size nr2 = fields[2].split(names, "|");
		vector<String> fields2;
		QList<QWidget*>::iterator wit;

		String found;
		widget_ = getMainControl();
		for (Position p = 1; p < nr2 && widget_ != 0; p++)
		{
			String name = names[p];
			found += name + "->";

			// do we have multiple childs with the same name?
			Position nr = 0;
			if (name.split(fields2, "#") > 1) nr = fields2[1].toUnsignedInt();
			name = fields2[0];
			
			// find the matching childs:
			QList<QWidget*> childs = widget_->findChildren<QWidget*>(name.c_str());
			if (childs.size() == 1)
			{
				// one hit: take it!
				widget_ = *childs.begin();
				continue;
			}
			else
			{
				if (childs.size() > (int)nr)
				{
					// we have multiple childs with the same name
					// get the correct one:
					wit = childs.begin();
					for (Position i = 0; i < nr; i++) wit++;
					widget_ = *wit;
					continue;
				}
			}

			// now look for matching actions in child widgets:
			childs = widget_->findChildren<QWidget*>();
			QList<QWidget*>::iterator wit = childs.begin();
			for (; wit != childs.end(); wit++)
			{
				QList<QAction*> actions = (*wit)->actions();
				QList<QAction*>::iterator ait = actions.begin();
				for (; ait != actions.end(); ait++)
				{
					if ((**ait).objectName() == name.c_str())
					{
						widget_ = *wit;
						action_ = *ait;
						break;
					}
				}
				if (action_ != 0) break;
			}

			if (action_ != 0) break;
		
			// last possibility: matching QAction in current widget
			QList<QAction*> actions = widget_->actions();
			QList<QAction*>::iterator ait = actions.begin();
			for (; ait != actions.end(); ait++)
			{
				if ((**ait).objectName() == name.c_str())
				{
 					action_ = *ait;
 					break;
				}
			}
			if (action_ != 0) break;

			BALLVIEW_DEBUG
			Log.error() << line << std::endl;
			Log.error() << found << std::endl;
			processEvent_();
			return;
		}

		// ok, we have the widget!
		// now parse the event string
		String event_string = fields[3];
		vector<String> event_fields;
		event_string.split(event_fields, "|");
		if (event_fields.size() < 3)
		{
			BALLVIEW_DEBUG
			Log.error() << line << std::endl;
			return;
		}

		type_ = event_fields[0].toInt();
		event_busy_ = event_fields[1].toBool();
		our_type_ = event_fields[2].toInt();
#define START 3
  	modifiers_ = event_fields[START].toInt();

		if (our_type_ == MOUSE)
		{
			x_ = event_fields[START + 3].toInt(); 
			y_ = event_fields[START + 4].toInt();
			QMenu* menu = dynamic_cast<QMenu*>(widget_);
			if (menu && action_)
			{
				QRect rect = menu->actionGeometry(action_);
				x_ += rect.x();
				y_ += rect.y();
			}

			QPoint global = widget_->mapToGlobal(QPoint(x_, y_));
			x_ = global.x();
			y_ = global.y();

			button_ = event_fields[START + 1].toInt();
			buttons_ = event_fields[START + 2].toInt();
		}
		else if (our_type_ == KEY)
		{
			button_ = event_fields[START + 1].toInt();
		}
		else if (our_type_ == SHORTCUT)
		{
			shortcut_ = event_fields[START + 1];
		}
		else
		{
			BALLVIEW_DEBUG
			Log.error() << our_type_ << ": " << line << std::endl;
			return;
		}
	}
	catch(Exception::InvalidFormat& e)
	{
		BALLVIEW_DEBUG
		Log.error() << line << std::endl;
		Log.error() << e << std::endl;
	}

	timer_.reset();
}


void TestFramework::checkMenu(MainControl& main_control)
{
	bool busy = main_control.isBusy();
	if (start_recording)
	{
		start_recording->setEnabled(!recording_ && !busy && filename_ != "");
		start_recording->setChecked(recording_);
	}
	
	if (stop_recording)
		stop_recording->setEnabled(recording_);
	
	if (run_test)
	{
		run_test->setEnabled(filename_ != "" && !recording_ && !test_running_ && !busy);
		run_test->setChecked(test_running_);
	}
	
	if (abort_test)
		abort_test->setEnabled(test_running_);

	if (save_file)
		save_file->setEnabled(!recording_ && !test_running_ && !busy);
	
	if (load_file)
		load_file->setEnabled(!recording_ && !test_running_ && !busy);
}


bool TestFramework::event(QEvent* e)
{
	if (e->type() < QEvent::User) return QWidget::event(e);

	if (e->type() == 9999) timeOut();

	return true;
}


bool TestFramework::canHandle(const String& fileformat) const
{
	return fileformat == "btf";
}


bool TestFramework::openFile(const String& filename)
{
	filename_ = filename;
	is_command_line_ = true;
	runTest();
	return true;
}


	} // VIEW
} // namespace BALL
