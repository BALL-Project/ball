// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: client.h,v 1.14 2003/02/21 16:07:50 anhi Exp $

#ifndef BALL_VIEW_KERNEL_CLIENT_H
#define BALL_VIEW_KERNEL_CLIENT_H

#ifndef BALL_COMMON_H
# include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_COMPOSITE_H
#	include <BALL/CONCEPT/composite.h>
#endif

#ifndef BALL_SYSTEM_SOCKET_H
#	include <BALL/SYSTEM/socket.h>
#endif

#ifndef BALL_CONCEPT_PERSISTENTOBJECT_H       
# include <BALL/CONCEPT/persistentObject.h>
#endif

#ifndef BALL_CONCET_TEXTPERSISTENCEMANAGER_H
# include <BALL/CONCEPT/textPersistenceManager.h>
#endif

#ifndef BALL_VIEW_COMMON_GLOBAL_H
#	include <BALL/VIEW/COMMON/global.h>
#endif

namespace BALL
{
	namespace VIEW
	{

		/** Client class.	
				The class Client introduces a rudimentary interface for sending  \link Composite Composite \endlink 
				objects to the visualization.
				In this version only the one way direction from {\em *this} client to the
				visualization is possible. In the future there is also a bidirectional
				data exchange intended. If a  \link Composite Composite \endlink  is sent two times than the 
				graphical representation of the first one will be replaced through the
				new graphical representation of the second one. For example if the structure
				of the  \link Composite Composite \endlink  is continually changed by a program so the visualization
				of this  \link Composite Composite \endlink  changes as well.
				The Client class connects to the  \link Server Server \endlink  class that is also a part of
				the VIEW library. See  \link Server Server \endlink  for further information.
				<b>Definition:</b> BALL/VIEW/KERNEL/client.h
				@see     Server
		*/
		class Client
		{
			public:

			/**	@name	Exceptions
			*/
			//@{
			
			/** InvalidClient exception class.
					This exception will be thrown whenever a client action will be executed without
					an open working connection.
					@see         Exception::GeneralException			
			*/
			class InvalidClient: public Exception::GeneralException
			{
				public:

				InvalidClient(const char* file, int line)
					throw();
			};

			/** NoPersistentObject exception class.
					This exception will be thrown whenever an object that is not a 
					 \link PersistentObject PersistentObject \endlink  will be tried to sent through the client.
					Only  \link PersistentObject PersistentObject \endlink 's can be sent through {\em *this} client.
					@see         Exception::GeneralException			
			*/
			class NoPersistentObject:	public Exception::GeneralException
			{
				public:

				NoPersistentObject(const char* file, int line)
					throw();
			};

			//@}
			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Constructs new client.
					The client will have no working connection yet.
					@return      Client new constructed client
					@see         connect
			*/
			Client()
				throw();

			/** Detailed state initializing constructor.
					Constructs new client.
					The state of {\em *this} client is:
					\begin{itemize}
						\item host is {\em host}
						\item port is {\em port} (Default = VIEW_DEFAULT_PORT)
					\end{itemize}		
     			@param       host the host to connect to
					@param       port the port of the host to connect to
					@return      Client new constructed client
					@see         connect
			*/
			Client(const String& host, int port = VIEW_DEFAULT_PORT)
				throw();

			//@}
			/** @name Destructors */
			//@{

			/** Destructor.
					Default destruction of {\em *this} client.
					Calls  \link destroy destroy \endlink .
					@see         destroy
			*/
			virtual ~Client()
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

			/** Connects to a specified host and port.
					This method connects {\em *this} client to a specified host and port.
					Must be called before any other methodes!
					@param   host the host (a string) to connect to
					@param   port the port number of the host
			*/
			void connect(const String& host, int port = VIEW_DEFAULT_PORT)
				throw();

			/** Adds a new composite.
					Inserts a new  \link Composite Composite \endlink  to {\em *this} client. 
					With this method  \link Composite Composite \endlink 's can be added to the visualization.
					If the same  \link Composite Composite \endlink  was already added before it will be replaced 
					by the new representation of {\em composite}.
					The  \link Composite Composite \endlink  will be put into the  \link TextPersistenceManager TextPersistenceManager \endlink  that
					has a connection to a socket stream connected to the given host and port.
					The  \link TextPersistenceManager TextPersistenceManager \endlink  will then use that socket stream to write
					the  \link Composite Composite \endlink  into.
					@param   composite the  \link Composite Composite \endlink  to be added to the client (visualization)
					@see     TextPersistenceManager::TextPersistenceManager
					@exception  InvalidClient if the client has no connection to a server
					@exception  NoPersistentObject if the composite is not a  \link PersistentObject PersistentObject \endlink 
			*/
			void insert(Composite &composite)
				throw(InvalidClient, NoPersistentObject);
			//@}
			/**	@name	debuggers and diagnostics
			*/
			//@{

			/** Internal state and consistency self-validation.
					Initiates self-validation of the internal state of {\em *this} client. 
					If the {\em *this} client has a connection to a server than <tt>true</tt>
					is returned, <tt>false</tt> otherwise. 
					@return			bool -
											<tt>true</tt> if {\em *this} client has a connection to a server,
					 						<tt>false</tt> otherwise
			*/
			virtual bool isValid() const
				throw();

			/** Internal value dump.
					Dumps the current host and port of {\em *this} client to 
					the output ostream {\em s} with dumping depth {\em depth}.
					@param   s output stream where to output the host and port of {\em *this} client
					@param   depth the dumping depth
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
				throw();

			//@}
			
			private:

			String	host_;
			int			port_;

			TextPersistenceManager pm_;
		};

	} // namespace VIEW
	
} // namespace BALL

#endif // BALL_VIEW_KERNEL_CLIENT_H
