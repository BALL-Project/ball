// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

typedef float GridDataType;
class RegularData2D
{
%TypeHeaderCode
	#include <BALL/DATATYPE/regularData2D.h>
	using namespace BALL;
	using namespace BALL::Exception;
	// typedef TVector2<float> Vector2;
%End
	public:
	class IndexType
	{
%TypeHeaderCode
	#include <BALL/DATATYPE/regularData2D.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End
		public:
		Position x;
		Position y;
	};

	public:

	// BALL_CREATE_DEEP(RegularData2D)
  RegularData2D() throw();
  RegularData2D(const RegularData2D&) throw(OutOfMemory);
  RegularData2D(const Vector2&, const Vector2&, 
			const Vector2&) 
		throw(OutOfMemory);
  RegularData2D(const IndexType&, const Vector2&,
			const Vector2&)
		throw(OutOfMemory);
  ~RegularData2D() throw();
  void clear() throw();
  // const RegularData2D& operator = (const RegularData2D&)
	//	throw(OutOfMemory);
  bool operator == (const RegularData2D&) const throw();
  bool operator != (const RegularData2D&) const throw();
  bool empty() const throw();
  bool isInside(const Vector2&) const throw();
  // ConstIterator begin() const throw();
  // ConstIterator end() const throw();
  // Iterator begin() throw();
  // Iterator end() throw();
  // size_type size() const throw();
  // size_type max_size() const throw();
  void swap(RegularData2D&) throw();
  float getData(const IndexType&) const throw(OutOfGrid);
  // float getData(const IndexType&) throw(OutOfGrid);
  // float getData(Position) const throw(OutOfGrid);
	// float getData(Position) throw(OutOfGrid);
	// 	float operator [] (const IndexType&) const throw();

  float operator [] (const IndexType&) throw();
%MethodCode
  sipRes = sipCpp->operator [] (*a0);
%End

	//  float operator [] (Position) const throw();
 	// float operator [] (Position) throw();

  float operator () (const Vector2&) const throw();
%MethodCode
  sipRes = sipCpp->operator () (*a0);
%End

  float getInterpolatedValue(const Vector2&) const throw(OutOfGrid);
  float getClosestValue(const Vector2&) const throw(OutOfGrid);
  // float getClosestValue(const Vector2&) throw(OutOfGrid);
  IndexType getLowerIndex(const Vector2&) const throw(OutOfGrid);
  IndexType getClosestIndex(const Vector2&) const throw(OutOfGrid);
  const IndexType& getSize() const throw();
  const Vector2& getOrigin() const throw();
  const Vector2& getSpacing() const throw();
  void setOrigin(const Vector2&) throw();
  const Vector2& getDimension() const throw();
  void setDimension(const Vector2&) throw();
  void resize(const IndexType&) throw(OutOfMemory);
  void rescale(const IndexType&) throw(OutOfMemory);
  Vector2 getCoordinates(const IndexType&) const throw(OutOfGrid);
  Vector2 getCoordinates(Position) const throw(OutOfGrid);
  void getEnclosingIndices(const Vector2&, Position&,
			Position&, Position&, Position&) const throw(OutOfGrid);
  void getEnclosingValues(const Vector2&, float, float,
			float, float) const throw(OutOfGrid);
  void binaryWrite(const String&) const throw();
  void binaryRead(const String&) throw();

};
