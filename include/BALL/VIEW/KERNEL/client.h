// $Id: client.h,v 1.5 2000/05/14 15:36:35 hekl Exp $

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

		/**	
		*/
		class Client
		{
			public:

			/**	@name	Constructors and Destructors
			*/
			//@{

			Client();

			Client(const Client& client, bool deep = true);

			/**	Create a client and connect to a host.
					Same as 
					\begin{verbatim}
						Client client;
						client.connect(host, port);
					\end{verbatim}
					@param	host the host to connect to
					@param	port the port the server is listening to
			*/
			Client(const String& host, int port = VIEW_DEFAULT_PORT);

			virtual ~Client();

			virtual void clear();

			virtual void destroy();
			//@}
		
			/**	@name	Exceptions
			*/
			//@{
			
			class InvalidClient
				:	public Exception::GeneralException
			{
				public:

				InvalidClient(const char* file, int line);
			};

			class NoPersistentObject
				:	public Exception::GeneralException
			{
				public:

				NoPersistentObject(const char* file, int line);
			};
			//@}

			/**	@name	Accessors
			*/
			//@{

			/** connect to a specified host and port. Must be called before any other methodes!
			*/
			void connect(const String& host, int port = VIEW_DEFAULT_PORT);

			void insert(Composite &composite);

			void setCreatorValue(int address, int value);

			int getCreatorValue(int address);

			bool hasCreatorValue(int address, int value);
			//@}

			/**	@name	Debugging and Diagnostics
			*/
			//@{

			virtual bool isValid() const;

			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;
			//@}

			/**	@name	Storers
			*/
			//@{

			virtual void read(std::istream& s);

			virtual void write(std::ostream& s) const;

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
