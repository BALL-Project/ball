// $Id: regularData2D.sip,v 1.3 2001/07/29 17:31:17 oliver Exp $

// dummy class to emulate nested classes
class GridIndex2D
{
%HeaderCode
  #include <BALL/DATATYPE/regularData2D.h>
  using namespace BALL;
  typedef BALL::RegularData2D<float>::GridIndex GridIndex2D;
%End
  public:
  Position x;
  Position y;
};

typedef float GridDataType;

class FloatRegularData2D
{
%HeaderCode
	#include <BALL/DATATYPE/regularData2D.h>
	#include "sipBALLOutOfMemory.h"
	#include "sipBALLOutOfGrid.h"
	typedef BALL::RegularData2D<float> FloatRegularData2D;
	typedef BALL::RegularData2D<float>::GridIndex GridIndex2D;
	using namespace BALL::Exception;
%End
	public:
	// BALL_CREATE_DEEP(FloatRegularData2D)
	// typedef TVector2<Position> GridIndex2D;

	FloatRegularData2D() throw();
	FloatRegularData2D(const FloatRegularData2D& /grid/)
					throw(OutOfMemory);

	FloatRegularData2D(float /lower_x/, float /lower_y/,
									float /upper_x/, float /upper_y/,
									Size /grid_points_x/, Size /grid_points_y/)
					 throw(OutOfMemory);

	FloatRegularData2D
					(const Vector2& /lower/, const Vector2& /upper/,
					 Size /grid_points_x/, Size /grid_points_y/)
					 throw(OutOfMemory);
	FloatRegularData2D(const Vector2& /lower/, const Vector2& /upper/, float /spacing/)
					throw(OutOfMemory);

	~FloatRegularData2D() throw();
	virtual void clear() throw();

	void set(const FloatRegularData2D& /grid/) 
		throw(OutOfMemory);
	//const FloatRegularData2D& operator = (const FloatRegularData2D& /grid/) 
	//	throw(OutOfMemory);

	//virtual void dump(std::ostream& stream) const throw();
	bool isValid() const throw();
	float getMaxX() const throw();
	float getMaxY() const throw();
	float getMinX() const throw();
	float getMinY() const throw();
	Size getMaxXIndex() const throw();
	Size getMaxYIndex() const throw();
	Size getSize() const throw();
	float getXSpacing() const throw();
	float getYSpacing() const throw();
	GridIndex2D getIndex(const Vector2& /v/) const 
		throw(OutOfGrid);
	GridIndex2D getIndex(float /x/, float /y/) const
		throw(OutOfGrid);
	//GridDataType getData(const Position /i/, const Position /j/)
	//	throw(OutOfGrid);
	//GridDataType getData(const Vector2& /r/) 
	//	throw(OutOfGrid);
	//GridDataType getData(const Position /position/) 
	//	throw(OutOfGrid);
	//GridDataType operator [] (const Position /position/) 
	//	throw(OutOfGrid);

	GridDataType operator [] (const Vector2& /vector/) 
		throw(OutOfGrid);
	Vector2 getGridCoordinates(const Position /i/, const Position /j/) const
		throw(OutOfGrid);
	Vector2 getGridCoordinates(const Position /position/) const 
		throw(OutOfGrid);

	//void getRectangleIndices
	//				(const Vector2& /vector/, Position& /ll/, Position& /lr/, Position& /ul/, Position& /ur/) const
	//				throw(OutOfGrid);

	//void getRectangleData (const Vector2& /vector/,
	//				GridDataType& /ll/, GridDataType& /lr/, GridDataType& /ul/, GridDataType& /ur/) const
	//				throw(OutOfGrid);
	Vector2& getOrigin() throw();
	const Vector2& getOrigin() const throw();
	void setOrigin(const Vector2& /origin/) throw();
	void setOrigin(float /x/, float /y/) throw();
	Vector2& getDimension() throw();
	const Vector2& getDimension() const throw();
	bool has(const Vector2& /vector/) const throw();
	bool has(float /x/, float /y/) const throw();

	GridDataType getInterpolatedValue(const Vector2& /vector/) const 
		throw(OutOfGrid);
	GridDataType getMaxValue() const throw();
	GridDataType getMinValue() const throw();
	void rescale(float /minValue/, float /maxValue/) throw();
	bool operator == (const FloatRegularData2D& /grid/) const throw();
	//bool operator != (const FloatRegularData2D& /grid/) const throw();
	//float* data;
};
