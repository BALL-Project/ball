// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_KERNEL_SERVERWIDGET_H
#define BALL_VIEW_KERNEL_SERVERWIDGET_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_DATATYPE_HASHMAP_H
#	include <BALL/DATATYPE/hashMap.h>
#endif

#ifndef BALL_VIEW_KERNEL_MODULARWIDGET_H
#	include <BALL/VIEW/KERNEL/modularWidget.h>
#endif

#ifndef BALL_CONCEPT_OBJECTCREATOR_H
# include <BALL/CONCEPT/objectCreator.h>
#endif

#ifndef BALL_SYSTEM_NETWORKING_H
# include <BALL/SYSTEM/networking.h>
#endif

#ifndef BALL_VIEW_KERNEL_THREADS_H
# include <BALL/VIEW/KERNEL/threads.h>
#endif

#include <QtCore/QTimer>
#include <QtWidgets/QLabel>

class QLabel;

namespace BALL
{
	class Composite;

	namespace VIEW
	{
		class ServerPreferences;
		class Preferences;

		/** ServerWidget class.
				The class ServerWidget handles all incoming PersistentObject objects,
				converts them into Composite objects (if possible) and sents
				them through the ConnectionObject tree with the message
				NewCompositeMessage. Also it stores all received Composite objects
				and replaces them if the same Composite object is received again.
				If a Composite object is replaced the message 
				RemovedCompositeMessage will be sent through the ConnectionObject
				tree and after that the the message NewCompositeMessage with the new
				received composite will be sent.

				\ingroup ViewKernelClient
		*/
		class BALL_VIEW_EXPORT ServerWidget
			  : public QObject,
					public ModularWidget
		{
			Q_OBJECT

			public:

			BALL_EMBEDDABLE(ServerWidget,ModularWidget)

			/**	@name	Exceptions
			*/
			//@{
			
			/** NotCompositeObject Exception class.
					This exeption will be thrown if a PersistentObject was received
					that was not a Composite object.
					\see         GeneralException			
			*/
			class BALL_VIEW_EXPORT NotCompositeObject:	public Exception::GeneralException
			{
				public:

				NotCompositeObject(const char* file, int line);
			};

			//@}
			/** BALLView server thread.
			 *  This class handles the incoming connections for the simple
			 *  BALL-protocol for sending composites over the net.
			 */
			class BALLViewServer
				: public virtual BALLThread,
					public virtual TCPServerThread
			{
				public:
					BALLViewServer(ServerWidget* parent, Size port, bool restart = true);

					/** private methodes used for reacting to client requests.
					*/
					void sendObject()
						throw(NotCompositeObject);

					virtual void run();

					/** Handler for successful connections.
							Virtually overridden method.
							This method handles the socket stream. When a connection has been
							made, this function is automatically called through our base classes.
							Then, the socket stream will be passed onto the object creator and
							incoming objects will be received.
							At the moment only Composite objects will be accepted. If
							another object is received the exception NotCompositeObject
							will be thrown.				
							\exception NotCompositeObject thrown if another object than Composite object is received
					*/
					virtual void handleConnection();

					/** Used for communication with the parent **/
					void setLocked(bool is_locked);

				protected:
					ServerWidget* parent_widget_;
					Composite* received_composite_;

					typedef HashMap<unsigned long, Composite *> CompositeHashMap;
					CompositeHashMap composite_hashmap_;

					Size port_;
					bool is_locked_;
			};

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					The state of this server is:
					  - no object creator registered
						- server listening on <tt> VIEW_DEFAULT_PORT</tt> if activated
					\par
					\see         ModularWidget
			*/
			ServerWidget(QWidget* parent = 0, const char* name = 0);

			// only for Python interface
			ServerWidget(const ServerWidget& server);

			//@}
			/** @name Destructors 
			*/
			//@{

			/** Destructor.
			*/
			virtual ~ServerWidget();

			/** Explicit default initialization.
					Calls ConnectionObject::clear.
					\see ConnectionObject::clear
			*/
			virtual void clear();
			//@}

			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{

			/** Start the server.
			 */
			virtual void activate() {};

			/** Stop the server.
			 */
			virtual void deactivate() {};

			/**	Set the server port.
					\param  port the new port
			*/
			void setPort(const int port);

			/**	Return the server port.
					Return the port of this server.
					\return int the port of this server
			*/
			int getPort() const;

			/** Register a ObjectCreator that is used for converting 
					PersistentObject objects into Composite objects.
					Every ObjectCreator, that is still registered, when a ServerWidget instance is destructed, will be deleted.
					\see ObjectCreator
			*/
			void registerObjectCreator(const ObjectCreator& s);

			/** Reset the ObjectCreator.
					After calling this method PersistentObject objects will be converted
					using the default ObjectCreator.
					\see ObjectCreator
			*/
			void unregisterObjectCreator();

			/** Return the ObjectCreator.
			 */
			ObjectCreator& getObjectCreator();

			/** Return the ObjectCreator, const version.
			 */
			const ObjectCreator& getObjectCreator() const;

			/**	Initialize the server widget.
					This method initializes the icon of this server and adds it
					to MainControl. This method will be called by show of 
					the class MainControl.
				  \see  ModularWidget
					\see  show
			*/
			virtual void initializeWidget(MainControl& main_control);
			
			/**	Remove the server widget.
					This method deletes the icon of this server and removes it
					from MainControl.
					This method will be called by aboutToExit of 
					the class MainControl.
				  \see  ModularWidget
					\see  aboutToExit
			*/
			virtual void finalizeWidget(MainControl& main_control);
			
			/** Initialize a preferences tab for the server.
					This method creates the preferences tab ServerPreferences for
					this server and inserts it into the Preferences dialog
					of the MainControl.
					This method is called automatically by the method show of 
					the class MainControl at the start of the application.
					See ModularWidget	for more information concerning preferences tabs.\par
					\param  preferences the Preferences dialog of the MainControl
					\see    show
					\see    ServerPreferences
					\see    Preferences
			*/
			virtual void initializePreferencesTab(Preferences &preferences);
			
			/**	Remove the preferences tab.
					This method removes the ServerPreferences tab of this server
					from the Preferences dialog of the MainControl.
					This method is called automatically by the method aboutToExit
					method  of the class MainControl at the end of the application.
					See ModularWidget
					for more information concerning preferences tabs.\par
					\param  preferences the Preferences dialog of the MainControl
					\see    aboutToExit
					\see    ServerPreferences
					\see    Preferences
			*/
			virtual void finalizePreferencesTab(Preferences &preferences);
			
			/** Apply the preferences of the specific tab.
					This method applies the preferences of the own tab ServerPreferences
					to this server.
					This method is called automatically by the method applyPreferencesTab of 
					the class MainControl.
					See ModularWidget	for more information concerning preferences tabs.\par
					\param  preferences the Preferences dialog of the MainControl
					\see    applyPreferencesTab
					\see    ServerPreferences
					\see    Preferences
			*/
			virtual void applyPreferences();

			//@}
			/**	@name	debuggers and diagnostics
			*/
			//@{

			/** Internal state and consistency self-validation.
					Calls {ConnectionObject::isValid}.
					\return			bool <tt> true</tt> if the internal state of this server is correct
					\see        ConnectionObject::isValid
			*/
			virtual bool isValid() const;

			/** Dump the current state of this server to 
					the output ostream with a dumping depth.
					\param   s output stream where to output the state of this server
					\param   depth the dumping depth
					\see     ConnectionObject::dump
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;

			/** This function is used by the server thread to handle locking of composites across several threads.
			 *  
			 *  The protocol is rather complicated: the server thread reacts to a connection, creates a composite,
			 *  calls changeLock(), this emits the signal "lockRequested", which is finally handled in the slot
			 *  handleLocking(). The reason for this involved setup is the separation of server and GUI threads,
			 *  which does not allow to safely lock the composites in the server thread.
			 */
			virtual void changeLock(bool lock);

			//@}	
			/** @name Signals and Slots
			 */
			//@{
			public slots:
				void handleLocking(bool lock);

			signals:
				void lockRequested(bool lock);

			//@}
			private:

			/** The actual server thread **/
			BALLViewServer server_;

			/** private storage variables.
			*/
			ObjectCreator *object_creator_;

			// the port to bind to
			int							port_; 

			ServerPreferences  *server_preferences_;
			QLabel 					   *server_icon_;
			static const char  *mini_ray_xpm_[];
		};


#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/KERNEL/serverWidget.iC>
#		endif
  
	}// namespace VIEW
}// namespace BALL

#endif // BALL_VIEW_KERNEL_SERVER_H
