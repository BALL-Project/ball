// $Id: 

#ifndef BALL_VIEW_GUI_DIALOGS_PREFERENCES_H
#define BALL_VIEW_GUI_DIALOGS_PREFERENCES_H

#ifndef BALL_FORMAT_INIFILE_H
# include <BALL/FORMAT/INIFile.h>
#endif

#include <qtabdialog.h>
#include <qwidget.h>
#include <qstring.h>

//using namespace BALL;

namespace BALL
{
	namespace VIEW
	{

		/** Preferences class.
				The class Preferences is a tab dialog for other preference dialogs to be
				inserted. An interface for adding and removing other tab dialogs is
				available. {\em *this} preferences dialog will be created by the class
				\Ref{MainControl}. The \Ref{MainControl} will add other tab dialogs of
				\Ref{ModularWidget} objects to {\em *this} preferences dialog if those
				other \Ref{ModularWidget} objects have preferences tab dialogs. \\
				{\bf Definition:} \URL{BALL/VIEW/GUI/DIALOGS/preferences.h}
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
					Set the size of {\em *this} preferences to {\tt width} and {\tt height}.
					Tab dialogs that are inserted must have their size accordingly adjusted to the
					size given to {\em *this} preferences.
					There are two buttons in {\em *this} preferences:
					\begin{itemize}
					  \item  apply - apply the preferences
						\item  cancel - hide the preferences dialog
					\end{itemize}
					@param       parent the parent \Ref{QWidget} of {\em *this} preferences (See QFileDialog in the QT documentation)
					@param       name the name of {\em *this} preferences (See QFileDialog in the QT documentation)
					@param       width the width of {\em *this} preferences (Default {\tt width=400})
					@param       height the height of {\em *this} preferences (Default {\tt height=300})
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
					Calls \Ref{destroy}.
					@see         destroy
			*/
			virtual ~Preferences()
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

			/** Check if tabs are available.				
					@return bool {\tt true} if tabs are available in {\em *this} preferences, {\tt false} otherwise
			*/
			bool hasTabs()
				throw();

			/** Insert new tab dialog.
					Insert a new tab dialog into {\em *this} preferences.
					This method can be called inside the \Ref{initializePreferencesTab} from
					the class \Ref{ModularWidget}.
					@param  child a pointer to the new tab dialog to be inserted into {\em *this} preferences.
					@param  name the name of the new tab dialog
					@see    removeTab
			*/
			void insertTab(QWidget *child, const QString &name)
				throw();

			/** Remove a tab dialog.
					Remove a tab dialog previously inserted from {\em *this} preferences.
					This method can called inside the \Ref{finalizePreferencesTab} from
					the class \Ref{ModularWidget}.
					@param  child a pointer to the tab dialog to be removed from {\em *this} preferences
					@see    insertTab
			*/
			void removeTab(QWidget *child)
				throw();

			/** Fetch the preferences.
					Fetch the preferences (the position of {\em *this} preferences) from the
					\Ref{INIFile} {\em inifile}.
					This method will be called inside the method \Ref{show} from the class
					\Ref{MainControl}.
					@param  inifile the \Ref{INIFile} that contains the needed information for {\em *this} preferences
					@see    writePreferences
			*/
			void fetchPreferences(INIFile &inifile)
				throw();

			/** Write the preferences.
					Write the preferences (the position of {\em *this} preferences) to the
					\Ref{INIFile} {\em inifile}.
					This method will be called inside the method \Ref{aboutToExit} from the class
					\Ref{MainControl}.
					@param  inifile the \Ref{INIFile} to be written into
					@see    fetchPreferences
			*/
			void writePreferences(INIFile &inifile)
				throw();
			//@}


			public slots:

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

	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_GUI_DIALOGS_PREFERENCES_H
