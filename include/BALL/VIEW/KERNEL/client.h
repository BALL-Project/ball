// $Id: client.h,v 1.1 1999/08/26 07:53:14 oliver Exp $

#ifndef BALL_VIEW_KERNEL_CLIENT_H
#define BALL_VIEW_KERNEL_CLIENT_H

#ifndef BALL_COMMON_H
# include <BALL/common.h>
#endif

#ifndef BALL_SYSTEM_SOCKET_H
#	include <BALL/SYSTEM/socket.h>
#endif

#ifndef BALL_VIEW_COMMON_GLOBAL_H
#	include <BALL/VIEW/COMMON/global.h>
#endif

#ifndef BALL_VIEW_KERNEL_CLIENTSCENE_H
#	include <BALL/VIEW/KERNEL/clientScene.h>
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

			virtual ~Client();

			virtual void clear();

			virtual void destroy();
			//@}
		
			/**	@name	Accessors
			*/
			//@{

			/** connect to a specified host and port. Must be called before any other methodes!
			*/
			void connect(const char* host, int port);

			/** creates a new scene on the server and returns it.
			*/
			ClientScene getNewScene();

			//@}

			/**	@name	Debugging and Diagnostics
			*/
			//@{

			virtual bool isValid() const;

			virtual void dump(ostream& s = cout, unsigned long depth = 0) const;
			//@}

			/**	@name	Storers
			*/
			//@{

			virtual void read(istream& s);

			virtual void write(ostream& s) const;

			//@}
			
			private:

			char *host_;
			int port_;
		};


#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/KERNEL/client.iC>
#		endif
		
	} // namespace VIEW
	
} // namespace BALL

#endif // BALL_VIEW_KERNEL_CLIENT_H
