// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: 

#ifndef BALL_VIEW_GUI_DIALOGS_PREFERENCES_H
#define BALL_VIEW_GUI_DIALOGS_PREFERENCES_H

#ifndef BALL_FORMAT_INIFILE_H
# include <BALL/FORMAT/INIFile.h>
#endif

#include <qtabdialog.h>
#include <qwidget.h>
#include <qstring.h>

namespace BALL
{
	namespace VIEW
	{
    /**  \addtogroup  ViewGuiDialogs
     *  @{
     */
		/** Preferences class.
				The class Preferences is a tab dialog for other preference dialogs to be
				inserted. An interface for adding and removing other tab dialogs is
				available. {\em *this} preferences dialog will be created by the class
				 \link MainControl MainControl \endlink . The  \link MainControl MainControl \endlink  will add other tab dialogs of
				 \link ModularWidget ModularWidget \endlink  objects to {\em *this} preferences dialog if those
				other  \link ModularWidget ModularWidget \endlink  objects have preferences tab dialogs.  \par
				
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
					Construct new preferences.
					Set the size of {\em *this} preferences to <tt>width</tt> and <tt>height</tt>.
					Tab dialogs that are inserted must have their size accordingly adjusted to the
					size given to {\em *this} preferences.
					There are two buttons in {\em *this} preferences:

					  -  apply - apply the preferences
						-  cancel - hide the preferences dialog
					
					@param       parent the parent  \link QWidget QWidget \endlink  of {\em *this} preferences (See QFileDialog in the QT documentation)
					@param       name the name of {\em *this} preferences (See QFileDialog in the QT documentation)
					@param       width the width of {\em *this} preferences (Default <tt>width=400</tt>)
					@param       height the height of {\em *this} preferences (Default <tt>height=300</tt>)
					@return      Preferences new constructed preferences dialog
			*/
			Preferences(QWidget *parent = NULL,	const char *name = NULL, int width = 400, int height = 300)
				throw();

			//@}
			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} preferences.
					Calls  \link clear clear \endlink .
					@see         destroy
			*/
			virtual ~Preferences()
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

			/** Check if tabs are available.				
					@return bool <tt>true</tt> if tabs are available in {\em *this} preferences, <tt>false</tt> otherwise
			*/
			bool hasTabs()
				throw();

			/** Insert new tab dialog.
					Insert a new tab dialog into {\em *this} preferences.
					This method can be called inside the  \link initializePreferencesTab initializePreferencesTab \endlink  from
					the class  \link ModularWidget ModularWidget \endlink .
					@param  child a pointer to the new tab dialog to be inserted into {\em *this} preferences.
					@param  name the name of the new tab dialog
					@see    removeTab
			*/
			void insertTab(QWidget *child, const QString &name)
				throw();

			/** Remove a tab dialog.
					Remove a tab dialog previously inserted from {\em *this} preferences.
					This method can called inside the  \link finalizePreferencesTab finalizePreferencesTab \endlink  from
					the class  \link ModularWidget ModularWidget \endlink .
					@param  child a pointer to the tab dialog to be removed from {\em *this} preferences
					@see    insertTab
			*/
			void removeTab(QWidget *child)
				throw();

			/** Fetch the preferences.
					Fetch the preferences (the position of {\em *this} preferences) from the
					 \link INIFile INIFile \endlink  {\em inifile}.
					This method will be called inside the method  \link show show \endlink  from the class
					 \link MainControl MainControl \endlink .
					@param  inifile the  \link INIFile INIFile \endlink  that contains the needed information for {\em *this} preferences
					@see    writePreferences
			*/
			void fetchPreferences(INIFile &inifile)
				throw();

			/** Write the preferences.
					Write the preferences (the position of {\em *this} preferences) to the
					 \link INIFile INIFile \endlink  {\em inifile}.
					This method will be called inside the method  \link aboutToExit aboutToExit \endlink  from the class
					 \link MainControl MainControl \endlink .
					@param  inifile the  \link INIFile INIFile \endlink  to be written into
					@see    fetchPreferences
			*/
			void writePreferences(INIFile &inifile)
				throw();

			public slots:

			//@}
			/** @name Public slots
			*/
			//@{

			/** Open the preferences dialog.
					Open {\em *this} preferences.
			*/
			void openDialog();
						
			//@}

		  private:

			int number_of_tabs_;
		};
  /** @} */
	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_GUI_DIALOGS_PREFERENCES_H
