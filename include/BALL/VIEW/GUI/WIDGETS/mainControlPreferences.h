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

		/** MainControlPreferences class.
				The class MainControlPreferences is a widget that will be inserted into the
				tab dialog \Ref{Preferences}. It contains the style preferences 
				of the \Ref{MainControl}.
				{\em *this} mainControlPreferences will be created in the \Ref{MainControl}
				object and will be inserted into the \Ref{Preferences} dialog.
				{\bf Definition:} \URL{BALL/VIEW/GUI/WIDGETS/mainControlPreferences.h}
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
					\Ref{MainControl} can have.
					@param       parent the parent \Ref{QWidget} of {\em *this} mainControlPreferences (See QFileDialog in the QT documentation)
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
					Calls \Ref{destroy}.
					@see         destroy
			*/
			virtual ~MainControlPreferences()
				throw();

			/** Explicit default initialization.
					Empty for further purpose.
			*/
			virtual void clear()
				throw();

			/** Explicit destructor.
					Empty for further purpose.
			*/
			virtual void destroy()
				throw();
			//@}

			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{
			/** Inspection of the selected style.
					Access the selected style of {\em *this} mainControlPreferences.
					This method will be called from the method \Ref{MainControl::applyPreferences}.
					@return  QStyle* a pointer to the selected style of {\em *this} mainControlPreferences (See documentation of QT-library for information concerning styles)
			 */
			QStyle* getStyle()
				throw();
			
			/** Fetch the preferences.
					Fetch the preferences (the style of {\em *this} mainControlPreferences) from
					the	\Ref{INIFile} {\em inifile}.
					This method will be called from the method \Ref{MainControl::fetchPreferences}.
					@param  inifile the \Ref{INIFile} that contains the needed information for {\em *this} mainControlPreferences
					@see    writePreferences
			*/
			void fetchPreferences(INIFile &inifile)
				throw();

			/** Write the preferences.
					Write the preferences (the style of {\em *this} mainControlPreferences) to the
					\Ref{INIFile} {\em inifile}.
					This method will be called from the method 
					\Ref{MainControl::writePreferences}.
					@param  inifile the \Ref{INIFile} to be written into
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

	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_GUI_WIDGETS_MAINCONTROLPREFERENCES_H
