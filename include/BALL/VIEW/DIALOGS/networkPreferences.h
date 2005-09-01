// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: networkPreferences.h,v 1.1.2.2 2005/09/01 22:17:47 amoll Exp $
//

#ifndef BALL_VIEW_DIALOGS_NETWORKPREFERENCES_H
#define BALL_VIEW_DIALOGS_NETWORKPREFERENCES_H

#include <BALL/VIEW/UIC/networkPreferencesData.h>

#ifndef BALL_VIEW_KERNEL_PREFERENCESENTRY
# include <BALL/VIEW/KERNEL/preferencesEntry.h>
#endif

namespace BALL
{
	namespace VIEW
	{

/** NeworkPreferences is a widget that will be inserted into the
		tab dialog Preferences. It contains the following settings:
		- the used QT style 
		- if DockWidget 's contain a label with their name
		- if logging to a file is enabled
		\ingroup ViewDialogs
*/
class BALL_VIEW_EXPORT NetworkPreferences 
	: public NetworkPreferencesData,
		public PreferencesEntry
{
	Q_OBJECT

	public:
	
	/**	@name	Constructors
	*/	
	//@{

	/** Default Constructor.
	*/
	NetworkPreferences(QWidget *parent = NULL, const char *name = NULL, WFlags fl=0)
		throw();
	
	//@}
	/** @name Destructors 
	*/
	//@{

	/** Destructor.
	*/
	virtual ~NetworkPreferences()
		throw();

	//@}
	/**	@name	Accessors: inspectors and mutators 
	*/
	//@{

	///
	void applySettings();

	///
	void getSettings();
	//@}
};
  
} } // namespace

#endif // BALL_VIEW_DIALOGS_NETWORKPREFERENCES_H
