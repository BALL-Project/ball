// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: preferences.h,v 1.3 2003/09/04 22:17:47 amoll Exp $
//

#ifndef BALL_VIEW_DIALOGS_PREFERENCES_H
#define BALL_VIEW_DIALOGS_PREFERENCES_H

#include <qtabdialog.h>

namespace BALL
{
	class INIFile;

	namespace VIEW
	{
		/** Preferences class.
				The class Preferences is a tab dialog for other preference dialogs to be
				inserted. An interface for adding and removing other tab dialogs is
				available. this preferences dialog will be created by the class
				MainControl. The MainControl will add other tab dialogs of
				ModularWidget objects to this preferences dialog if those
				other ModularWidget objects have preferences tab dialogs. \par
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
					\param       parent the parent QWidget of this preferences (See QFileDialog in the QT documentation)
					\param       name the name of this preferences (See QFileDialog in the QT documentation)
					\param       width the width of this preferences (Default <tt> width=400</tt>)
					\param       height the height of this preferences (Default <tt> height=300</tt>)
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
					\return bool <tt> true</tt> if tabs are available in this preferences, <tt> false</tt> otherwise
			*/
			bool hasTabs()
				throw();

			/** Insert new tab dialog.
					Insert a new tab dialog into this preferences.
					This method can be called inside the initializePreferencesTab from
					the class ModularWidget.
					\param  child a pointer to the new tab dialog to be inserted into this preferences.
					\param  name the name of the new tab dialog
					\see    removeTab
			*/
			void insertTab(QWidget *child, const QString &name)
				throw();

			/** Remove a tab dialog.
					Remove a tab dialog previously inserted from this preferences.
					This method can called inside the finalizePreferencesTab from
					the class ModularWidget.
					\param  child a pointer to the tab dialog to be removed from this preferences
					\see    insertTab
			*/
			void removeTab(QWidget *child)
				throw();

			/** Fetch the preferences.
					Fetch the preferences (the position of this preferences) from the
					INIFile <b> inifile.
					This method will be called inside the method show from the class
					MainControl.
					\param  inifile the INIFile that contains the needed information for this preferences
					\see    writePreferences
			*/
			void fetchPreferences(INIFile &inifile)
				throw();

			/** Write the preferences.
					Write the preferences (the position of this preferences) to the
					INIFile <b> inifile.
					This method will be called inside the method aboutToExit from the class
					MainControl.
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
					Open this preferences.
			*/
			void openDialog();
						
			//@}

		  private:

			int number_of_tabs_;
		};
  
	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_DIALOGS_PREFERENCES_H
