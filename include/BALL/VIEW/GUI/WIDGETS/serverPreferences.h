// $Id: 

#ifndef BALL_VIEW_GUI_WIDGETS_SERVERPREFERENCES_H
#define BALL_VIEW_GUI_WIDGETS_SERVERPREFERENCES_H

#ifndef BALL_FORMAT_INIFILE_H
# include <BALL/FORMAT/INIFile.h>
#endif

#include <qwidget.h>
#include <qcheckbox.h>
#include <qlineedit.h>

namespace BALL
{
	namespace VIEW
	{

		/** ServerPreferences class.
				
				{\bf Framework:} BALL/VIEW/GUI/WIDGETS\\
				{\bf Definition:} \URL{BALL/VIEW/GUI/WIDGETS/mainControlPreferences.h}\\ \\
				The class ServerPreferences is a widget that will be inserted into the
				tab dialog \Ref{Preferences}. It contains the preferences 
				of the \Ref{Server}. The \Ref{Server} will access those preferences
				if the apply-button on the \Ref{Preferences} is pressed.
				{\em *this} serverPreferences will be created by the \Ref{Server}
				object in the \Ref{Server::initializePreferencesTab} and will be inserted
				into the \Ref{Preferences} dialog by the \Ref{MainControl}.
				@memo    ServerPreferences class (BALL VIEW gui widgets framework)
				@author  $Author: hekl $
				@version $Revision: 1.2 $
				@date    $Date: 2001/05/13 14:03:45 $
		*/
		class ServerPreferences : public QWidget
		{
			Q_OBJECT
				
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new serverPreferences.
					{\em *this} serverPreferences contains the preferences of the 
					\Ref{Server}.
					The attributs of {\em *this} serverPreferences are:
					\begin{itemize}
					  \item  server port set to {\tt VIEW_DEFAULT_PORT}
						\item  server status set to accept clients
					\end{itemize}
					@param       parent the parent \Ref{QWidget} of {\em *this} serverPreferences (See QFileDialog in the QT documentation)
					@param       name the name of {\em *this} serverPreferences (See QFileDialog in the QT documentation)
					@return      ServerPreferences new constructed serverPreferences widget
					@see         getPort
					@see         getServerStatus
			*/
			ServerPreferences(QWidget *parent = NULL, const char *name = NULL )
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} serverPreferences.
					Calls \Ref{destroy}.
					@see         destroy
			*/
			virtual ~ServerPreferences()
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
			/** Fetch the preferences.
					Fetch the preferences (the port and the status of {\em *this}
					serverPreferences) from	the	\Ref{INIFile} {\em inifile}.
					This method will be called from the method \Ref{Server::fetchPreferences}.
					@param  inifile the \Ref{INIFile} that contains the needed information for {\em *this} serverPreferences
					@see    writePreferences
			*/
			void fetchPreferences(INIFile& inifile)
				throw();

			/** Write the preferences.
					Write the preferences (the port and the status  of {\em *this} 
					serverPreferences) to the	\Ref{INIFile} {\em inifile}.
					This method will be called from the method 
					\Ref{Server::writePreferences}.
					@param  inifile the \Ref{INIFile} to be written into
					@see    fetchPreferences
			*/
			void writePreferences(INIFile& inifile)
				throw();
			
			/** Inspection of the port.
					Access the port of {\em *this} serverPreferences. This number is the port
					the \Ref{Server} tries to connect to.
					@return int the port number of {\em *this} serverPreferences
					@see    resetPort
			*/
			int getPort()
				throw();

			/** Check the server status.
					Access the server status. If the server status is set to {\tt true} than
					the \Ref{Server} will try to accepts clients.
					If set to {\tt false} the \Ref{Server} will not accept clients.
					@return bool {\tt true} the \Ref{Server} will try to accept clients, {\tt false} the \Ref{Server} will not accept clients.
			*/
			bool getServerStatus()
				throw();
			//@}
			

			public slots:

			/** @name Public slots
			*/
			//@{

			/** Reset the port.
					Reset the port of {\em *this} serverPreferences to the value 
					{\tt VIEW_DEFAULT_PORT}.
					This method is connected to the button {\tt Reset} of {\em *this}
					serverPreferences.
			*/
			void resetPort();
			//@}


		  private:

			QLineEdit *port_;
			QCheckBox *server_status_;
		};

	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_GUI_WIDGETS_SERVERPREFERENCES_H
