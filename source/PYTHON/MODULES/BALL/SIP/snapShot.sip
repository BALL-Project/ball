// $Id: snapShot.sip,v 1.2 2000/07/24 22:16:02 oliver Exp $

// class SnapShot
// {
// 	public:
//   SnapShot(); 
// 	SnapShot(int); 
// 	SnapShot(const SnapShot&, bool); 
// 	// ~SnapShot(); 
// 	// friend std::fstream& operator<<(std::fstream& os, const SnapShot&  snap_shot); 
//  // friend std::fstream& operator>>(std::fstream& in, SnapShot& snap_shot);
// 	// SnapShot &operator=(const SnapShot &rhs); 
// 	bool isValid();
// 
// 	bool valid;   
// 	int index; 
// 	int total_length; 
// 	int data_length; 
// 	int no_of_atoms; 
// 	double potential_energy; 
// 	double kinetic_energy; 
// 	//vector<Vector3> position;
// 	//vector<Vector3> velocity;
// 	//vector<Vector3> force;
// };

class SnapShotManager
{
%HeaderCode
	#include <BALL/MOLMEC/COMMON/snapShot.h>
	using namespace BALL;
%End
  public:
	SnapShotManager(); 
	SnapShotManager(System& /system/, ForceField& /forcefield/, const String& /filename/, bool /overwrite/ = false); 
	SnapShotManager(System& /system/, ForceField& /forcefield/, Options& /options/, const String& /filename/, bool /overwrite/ = true);
	SnapShotManager(const SnapShotManager&, bool);
	~SnapShotManager(); 
	bool setup( System&, ForceField&, Options&, const String&, bool);
	// SnapShotManager &operator=(const SnapShotManager &rhs);
	bool isValid() const;
	void setFlushToDiskFrequency(int); 
	int getFlushToDiskFrequency() const; 
	void takeSnapShot(); 
	void flushToDisk(); 
	void close(); 
	int getNumberOfSnapShots() const;
	// SnapShot getSnapShot(int); 
	System getSnapShotAsSystem(int); 
	Options options; 
};
