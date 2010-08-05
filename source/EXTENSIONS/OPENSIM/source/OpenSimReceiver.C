#include <OpenSimReceiver.h>
#include <OpenSimPlugin.h>

namespace BALL
{
	namespace VIEW
	{
		OpenSimReceiver::OpenSimReceiver(Size port, OpenSimPlugin* plugin)
			: TCPServerThread(port),
			  is_acknowledged_(false),
			  is_Process_Done_(true),
			  rwLock_(),
			  plugin_(plugin)
		{
		}
		
		OpenSimReceiver::~OpenSimReceiver()
		{
			if (is_running_)
				deactivate();
		}

		void OpenSimReceiver::run()
		{
			funcThread_ = new HandleCommandToBALLViewThread(plugin_);
			funcThread_->start();

			TCPServerThread::run();	
		}

		void OpenSimReceiver::deactivate()
		{
			TCPServerThread::deactivate();

			if (funcThread_)
			{
				funcThread_->deactivate();
				funcThread_->wait();

				delete(funcThread_);
				funcThread_ = 0;
			}
		}

		void OpenSimReceiver::handleAsyncConnection()
		{
			// here I can check the status of the opensim client and set (server_->is_acknowledged to true ??
			// this should be done only once (first time connection is made)
			while (connected_stream_.good() && isRunning())
			{
				String buffer;
				buffer.getline(connected_stream_);

				if (!buffer.isEmpty())
				{
					std::vector<String> split;
					buffer.split(split, ";");

					if (split.size() == 0)
						continue;

					Size command_index(split[0].trim().toInt());

					if (command_index == ACKNOWLEDGEMENT)
					{
						if (split.size() != 2)
							{
								Log.error() << "Damnit! This is not an acknowledgement!";
								break;
							}
						
						is_acknowledged_ = true;
					}
					else 
					{
						//ADD_ATOM, ADD_BOND, REMOVE_ATOM, REMOVE_BOND, 
						//SATURATE_FULL_WITH_HYDROGENS
						//RUN_MINIMIZATION,CHANGE_FORCE_FIELD,ADD_MOLECULE

						rwLock_.lockForWrite();
						incomingmessage_queue_.push(split);
						rwLock_.unlock();
					}
				}
				else
				{
					Log.error() << "Damnit! An empty buffer recieved!" << std::endl;
					break;
				}
			}
		}
		
		void OpenSimReceiver::checkClientStatus()
		{
			// TODO:
			// first need to check if connection exist?
			// Not sure how to do that
			
			TCPIOStream outstream(plugin_->getRemoteHost(), plugin_->getRemotePort());

			if (outstream.good())
			{
				outstream << CHECK_STATUS  << std::endl;
			}
		}

		void OpenSimReceiver::sendMessageString(const String& to_send)
		{
			TCPIOStream outstream(plugin_->getRemoteHost(), plugin_->getRemotePort());

			if (outstream.good())
			{
				outstream << to_send << std::endl;
				outstream.flush();
			}
		}

		bool OpenSimReceiver::hasMessage()
		{
			bool result;

			rwLock_.lockForRead();
			result = !(incomingmessage_queue_.empty());
			rwLock_.unlock();

			return result;
		}
	}
}
