// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_DIALOGS_EDITSETTINGS_H
#define BALL_VIEW_DIALOGS_EDITSETTINGS_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifndef BALL_VIEW_KERNEL_PREFERENCESENTRY
# include <BALL/VIEW/KERNEL/preferencesEntry.h>
#endif

#include <BALL/VIEW/UIC/ui_editSettings.h>

#include <QtWidgets/QWidget>

namespace BALL
{
	namespace VIEW
	{

/** EditSettings is a widget that will be inserted into the
		tab dialog Preferences. It contains the following settings:
		- maximum distance for an atom to be picked
		- maximum distance for an bond to be picked
		- only atom in highlighted container to be picked?
		\ingroup ViewDialogs
*/
class BALL_VIEW_EXPORT EditSettings 
	: public QWidget,
		public Ui_EditSettingsData,
		public PreferencesEntry
{
	Q_OBJECT

	public:
	
	///
	EditSettings(QWidget *parent = NULL, const char *name = "EditSettings", Qt::WindowFlags fl=0);
	
	///
	virtual ~EditSettings();
};
  
} } // namespace

#endif // BALL_VIEW_DIALOGS_EDITSETTINGS_H
