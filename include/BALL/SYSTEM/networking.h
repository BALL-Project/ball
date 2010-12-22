// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_SYSTEM_NETWORKING_H
#define BALL_SYSTEM_NETWORKING_H

#ifndef BALL_COMMON_GLOBAL_HH
# include<BALL/COMMON/global.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
# include <BALL/DATATYPE/string.h>
#endif

#ifdef BALL_HAS_BOOST_ASIO
#	include <boost/asio.hpp>
#else
#define BOOST_DATE_TIME_NO_LIB
#define BOOST_REGEX_NO_LIB
# include <asio.hpp>
#endif

#include <QtCore/QThread>

namespace BALL
{
	/** This class provides a very thin layer around the asio (or boost::asio, depending
	 *  on your setup) functions for handling asynchronous io.
	 *
	 *  Please use this class instead of asio directly whenever possible, since we want to
	 *  encapsulate a number of details (asio - namespace, correct names of header files, ...)
	 *  to allow boost-integrated asio as well as the pure library.
	 */
	class BALL_EXPORT TCPIOStream
		: public BALL_ASIO_NAMESPACE::ip::tcp::iostream
	{
		public:
			TCPIOStream() 
				: BALL_ASIO_NAMESPACE::ip::tcp::iostream()
			{
			}

			TCPIOStream(const String& hostname, const String& protocol)
				: BALL_ASIO_NAMESPACE::ip::tcp::iostream(hostname, protocol)
			{
			}

			TCPIOStream(const String& hostname, Position port)
				: BALL_ASIO_NAMESPACE::ip::tcp::iostream(hostname, String(port))
			{
			}
	};

	/** This class provides a base for very simple TCP-based servers.
	 *
	 * 	By default, the server handles only one connection. The first request is
	 * 	accepted and converted into a TCPIOStream. If restart_ is set to true (default), 
	 * 	the server listens again after the connection has been terminated.
	 *
	 * 	Note that the server is not automatically started but rather waits for a call
	 * 	to "run".
	 */
	class BALL_EXPORT TCPServer
	{
		public:
			TCPServer(Size port, bool restart = true)
				: port_(port), 
					restart_(restart),
					connected_stream_(),
					io_service_(),
					acceptor_(io_service_)
			{};

			virtual ~TCPServer();

			virtual void activate();
			virtual void deactivate();

			virtual void startAccepting();
			virtual void handleConnection();
			virtual void connectionRequested();

			void setPort(Size port);
			Size getPort() const;

		protected:
			Size port_;	
			bool restart_;

			TCPIOStream connected_stream_;

			BALL_ASIO_NAMESPACE::io_service io_service_;

			BALL_ASIO_NAMESPACE::ip::tcp::acceptor acceptor_;
	};

	/** This class provides a simple TCP Server running in its own QThread.
	 */
	class BALL_EXPORT TCPServerThread
		: public TCPServer,
			public virtual QThread
	{
		public:
			TCPServerThread(Size port, bool asynchronous = true, bool restart = true);
			
			virtual void run();
			virtual void deactivate();
			virtual void activate_async();
			virtual void handleAsyncConnection();
			virtual void handleClose();

			/** Returns the state of the server. **/
			bool isRunning();

		protected:
			bool use_async_io_;
			bool is_running_;
	};
} // namespace BALL

#endif	// BALL_SYSTEM_NETWORKING_H
