#ifndef BALL_VIEW_KERNEL_SNAPSHOT_MANAGER_INTERFACE_H
#define BALL_VIEW_KERNEL_SNAPSHOT_MANAGER_INTERFACE_H

#include <BALL/MOLMEC/COMMON/snapShotManager.h>

#include <vector>
#include <deque>

namespace BALL
{
	class String;

	namespace VIEW
	{
		class SnapshotManagerInterface
		{
			public:
				SnapshotManagerInterface();
				void setup();
				void takeSnapshot(const String& name);
				void applySnapshot(unsigned int i);

				void rename(unsigned int i, const String& name);

			protected:
				unsigned int num_snapshots_;
				std::vector<boost::shared_ptr<SnapShotManager> > managers_;
				std::deque<String> names_;
		};

	}
}

#endif //BALL_VIEW_KERNEL_SNAPSHOT_MANAGER_INTERFACE_H
