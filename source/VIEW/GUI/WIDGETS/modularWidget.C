// $Id: modularWidget.C,v 1.1 2000/09/23 13:28:39 hekl Exp $

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
{
	cerr << "destructing ModularWidget at " << this << endl;
	QWidget* widget = dynamic_cast<QWidget*>(this);
	if (widget != 0)
	{
		MainControl* mc = MainControl::getMainControl(widget);
		if (mc != 0)
		{
			finalizeWidget(*mc);
			mc->removeModularWidget(this);
		}
	}
}

void ModularWidget::registerWidget(ModularWidget* mwidget)
{
	cerr << "registering ModularWIdget at " << mwidget << endl;
	QWidget* widget = dynamic_cast<QWidget*>(mwidget);
	if (widget != 0)
	{
		MainControl* mc = MainControl::getMainControl(widget);
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
