// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class RegularData3D 
{
%TypeHeaderCode
	#include <BALL/DATATYPE/regularData3D.h>
	#include <BALL/PYTHON/pyListHelper.h>
	using namespace BALL;
	using namespace BALL::Exception;
  // typedef TVector3<float> Vector3;
  // typedef typename std::vector<float>::iterator Iterator;
  // typedef typename std::vector<float>::const_iterator ConstIterator;
%End
	public:
	class IndexType
	{
%TypeHeaderCode
	#include <BALL/DATATYPE/regularData3D.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End
		public:
		Position x;
		Position y;
		Position z;
	};

	public:
	RegularData3D() throw(); 
	RegularData3D(const RegularData3D&)
		throw(OutOfMemory); 
	// sip: RegularData3D unsupported ctor argument type - provide explicit code
	// RegularData3D(const Vector3&,
	//		const Vector3&,
	//		const Vector3&)
	//	throw(OutOfMemory);
	//RegularData3D(const IndexType&, const Vector3&, const
	//		Vector3&)
	//	throw(OutOfMemory);
	~RegularData3D() throw();
	void clear() throw();
	// const RegularData3D& operator = (const RegularData3D&) 
	//	throw(OutOfMemory);
	bool operator != (const RegularData3D&) 
		const throw();
	bool operator == (const RegularData3D&) 
		const throw();
	bool empty() const throw();
	bool isInside(const Vector3&) const throw();
	// virtual void dump(std::ostream&) const throw(); 
  // ConstIterator begin() const throw();
  // ConstIterator end() const throw();
  // Iterator begin() throw();
  // Iterator end() throw();
  // size_type size() const throw();
  // size_type max_size() const throw();
  void swap(RegularData3D&);
  float getData(const IndexType&) const	throw(OutOfGrid);
	// float getData(const IndexType&)	throw(OutOfGrid);
	float getData(Position) const	throw(OutOfGrid);
	// float getData(Position)	throw(OutOfGrid);
	//	/local/anker/BALL/include/BALL/DATATYPE/regularData3D.h: In member function 
	//		 `ValueType& BALL::TRegularData3D<ValueType>::getClosestValue(const 
	//		 BALL::TVector3<float>&) [with ValueType = float]':
	//	sipBALLRegularData3D.C:512:   instantiated from here
	//	/local/anker/BALL/include/BALL/DATATYPE/regularData3D.h:963: no match for `
	//		 std::vector<float, std::allocator<float> >& [
	//		 BALL::TRegularData3D<float>::IndexType&]' operator
	//	/opt/gnu/gcc/3.2.1/include/c++/3.2.1/bits/stl_vector.h:289: candidates are: 
	//		 _Tp& std::vector<_Tp, _Alloc>::operator[](unsigned int) [with _Tp = float, 
	//		 _Alloc = std::allocator<float>]
	//	/opt/gnu/gcc/3.2.1/include/c++/3.2.1/bits/stl_vector.h:300:                 
	//		 const _Tp& std::vector<_Tp, _Alloc>::operator[](unsigned int) const [with 
	//		 _Tp = float, _Alloc = std::allocator<float>]
	// float operator [] (const IndexType&) const throw();
	// float operator [] (const IndexType&) throw();
	float operator [] (Position) const throw();
%MethodCode
	sipRes = sipCpp->operator [] (*a0);
%End
	// float operator [] (Position) throw();
	float operator () (const Vector3&) const throw();
%MethodCode
	sipRes = sipCpp->operator () (*a0);
%End

	float getInterpolatedValue(const Vector3&) const throw(OutOfGrid);
	float getClosestValue(const Vector3&) const throw(OutOfGrid);
	// float getClosestValue(const Vector3&) throw(OutOfGrid);
	IndexType getClosestIndex(const Vector3&) const	throw(OutOfGrid);
	IndexType getLowerIndex(const Vector3&) const	throw(OutOfGrid);
	const IndexType& getSize() const throw();
	const Vector3& getOrigin() const throw();
	const Vector3& getSpacing() const throw();
	void setOrigin(const Vector3&) throw();
	const Vector3& getDimension() const throw();
	void setDimension(const Vector3&) throw();
	void resize(const IndexType&) throw(OutOfMemory);
	void rescale(const IndexType&) throw(OutOfMemory);
	Vector3 getCoordinates(const IndexType&) const
		throw(OutOfGrid);
	Vector3 getCoordinates(Position) const
		throw(OutOfGrid);
	void getEnclosingValues
		(const Vector3&,
		 float, float, float, float,
		 float, float, float, float) const
		throw(OutOfGrid);
	void getEnclosingIndices
		(const Vector3&,
		 Position&, Position&, Position&, Position&,
		 Position&, Position&, Position&, Position&) const
		throw(OutOfGrid);
	void binaryWrite(const String&) const
		throw();
	void binaryRead(const String&)
		throw();
};
