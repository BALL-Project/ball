// $Id: regularData2D.sip,v 1.1 2001/01/16 17:27:48 anker Exp $

class FloatRegularData2D
{
%HeaderCode
	#include <BALL/DATATYPE/regularData2D.h>
	typedef BALL::RegularData2D<float> FloatRegularData2D;
%End
	public:
	// typedef std::vector<float>	VectorType;
	FloatRegularData2D();
	FloatRegularData2D(const FloatRegularData2D& /data/);
	// virtual ~floatRegularData2D();
	virtual void clear();
	virtual void destroy();
	// const FloatRegularData2D operator = (const FloatRegularData2D data);
	// const FloatRegularData2D operator = (const Vectorfloatype& data);
	// bool operator == (const FloatRegularData2D data) const;
	// const float& operator [] (Position index) const;
	// float& operator [] (Position index);
	// void getNearestPosition(double /x/, double /y/, std::pair<Position, Position>& /dum/);
	// void getConvertedPosition(Position /x/, Position /y/, std::pair<float, float>& /dum/) const;
	// BAUSTELLE: stems from the template const T& ...
	// const float& getValue(float /valx/, float /valy/);
	void createGroundState();
	// const float& getGroundState();
	// const float& getSigmaGroundState();
	Size getSize() const;
	// const float& getUpperBound() const;
	// const float& getLowerBound() const;
	Size getXSize() const;
	Size getYSize() const;
	double getXLower() const;
	double getXUpper() const;
	double getYLower() const;
	double getYUpper() const;
	void setUpperBound(float /ub/);
	void setLowerBound(float /lb/);
	void setXSize(Size /xsize/);
	void setYSize(Size /ysize/);
	void setXUpper(double /xupper/);
	void setXLower(double /xlower/);
	void setYUpper(double /yupper/);
	void setYLower(double /ylower/);
	void resize(Size /new_size_x/, Size /new_size_y/);
};
