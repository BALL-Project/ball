// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: mainControlPreferences.h,v 1.5 2003/09/04 22:17:46 amoll Exp $
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
		/** Preferences tab for the MainControl.
				The class MainControlPreferences is a widget that will be inserted into the
				tab dialog Preferences. It contains the style preferences 
				of the MainControl.
				this mainControlPreferences will be created in the MainControl
				object and will be inserted into the Preferences dialog.
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
					This mainControlPreferences contains the various styles the 
					MainControl can have.
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
					Access the selected style of this mainControlPreferences.
					This method will be called from the method MainControl::applyPreferences.
					\return  QStyle* a pointer to the selected style of this mainControlPreferences 
					(See documentation of QT-library for information concerning styles)
			 */
			QStyle* getStyle()
				throw();
			
			/** Fetch the preferences.
					Fetch the preferences (the style of this mainControlPreferences) from
					the	INIFile <b> inifile.
					This method will be called from the method MainControl::fetchPreferences.
					\param  inifile the INIFile that contains the needed information for 
									this MainControlPreferences
					\see    writePreferences
			*/
			void fetchPreferences(INIFile &inifile)
				throw();

			/** Write the preferences (the style of this mainControlPreferences) to the
					INIFile <b> inifile.
					This method will be called from the method 
					MainControl::writePreferences.
					\param  inifile the INIFile to be written into
					\see    fetchPreferences
			*/
			void writePreferences(INIFile &inifile)
				throw();
			//@}
		};
  
	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_DIALOGS_MAINCONTROLPREFERENCES_H
