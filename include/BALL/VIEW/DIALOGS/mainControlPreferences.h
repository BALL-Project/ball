// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_DIALOGS_MAINCONTROLPREFERENCES_H
#define BALL_VIEW_DIALOGS_MAINCONTROLPREFERENCES_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifndef BALL_FORMAT_INIFILE_H
# include <BALL/FORMAT/INIFile.h>
#endif

#ifndef BALL_VIEW_KERNEL_PREFERENCESENTRY
# include <BALL/VIEW/KERNEL/preferencesEntry.h>
#endif

#include <BALL/VIEW/UIC/ui_mainControlPreferences.h>

#include <QtWidgets/QWidget>
#include <QtGui/QFont>

namespace BALL
{
	namespace VIEW
	{

/** MainControlPreferences is a widget that will be inserted into the
		tab dialog Preferences. It contains the following settings:
		- the used QT style 
		- if DockWidget 's contain a label with their name
		- if logging to a file is enabled
		\ingroup ViewDialogs
*/
class BALL_VIEW_EXPORT MainControlPreferences 
	: public QWidget,
		public Ui_MainControlPreferencesData,
		public PreferencesEntry
{
	Q_OBJECT

	public:
	
	/// Default Constructor.
	MainControlPreferences(QWidget *parent = NULL, const char *name = "MainControlPreferences", 
												 Qt::WindowFlags fl=0);

	////Destructor.
	virtual ~MainControlPreferences();

	/** Inspection of the selected style.
			This method will be called from MainControl::applyPreferences().
			\return  QStyle* a pointer to the selected style.
			(See documentation of QT-library for information concerning styles)
	 */
	QStyle* setStyle();

	/// Enable logging to file
	void enableLoggingToFile(bool state);

	///
	bool loggingToFileEnabled() const;

	///
	QFont getFont();

	bool getLanguageChanged() {
		return last_index_ != languageComboBox_->currentIndex();
	}

	void resetLanguageChanged() {
		last_index_ = languageComboBox_->currentIndex();
	}

	///
	void writePreferenceEntries(INIFile& inifile);

	///
	void readPreferenceEntries(const INIFile& inifile);

	public slots:

	/// Get the application's default font
	void selectFont();

	/// Set the application's default font
	void setFont(QFont font);

	protected:

	QFont font_;
	int last_index_;
};
  
} } // namespace

#endif // BALL_VIEW_DIALOGS_MAINCONTROLPREFERENCES_H
