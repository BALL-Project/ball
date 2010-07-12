#include <OpenSimCommandExecutionThread.h>
#include <OpenSimPlugin.h>

namespace BALL
{
	namespace VIEW
	{
		OpenSimCommandExecutionThread::OpenSimCommandExecutionThread(OpenSimPlugin* plugin/*,Message* message*/)
			: bvcmdplugin_(plugin),
				terminate_requested_(false)
		{
		}
		
		OpenSimCommandExecutionThread::~OpenSimCommandExecutionThread()
		{
			deactivate();
		}

		void OpenSimCommandExecutionThread::deactivate()
		{
			terminate_requested_ = true;
		}
		
		void OpenSimCommandExecutionThread::run()
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
						bvcmdplugin_->handleNewComposite(task);
					}
					else if (task.type == OpenSimTask::CHANGED_COMPOSITE_HIERARCHY) 
					{
						bvcmdplugin_->handleChangedComposite(task);
					}
					else if (task.type == OpenSimTask::REMOVED_COMPOSITE)
					{
						bvcmdplugin_->handleRemovedComposite(task);
					}
					else if (task.type == OpenSimTask::REPRESENTATION)
					{
						bvcmdplugin_->handleRepresentation(task);
					}
					else
					{
						Log.info() << "OpenSimCommandExecutionThread::run(): Unknown task type " << task.type << ". " << __FILE__ <<  " " << __LINE__<< std::endl;
					}
				
					bvcmdplugin_->pluginrwLock_.unlock();
				}
				// should we uSleep here?	
			}
		}
	}
}
