// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: 

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
    /**  \addtogroup  ViewGuiWidgets
     *  @{
     */
		/** MainControlPreferences class.
				The class MainControlPreferences is a widget that will be inserted into the
				tab dialog  \link Preferences Preferences \endlink . It contains the style preferences 
				of the  \link MainControl MainControl \endlink .
				{\em *this} mainControlPreferences will be created in the  \link MainControl MainControl \endlink 
				object and will be inserted into the  \link Preferences Preferences \endlink  dialog.
				
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
					{\em *this} mainControlPreferences contains the various styles the 
					 \link MainControl MainControl \endlink  can have.
					@param       parent the parent  \link QWidget QWidget \endlink  of {\em *this} mainControlPreferences (See QFileDialog in the QT documentation)
					@param       name the name of {\em *this} mainControlPreferences (See QFileDialog in the QT documentation)
					@return      MainControlPreferences new constructed mainControlPreferences widget
			*/
			MainControlPreferences(QWidget *parent = NULL,	const char *name = NULL )
				throw();
			
			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} mainControlPreferences.
					Calls  \link clear clear \endlink .
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
					Access the selected style of {\em *this} mainControlPreferences.
					This method will be called from the method  \link MainControl::applyPreferences MainControl::applyPreferences \endlink .
					@return  QStyle* a pointer to the selected style of {\em *this} mainControlPreferences (See documentation of QT-library for information concerning styles)
			 */
			QStyle* getStyle()
				throw();
			
			/** Fetch the preferences.
					Fetch the preferences (the style of {\em *this} mainControlPreferences) from
					the	 \link INIFile INIFile \endlink  {\em inifile}.
					This method will be called from the method  \link MainControl::fetchPreferences MainControl::fetchPreferences \endlink .
					@param  inifile the  \link INIFile INIFile \endlink  that contains the needed information for {\em *this} mainControlPreferences
					@see    writePreferences
			*/
			void fetchPreferences(INIFile &inifile)
				throw();

			/** Write the preferences.
					Write the preferences (the style of {\em *this} mainControlPreferences) to the
					 \link INIFile INIFile \endlink  {\em inifile}.
					This method will be called from the method 
					 \link MainControl::writePreferences MainControl::writePreferences \endlink .
					@param  inifile the  \link INIFile INIFile \endlink  to be written into
					@see    fetchPreferences
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
  /** @} */
	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_GUI_WIDGETS_MAINCONTROLPREFERENCES_H
