// $Id: snapShotManager.sip,v 1.1 2001/07/25 11:32:36 oliver Exp $
class SnapShotManager
{
%HeaderCode
  #include <BALL/CONCEPT/processor.h>
	#include <BALL/MOLMEC/COMMON/snapShotManager.h>
  using namespace BALL;
%End
    public:
    //struct Option
    //{
    //  static const char* FLUSH_TO_DISK_FREQUENCY;
    //};
    //struct Default
    //{
    // static const unsigned int FLUSH_TO_DISK_FREQUENCY;
    //};

    // BALL_CREATE(SnapShotManager)

    SnapShotManager()
			throw();

    SnapShotManager
			(System*, const ForceField*,
			 TrajectoryFile*, bool /overwrite/ = true)
			throw();

    SnapShotManager
			(System* /my_system/, const ForceField* /my_force_field/,
			 const Options& /my_options/, TrajectoryFile* /file/, bool /overwrite/ = true)
			throw();

    SnapShotManager(const SnapShotManager& /manager/)
			throw();

    ~SnapShotManager()
			throw();

		bool setup(System* /my_system/, const ForceField* /my_forcefield/,
								TrajectoryFile* /my_snapshot_file/)
			throw();

    virtual bool setup()
			throw();

    // const SnapShotManager& operator = (const SnapShotManager&)
		//	throw();

		virtual void clear()
			throw();

    virtual bool isValid() const
			throw();

		void setSystem(System* /my_system/)
			throw();
	
		System* getSystem() const
				throw();

		void setForceField(const ForceField* /my_ff/)
			throw();
                
		const ForceField* getForceField() const
			throw();

		void setTrajectoryFile(TrajectoryFile* /my_file/)
			throw();
                
		TrajectoryFile* getTrajectoryFile() const
			throw();

    void setFlushToDiskFrequency(Size /number/)
			throw();

    Size getFlushToDiskFrequency() const
			throw();

    virtual void takeSnapShot()
			throw();
                
		virtual bool applySnapShot(Size /number/)
			throw();
                
		virtual bool applyFirstSnapShot()
			throw();
                
		virtual bool applyNextSnapShot()
			throw();
                
		virtual bool applyLastSnapShot()
			throw();

    virtual void flushToDisk()
			throw();

    Options options;

	  // protected:
    // System* system_ptr_;
    // const ForceField* force_field_ptr_;
    // vector<SnapShot> snapshot_buffer_;
    // TrajectoryFile* trajectory_file_ptr_;
    // Size flush_to_disk_frequency_;
    // Size buffer_counter_;
    // bool valid_;
    // double calculateKineticEnergy_();
};
