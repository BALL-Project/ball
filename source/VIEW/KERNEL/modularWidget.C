// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: modularWidget.C,v 1.20.4.10 2005/11/15 23:46:35 amoll Exp $
//

#include <BALL/VIEW/KERNEL/modularWidget.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/FORMAT/INIFile.h>
#include <BALL/VIEW/KERNEL/preferencesEntry.h>
#include <qmenubar.h>

using namespace std;

namespace BALL
{
  namespace VIEW
	{

		ModularWidget::ModularWidget(const char* name)
			throw()
			: Embeddable(),
				ConnectionObject(),
				window_menu_entry_id_(-1),
				show_window_enty_(false),
				default_visible_(true),
				last_id_(-1)
		{
			if (name) setIdentifier(name);
		}

		ModularWidget::ModularWidget(const ModularWidget& widget)
			throw()
			: Embeddable(widget),
				ConnectionObject(widget),
				last_id_(-1)
		{
		}

		ModularWidget::~ModularWidget()
			throw()
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
			throw()
		{
		}

		void ModularWidget::registerWidget(ModularWidget* mwidget)
			throw(Exception::NullPointer)
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "registering ModularWidget at " << mwidget << endl;
			#endif

			if (!mwidget) throw(Exception::NullPointer(__FILE__, __LINE__));

			if (!RTTI::isKindOf<QObject>(*mwidget)) 
			{
				Log.error() << "ModularWidget::ModularWidget: widget " << mwidget << " is not " 
										<< "in a MainControl object!" << endl;
				return;
			}

			MainControl* mc = MainControl::getMainControl(dynamic_cast<QObject*>(mwidget));
			if (!mc)
			{
				Log.error() << "ModularWidget::ModularWidget(): widget " << mwidget << " is not in a MainControl object!" << endl;
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
			removeMenuEntries();
		}

		void ModularWidget::checkMenu(MainControl& /* main_control */)
			throw()
		{
		}

		void ModularWidget::initializePreferencesTab(Preferences & /* preferences */)
			throw()
		{
		}

		void ModularWidget::finalizePreferencesTab(Preferences & /* preferences */)
			throw()
		{
		}

		void ModularWidget::fetchPreferences(INIFile& inifile)
			throw()
		{
			QWidget* widget= dynamic_cast<QWidget*>(this);
			if (!widget) return;

			if (inifile.hasEntry("WINDOWS", getIdentifier() + "::x"))
			{
				widget->move(inifile.getValue("WINDOWS", getIdentifier() + "::x").toUnsignedInt(),
										 inifile.getValue("WINDOWS", getIdentifier() + "::y").toUnsignedInt());
				widget->resize(inifile.getValue("WINDOWS", getIdentifier() + "::width").toUnsignedInt(),
											 inifile.getValue("WINDOWS", getIdentifier() + "::height").toUnsignedInt());
			} 

			PreferencesEntry* entry = dynamic_cast<PreferencesEntry*>(this);
			if (entry == 0) return;
			entry->readPreferenceEntries(inifile);
		}

		void ModularWidget::writePreferences(INIFile& inifile)
			throw()
		{
			QWidget* widget= dynamic_cast<QWidget*>(this);
			if (!widget) return;

			if (window_menu_entry_id_ != -1)
			{
				inifile.insertValue("WINDOWS", getIdentifier() + "::on", 
					String(getMainControl()->menuBar()->isItemChecked(window_menu_entry_id_)));
			}

			inifile.insertValue("WINDOWS", getIdentifier() + "::x", String(widget->x()));
			inifile.insertValue("WINDOWS", getIdentifier() + "::y", String(widget->y()));
			inifile.insertValue("WINDOWS", getIdentifier() + "::width", String(widget->width()));
			inifile.insertValue("WINDOWS", getIdentifier() + "::height", String(widget->height()));

			PreferencesEntry* entry = dynamic_cast<PreferencesEntry*>(this);
			if (entry == 0) return;
			entry->writePreferenceEntries(inifile);
		}

