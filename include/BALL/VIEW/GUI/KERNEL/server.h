// $Id: server.h,v 1.8 2001/07/14 12:55:51 amoll Exp $

#ifndef BALL_VIEW_GUI_KERNEL_SERVER_H
#define BALL_VIEW_GUI_KERNEL_SERVER_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_COMPOSITE_H
#	include <BALL/CONCEPT/composite.h>
#endif

#ifndef BALL_DATATYPE_HASHMAP_H
#	include <BALL/DATATYPE/hashMap.h>
#endif

#ifndef BALL_FORMAT_INIFILE_H
# include <BALL/FORMAT/INIFile.h>
#endif

#ifndef BALL_SYSTEM_SOCKET_H
#	include <BALL/SYSTEM/socket.h>
#endif

#ifndef BALL_VIEW_COMMON_GLOBAL_H
#	include <BALL/VIEW/COMMON/global.h>
#endif

#ifndef BALL_VIEW_KERNEL_QTTIMER_H
#	include <BALL/VIEW/KERNEL/QTTimer.h>
#endif

#ifndef BALL_VIEW_GUI_KERNEL_OBJECTCREATOR_H
#	include <BALL/VIEW/GUI/KERNEL/objectCreator.h>
#endif

#ifndef BALL_VIEW_GUI_DIALOGS_PREFERENCES_H
# include <BALL/VIEW/GUI/DIALOGS/preferences.h>
#endif

#ifndef BALL_VIEW_GUI_WIDGETS_MODULARWIDGET_H
#	include <BALL/VIEW/GUI/WIDGETS/modularWidget.h>
#endif

#ifndef BALL_VIEW_GUI_WIDGETS_SERVERPREFERENCES_H
# include <BALL/VIEW/GUI/WIDGETS/serverPreferences.h>
#endif

#include <qlabel.h>

namespace BALL
{
	
	namespace VIEW
	{

		/** Server class.
				{\bf Framework:} BALL/VIEW/GUI/KERNEL\\
				{\bf Definition:} \URL{BALL/VIEW/GUI/KERNEL/server.h} \\
				{\bf Category:} \Ref{Composite} container\\	\\
				The class Server handles all incoming \Ref{PersistentObject} objects,
				converts them into \Ref{Composite} objects (if possible) and sents
				them through the \Ref{ConnectionObject} tree with the message
				\Ref{NewCompositeMessage}. Also it stores all received \Ref{Composite} objects
				and replaces them if the same \Ref{Composite} object is received again.
				If a \Ref{Composite} object is replaced the message 
				\Ref{RemovedCompositeMessage} will be sent through the \Ref{ConnectionObject}
				tree and after that the the message \Ref{NewCompositeMessage} with the new
				received composite will be sent.
				@memo    Server class (BALL VIEW gui kernel framework)
				@author  $Author: amoll $
				@version $Revision: 1.8 $
				@date    $Date: 2001/07/14 12:55:51 $
		*/
		class Server
			: public QTTimer,
   			public ModularWidget
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new server.
					The state of {\em *this} server is:
					\begin{itemize}
					  \item no object creator registered
						\item server listening on {\tt VIEW_DEFAULT_PORT} if activated
					\end{itemize}
					@return      Server new constructed server
					@see         QTTimer
					@see         ModularWidget
			*/
			Server(QWidget* parent = 0, const char* name = 0)
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} server.
					Calls \Ref{destroy}.
					@see         destroy
			*/
			virtual ~Server()
				throw();

			/** Explicit default initialization.
					Set the state of {\em *this} server to the default values.
					Calls \Ref{QTTimer::clear}.
					Calls \Ref{ConnectionObject::clear}.
					@see QTTimer::clear
					@see ConnectionObject::clear
			*/
			virtual void clear()
				throw();

			/** Explicit destructor.
					Destroy {\em *this} server.
					Calls \Ref{QTTimer::destroy}.
					Calls \Ref{ConnectionObject::destroy}.
					@see         QTTimer::destroy
					@see         ConnectionObject::destroy
			*/
			virtual void destroy()
				throw();
			//@}

			/**	@name	Exceptions
			*/
			//@{
			
			/** NotCompositeObject Exception class.
					This exeption will be thrown if a \Ref{PersistentObject} was received
					that was not a \Ref{Composite} object.
					@see         GeneralException			
			*/
			class NotCompositeObject:	public Exception::GeneralException
			{
				public:

