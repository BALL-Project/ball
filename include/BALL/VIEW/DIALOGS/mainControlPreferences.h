// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: mainControlPreferences.h,v 1.13 2004/09/28 21:40:09 amoll Exp $
//

#ifndef BALL_VIEW_DIALOGS_MAINCONTROLPREFERENCES_H
#define BALL_VIEW_DIALOGS_MAINCONTROLPREFERENCES_H

#ifndef BALL_FORMAT_INIFILE_H
# include <BALL/FORMAT/INIFile.h>
#endif

#include <BALL/VIEW/UIC/mainControlPreferencesData.h>

#ifndef BALL_VIEW_KERNEL_PREFERENCESENTRY
# include <BALL/VIEW/KERNEL/preferencesEntry.h>
#endif

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
class BALL_EXPORT MainControlPreferences 
	: public MainControlPreferencesData,
		public PreferencesEntry
{
	Q_OBJECT

	public:
	
	/**	@name	Constructors
	*/	
	//@{

	/** Default Constructor.
	*/
	MainControlPreferences(QWidget *parent = NULL, const char *name = NULL, WFlags fl=0)
		throw();
	
	//@}
	/** @name Destructors 
	*/
	//@{

	/** Destructor.
	*/
	virtual ~MainControlPreferences()
		throw();

	//@}
	/**	@name	Accessors: inspectors and mutators 
	*/
	//@{

	/** Inspection of the selected style.
			This method will be called from MainControl::applyPreferences().
			\return  QStyle* a pointer to the selected style.
			(See documentation of QT-library for information concerning styles)
	 */
	QStyle* getStyle()
		throw();

	/// Inspection, wheter the name labels in the DockWidget instances should be enabled.
	bool showLabelsEnabled() const
		throw();


	/// Enable logging to file
	void enableLoggingToFile(bool state)
		throw();

	///
	bool loggingToFileEnabled() const
		throw();
	//@}
};
  
} } // namespace

#endif // BALL_VIEW_DIALOGS_MAINCONTROLPREFERENCES_H
