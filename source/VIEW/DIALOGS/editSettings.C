// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/DIALOGS/editSettings.h>
#include <BALL/VIEW/KERNEL/common.h>

#include <QtGui/QCheckBox>
#include <QtGui/QLineEdit>

namespace BALL
{
	namespace VIEW
	{

EditSettings::EditSettings(QWidget* parent, const char* name, Qt::WFlags fl)
	: QWidget(parent, fl),
		Ui_EditSettingsData(),
		PreferencesEntry()
{
	setupUi(this);
	setObjectName(name);
	setINIFileSectionName("EDITING");
	setWidgetStackName((String)tr("Editing"));
	registerWidgetForHelpSystem_(this, "scene.html#editing");
	registerWidgets_();
}

EditSettings::~EditSettings()
{
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "Destructing object " << (void *)this 
								<< " of class EditSettings" << endl;
	#endif 
}

} } // namespaces
