class SnapShot
{
%HeaderCode
#include <BALL/MOLMEC/COMMON/snapShot.h>
using namespace BALL;
%End
	public:
  SnapShot(); 
  SnapShot(int); 
  SnapShot(SnapShot&, bool); 

  //  friend std::fstream& operator<<(std::fstream& os, SnapShot&  snap_shot); 
  //  friend std::fstream& operator>>(std::fstream& in, SnapShot& snap_shot);
  //  SnapShot &operator=(SnapShot &rhs); 
  bool isValid();
  bool valid;   
  int index; 
  int total_length; 
  int data_length; 
  int no_of_atoms; 
  double potential_energy; 
  double kinetic_energy; 
  // vector<Vector3> position;
  // vector<Vector3> velocity;
  // vector<Vector3> force;
};
  
class SnapShotManager
{
%HeaderCode
#include <BALL/MOLMEC/COMMON/snapShot.h>
using namespace BALL;
%End
 public:
 SnapShotManager(); 
 SnapShotManager(System&, ForceField&, char*); 
 SnapShotManager(System&, ForceField&, char*, Options&);
 SnapShotManager(SnapShotManager&, bool);

 virtual bool setup(System&, ForceField&, char*, Options&);
 // SnapShotManager &operator=(SnapShotManager &rhs);
 virtual bool isValid() const;
 virtual void takeSnapShot();
 virtual void flushToDisk(); 
 virtual void close(); 
 int getNumberOfSnapShots() const;
 virtual SnapShot getSnapShot(int); 
 virtual System getSnapShotAsSystem(int); 

 Options options; 
};   
