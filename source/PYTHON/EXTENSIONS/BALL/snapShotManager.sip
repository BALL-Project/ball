// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class SnapShotManager
{
%TypeHeaderCode
  #include <BALL/CONCEPT/processor.h>
	#include <BALL/MOLMEC/COMMON/snapShotManager.h>
  #include <BALL/SYSTEM/file.h>
  using namespace BALL;
%End
    public:
    struct Option
    {
      static const char* FLUSH_TO_DISK_FREQUENCY;
    };
    struct Default
    {
     static const Size FLUSH_TO_DISK_FREQUENCY;
    };

    SnapShotManager();
    SnapShotManager(System*, const ForceField*, TrajectoryFile*);
    SnapShotManager(System*, TrajectoryFile* file = 0);
    SnapShotManager(System*, const ForceField*, const Options&, TrajectoryFile*);
    SnapShotManager(const SnapShotManager& /manager/);
    ~SnapShotManager();

		bool setup(System*, TrajectoryFile*);
		bool setup(System*, const ForceField*, TrajectoryFile*);

    virtual bool setup();

		virtual void clear();
    virtual bool isValid() const;

		void setSystem(System* /my_system/);
		System* getSystem() const;
		void setForceField(const ForceField* /my_ff/);
		const ForceField* getForceField() const;
		void setTrajectoryFile(TrajectoryFile* /my_file/);
		TrajectoryFile* getTrajectoryFile() const;
    void setFlushToDiskFrequency(Size /number/);
    Size getFlushToDiskFrequency() const;
    virtual void takeSnapShot() throw(File::CannotWrite);
		virtual bool applySnapShot(Size /number/);
		virtual bool applyFirstSnapShot();
		virtual bool applyNextSnapShot();
		virtual bool applyLastSnapShot();
    virtual void flushToDisk() throw(File::CannotWrite);
		Size getNumberOfSnapShotsInBuffer();
		bool readFromFile();
		void clearBuffer();
    Options options;
};
