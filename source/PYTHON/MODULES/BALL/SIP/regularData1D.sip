class FloatRegularData1D
{
%HeaderCode
	#include <BALL/DATATYPE/regularData1D.h>
	using namespace BALL;
	typedef TRegularData1D<float> FloatRegularData1D;
%End
	public:
	FloatRegularData1D() throw();
	FloatRegularData1D(const FloatRegularData1D& /data/) throw();
	// virtual ~FloatRegularData1D() throw();
	virtual void clear() throw();
	virtual void destroy() throw();
	// const FloatRegularData1D& operator = (const FloatRegularData1D& data) throw();
	// const FloatRegularData1D& operator = (const VectorType& data) throw();
	// bool operator == (const FloatRegularData1D& data) const throw();
	// const float& operator [] (Position index) const throw(IndexOverflow);
	// float& operator [] (Position index) throw(IndexOverflow);
	Size getSize() const throw();
	double getLowerBound() const throw();
	double getUpperBound() const throw();
	void setUpperBound(double /upper/) throw();
	void setLowerBound(double /lower/) throw();
	// void resize(Size /new_size/) throw();
};