				NotCompositeObject(const char* file, int line)
					throw();
			};
			//@}

			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{

			/** Activates the server.
					Creates a new socket stream with the given port and enables the timer
					that will check
					every second whether an object will be available at the stream.
					After this method the {\em timer} method will be called every second.
					Must be called before other methods!
					Calls \Ref{QTTimer::startTimer}
					@see QTTimer::startTimer
					@see timer
			*/
			void activate()
				throw();

			/** Deactivates the server.
					If {\em *this} server is already running this method stops the server
					and closes the socket stream.
					Calls \Ref{QTTimer::stopTimer}
					@see QTTimer::stopTimer
			*/
			void deactivate()
				throw();

			/**	Set the server port.
					Set port of {\em *this} server. Must be called before \Ref{activate}
					to have any effect.
					@param  port the new port
			*/
			void setPort(const int port)
				throw();

			/**	Return the server port.
					Return the port of {\em *this} server.
					@return int the port of {\em *this} server
			*/
			int getPort() const
				throw();

			/** Register a ObjectCreator.
					Register a \Ref{ObjectCreator} that is used for converting 
					\Ref{PersistentObject} objects into \Ref{Composite} objects.
					@see ObjectCreator
			*/
			void registerObjectCreator(const ObjectCreator& s)
				throw();

			/** Reset the ObjectCreator.
					After calling this method \Ref{PersistentObject} objects will be converted
					using the default \Ref{ObjectCreator}.
					@see ObjectCreator
			*/
			void unregisterObjectCreator()
				throw();

			/**	Initialize the server widget.
					This method initializes the icon of {\em *this} server and adds it
					to \Ref{MainControl}. This method will be called by \Ref{show} of 
					the class \Ref{MainControl}.
				  @see  ModularWidget
					@see  show
			*/
			virtual void initializeWidget(MainControl& main_control)
				throw();
			
			/**	Remove the server widget.
					This method deletes the icon of {\em *this} server and removes it
					from \Ref{MainControl}.
					This method will be called by \Ref{aboutToExit} of 
					the class \Ref{MainControl}.
				  @see  ModularWidget
					@see  aboutToExit
			*/
			virtual void finalizeWidget(MainControl& main_control)
				throw();
			
			/** Menu checking method.
					This method checks, enables or disables all inserted menu entries of 
					{\em *this} server. It will be called by \Ref{checkMenus} of 
					the class \Ref{MainControl}.	
					See \Ref{ModularWidget} for further information concerning menu structure
					creation of \Ref{ModularWidget} objects.\\
					{\bf Note:} Because {\em *this} server has no menu entries this method
					is empty.
				  @see        ModularWidget
					@see        checkMenus
			*/
			virtual void checkMenu(MainControl& main_control)
				throw();
			
			/** Initialize a preferences tab for the server.
					This method creates the preferences tab \Ref{ServerPreferences} for
					{\em *this} server and inserts it into the \Ref{Preferences} dialog
					of the \Ref{MainControl}.
					This method is called automatically by the method \Ref{show} of 
					the class \Ref{MainControl} at the start of the application.
					See \Ref{ModularWidget}	for more information concerning preferences tabs.\\
					@param  preferences the \Ref{Preferences} dialog of the \Ref{MainControl}
					@see    show
					@see    ServerPreferences
					@see    Preferences
			*/
			virtual void initializePreferencesTab(Preferences &preferences)
				throw();
			
			/**	Remove the preferences tab.
					This method removes the \Ref{ServerPreferences} tab of {\em *this} server
					from the \Ref{Preferences} dialog of the \Ref{MainControl}.
					This method is called automatically by the method \Ref{aboutToExit}
					method  of the class \Ref{MainControl} at the end of the application.
					See \Ref{ModularWidget}
					for more information concerning preferences tabs.\\
					@param  preferences the \Ref{Preferences} dialog of the \Ref{MainControl}
					@see    aboutToExit
					@see    ServerPreferences
					@see    Preferences
			*/
			virtual void finalizePreferencesTab(Preferences &preferences)
				throw();
			
			/** Apply the preferences of the specific tab.
					This method applies the preferences of the own tab \Ref{ServerPreferences}
					to {\em *this} server.
					This method is called automatically by the method \Ref{applyPreferencesTab} of 
					the class \Ref{MainControl}.
					See \Ref{ModularWidget}	for more information concerning preferences tabs.\\
					@param  preferences the \Ref{Preferences} dialog of the \Ref{MainControl}
					@see    applyPreferencesTab
					@see    ServerPreferences
					@see    Preferences
			*/
			virtual void applyPreferences(Preferences &preferences)
				throw();
			
			/** Fetch the widgets preferences from the inifile.
					This method fetches the preferences of {\em *this} server 
					from the {\em inifile}.\\
					This method is called automatically by the method \Ref{show} from the \Ref{MainControl}
					object.
					See \Ref{ModularWidget}	for more information concerning preferences tabs.\\
					@param  inifile the \Ref{INIFile} that contains the needed values
					@see    MainControl
					@see    show
					@see    INIFile
					@see    ServerPreferences
			*/
			virtual void fetchPreferences(INIFile &inifile)
				throw();
			
			/** Writes the widgets preferences to the inifile.
					This method writes the preferences of {\em *this} server to the
					{\em inifile}.\\
					This method is called automatically by the method \Ref{aboutToExit} from the
					\Ref{MainControl}
					object.
					@param  inifile the \Ref{INIFile} that contains the needed values
					@see    MainControl
					@see    aboutToExit
					@see    INIFile
					@see    ServerPreferences
			*/
			virtual void writePreferences(INIFile &inifile)
				throw();
			//@}

			/**	@name	debuggers and diagnostics
			*/
			//@{
			/** Internal state and consistency self-validation.
					Initiate self-validation of the internal state and data structure consistencies
					of {\em *this} server.
					If the internal state of {\em *this} server is correct (self-validated) and 
					consistent {\tt true} is returned, {\tt false} otherwise. 
					Calls {ConnectionObject::isValid}.
					@return			bool {\tt true} if the internal state of {\em *this} server is correct (self-validated) and consistent, {\tt false} otherwise
					@see        ConnectionObject::isValid
			*/
			virtual bool isValid() const
				throw();

			/** Internal value dump.
					Dump the current state of {\em *this} server to 
					the output ostream {\em s} with dumping depth {\em depth}.
					@param   s output stream where to output the state of {\em *this} server
					@param   depth the dumping depth
					@see     ConnectionObject::dump
					@see     QTTimer::dump
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
				throw();
			//@}

			/**	@name	Storers
			*/	
			//@{

			/** Persistent stream output and state restorage.
  			  Read persistent server data from the input stream {\em s} and 
				  restore the state of {\em *this} server.\\
				  {\bf Note:} Not yet implemented.
				  @param s input stream from where to restore the internal state of {\em *this} server
			*/
			virtual void read(std::istream& s)
				throw();

			/** Persistent stream output and state storage.
  			  Write persistent server data to the output stream {\em s} and 
				  store the state of {\em *this} server.\\
				  {\bf Note:} Not yet implemented.
				  @param s output stream to where to store the internal state of {\em *this} server
			*/
			virtual void write(std::ostream& s) const
				throw();
			//@}

			
			protected:

			/** @name Timer method.
			*/
			//@{
			
			/** Timer method.
					Virtually overridden method.
					This method handles the socket stream. Every second it checks whether
					a new object is available at the stream. If this is the case the stream
					will be accepted and the incoming object will be reveiced.
					At the moment only \Ref{Composite} objects will be accepted. If
					another object is received the exception \Ref{NotCompositeObject}
					will be thrown.				
					@see    QTTimer::timer
					@exception NotCompositeObject thrown if another object than \Ref{Composite} object is received
			*/
			virtual void timer();
			//@}


			private:

			/** private methodes used for reacting to client requests.
			*/
			void sendObject(IOStreamSocket& iostream_socket)
				throw(NotCompositeObject);

			void setCreatorValue(IOStreamSocket& iostream_socket);
			void getCreatorValue(IOStreamSocket& iostream_socket);
			void hasCreatorValue(IOStreamSocket& iostream_socket);


			/** private storage variables.
			*/
			ObjectCreator *object_creator_;

			Composite* received_composite_;

			typedef HashMap<unsigned long, Composite *> CompositeHashMap;

			CompositeHashMap composite_hashmap_;

			IOStreamSocket*	iostream_socket_;
			SockInetBuf*		sock_inet_buf_;
				
			// the port to bind to
			int							port_; 

			ServerPreferences  *server_preferences_;
			QLabel 					   *server_icon_;
			static const char  *mini_ray_xpm_[];
		};



#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/KERNEL/server.iC>
#		endif

	}// namespace VIEW
		
}// namespace BALL

#endif // BALL_VIEW_GUI_KERNEL_SERVER_H
