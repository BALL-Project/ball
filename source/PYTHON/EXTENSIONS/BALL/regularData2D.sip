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
  RegularData2D(const RegularData2D& /data/) throw(OutOfMemory);
  RegularData2D(const Vector2& /origin/, const Vector2& /dimension/, 
			const Vector2& /spacings/) 
		throw(OutOfMemory);
  RegularData2D(const IndexType& /size/, const Vector2& /origin/,
			const Vector2& /dimension/)
		throw(OutOfMemory);
  ~RegularData2D() throw();
  void clear() throw();
  // const RegularData2D& operator = (const RegularData2D& /data/)
	//	throw(OutOfMemory);
  bool operator == (const RegularData2D& /data/) const throw();
  bool operator != (const RegularData2D& /data/) const throw();
  bool empty() const throw();
  bool isInside(const Vector2& /x/) const throw();
  // ConstIterator begin() const throw();
  // ConstIterator end() const throw();
  // Iterator begin() throw();
  // Iterator end() throw();
  // size_type size() const throw();
  // size_type max_size() const throw();
  void swap(RegularData2D& /data/) throw();
  float getData(const IndexType& /index/) const throw(OutOfGrid);
  // float getData(const IndexType& /index/) throw(OutOfGrid);
  // float getData(Position /index/) const throw(OutOfGrid);
	// float getData(Position /index/) throw(OutOfGrid);
	// 	float operator [] (const IndexType& /index/) const throw();

  float operator [] (const IndexType& /index/) throw();
%MethodCode
  sipRes = sipCpp->operator [] (*a0);
%End

	//  float operator [] (Position /index/) const throw();
 	// float operator [] (Position /index/) throw();

  float operator () (const Vector2& /x/) const throw();
%MethodCode
  sipRes = sipCpp->operator () (*a0);
%End

  float getInterpolatedValue(const Vector2& /x/) const throw(OutOfGrid);
  float getClosestValue(const Vector2& /x/) const throw(OutOfGrid);
  // float getClosestValue(const Vector2& /x/) throw(OutOfGrid);
  IndexType getLowerIndex(const Vector2& /v/) const throw(OutOfGrid);
  IndexType getClosestIndex(const Vector2& /v/) const throw(OutOfGrid);
  const IndexType& getSize() const throw();
  const Vector2& getOrigin() const throw();
  const Vector2& getSpacing() const throw();
  void setOrigin(const Vector2& /origin/) throw();
  const Vector2& getDimension() const throw();
  void setDimension(const Vector2& /dimension/) throw();
  void resize(const IndexType& /new_size/) throw(OutOfMemory);
  void rescale(const IndexType& /new_size/) throw(OutOfMemory);
  Vector2 getCoordinates(const IndexType& /index/) const throw(OutOfGrid);
  Vector2 getCoordinates(Position /index/) const throw(OutOfGrid);
  void getEnclosingIndices(const Vector2& /r/, Position& /ll/,
			Position& /lr/, Position& /ul/, Position& /ur/) const throw(OutOfGrid);
  void getEnclosingValues(const Vector2& /r/, float /ll/, float /lr/,
			float /ul/, float /ur/) const throw(OutOfGrid);
  void binaryWrite(const String& /filename/) const throw();
  void binaryRead(const String& /filename/) throw();

};