		void ModularWidget::setStatusbarText(String text, bool important)
			throw()
		{
			if (getMainControl() == 0) return;
			getMainControl()->setStatusbarText(text, important);
		}

		MainControl* ModularWidget::getMainControl() const
			throw()
		{ 
			if (getParent() == 0) return 0;
			
			ConnectionObject* root = (const_cast<ModularWidget*>(this))->getRoot();
			
			if (!RTTI::isKindOf<MainControl>(*root)) return 0;

			return (dynamic_cast<MainControl*>(root));
		}

		FragmentDB& ModularWidget::getFragmentDB() const
			throw()
		{
			return * const_cast<FragmentDB*>(&getMainControl()->getFragmentDB());
		}

		void ModularWidget::dump(ostream& s, Size depth) const
			throw()
		{
			ConnectionObject::dump(s, depth);
		}

		QMenuBar* ModularWidget::menuBar()
			throw()
		{
			return getMainControl()->menuBar();
		}

		String ModularWidget::getWorkingDir()
			throw() 
		{ 
			return getMainControl()->getWorkingDir();
		}

		void ModularWidget::setWorkingDir(const String& dir)
			throw() 
		{ 
			getMainControl()->setWorkingDir(dir);
		}

		void ModularWidget::setWorkingDirFromFilename_(String filename)
			throw()
		{
			String separators(FileSystem::PATH_SEPARATOR );
			// workaround on windows: QT returns the filename in linux style
			// but I am not sure, if this will stay this way.
			#ifdef BALL_PLATFORM_WINDOWS
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
			throw()
		{
			if (getMainControl() == 0) return false;
			return getMainControl()->lockCompositesFor(this);
		}

		bool ModularWidget::unlockComposites()
			throw()
		{
			if (getMainControl() == 0) return false;
			return getMainControl()->unlockCompositesFor(this);
		}

		void ModularWidget::removeMenuEntries()
		{
			if (getMainControl() == 0) return;

			for (Position i = 0; i < menu_ids_.size(); i++)
			{
				getMainControl()->removeMenuEntry(menu_ids_[i].first, menu_ids_[i].second);
			}

			menu_ids_.clear();
			last_id_ = -1;
		}
				
		Index ModularWidget::insertMenuEntry(Index parent_id, const String& name, 
												const QObject* receiver, const char* slot, Index accel, Index pos)
			throw()
		{
			if (getMainControl() == 0) return -1;

			last_id_ = getMainControl()->insertMenuEntry(parent_id, name, receiver, slot, accel, pos);
			if (last_id_ == -1) return -1;

			menu_ids_.push_back(pair<Index, Index>(parent_id, last_id_));

			last_parent_id_ = parent_id;

			return last_id_;
		}

		void ModularWidget::setMenuHint(const String& hint)
		{
			if (last_id_ 				== -1 ||
					getMainControl() == 0)
			{
				return;
			}

			getMainControl()->setMenuHint(last_id_, hint);
		}

		void ModularWidget::setMenuHelp(const String& url)
		{
			if (last_id_ 				== -1 ||
					getMainControl() == 0)
			{
				return;
			}

			registerMenuEntryForHelpSystem(last_id_, url);
		}

		void ModularWidget::showHelp(const String& url)
		{
			notify_(new ShowHelpMessage(url));
		}

		void ModularWidget::registerWidgetForHelpSystem(const QWidget* widget, const String& url)
		{
			RegisterHelpSystemMessage* msg = new RegisterHelpSystemMessage();
			msg->setWidget(widget);
			msg->setURL(url);
			notify_(msg);
		}

		void ModularWidget::registerMenuEntryForHelpSystem(Index entry, const String& docu_entry)
		{
			RegisterHelpSystemMessage* msg = new RegisterHelpSystemMessage();
			msg->setMenuEntry(entry);
			msg->setURL(docu_entry);
			notify_(msg);
		}


	} // namespace VIEW
} // namespace BALL
