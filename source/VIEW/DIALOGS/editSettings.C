// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: editSettings.C,v 1.1.6.1 2007/03/25 22:01:57 oliver Exp $
//

#include <BALL/VIEW/DIALOGS/editSettings.h>
#include <BALL/VIEW/KERNEL/common.h>

#include <QtGui/qcheckbox.h>
#include <QtGui/qlineedit.h>

namespace BALL
{
	namespace VIEW
	{

EditSettings::EditSettings(QWidget* parent, const char* name, Qt::WFlags fl)
	throw()
	: QWidget(parent, fl),
		Ui_EditSettingsData(),
		PreferencesEntry()
{
	setupUi(this);
	setObjectName(name);
	setINIFileSectionName("EDITING");
	setWidgetStackName("Editing");
	registerWidgetForHelpSystem_(this, "scene.html#editing");
	registerWidgets_();
}

EditSettings::~EditSettings()
	throw()
{
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "Destructing object " << (void *)this 
								<< " of class EditSettings" << endl;
	#endif 
}

} } // namespaces
