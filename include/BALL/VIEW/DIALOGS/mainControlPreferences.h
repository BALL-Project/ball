// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: mainControlPreferences.h,v 1.7 2003/10/15 14:19:02 amoll Exp $
//

#ifndef BALL_VIEW_DIALOGS_MAINCONTROLPREFERENCES_H
#define BALL_VIEW_DIALOGS_MAINCONTROLPREFERENCES_H

#ifndef BALL_FORMAT_INIFILE_H
# include <BALL/FORMAT/INIFile.h>
#endif

#include <BALL/VIEW/UIC/mainControlPreferencesData.h>

namespace BALL
{
	namespace VIEW
	{

/** MainControlPreferences is a widget that will be inserted into the
		tab dialog Preferences. It contains the style preferences 
		of the MainControl.
		\ingroup ViewDialogs
*/
class MainControlPreferences 
	: public MainControlPreferencesData
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

	/// Inspection, wheter the name labels in the controls should be enabled.
	bool showLabelsEnabled() const
		throw();
	
	/** Fetch the preferences (the style) from the INIFile <tt>inifile</tt>.
			This method will be called from MainControl::fetchPreferences.
			\param  inifile the INIFile that contains the needed information
			\see    writePreferences
	*/
	void fetchPreferences(INIFile &inifile)
		throw();

		/** Write the preferences (the style) to the INIFile <tt>inifile</tt>.
				This method will be called from MainControl::writePreferences.
				\param  inifile the INIFile to be written into
				\see    fetchPreferences
	*/
	void writePreferences(INIFile &inifile)
		throw();
	//@}
};
  
} } // namespace

#endif // BALL_VIEW_DIALOGS_MAINCONTROLPREFERENCES_H
