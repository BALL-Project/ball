#include <handleCommandFromBALLViewThread.h>
#include <OpenSimPlugin.h>

namespace BALL
{
	namespace VIEW
	{
		HandleCommandFromBALLViewThread::HandleCommandFromBALLViewThread(OpenSimPlugin* plugin/*,Message* message*/)
			: bvcmdplugin_(plugin),
				terminate_requested_(false)
		{
		}
		
		HandleCommandFromBALLViewThread::~HandleCommandFromBALLViewThread()
		{
			deactivate();
		}

		void HandleCommandFromBALLViewThread::deactivate()
		{
			terminate_requested_ = true;
		}
		
		void HandleCommandFromBALLViewThread::run()
		{
			terminate_requested_ = false;

			while (!terminate_requested_)
			{
				if (bvcmdplugin_->hasMessage())
				{
					bvcmdplugin_->pluginrwLock_.lockForWrite();
					OpenSimTask task = bvcmdplugin_->ballviewmessage_queue_.front();		
					bvcmdplugin_->ballviewmessage_queue_.pop();	
					
					if (task.type == OpenSimTask::NEW_COMPOSITE)
					{		
						bvcmdplugin_->sendNewComposite(task);
					}
					else if (task.type == OpenSimTask::CHANGED_COMPOSITE_HIERARCHY) 
					{
						bvcmdplugin_->sendChangedComposite(task);
					}
					else if (task.type == OpenSimTask::REMOVED_COMPOSITE)
					{
						bvcmdplugin_->sendRemovedComposite(task);
					}
					else if (task.type == OpenSimTask::REPRESENTATION)
					{
						bvcmdplugin_->sendRepresentation(task);
					}
					else
					{
						Log.info() << "HandleCommandFromBALLViewThread::run(): Unknown task type " << task.type << ". " << __FILE__ <<  " " << __LINE__<< std::endl;
					}
				
					bvcmdplugin_->pluginrwLock_.unlock();
				}
				// should we uSleep here?	
			}
		}
	}
}
