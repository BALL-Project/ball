#include <OpenSimCommandExecutionThread.h>
#include <BALLViewOpenSimPlugin.h>

namespace BALL
{
	namespace VIEW
	{
		OpenSimCommandExecutionThread::OpenSimCommandExecutionThread(BALLViewOpenSimPlugin* plugin/*,Message* message*/)
			: bvcmdplugin_(plugin),
				terminate_requested_(false)
		{
		}
		
		OpenSimCommandExecutionThread::~OpenSimCommandExecutionThread()
		{
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
					
					if(task.type == OpenSimTask::NEW_COMPOSITE)
					{		
						bvcmdplugin_->handleNewComposite(task);
					}
					else if(task.type == OpenSimTask::CHANGED_COMPOSITE_HIERARCH) 
					{
						// Dont use this 
						//bvcmdplugin_->handleChangedComposite(message);
					}
					else if(task.type == OpenSimTask::REMOVED_COMPOSITE )
					{
						bvcmdplugin_->handleRemovedComposite(task);
					}
					else if(task.type == OpenSimTask::REPRESENTATION)
					{
						bvcmdplugin_->handleRepresentation(task);
					}
				
					bvcmdplugin_->pluginrwLock_.unlock();
				}
				// should we uSleep here?	
			}
		}
	}
}
