#include <BALL/VIEW/KERNEL/UIOperationMode.h>

namespace BALL
{
	namespace VIEW
	{
		Mutex UIOperationMode::create_mutex_;
		boost::shared_ptr<UIOperationMode> UIOperationMode::instance_;

		UIOperationMode::UIOperationMode()
			: mode_(MODE_FULL)
		{
		}

		UIOperationMode::~UIOperationMode()
		{
		}

		UIOperationMode& UIOperationMode::instance()
		{
			if (!instance_) 
			{
				//Another thread could have taken over control right now
				//so lock a mutex to ensure the class is created only once.
				create_mutex_.lock();
				//Check that the manager has not been created by a concurring thread
				if(!instance_) 
				{
					instance_ = boost::shared_ptr<UIOperationMode>(new UIOperationMode());
				}
				create_mutex_.unlock();
			}

			return *instance_;
		}

		UIOperationMode::OperationMode UIOperationMode::getMode()
		{
			return mode_;
		}

		void UIOperationMode::setMode(OperationMode new_mode)
		{
			mode_ = new_mode;
		}

		bool UIOperationMode::isFull()
		{
			return (mode_ == MODE_FULL);
		}

		bool UIOperationMode::isAdvanced()
		{
			return (mode_ == MODE_ADVANCED);
		}

		bool UIOperationMode::isKiosk()
		{
			return (mode_ == MODE_KIOSK);
		}
	}
}
