// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/KERNEL/modularWidget.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/iconLoader.h>
#include <BALL/FORMAT/INIFile.h>
#include <BALL/VIEW/KERNEL/preferencesEntry.h>
#include <QtWidgets/QMenuBar>

using namespace std;

namespace BALL
{
  namespace VIEW
	{

		ModularWidget::ModularWidget(const char* name)
			: Embeddable(),
				ConnectionObject(),
				window_menu_entry_(0),
				show_window_enty_(false),
				default_visible_(true)
		{
			if (name) setIdentifier(name);
		}

		ModularWidget::ModularWidget(const ModularWidget& widget)
			: Embeddable(widget),
				ConnectionObject(widget)
		{
		}

		ModularWidget::~ModularWidget()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << (void *)this 
									 << " of class ModularWidget" << endl;
			#endif 

			if (getMainControl() != 0)
			{
				getMainControl()->removeModularWidget(this);
			}
		}

		void ModularWidget::clear()
		{
		}

		void ModularWidget::registerWidget(ModularWidget* mwidget)
			throw(Exception::NullPointer)
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "registering ModularWidget at " << mwidget << endl;
			#endif

			if (!mwidget) throw(Exception::NullPointer(__FILE__, __LINE__));

            if (!RTTI::isKindOf<QObject>(mwidget))
			{
				Log.error() << "ModularWidget::ModularWidget: widget " 
										<< mwidget << " is not " 
										<< "a QObject!" << endl;
				return;
			}

			QObject* object = dynamic_cast<QObject*>(mwidget);
			MainControl* mc = MainControl::getMainControl(object);
			if (!mc)
			{
				Log.error() << "ModularWidget::ModularWidget(): widget " 
										<< ascii(object->objectName()) << " " 
										<< mwidget << " is not in a MainControl object!" << endl;
				return;
			}

			mc->addModularWidget(mwidget);

