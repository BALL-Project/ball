// $Id: pointGrid.sip,v 1.1 2000/07/18 17:02:38 oliver Exp $

// dummy class to emulate nested classes
class GridIndex 
{
%HeaderCode
	#include <BALL/DATATYPE/pointGrid.h>
	using namespace BALL;
	typedef PointGrid<float>::GridIndex GridIndex;
%End
	public:
  Position x;
  Position y;
  Position z;
};

class FloatPointGrid 
{
%HeaderCode
	#include <BALL/DATATYPE/pointGrid.h>
	typedef BALL::PointGrid<float> FloatPointGrid;
%End
  public:
  // BALL_CREATE(FloatPointGrid)
  FloatPointGrid(); 
  FloatPointGrid(const FloatPointGrid& /grid/, bool /deep/ = true); 
  FloatPointGrid
   (float /lower_x/, float /lower_y/, float /lower_z/,
    float /upper_x/, float /upper_y/, float /upper_z/,
    Size /grid_points_x/, Size /grid_points_y/, Size /grid_points_z/);
  FloatPointGrid
   (const Vector3& /lower/, 
    const Vector3& /upper/,
    Size /grid_points_x/, Size /grid_points_y/, Size /grid_points_z/);
  FloatPointGrid
		(const Vector3& /lower/, const Vector3& /upper/, float /spacing/);
  ~FloatPointGrid();
  void set(const FloatPointGrid& /grid/);
  // const FloatPointGrid& operator = (const FloatPointGrid& grid);
  // virtual void dump(std::ostream& stream) const; 
  bool isValid() const;
  float getMaxX() const;
  float getMaxY() const;
  float getMaxZ() const;
  float getMinX() const;
  float getMinY() const;
  float getMinZ() const;
  Size getMaxXIndex() const;
  Size getMaxYIndex() const;
  Size getMaxZIndex() const;
  Size getSize() const;
  float getXSpacing() const;
  float getYSpacing() const;
  float getZSpacing() const;
  GridIndex getIndex(const Vector3&) const;
  GridIndex getIndex(float, float, float) const;
  // float getData(const Position /i/, const Position /j/, const Position /k/);
  // float getData(const Vector3& /r/);
  // float getData(const Position /position/);
  float operator[](const Position /position/);
  float operator[](const Vector3& /vector/);
  Vector3 getGridCoordinates(const Position /i/, const Position /j/, const Position /k/) const;
  Vector3 getGridCoordinates(const Vector3& /r/) const;
  Vector3 getGridCoordinates(const Position /position/) const;
  void getBoxIndices
   (const Vector3& /vector/,
   Position& /llf/, Position& /rlf/, Position& /luf/, Position& /ruf/,
   Position& /llb/, Position& /rlb/, Position& /lub/, Position& /rub/) const;
  // void getBoxData    // immutable!
  // (const Vector3& /vector/,
  // 	GridDataType& /llf/, GridDataType& /rlf/, GridDataType& /luf/, GridDataType& /ruf/,
	//  GridDataType& /llb/, GridDataType& /rlb/, GridDataType& /lub/, GridDataType& /rub/) const;
  Vector3& getOrigin();
  const Vector3& getOrigin() const;
  void setOrigin(const Vector3& /origin/);
  void setOrigin(float /x/, float /y/, float /z/);
  Vector3& getDimension();
  const Vector3& getDimension() const;
  bool has(const Vector3& /vector/) const;
  bool has(float /x/, float /y/,float /z/) const;
 	float getInterpolatedValue(const Vector3& /vector/) const;
  // GridDataType* data;

	// __repr__ of FloatPointGrid
	operator const char* ();
%MemberCode
  FloatPointGrid* ptr;
  if ((ptr = (FloatPointGrid*)sipGetCppPtr(sipThis,sipClass_FloatPointGrid)) == NULL)
    return NULL;

  return PyString_FromString(String(String("FloatPointGrid ") 
        + " { " + String(ptr->getMaxXIndex()) + "x"
        + String(ptr->getMaxYIndex()) + "x" + String(ptr->getMaxZIndex()) 
				+ " }").c_str());
%End
};
