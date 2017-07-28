class RegularData3D
{
%TypeHeaderCode
	#include <BALL/DATATYPE/regularData3D.h>
	#include <BALL/PYTHON/pyListHelper.h>
%End
	public:

	class IndexType
	{
		public:
		Position x;
		Position y;
		Position z;
	};

	RegularData3D() throw();
	RegularData3D(const RegularData3D&) throw(OutOfMemory);
	~RegularData3D() throw();
	void clear() throw();
	bool operator != (const RegularData3D&) const throw();
	bool operator == (const RegularData3D&) const throw();
	bool empty() const throw();
	bool isInside(const Vector3&) const throw();
	void swap(RegularData3D&);
	float getData(const IndexType&) const throw(OutOfGrid);
	float getData(Position) const throw(OutOfGrid);

	float operator [] (Position) const throw();
%MethodCode
	sipRes = sipCpp->operator [] (*a0);
%End

	float operator () (const Vector3&) const throw();
%MethodCode
	sipRes = sipCpp->operator () (*a0);
%End

	float getInterpolatedValue(const Vector3&) const throw(OutOfGrid);
	float getClosestValue(const Vector3&) const throw(OutOfGrid);
	IndexType getClosestIndex(const Vector3&) const throw(OutOfGrid);
	IndexType getLowerIndex(const Vector3&) const throw(OutOfGrid);
	const IndexType& getSize() const throw();
	const Vector3& getOrigin() const throw();
	const Vector3& getSpacing() const throw();
	void setOrigin(const Vector3&) throw();
	const Vector3& getDimension() const throw();
	void setDimension(const Vector3&) throw();
	void resize(const IndexType&) throw(OutOfMemory);
	void rescale(const IndexType&) throw(OutOfMemory);
	Vector3 getCoordinates(const IndexType&) const throw(OutOfGrid);
	Vector3 getCoordinates(Position) const throw(OutOfGrid);
	void getEnclosingValues
		(const Vector3&,
		 float, float, float, float,
		 float, float, float, float) const throw(OutOfGrid);
	void getEnclosingIndices
		(const Vector3&,
		 Position&, Position&, Position&, Position&,
		 Position&, Position&, Position&, Position&) const throw(OutOfGrid);
	void binaryWrite(const String&) const throw();
	void binaryRead(const String&) throw();
};
