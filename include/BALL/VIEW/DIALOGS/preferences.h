// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: preferences.h,v 1.4 2003/10/15 14:19:39 amoll Exp $
//

#ifndef BALL_VIEW_DIALOGS_PREFERENCES_H
#define BALL_VIEW_DIALOGS_PREFERENCES_H

#include <qtabdialog.h>

namespace BALL
{
	class INIFile;

	namespace VIEW
	{

/** Preferences is a tab dialog for other preference dialogs to be
		inserted. An interface for adding and removing the tab dialogs is
		available. The Preferences dialog will be created by the MainControl. 
		All ModularWidget classes can add their own prefrences tabs.
		\ingroup ViewDialogs
*/
class Preferences 
	: public QTabDialog
{
	Q_OBJECT
	
	public:

	/**	@name	Constructors
	*/	
	//@{

	/** Default Constructor.
			Set the size of this preferences to <tt> width</tt> and <tt> height</tt>.
			Tab dialogs that are inserted must have their size accordingly adjusted to the
			size given to this preferences.
			There are two buttons in this preferences:
				-  apply - apply the preferences
				-  cancel - hide the preferences dialog
			\par
			\param       parent the parent QWidget (See QTabDialog in the QT documentation)
			\param       name (See QTabeDialog in the QT documentation)
			\param       width (Default <tt>width=400</tt>)
			\param       height (Default <tt>height=300</tt>)
	*/
	Preferences(QWidget *parent = NULL,	const char *name = NULL, int width = 400, int height = 300)
		throw();

	//@}
	/** @name Destructors 
	*/
	//@{

	/** Destructor.
	*/
	virtual ~Preferences()
		throw();

	//@}
	/**	@name	Accessors: inspectors and mutators 
	*/
	//@{

	/** Check if tabs are available.				
			\return bool <tt>true</tt> if tabs are available
	*/
	bool hasTabs()
		throw();

	/** Insert a new tab dialog 
			This method can be called inside ModularWidget::initializePreferencesTab
			\param  child a pointer to the new tab dialog
			\param  name the name of the new tab dialog
			\see    removeTab
	*/
	void insertTab(QWidget *child, const QString &name)
		throw();

	/** Remove a tab dialog previously inserted from this preferences.
			This method can called inside ModularWidget::finalizePreferencesTab 
			\param  child a pointer to the tab dialog to be removed 
			\see    insertTab
	*/
	void removeTab(QWidget *child)
		throw();

	/** Fetch the preferences (the position) from the INIFile <tt>inifile</tt>.
			This method will be called inside MainControl::show().
			\param  inifile the INIFile that contains the needed information
			\see    writePreferences
	*/
	void fetchPreferences(INIFile &inifile)
		throw();

	/** Write the preferences (the position) to the INIFile <tt> inifile</tt>.
			This method will be called in MainControl::aboutToExit.
			\param  inifile the INIFile to be written into
			\see    fetchPreferences
	*/
	void writePreferences(INIFile &inifile)
		throw();

	public slots:

	//@}
	/** @name Public slots
	*/
	//@{

	/** Open the preferences dialog.
	*/
	void show();
				
	//@}

	private:

	int number_of_tabs_;
};
  
} } // namespace

#endif // BALL_VIEW_DIALOGS_PREFERENCES_H
