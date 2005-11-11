// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: representation.sip,v 1.10.4.6 2005/11/11 17:21:25 leonhardt Exp $
//

class Representation
  : PropertyManager
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/representation.h>
	using namespace BALL;
	using namespace BALL::VIEW;
	typedef Representation::GeometricObjectList GeometricObjectList;
%End
  public:
	// BALL_CREATE(Representation);
	enum Properties
	{
		PROPERTY__ALWAYS_FRONT = 0,
		PROPERTY__IS_COORDINATE_SYSTEM
	};
	Representation() throw();
	Representation(ModelType /model_type/, DrawingPrecision /drawing_precision/, DrawingMode /drawing_mode/) throw();
	Representation(const Representation& /representation/) throw();
  ~Representation() throw();
	// const Representation& operator = (const Representation& /representation/) throw();

	void clear() throw();
	
	void setHidden(bool) throw();
	bool isHidden() const throw();

	void setDrawingPrecision(DrawingPrecision /precision/) throw();
	DrawingPrecision getDrawingPrecision() const throw();
	void setSurfaceDrawingPrecision(float /precision/) throw();
	float getSurfaceDrawingPrecision() const throw();

 	DrawingMode getDrawingMode() const throw();
	void setDrawingMode(DrawingMode /mode/) throw();

	Size getTransparency() const throw();
	void setTransparency(Size /value/) throw();

	GeometricObjectList& getGeometricObjects() throw();
	const GeometricObjectList& getGeometricObjects() const throw();
	void insert(GeometricObject& /Transfer/) throw();

	//const PyCompositeList& getComposites() const throw();
	//void setComposites(const PyCompositeList&) throw();

	ModelProcessor* getModelProcessor() throw();
	const ModelProcessor* getModelProcessor() const throw();
	void setModelProcessor(ModelProcessor* /Transfer/) throw();

	ColorProcessor* getColorProcessor() throw();
	void setColorProcessor(ColorProcessor* /Transfer/) throw();

	String getModelName() const throw();

	void setModelType(ModelType /type/) throw();
	ModelType getModelType() const throw();

	void setColoringMethod(ColoringMethod /type/) throw();
	ColoringMethod getColoringMethod() const throw();

	void enableModelUpdate(bool);
	void enableColoringUpdate(bool);
	bool modelUpdateEnabled() const;
	bool coloringUpdateEnabled() const;

	String getProperties() const throw();
	bool isValid() const throw();
	void update(bool /rebuild/) throw();
	void clearGeometricObjects() throw();
	const PreciseTime& getModelBuildTime() const throw();
	bool needsUpdate() const throw();
	void setNeedsUpdate() throw();
	//	void dump(std::ostream& s, Size depth) const throw();
	String toString() const throw();
	//bool operator == (const Representation&) const throw();
	//bool operator < (const Representation& object) const throw();
};
