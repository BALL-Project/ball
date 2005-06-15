// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: representation.sip,v 1.10.4.2 2005/06/15 11:45:10 amoll Exp $
//

class Representation
  : PropertyManager
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/representation.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
  public:
	// BALL_CREATE(Representation);
	Representation() throw();
	Representation(ModelType /model_type/, DrawingPrecision /drawing_precision/, DrawingMode /drawing_mode/) throw();
	Representation(const GeometricObjectList& /object_list/) throw();
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
	void insert(GeometricObject& /object/) throw();

	PyCompositeList getComposites() throw();
	PyCompositeList getComposites() const throw();
//	void setComposites(const List<const Composite*>&) throw();

	ModelProcessor* getModelProcessor() throw();
	const ModelProcessor* getModelProcessor() const throw();
	void setModelProcessor(ModelProcessor* /processor/) throw();

	ColorProcessor* getColorProcessor() throw();
	void setColorProcessor(ColorProcessor* /processor/) throw();

	String getModelName() const throw();

	void setModelType(ModelType /type/) throw();
	ModelType getModelType() const throw();

	ColoringMethod getColoringMethod() const throw();
	void setColoringMethod(ColoringMethod /type/) throw();

	void enableModelUpdate(bool);
	void enableColoringUpdate(bool);
	bool modelUpdateEnabled() const;
	bool coloringUpdateEnabled() const;

	String getProperties() const throw();
	bool isValid() const throw();
	//	void dump(std::ostream& s, Size depth) const throw();
	String toString() const throw();
	
	void update(bool /rebuild/) throw();

	void clearGeometricObjects() throw();

	const PreciseTime& getModelBuildTime() const throw();

	bool needsUpdate() const throw();
	void setNeedsUpdate() throw();
};
