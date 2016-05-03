// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <boost/bind.hpp>
#include <BALL/SYSTEM/networking.h>

namespace BALL
{
	TCPServer::~TCPServer()
	{
	}

	void TCPServer::activate()
	{
		acceptor_.open(boost::asio::ip::tcp::v4());
		acceptor_.bind(boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port_));

		if (port_ == 0)
			port_ = acceptor_.local_endpoint().port();

		acceptor_.listen();
		startAccepting();

		// when the first accept returned, we have to decide whether
		// to keep accepting connections
		while (restart_)
			startAccepting();
	}

	void TCPServer::deactivate()
	{
		acceptor_.close();
	}

	void TCPServer::startAccepting()
	{
		connected_stream_.close();
		acceptor_.accept(*connected_stream_.rdbuf());

		handleConnection();
	}

	void TCPServer::handleConnection()
	{
	}

	// not needed as long as we do this synchronously
	void TCPServer::connectionRequested()
	{
		if (restart_)
			startAccepting();
	}

	void TCPServer::setPort(Size port)
	{
		port_ = port;
	}

	Size TCPServer::getPort() const
	{
		return port_;
	}

	TCPServerThread::TCPServerThread(Size port, bool asynchronous, bool restart)
		: QThread(),
			TCPServer(port, restart),
			use_async_io_(asynchronous),
			is_running_(false)
	{
	}

	void TCPServerThread::run()
	{
		if (!use_async_io_)
			activate();
		else
		{
			try 
			{
				acceptor_.open(boost::asio::ip::tcp::v4());
				acceptor_.bind(boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port_));

				if (port_ == 0)
					port_ = acceptor_.local_endpoint().port();

				acceptor_.listen();

				is_running_ = true;

				activate_async();

				while (restart_ && is_running_)
				{
					activate_async();
				}
			} 
			catch (std::exception& e)
			{
				Log.error() << "Could not start server!" << std::endl;
				Log.error() << "Reason given was " << e.what() << std::endl;

				is_running_ = false;
			}
		}
	}

	void TCPServerThread::deactivate()
	{
		is_running_ = false;
		io_service_.post(boost::bind(&TCPServerThread::handleClose, this));
	}

	void TCPServerThread::handleClose()
	{
		acceptor_.close();
		connected_stream_.close();
		io_service_.stop();
	}

	void TCPServerThread::activate_async()
	{
		connected_stream_.close();

		acceptor_.async_accept(*connected_stream_.rdbuf(),
			boost::bind(&TCPServerThread::handleAsyncConnection, this));

		io_service_.run();
		io_service_.reset();
	}

	void TCPServerThread::handleAsyncConnection()
	{
	}

	bool TCPServerThread::isRunning()
	{
		return is_running_;
	}
}


