// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: modularWidget.C,v 1.14 2004/08/30 15:36:04 amoll Exp $
//

#include <BALL/VIEW/KERNEL/modularWidget.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/FORMAT/INIFile.h>
#include <BALL/VIEW/DIALOGS/preferences.h>
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
		default_visible_(true)
{
	if (name) setIdentifier(name);
}

ModularWidget::ModularWidget(const ModularWidget& widget)
	throw()
	: Embeddable(widget),
		ConnectionObject(widget)
{
}

ModularWidget::~ModularWidget()
	throw()
{
	#ifdef BALL_VIEW_DEBUG
		Log.info() << "Destructing object " << (void *)this 
							 << " of class " << RTTI::getName<ModularWidget>() << endl;
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
	throw()
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
//	throw()
{
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
}

void ModularWidget::setStatusbarText(String text)
	throw()
{
	getMainControl()->setStatusbarText(text);
}

MainControl* ModularWidget::getMainControl() const
	throw()
{ 
	return (MainControl*) ((ConnectionObject*)this)->getRoot();
}

FragmentDB& ModularWidget::getFragmentDB() const
	throw()
{
	return *((FragmentDB*)&getMainControl()->getFragmentDB());
}

void ModularWidget::dump(ostream& s, Size depth) const
	throw()
{
	ConnectionObject::dump(s, depth);
}

int ModularWidget::insertMenuEntry(int ID, const String& name, const QObject* receiver, const char* slot, 
																	 int accel, int entry_ID, String hint)
	throw()
{
	return getMainControl()->insertMenuEntry(ID, name, receiver, slot, accel, entry_ID, hint);
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
	String seperators(FileSystem::PATH_SEPARATOR);
	// workaround on windows: QT returns the filename in linux style
	// but I am not sure, if this will stay this way.
#ifdef BALL_PLATFORM_WINDOWS
	 seperators += "/";
#endif
	vector<String> fields;
	Position p = filename.split(fields, seperators.c_str()) -1;
	if (p == 0) return;
	String suffix = fields[p];				
	setWorkingDir(filename.getSubstring(0, filename.size() - (suffix.size() + 1)));
}


} } // namespaces
