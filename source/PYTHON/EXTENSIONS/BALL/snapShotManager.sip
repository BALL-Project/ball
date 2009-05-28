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
     static const unsigned int FLUSH_TO_DISK_FREQUENCY;
    };

    SnapShotManager() throw();
    SnapShotManager(System*, const ForceField*, TrajectoryFile*) throw();
    SnapShotManager
			(System* /my_system/, const ForceField* /my_force_field/,
			 const Options& /my_options/, TrajectoryFile* /file/)
			throw();
    SnapShotManager(const SnapShotManager& /manager/) throw();
    ~SnapShotManager() throw();

		bool setup(System* /my_system/, const ForceField* /my_forcefield/,
								TrajectoryFile* /my_snapshot_file/) throw();

    virtual bool setup() throw();

    // const SnapShotManager& operator = (const SnapShotManager&)
		//	throw();

		virtual void clear() throw();
    virtual bool isValid() const throw();

		void setSystem(System* /my_system/) throw();
		System* getSystem() const throw();
		void setForceField(const ForceField* /my_ff/) throw();
		const ForceField* getForceField() const throw();
		void setTrajectoryFile(TrajectoryFile* /my_file/) throw();
		TrajectoryFile* getTrajectoryFile() const throw();
    void setFlushToDiskFrequency(Size /number/) throw();
    Size getFlushToDiskFrequency() const throw();
    virtual void takeSnapShot() throw(File::CannotWrite);
		virtual bool applySnapShot(Size /number/) throw();
		virtual bool applyFirstSnapShot() throw();
		virtual bool applyNextSnapShot() throw();
		virtual bool applyLastSnapShot() throw();
    virtual void flushToDisk() throw(File::CannotWrite);
		Size getNumberOfSnapShotsInBuffer() throw();
		bool readFromFile() throw();
		void clearBuffer() throw();
    Options options;
};
