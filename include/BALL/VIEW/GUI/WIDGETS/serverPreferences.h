// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
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
				The class ServerPreferences is a widget that will be inserted into the
				tab dialog  \link Preferences Preferences \endlink . It contains the preferences 
				of the  \link Server Server \endlink . The  \link Server Server \endlink  will access those preferences
				if the apply-button on the  \link Preferences Preferences \endlink  is pressed.
				{\em *this} serverPreferences will be created by the  \link Server Server \endlink 
				object in the  \link Server::initializePreferencesTab Server::initializePreferencesTab \endlink  and will be inserted
				into the  \link Preferences Preferences \endlink  dialog by the  \link MainControl MainControl \endlink .
				<b>Definition:</b> BALL/VIEW/GUI/WIDGETS/mainControlPreferences.h
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
					 \link Server Server \endlink .
					The attributs of {\em *this} serverPreferences are:
					\begin{itemize}
					  \item  server port set to <tt>VIEW_DEFAULT_PORT</tt>
						\item  server status set to accept clients
					\end{itemize}
					@param       parent the parent  \link QWidget QWidget \endlink  of {\em *this} serverPreferences (See QFileDialog in the QT documentation)
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
					Calls  \link destroy destroy \endlink .
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
					serverPreferences) from	the	 \link INIFile INIFile \endlink  {\em inifile}.
					This method will be called from the method  \link Server::fetchPreferences Server::fetchPreferences \endlink .
					@param  inifile the  \link INIFile INIFile \endlink  that contains the needed information for {\em *this} serverPreferences
					@see    writePreferences
			*/
			void fetchPreferences(INIFile& inifile)
				throw();

			/** Write the preferences.
					Write the preferences (the port and the status  of {\em *this} 
					serverPreferences) to the	 \link INIFile INIFile \endlink  {\em inifile}.
					This method will be called from the method 
					 \link Server::writePreferences Server::writePreferences \endlink .
					@param  inifile the  \link INIFile INIFile \endlink  to be written into
					@see    fetchPreferences
			*/
			void writePreferences(INIFile& inifile)
				throw();
			
			/** Inspection of the port.
					Access the port of {\em *this} serverPreferences. This number is the port
					the  \link Server Server \endlink  tries to connect to.
					@return int the port number of {\em *this} serverPreferences
					@see    resetPort
			*/
			int getPort()
				throw();

			/** Check the server status.
					Access the server status. If the server status is set to <tt>true</tt> than
					the  \link Server Server \endlink  will try to accepts clients.
					If set to <tt>false</tt> the  \link Server Server \endlink  will not accept clients.
					@return bool <tt>true</tt> the  \link Server Server \endlink  will try to accept clients, <tt>false</tt> the  \link Server Server \endlink  will not accept clients.
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
					<tt>VIEW_DEFAULT_PORT</tt>.
					This method is connected to the button <tt>Reset</tt> of {\em *this}
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
