// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_CONCEPT_CLIENT_H
#define BALL_CONCEPT_CLIENT_H

#ifndef BALL_COMMON_H
# include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_COMPOSITE_H
#	include <BALL/CONCEPT/composite.h>
#endif

#ifndef BALL_SYSTEM_NETWORKING_H
#	include <BALL/SYSTEM/networking.h>
#endif

#ifndef BALL_CONCEPT_PERSISTENTOBJECT_H       
# include <BALL/CONCEPT/persistentObject.h>
#endif

#ifndef BALL_CONCET_TEXTPERSISTENCEMANAGER_H
# include <BALL/CONCEPT/textPersistenceManager.h>
#endif

#define DEFAULT_PORT 20000

namespace BALL
{
	/** Client class.	
			The class Client introduces a rudimentary interface for sending Composite
			objects to the visualization.
			In this version only the one way direction from this client to the
			visualization is possible. In the future there is also a bidirectional
			data exchange intended. If a Composite is sent two times than the 
			graphical representation of the first one will be replaced through the
			new graphical representation of the second one. For example if the structure
			of the Composite is continually changed by a program so the visualization
			of this Composite changes as well.
			The Client class connects to the Server class that is part of
			the VIEW library. See Server for further information.
			\see     Server
			\ingroup  Persistence
	*/
	class BALL_EXPORT Client
	{
		public:

		/** Command enums for the Client and Server.
				These enums specify the commands the server is able
				to understand (at the moment).
				\see  Server
				\see  Client
		*/
		enum Command
		{
			/// unknown command.
			COMMAND__UNKOWN              = 0,

			/// sent command. This command will be used for indicating a new object that is about to be received
			COMMAND__SEND_OBJECT         = 1,

			/// next free command.
			NUMBER_OF_COMMANDS
		};	

		/**	@name	Exceptions
		*/
		//@{
		
		/** InvalidClient exception class.
				This exception will be thrown whenever a client action will be executed without
				an open working connection.
				\see         Exception::GeneralException			
		*/
		class BALL_EXPORT InvalidClient 
			: public Exception::GeneralException
		{
			public:
				InvalidClient(const char* file, int line);
		};

		/** NoPersistentObject exception class.
				This exception will be thrown whenever an object that is not a 
				PersistentObject will be tried to sent through the client.
				Only PersistentObject's can be sent through this client.
				\see         Exception::GeneralException			
		*/
		class BALL_EXPORT NoPersistentObject
		  :	public Exception::GeneralException
		{
			public:
				NoPersistentObject(const char* file, int line);
		};

		//@}
		/**	@name	Constructors
		*/	
		//@{

		/** Default Constructor.
				Constructs new client.
				The client will have no working connection yet.
				\return      Client new constructed client
				\see         connect
		*/
		Client();

		/** Detailed state initializing constructor.
				Constructs new client.
				The state of this client is:
					- host is <b> host</b>
					- port is <b> port</b> (Default = 20000
				\par		
				\param       host the host to connect to
				\param       port the port of the host to connect to
				\return      Client new constructed client
				\see         connect
		*/
		Client(const String& host, int port = 20000);

		//@}
		/** @name Destructors */
		//@{

		/** Destructor.
		*/
		virtual ~Client();

		/** Explicit default initialization.
				Empty for further purpose.
		*/
		virtual void clear();

		//@}
		/**	@name	Accessors: inspectors and mutators 
		*/
		//@{

		/** Connects to a specified host and port.
				This method connects this client to a specified host and port.
				Must be called before any other methodes!
				\param   host the host (a string) to connect to
				\param   port the port number of the host
		*/
		void connect(const String& host, int port = 20000);

		/** Adds a new composite.
				Inserts a new Composite to this client. 
				With this method Composite's can be added to the visualization.
				If the same Composite was already added before it will be replaced 
				by the new representation of <b> composite</b>.
				The Composite will be put into the TextPersistenceManager that
				has a connection to a socket stream connected to the given host and port.
				The TextPersistenceManager will then use that socket stream to write
				the Composite into.
				\param   composite the Composite to be added to the client (visualization)
				\see     TextPersistenceManager::TextPersistenceManager
				\exception  InvalidClient if the client has no connection to a server
				\exception  NoPersistentObject if the composite is not a PersistentObject
		*/
		void insert(Composite &composite);
		//@}
		/**	@name	debuggers and diagnostics
		*/
		//@{

		/** Internal state and consistency self-validation.
				Initiates self-validation of the internal state of this client. 
				If the this client has a connection to a server than <tt> true</tt>
				is returned, <tt> false</tt> otherwise. 
				\return			bool -
										<tt> true</tt> if this client has a connection to a server,
										<tt> false</tt> otherwise
		*/
		virtual bool isValid() const;

		/** Internal value dump.
				Dumps the current host and port of this client to 
				the output ostream <b> s</b> with dumping depth <b> depth</b>.
				\param   s output stream where to output the host and port of this client
				\param   depth the dumping depth
		*/
		virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;

		//@}
		
		private:

		String	host_;
		int			port_;

		TextPersistenceManager pm_;
	};
  
} // namespace BALL

#endif // BALL_CONCEPT_CLIENT_H
