// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: mainControlPreferences.h,v 1.11 2003/08/26 08:05:04 oliver Exp $
//

#ifndef BALL_VIEW_GUI_WIDGETS_MAINCONTROLPREFERENCES_H
#define BALL_VIEW_GUI_WIDGETS_MAINCONTROLPREFERENCES_H

#ifndef BALL_FORMAT_INIFILE_H
# include <BALL/FORMAT/INIFile.h>
#endif

#include <qwidget.h>
#include <qradiobutton.h>

namespace BALL
{
	namespace VIEW
	{
		/** MainControlPreferences class.
				The class MainControlPreferences is a widget that will be inserted into the
				tab dialog Preferences. It contains the style preferences 
				of the MainControl.
				this mainControlPreferences will be created in the MainControl
				object and will be inserted into the Preferences dialog.
		\ingroup ViewGuiWidgets		
		*/
		class MainControlPreferences : public QWidget
		{
			Q_OBJECT

			public:
			
			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new mainControlPreferences.
					this mainControlPreferences contains the various styles the 
					MainControl can have.
					\param       parent the parent QWidget of this mainControlPreferences (See QFileDialog in the QT documentation)
					\param       name the name of this mainControlPreferences (See QFileDialog in the QT documentation)
					\return      MainControlPreferences new constructed mainControlPreferences widget
			*/
			MainControlPreferences(QWidget *parent = NULL,	const char *name = NULL )
				throw();
			
			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of this mainControlPreferences.
					Calls clear.
			*/
			virtual ~MainControlPreferences()
				throw();

			/** Explicit default initialization.
					Empty for further purpose.
			*/
			virtual void clear()
				throw();

			//@}

			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{
			/** Inspection of the selected style.
					Access the selected style of this mainControlPreferences.
					This method will be called from the method MainControl::applyPreferences.
					\return  QStyle* a pointer to the selected style of this mainControlPreferences (See documentation of QT-library for information concerning styles)
			 */
			QStyle* getStyle()
				throw();
			
			/** Fetch the preferences.
					Fetch the preferences (the style of this mainControlPreferences) from
					the	INIFile <b> inifile.
					This method will be called from the method MainControl::fetchPreferences.
					\param  inifile the INIFile that contains the needed information for this mainControlPreferences
					\see    writePreferences
			*/
			void fetchPreferences(INIFile &inifile)
				throw();

			/** Write the preferences.
					Write the preferences (the style of this mainControlPreferences) to the
					INIFile <b> inifile.
					This method will be called from the method 
					MainControl::writePreferences.
					\param  inifile the INIFile to be written into
					\see    fetchPreferences
			*/
			void writePreferences(INIFile &inifile)
				throw();
			//@}

      private:
			
			QRadioButton *is_platinum_style;
			QRadioButton *is_windows_style;
			QRadioButton *is_motif_style;
			QRadioButton *is_cde_style;
		};
  
	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_GUI_WIDGETS_MAINCONTROLPREFERENCES_H
