// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: server.h,v 1.17 2003/03/14 11:50:15 sturm Exp $

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
    /**  \addtogroup  ViewGuiKernelServer
     *  @{
     */
		/** Server class.
				The class Server handles all incoming  \link PersistentObject PersistentObject \endlink  objects,
				converts them into  \link Composite Composite \endlink  objects (if possible) and sents
				them through the  \link ConnectionObject ConnectionObject \endlink  tree with the message
				 \link NewCompositeMessage NewCompositeMessage \endlink . Also it stores all received  \link Composite Composite \endlink  objects
				and replaces them if the same  \link Composite Composite \endlink  object is received again.
				If a  \link Composite Composite \endlink  object is replaced the message 
				 \link RemovedCompositeMessage RemovedCompositeMessage \endlink  will be sent through the  \link ConnectionObject ConnectionObject \endlink 
				tree and after that the the message  \link NewCompositeMessage NewCompositeMessage \endlink  with the new
				received composite will be sent.
				
		*/
		class Server
			: public QTTimer,
   			public ModularWidget
		{
			public:
			
			BALL_EMBEDDABLE(Server)

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new server.
					The state of {\em *this} server is:

					  - no object creator registered
						- server listening on <tt>VIEW_DEFAULT_PORT</tt> if activated
					
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
					Calls  \link destroy destroy \endlink .
					@see         destroy
			*/
			virtual ~Server()
				throw();

			/** Explicit default initialization.
					Set the state of {\em *this} server to the default values.
					Calls  \link QTTimer::clear QTTimer::clear \endlink .
					Calls  \link ConnectionObject::clear ConnectionObject::clear \endlink .
					@see QTTimer::clear
					@see ConnectionObject::clear
			*/
			virtual void clear()
				throw();

			/** Explicit destructor.
					Destroy {\em *this} server.
					Calls  \link QTTimer::destroy QTTimer::destroy \endlink .
					Calls  \link ConnectionObject::destroy ConnectionObject::destroy \endlink .
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
					This exeption will be thrown if a  \link PersistentObject PersistentObject \endlink  was received
					that was not a  \link Composite Composite \endlink  object.
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
					Calls  \link QTTimer::startTimer QTTimer::startTimer \endlink 
					@see QTTimer::startTimer
					@see timer
			*/
			void activate()
				throw();

			/** Deactivates the server.
					If {\em *this} server is already running this method stops the server
					and closes the socket stream.
					Calls  \link QTTimer::stopTimer QTTimer::stopTimer \endlink 
					@see QTTimer::stopTimer
			*/
			void deactivate()
				throw();

			/**	Set the server port.
					Set port of {\em *this} server. Must be called before  \link activate activate \endlink 
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
					Register a  \link ObjectCreator ObjectCreator \endlink  that is used for converting 
					 \link PersistentObject PersistentObject \endlink  objects into  \link Composite Composite \endlink  objects.
					@see ObjectCreator
			*/
			void registerObjectCreator(const ObjectCreator& s)
				throw();

			/** Reset the ObjectCreator.
					After calling this method  \link PersistentObject PersistentObject \endlink  objects will be converted
					using the default  \link ObjectCreator ObjectCreator \endlink .
					@see ObjectCreator
			*/
			void unregisterObjectCreator()
				throw();

			/**	Initialize the server widget.
					This method initializes the icon of {\em *this} server and adds it
					to  \link MainControl MainControl \endlink . This method will be called by  \link show show \endlink  of 
					the class  \link MainControl MainControl \endlink .
				  @see  ModularWidget
					@see  show
			*/
			virtual void initializeWidget(MainControl& main_control)
				throw();
			
			/**	Remove the server widget.
					This method deletes the icon of {\em *this} server and removes it
					from  \link MainControl MainControl \endlink .
					This method will be called by  \link aboutToExit aboutToExit \endlink  of 
					the class  \link MainControl MainControl \endlink .
				  @see  ModularWidget
					@see  aboutToExit
			*/
			virtual void finalizeWidget(MainControl& main_control)
				throw();
			
			/** Menu checking method.
					This method checks, enables or disables all inserted menu entries of 
					{\em *this} server. It will be called by  \link checkMenus checkMenus \endlink  of 
					the class  \link MainControl MainControl \endlink .	
					See  \link ModularWidget ModularWidget \endlink  for further information concerning menu structure
					creation of  \link ModularWidget ModularWidget \endlink  objects. \par
					<b>Note:</b> Because {\em *this} server has no menu entries this method
					is empty.
				  @see        ModularWidget
					@see        checkMenus
			*/
			virtual void checkMenu(MainControl& main_control)
				throw();
			
			/** Initialize a preferences tab for the server.
					This method creates the preferences tab  \link ServerPreferences ServerPreferences \endlink  for
					{\em *this} server and inserts it into the  \link Preferences Preferences \endlink  dialog
					of the  \link MainControl MainControl \endlink .
					This method is called automatically by the method  \link show show \endlink  of 
					the class  \link MainControl MainControl \endlink  at the start of the application.
					See  \link ModularWidget ModularWidget \endlink 	for more information concerning preferences tabs. \par
					@param  preferences the  \link Preferences Preferences \endlink  dialog of the  \link MainControl MainControl \endlink 
					@see    show
					@see    ServerPreferences
					@see    Preferences
			*/
			virtual void initializePreferencesTab(Preferences &preferences)
				throw();
			
			/**	Remove the preferences tab.
					This method removes the  \link ServerPreferences ServerPreferences \endlink  tab of {\em *this} server
					from the  \link Preferences Preferences \endlink  dialog of the  \link MainControl MainControl \endlink .
					This method is called automatically by the method  \link aboutToExit aboutToExit \endlink 
					method  of the class  \link MainControl MainControl \endlink  at the end of the application.
					See  \link ModularWidget ModularWidget \endlink 
					for more information concerning preferences tabs. \par
					@param  preferences the  \link Preferences Preferences \endlink  dialog of the  \link MainControl MainControl \endlink 
					@see    aboutToExit
					@see    ServerPreferences
					@see    Preferences
			*/
			virtual void finalizePreferencesTab(Preferences &preferences)
				throw();
			
			/** Apply the preferences of the specific tab.
					This method applies the preferences of the own tab  \link ServerPreferences ServerPreferences \endlink 
					to {\em *this} server.
					This method is called automatically by the method  \link applyPreferencesTab applyPreferencesTab \endlink  of 
					the class  \link MainControl MainControl \endlink .
					See  \link ModularWidget ModularWidget \endlink 	for more information concerning preferences tabs. \par
					@param  preferences the  \link Preferences Preferences \endlink  dialog of the  \link MainControl MainControl \endlink 
					@see    applyPreferencesTab
					@see    ServerPreferences
					@see    Preferences
			*/
			virtual void applyPreferences(Preferences &preferences)
				throw();
			
			/** Fetch the widgets preferences from the inifile.
					This method fetches the preferences of {\em *this} server 
					from the {\em inifile}. \par
					This method is called automatically by the method  \link show show \endlink  from the  \link MainControl MainControl \endlink 
					object.
					See  \link ModularWidget ModularWidget \endlink 	for more information concerning preferences tabs. \par
					@param  inifile the  \link INIFile INIFile \endlink  that contains the needed values
					@see    MainControl
					@see    show
					@see    INIFile
					@see    ServerPreferences
			*/
			virtual void fetchPreferences(INIFile &inifile)
				throw();
			
			/** Writes the widgets preferences to the inifile.
					This method writes the preferences of {\em *this} server to the
					{\em inifile}. \par
					This method is called automatically by the method  \link aboutToExit aboutToExit \endlink  from the
					 \link MainControl MainControl \endlink 
					object.
					@param  inifile the  \link INIFile INIFile \endlink  that contains the needed values
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
					consistent <tt>true</tt> is returned, <tt>false</tt> otherwise. 
					Calls {ConnectionObject::isValid}.
					@return			bool <tt>true</tt> if the internal state of {\em *this} server is correct
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
			protected:

			/** @name Timer method.
			*/
			//@{
			
			/** Timer method.
					Virtually overridden method.
					This method handles the socket stream. Every second it checks whether
					a new object is available at the stream. If this is the case the stream
					will be accepted and the incoming object will be reveiced.
					At the moment only  \link Composite Composite \endlink  objects will be accepted. If
					another object is received the exception  \link NotCompositeObject NotCompositeObject \endlink 
					will be thrown.				
					@see    QTTimer::timer
					@exception NotCompositeObject thrown if another object than  \link Composite Composite \endlink  object is received
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
  /** @} */
	}// namespace VIEW
		
}// namespace BALL

#endif // BALL_VIEW_GUI_KERNEL_SERVER_H
