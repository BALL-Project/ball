// $Id: server.h,v 1.3 2000/09/23 14:15:05 hekl Exp $

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

#ifndef BALL_VIEW_KERNEL_CONNECTIONOBJECT_H
#	include <BALL/VIEW/KERNEL/connectionObject.h>
#endif


namespace BALL
{
	
	namespace VIEW
	{

		class Server
			: public QTTimer,
			  public ConnectionObject
		{
			public:

			/**	@name	 Constructors and Destructors	
			*/
			//@{

			/** default constructor. creates an instance of server.
			*/
			Server();

			/**	Copy constructor
			*/
			Server(const Server& server, bool deep = true);

			/** default destructor. destroys an instance of server.
			*/
			virtual ~Server();

			/** clear methode. resets the timer used to control the server
			*/
			virtual void clear();
			//@}

			/**	@name	Exceptions
			*/
			//@{
			
			/** Composite Exception. returned if the creation of a received object failed.
			*/
			class CompositePointerIsNull
				:	public Exception::GeneralException
			{
				public:

				CompositePointerIsNull(const char* file, int line);
			};
			//@}

			/**	@name	Accessors
			*/
			//@{

			/** Activates the server. Must be called before other methods!
			*/
			void activate();

			/** Deactivates the server. Must be called before other methods!
			*/
			void deactivate();

			/**	Set the server port.
			*/
			void setPort(const int);

			/**	Return the server port.
			*/
			int getPort() const;

			/** returns the composite received from client
			*/
			const Composite* getReceivedComposite() const;

			/** registers a ObjectCreator that is used for converting presistent objects
					into composites.
			*/
			void registerObjectCreator(const ObjectCreator& s);

			/** resets the object creator. After calling this methode object will be converted
					using default object creator.
			*/
			void deregisterObjectCreator();

			/** handels notification requests
			*/
			//			virtual void onNotify(Message *message);
			//@}

			/**	@name	Debugging and Diagnostics
			*/
			//@{

			/** returns true if the underlying timer is valid.
		  */
			virtual bool isValid() const;

			/** dumps the timer states.
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;
			//@}

			/**	@name	Storers
			*/	
			//@{

			/** Not yet implemented.
			*/
			virtual void read(std::istream& s);

			/** Not yet implemented.
			*/
			virtual void write(std::ostream& s) const;
			//@}

			
			protected:

			/** overridden timer call.
			*/
			virtual void timer();


			private:

			/** private methodes used for reacting to client requests.
			*/
			void sendObject(IOStreamSocket& iostream_socket);

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
		};



#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/KERNEL/server.iC>
#		endif

	}// namespace VIEW
		
}// namespace BALL

#endif // BALL_VIEW_GUI_KERNEL_SERVER_H
