// $Id: modularWidget.C,v 1.4 2001/02/11 13:06:32 hekl Exp $

#include <BALL/VIEW/GUI/WIDGETS/modularWidget.h>

#include <qwidget.h>

using namespace std;

ModularWidget::ModularWidget(const char* name)
	: Embeddable(),
		ConnectionObject()
{
	if (name != 0)
	{
		setIdentifier(name);
	}
}

ModularWidget::ModularWidget(const ModularWidget& widget)
	: Embeddable(widget),
		ConnectionObject(widget)
{
}

ModularWidget::~ModularWidget()
	throw()
{
	cerr << "destructing ModularWidget at " << this << endl;

	// do not function because the mainControl is already destroyed
	// at this time => moved to maincontrol
	/* 
	QObject* object = dynamic_cast<QObject*>(this);
	if (object != 0)
	{
		MainControl* mc = MainControl::getMainControl(object);
		if (mc != 0)
		{
			writePreferences(mc->getINIFile());
			finalizePreferencesTab(*(mc->getPreferences()));
			finalizeWidget(*mc);
			mc->removeModularWidget(this);
		}
	}

	*/
}

void ModularWidget::registerWidget(ModularWidget* mwidget)
{
	cerr << "registering ModularWidget at " << mwidget << endl;
	QObject* object = dynamic_cast<QObject*>(mwidget);
	if (object != 0)
	{
		MainControl* mc = MainControl::getMainControl(object);
		if (mc != 0)
		{
			mc->addModularWidget(mwidget);
			cerr << "ModularWidget::registered: " <<mwidget << endl;
		}
		else 
		{
			cerr << "ModularWidget::ModularWidget: widget is not in a MainControl object!" << endl;
		}
	}
}

void ModularWidget::initializeWidget(MainControl& /* main_control */)
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

void ModularWidget::applyPreferences(Preferences & /* preferences */)
{
}

void ModularWidget::fetchPreferences(INIFile & /* inifile */)
{
}

void ModularWidget::writePreferences(INIFile & /* inifile */)
{
}
