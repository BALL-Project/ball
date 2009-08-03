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
	RegularData3D(const RegularData3D& /grid/)
		throw(OutOfMemory); 
	// sip: RegularData3D unsupported ctor argument type - provide explicit code
	// RegularData3D(const Vector3& /origin/,
	//		const Vector3& /dimension/,
	//		const Vector3& /spacing/)
	//	throw(OutOfMemory);
	//RegularData3D(const IndexType& /size/, const Vector3& /origin/, const
	//		Vector3& /dimension/)
	//	throw(OutOfMemory);
	~RegularData3D() throw();
	void clear() throw();
	// const RegularData3D& operator = (const RegularData3D& /data/) 
	//	throw(OutOfMemory);
	bool operator != (const RegularData3D& /grid/) 
		const throw();
	bool operator == (const RegularData3D& /grid/) 
		const throw();
	bool empty() const throw();
	bool isInside(const Vector3& /r/) const throw();
	// virtual void dump(std::ostream& /stream/) const throw(); 
  // ConstIterator begin() const throw();
  // ConstIterator end() const throw();
  // Iterator begin() throw();
  // Iterator end() throw();
  // size_type size() const throw();
  // size_type max_size() const throw();
  void swap(RegularData3D& /grid/);
  float getData(const IndexType& /index/) const	throw(OutOfGrid);
	// float getData(const IndexType& /index/)	throw(OutOfGrid);
	float getData(Position /index/) const	throw(OutOfGrid);
	// float getData(Position /index/)	throw(OutOfGrid);
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
	// float operator [] (const IndexType& /index/) const throw();
	// float operator [] (const IndexType& /index/) throw();
	float operator [] (Position /index/) const throw();
%MethodCode
	sipRes = sipCpp->operator [] (*a0);
%End
	// float operator [] (Position /index/) throw();
	float operator () (const Vector3& /x/) const throw();
%MethodCode
	sipRes = sipCpp->operator () (*a0);
%End

	float getInterpolatedValue(const Vector3& /x/) const throw(OutOfGrid);
	float getClosestValue(const Vector3& /x/) const throw(OutOfGrid);
	// float getClosestValue(const Vector3& /x/) throw(OutOfGrid);
	IndexType getClosestIndex(const Vector3& /v/) const	throw(OutOfGrid);
	IndexType getLowerIndex(const Vector3& /v/) const	throw(OutOfGrid);
	const IndexType& getSize() const throw();
	const Vector3& getOrigin() const throw();
	const Vector3& getSpacing() const throw();
	void setOrigin(const Vector3& /origin/) throw();
	const Vector3& getDimension() const throw();
	void setDimension(const Vector3& /dimension/) throw();
	void resize(const IndexType& /size/) throw(OutOfMemory);
	void rescale(const IndexType& /new_size/) throw(OutOfMemory);
	Vector3 getCoordinates(const IndexType& /index/) const
		throw(OutOfGrid);
	Vector3 getCoordinates(Position /index/) const
		throw(OutOfGrid);
	void getEnclosingValues
		(const Vector3& /r/,
		 float /llf/, float /rlf/, float /luf/, float /ruf/,
		 float /llb/, float /rlb/, float /lub/, float /rub/) const
		throw(OutOfGrid);
	void getEnclosingIndices
		(const Vector3& /r/,
		 Position& /llf/, Position& /rlf/, Position& /luf/, Position& /ruf/,
		 Position& /llb/, Position& /rlb/, Position& /lub/, Position& /rub/) const
		throw(OutOfGrid);
	void binaryWrite(const String& /filename/) const
		throw();
	void binaryRead(const String& /filename/)
		throw();
};
