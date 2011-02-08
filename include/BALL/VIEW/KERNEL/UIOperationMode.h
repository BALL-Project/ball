// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_KERNEL_UIOPERATIONMODE_H
#define BALL_VIEW_KERNEL_UIOPERATIONMODE_H

#ifndef BALL_CONCEPT_EMBEDDABLE_H
# include <BALL/COMMON/global.h>
#endif

#ifndef BALL_SYSTEM_MUTEX_H
# include <BALL/SYSTEM/mutex.h>
#endif

#include <boost/shared_ptr.hpp>

namespace BALL
{
	namespace VIEW
	{
		/**
		 * A singleton that manages the operation mode of our user interface.
		 */
		class BALL_VIEW_EXPORT UIOperationMode
		{
			public:
				
				enum OperationMode
				{
					MODE_FULL = 0,
					MODE_ADVANCED,
					MODE_KIOSK,
					MODE_ALL
				};

				virtual ~UIOperationMode();

				static UIOperationMode& instance();

				OperationMode getMode();
				void setMode(OperationMode new_mode);

				bool isFull();
				bool isAdvanced();
				bool isKiosk();

			protected:
				// prevent outside use of constructors
				UIOperationMode();
				UIOperationMode(const UIOperationMode&);
				const UIOperationMode& operator=(const UIOperationMode&);

				// our instance pointer
				static boost::shared_ptr<UIOperationMode> instance_;

				static Mutex create_mutex_;

				OperationMode mode_;
		};
	}
}

#endif // BALL_VIEW_KERNEL_UIOPERATIONMODE_H
