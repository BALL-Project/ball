// $Id: client.h,v 1.9 2001/07/08 11:30:09 aubertin Exp $

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
				{\bf Framework:} BALL/VIEW/KERNEL\\
				{\bf Definition:} \URL{BALL/VIEW/KERNEL/client.h}	\\ \\
				The class Client introduces a rudimentary interface for sending \Ref{Composite}
				objects to the visualization.
				In this version only the one way direction from {\em *this} client to the
				visualization is possible. In the future there is also a bidirectional
				data exchange intended. If a \Ref{Composite} is sent two times than the 
				graphical representation of the first one will be replaced through the
				new graphical representation of the second one. For example if the structure
				of the \Ref{Composite} is continually changed by a program so the visualization
				of this \Ref{Composite} changes as well.
				The Client class connects to the \Ref{Server} class that is also a part of
				the VIEW library. See \Ref{Server} for further information.
				@see     Server
				@memo    Client class (BALL VIEW kernel framework)
				@author  $Author: aubertin $
				@version $Revision: 1.9 $
				@date    $Date: 2001/07/08 11:30:09 $
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
					\Ref{PersistentObject} will be tried to sent through the client.
					Only \Ref{PersistentObject}'s can be sent through {\em *this} client.
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
					Calls \Ref{destroy}.
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
					Inserts a new \Ref{Composite} to {\em *this} client. 
					With this method \Ref{Composite}'s can be added to the visualization.
					If the same \Ref{Composite} was already added before it will be replaced 
					by the new representation of {\em composite}.
					The \Ref{Composite} will be put into the \Ref{TextPersistenceManager} that
					has a connection to a socket stream connected to the given host and port.
					The \Ref{TextPersistenceManager} will then use that socket stream to write
					the \Ref{Composite} into.
					@param   composite the \Ref{Composite} to be added to the client (visualization)
					@see     TextPersistenceManager::TextPersistenceManager
					@exception  InvalidClient if the client has no connection to a server
					@exception  NoPersistentObject if the composite is not a \Ref{PersistentObject}
			*/
			void insert(Composite &composite)
				throw(InvalidClient, NoPersistentObject);
			//@}
			/**	@name	debuggers and diagnostics
			*/
			//@{

			/** Internal state and consistency self-validation.
					Initiates self-validation of the internal state of {\em *this} client. 
					If the {\em *this} client has a connection to a server than {\tt true}
					is returned, {\tt false} otherwise. 
					@return			bool -
											{\tt true} if {\em *this} client has a connection to a server,
					 						{\tt false} otherwise
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
			/**	@name	Storers
			*/
			//@{

			/** Persistent stream output and state restorage.
  			  Reads persistent client data from the input stream {\em s} and 
				  restores the state of {\em *this}. \\
				  {\bf Note:} Not yet implemented.
			 	  @param       s input stream from where to restore the internal state of {\em *this} client
			*/
			virtual void read(std::istream& s)
				throw();

			/** Persistent stream output and state storage.
  			  Writes persistent client data to the output stream {\em s} and 
				  stores the state of {\em *this}.\\
				  {\bf Note:} Not yet implemented.
				  @param       s output stream to where to store the internal state of {\em *this} client
			*/
			virtual void write(std::ostream& s) const
				throw();

			//@}
			
			private:

			String	host_;
			int			port_;

			TextPersistenceManager pm_;
		};


#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/KERNEL/client.iC>
#		endif
		
	} // namespace VIEW
	
} // namespace BALL

#endif // BALL_VIEW_KERNEL_CLIENT_H
