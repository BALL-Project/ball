// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: serverPreferences.h,v 1.1 2003/08/26 12:26:58 oliver Exp $
//

#ifndef BALL_VIEW_GUI_WIDGETS_SERVERPREFERENCES_H
#define BALL_VIEW_GUI_WIDGETS_SERVERPREFERENCES_H

#include <qwidget.h>
#include <qcheckbox.h>
#include <qlineedit.h>

namespace BALL
{
	class INIFile;

	namespace VIEW
	{
		/** ServerPreferences class.
				The class ServerPreferences is a widget that will be inserted into the
				tab dialog Preferences. It contains the preferences 
				of the Server. The Server will access those preferences
				if the apply-button on the Preferences is pressed.
				this serverPreferences will be created by the Server
				object in the Server::initializePreferencesTab and will be inserted
				into the Preferences dialog by the MainControl.
		*/
		class ServerPreferences : public QWidget
		{
			Q_OBJECT
				
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					The attributs of this serverPreferences are:
					  -  server port set to <tt> VIEW_DEFAULT_PORT</tt>
						-  server status set to accept clients
					\par
					\param       parent the parent QWidget of this serverPreferences (See QFileDialog in the QT documentation)
					\param       name the name of this serverPreferences (See QFileDialog in the QT documentation)
					\see         getPort
					\see         getServerStatus
			*/
			ServerPreferences(QWidget *parent = NULL, const char *name = NULL )
				throw();

			//@}
			/** @name Destructors 
			*/
			//@{

			/** Destructor.
			*/
			virtual ~ServerPreferences()
				throw();
			
			/** Explicit default initialization.
					Empty for further purpose.
			*/
			virtual void clear()
				throw() {};

			//@}
			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{

			/** Fetch the preferences.
					Fetch the preferences (the port and the status of this
					serverPreferences) from	the	INIFile <b> inifile.
					This method will be called from the method Server::fetchPreferences.
					\param  inifile the INIFile that contains the needed information for this serverPreferences
					\see    writePreferences
			*/
			void fetchPreferences(INIFile& inifile)
				throw();

			/** Write the preferences.
					Write the preferences (the port and the status  of this 
					serverPreferences) to the	INIFile <b> inifile.
					This method will be called from the method 
					Server::writePreferences.
					\param  inifile the INIFile to be written into
					\see    fetchPreferences
			*/
			void writePreferences(INIFile& inifile)
				throw();
			
			/** Inspection of the port.
					Access the port of this serverPreferences. This number is the port
					the Server tries to connect to.
					\return int the port number of this serverPreferences
					\see    resetPort
			*/
			int getPort()
				throw();

			/** Check the server status.
					Access the server status. If the server status is set to <tt> true</tt> than
					the Server will try to accepts clients.
					If set to <tt> false the Server will not accept clients.
					\return bool <tt> true the Server will try to accept clients, <tt> false the Server will not accept clients.
			*/
			bool getServerStatus()
				throw();
			//@}
			

			public slots:

			/** @name Public slots
			*/
			//@{

			/** Reset the port.
					Reset the port of this serverPreferences to the value 
					<tt> VIEW_DEFAULT_PORT</tt>.
					This method is connected to the button <tt> Reset</tt> of this
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
