// $Id: snapShot.sip,v 1.3 2001/01/16 17:31:51 anker Exp $

class SnapShot
{
%HeaderCode
	#include <BALL/MOLMEC/COMMON/snapShot.h>
	using namespace BALL;
%End
	public:
	SnapShot();
	SnapShot(const SnapShot& /snapshot/);
	SnapShot(Size /no_of_atoms/);
	// virtual ~SnapShot();
	// const SnapShot& operator = (const SnapShot& /snapshot/);
	virtual void clear();
	// bool operator == (const SnapShot& /snapshot/) const;
	bool isValid() const;
	void setIndex(Size /index/);
	Size getIndex() const;
	void setNumberOfAtoms(Size /number_of_atoms/);
	Size getNumberOfAtoms() const;
	Size getDataLength() const;
	Size getTotalLength() const;
	double getPotentialEnergy() const;
	double getKineticEnergy() const;
	// void setAtomPositions(const std::vector<Vector3>& /atom_postions/);
	// const std::vector<Vector3>& getAtomPositions() const;
	// void setAtomVelocitites(const std::vector<Vector3>& /atom_velocities/);
	// const std::vector<Vector3>& getAtomVelocities() const;
	// void setAtomForces(const std::vector<Vector3>& /atom_forces/);
	// const std::vector<Vector3>& getAtomForces() const;
};

class SnapShotManager
{
%HeaderCode
	#include <BALL/MOLMEC/COMMON/snapShot.h>
	using namespace BALL;
%End
	public:
	SnapShotManager();
	SnapShotManager(const System* /my_system/,
			const ForceField* /my_force_field/, TrajectoryFile* /my_snapshot_file/,
			bool /overwrite/ = true);
	SnapShotManager(const System* /my_system/,
			const ForceField* /my_force_field/, const Options& /my_options/,
			TrajectoryFile* /file/, bool /overwrite/ = true);
	SnapShotManager(const SnapShotManager& /manager/);
	// virtual ~SnapShotManager();
	virtual bool setup();
	// const SnapShotManager& operator = (const SnapShotManager& /manager/);
	virtual void clear();
	virtual bool isValid() const;
	void setFlushToDiskFrequency(Size /number/);
	Size getFlushToDiskFrequency() const;
	virtual void takeSnapShot();
	virtual void flushToDisk();
	Size getNumberOfSnapShots() const;
	Size getNumberOfAtoms() const;
};
