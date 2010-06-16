#include <BALL/VIEW/KERNEL/snapshotManagerInterface.h>

#include <BALL/KERNEL/system.h>
#include <BALL/VIEW/KERNEL/mainControl.h>

namespace BALL
{
	namespace VIEW
	{

		SnapshotManagerInterface::SnapshotManagerInterface()
		{
			setup();
		}

		void SnapshotManagerInterface::setup()
		{
			typedef HashSet<Composite*> Comps;

			num_snapshots_ = 0;
			managers_.clear();

			if(!MainControl::getInstance(0))
			{
				return;
			}

			Comps& comps = MainControl::getInstance(0)->getCompositeManager().getComposites();

			managers_.resize(comps.size());

			int i = 0;
			for(Comps::Iterator it = comps.begin(); it != comps.end(); ++i, ++it)
			{
				managers_[i] = boost::shared_ptr<SnapShotManager>(new SnapShotManager(reinterpret_cast<System*>(*it), 0, 0));
			}
		}

		void SnapshotManagerInterface::takeSnapshot(const String& name)
		{
			++num_snapshots_;

			for(size_t i = 0; i < managers_.size(); ++i)
			{
				managers_[i]->takeSnapShot();
			}

			names_.push_back(name);
		}

		void SnapshotManagerInterface::applySnapshot(unsigned int index)
		{
			if(managers_.empty())
			{
				return;
			}

			if(index >= num_snapshots_)
			{
				return;
			}

			MainControl* mc = MainControl::getInstance(0);
			for(size_t i = 0; i < managers_.size(); ++i)
			{
				managers_[i]->applySnapShot(index);
				mc->updateRepresentationsOf(*managers_[i]->getSystem());
			}

		}

		void SnapshotManagerInterface::rename(unsigned int i, const String& name)
		{
			if(i < names_.size())
			{
				names_[i] = name;
			}
		}

	}
}