			#ifdef BALL_VIEW_DEBUG
				Log.info() << "ModularWidget::registered: " << mwidget << endl;
			#endif
		}

		void ModularWidget::initializeWidget(MainControl& /*main_control*/)
		{
		}

		void ModularWidget::finalizeWidget(MainControl& /* main_control */)
		{
		}

		void ModularWidget::checkMenu(MainControl& /* main_control */)
		{
		}

		void ModularWidget::initializePreferencesTab(Preferences & /* preferences */)
		{
		}

		void ModularWidget::finalizePreferencesTab(Preferences & /* preferences */)
		{
		}

		void ModularWidget::fetchPreferences(INIFile& inifile)
		{
			QWidget* widget= dynamic_cast<QWidget*>(this);
			if (!widget) return;

			if (inifile.hasEntry("WINDOWS", getIdentifier() + "::x"))
			{
				Index x = inifile.getValue("WINDOWS", getIdentifier() + "::x").toInt();
				Index y = inifile.getValue("WINDOWS", getIdentifier() + "::y").toInt();
				Position w = inifile.getValue("WINDOWS", getIdentifier() + "::width").toUnsignedInt();
				Position h = inifile.getValue("WINDOWS", getIdentifier() + "::height").toUnsignedInt();
				x = BALL_MAX(x, 0);
				y = BALL_MAX(y, 0);

				widget->resize(QSize(w, h));
				widget->move(x, y);
			} 

			PreferencesEntry* entry = dynamic_cast<PreferencesEntry*>(this);
			if (entry == 0) return;
			entry->readPreferenceEntries(inifile);
		}

		void ModularWidget::writePreferences(INIFile& inifile)
		{
			QWidget* widget= dynamic_cast<QWidget*>(this);
			if (!widget) return;

			if (window_menu_entry_ != 0)
			{
 				inifile.insertValue("WINDOWS", getIdentifier() + "::on", String(window_menu_entry_->isChecked()));
			}

			inifile.insertValue("WINDOWS", getIdentifier() + "::x", String(widget->pos().x()));
			inifile.insertValue("WINDOWS", getIdentifier() + "::y", String(widget->pos().y()));
			inifile.insertValue("WINDOWS", getIdentifier() + "::width", String(widget->size().width()));
			inifile.insertValue("WINDOWS", getIdentifier() + "::height", String(widget->size().height()));

			PreferencesEntry* entry = dynamic_cast<PreferencesEntry*>(this);
			if (entry == 0) return;
			entry->writePreferenceEntries(inifile);
		}

		void ModularWidget::setStatusbarText(const String& text, bool important)
		{
			if (getMainControl() == 0) return;
			getMainControl()->setStatusbarText(text, important);
		}

		void ModularWidget::setStatusbarText(const QString& text, bool important)
		{
			if (getMainControl() == 0) return;
			getMainControl()->setStatusbarText(text, important);
		}

		MainControl* ModularWidget::getMainControl() const
		{
			if (getParent() == 0) return VIEW::getMainControl();

			ConnectionObject* root = (const_cast<ModularWidget*>(this))->getRoot();

            if (!RTTI::isKindOf<MainControl>(root)) return VIEW::getMainControl();

			return (dynamic_cast<MainControl*>(root));
		}

		FragmentDB& ModularWidget::getFragmentDB() const
		{
			return * const_cast<FragmentDB*>(&getMainControl()->getFragmentDB());
		}

		void ModularWidget::dump(ostream& s, Size depth) const
		{
			ConnectionObject::dump(s, depth);
		}

		String ModularWidget::getWorkingDir()
		{ 
			return getMainControl()->getWorkingDir();
		}

		void ModularWidget::setWorkingDir(const String& dir)
		{ 
			getMainControl()->setWorkingDir(dir);
		}

		void ModularWidget::setWorkingDirFromFilename_(String filename)
		{
			String separators(FileSystem::PATH_SEPARATOR );
			// workaround on windows: QT returns the filename in linux style
			// but I am not sure, if this will stay this way.
			#ifdef BALL_OS_WINDOWS
				separators += "/";
			#endif
			vector<String> fields;
			Position p = filename.split(fields, separators.c_str()) -1;
			if (p == 0) return;
			String suffix = fields[p];				
			String result = filename.getSubstring(0, filename.size() - (suffix.size() + 1));
			setWorkingDir(result);
		}

		bool ModularWidget::lockComposites()
		{
			if (getMainControl() == 0) return false;
			return getMainControl()->lockCompositesFor(this);
		}

		bool ModularWidget::unlockComposites()
		{
			if (getMainControl() == 0) return false;
			return getMainControl()->unlockCompositesFor(this);
		}

		QAction* ModularWidget::insertMenuEntry(Position menu_id, const QString& name,       const QObject* receiver, 
		                                        const char* slot, const String& description, QKeySequence shortcut,
																						const QString& menu_hint, UIOperationMode::OperationMode minimal_mode)
		{
			if (getMainControl() == 0) return 0;

			QAction* result = getMainControl()->insertMenuEntry(menu_id, (String)name, receiver, slot, 
			                                                    description, shortcut, minimal_mode);

			if (result && (menu_hint != ""))
			{
				result->setToolTip(menu_hint);
				getMainControl()->setMenuHint(result, (String)menu_hint);
			}

			return result;
		}

		void ModularWidget::setMenuHelp(QAction* action, const String& url)
		{
			if (!action || !getMainControl())
			{
				return;
			}

			registerForHelpSystem(action, url);
		}

		void ModularWidget::showHelp(const String& url)
		{
			notify_(new ShowHelpMessage(url));
		}

		void ModularWidget::registerForHelpSystem(const QObject* object, const String& url)
		{
			RegisterHelpSystemMessage* msg = new RegisterHelpSystemMessage();
			msg->setObject(object);
			msg->setURL(url);
			notify_(msg);
		}

		void ModularWidget::setIcon(QAction* action, const String& filename, bool add_to_main_toolbar)
		{
			if (!action) return;

			action->setIcon(IconLoader::instance().getIcon(filename.c_str()));

			if (add_to_main_toolbar)
			{
				main_toolbar_actions_.push_back(action);
			}
		}

		void ModularWidget::addToolBarEntries(QToolBar* tb)
		{
			tb->addActions(main_toolbar_actions_);
		}

	} // namespace VIEW
} // namespace BALL
