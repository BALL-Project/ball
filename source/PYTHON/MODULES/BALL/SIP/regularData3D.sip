// dummy class to emulate nested classes
class GridIndex3D 
{
%HeaderCode
	#include <BALL/DATATYPE/regularData3D.h>
	using namespace BALL;
	typedef BALL::RegularData3D<float>::GridIndex GridIndex3D;
%End
	public:
  Position x;
  Position y;
  Position z;
};

class FloatRegularData3D 
{
%HeaderCode
	#include <BALL/DATATYPE/regularData3D.h>
	#include <BALL/COMMON/exception.h>
	#include "sipBALLOutOfMemory.h"
	#include "sipBALLOutOfGrid.h"
	using namespace BALL;
	using namespace BALL::Exception;
	typedef BALL::RegularData3D<float> FloatRegularData3D;
%End
	public:
	FloatRegularData3D() throw(); 
	FloatRegularData3D(const FloatRegularData3D& /grid/)
		throw(OutOfMemory); 
	FloatRegularData3D
		(float /lower_x/, 
		 float /lower_y/, 
		 float /lower_z/,
		 float /upper_x/, 
		 float /upper_y/, 
		 float /upper_z/,
		 Size /grid_points_x/, 
		 Size /grid_points_y/, 
		 Size /grid_points_z/)
		throw(OutOfMemory);
	FloatRegularData3D
		(Vector3& /lower/, 
		 Vector3& /upper/,
		 Size /grid_points_x/, 
		 Size /grid_points_y/, 
		 Size /grid_points_z/)
		throw(OutOfMemory);
	FloatRegularData3D(const Vector3& /lower/, const Vector3& /upper/,
			float /spacing/)
		throw(OutOfMemory);
	~FloatRegularData3D() throw();
	virtual void clear() throw();
	void set(const FloatRegularData3D& /grid/) throw(OutOfMemory);
	// const FloatRegularData3D& operator = (const FloatRegularData3D& /grid/) throw(OutOfMemory);
	// virtual void dump(std::ostream& /stream/) const throw(); 
	bool isValid() const throw();
	float getMaxX() const throw();
	float getMaxY() const throw();
	float getMaxZ() const throw();
	float getMinX() const throw();
	float getMinY() const throw();
	float getMinZ() const throw();
	Size getMaxXIndex() const throw();
	Size getMaxYIndex() const throw();
	Size getMaxZIndex() const throw();
	Size getSize() const throw();
	float getXSpacing() const throw();
	float getYSpacing() const throw();
	float getZSpacing() const throw();
	GridIndex3D getIndex(const Vector3&) const throw(OutOfGrid);
	GridIndex3D getIndex(float, float, float) const
		throw(OutOfGrid);
	// float* getData(const Position /i/, const Position /j/, const Position /k/)
	//	throw(OutOfGrid);
	// float* getData(const Vector3& /r/) throw(OutOfGrid);
	// float* getData(const Position /position/) throw(OutOfGrid);
	// float& operator[](const Position /position/) throw(OutOfGrid);
	// float& operator[](const Vector3& /vector/) throw(OutOfGrid);
	Vector3 getGridCoordinates(const Position /i/, const Position /j/, const
			Position /k/) const
		throw(OutOfGrid);
	Vector3 getGridCoordinates(const Vector3& /r/) const 
		throw(OutOfGrid);
	Vector3 getGridCoordinates(const Position /position/) const 
		throw(OutOfGrid);
	void getBoxIndices
		(const Vector3& /vector/,
		 Position& /llf/, Position& /rlf/, Position& /luf/, Position& /ruf/,
		 Position& /llb/, Position& /rlb/, Position& /lub/, Position& /rub/) const
		throw(OutOfGrid);
	// void getBoxData
	//	(const Vector3& /vector/,
	//	 float& /llf/, float& /rlf/, float& /luf/, float& /ruf/,
	//	 float& /llb/, float& /rlb/, float& /lub/, float& /rub/) const
	//	throw(OutOfGrid);
	Vector3& getOrigin() throw();
	const Vector3& getOrigin() const throw();
	void setOrigin(const Vector3& /origin/) throw();
	void setOrigin(float /x/, float /y/, float /z/) throw();
	Vector3& getDimension() throw();
	const Vector3& getDimension() const throw();
	bool has(const Vector3& /vector/) const throw();
	// bool has(const float& /x/, const float& /y/, const float& /z/) const throw();
	float getInterpolatedValue(const Vector3& /vector/) const 
		throw(OutOfGrid);
	bool operator == (const FloatRegularData3D& /grid/) const throw();
	// bool operator != (const FloatRegularData3D& grid) const throw();
};
