#include <OpenSimWorkerThread.h>
#include <BALLViewOpenSimPlugin.h>

namespace BALL
{
	namespace VIEW
	{
		OpenSimWorkerThread::OpenSimWorkerThread(BALLViewOpenSimPlugin* plugin)
			: bvplugin_(plugin),
				terminate_requested_(false)
		{
		}
		
		OpenSimWorkerThread::~OpenSimWorkerThread()
		{
		}

		void OpenSimWorkerThread::deactivate()
		{
			terminate_requested_ = true;
		}
		
		void OpenSimWorkerThread::run()
		{
			terminate_requested_ = false;

			while (!terminate_requested_)
			{
				if (bvplugin_->server_->hasMessage()) 
				{
					std::vector<String> data;

					bvplugin_->server_->rwLock_.lockForWrite();
					data = 	bvplugin_->server_->incomingmessage_queue_.front();		
					bvplugin_->server_->incomingmessage_queue_.pop();
					bvplugin_->server_->rwLock_.unlock();

					bvplugin_->handleMolecularModeling(data);
				}
			}
				 
		}
	
	}
}
