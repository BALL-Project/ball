// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: serverPreferences.h,v 1.11 2004/10/08 14:49:20 amoll Exp $
//

#ifndef BALL_VIEW_DIALOGS_SERVERPREFERENCES_H
#define BALL_VIEW_DIALOGS_SERVERPREFERENCES_H

#include <qwidget.h>
#include <qcheckbox.h>
#include <qlineedit.h>

#ifndef BALL_VIEW_KERNEL_PREFERENCESENTRY
# include <BALL/VIEW/KERNEL/preferencesEntry.h>
#endif

namespace BALL
{
	class INIFile;

	namespace VIEW
	{
		/** Preferences tab for the Server.
				It will be created by the Server
				object in the Server::initializePreferencesTab and will be inserted
				into the Preferences dialog by the MainControl.
				There are currently two settings: The port on which the data will be received and
				if the server will accept connections. <br>
				This is one of the view dialogs, that is not implemented by an ui-file!
				\ingroup ViewDialogs
		*/
		class BALL_EXPORT ServerPreferences 
			: public QWidget,
				public PreferencesEntry
		{
			Q_OBJECT
				
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					The attributs of this serverPreferences are:
					  -  server port set to <tt> VIEW_DEFAULT_PORT</tt>
						-  server status set to not accept clients
					\par
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
			
			//@}
			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{

			/// 
			void setDefaultValues(bool /*all*/ = false)
				throw();

			/** Access the port the Server tries to connect to.
					\return int the port number of this serverPreferences
			*/
			int getPort()
				throw();

			/** Check the server status.
					If the server status is set to <tt> true</tt> than
					the Server will try to accepts clients.
					If set to false the Server will not accept clients.
					\return bool true the Server will try to accept clients, false the Server will not accept clients.
			*/
			bool getServerStatus()
				throw();
			//@}
			
		  private:

			QLineEdit *port_;
			QCheckBox *server_status_;
		};
  
	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_DIALOGS_SERVERPREFERENCES_H
