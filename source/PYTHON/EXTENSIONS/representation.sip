// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: representation.sip,v 1.9 2004/07/26 13:46:02 amoll Exp $
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
	ColorProcessor* getColorProcessor() throw();
	PyCompositeList getComposites() throw();
	// CompositesConstIterator begin() const throw();
	// CompositesConstIterator end() const throw();
	// CompositesIterator begin() throw();
	// CompositesIterator end() throw();
	GeometricObjectList& getGeometricObjects() throw();
	ColoringMethod getColoringMethod() const throw();
 	DrawingMode getDrawingMode() const throw();
	DrawingPrecision getDrawingPrecision() const throw();
	ModelType getModelType() const throw();
	ModelProcessor* getModelProcessor() throw();
	Representation() throw();
	Representation(ModelType /model_type/, DrawingPrecision /drawing_precision/, DrawingMode /drawing_mode/) throw();
	Representation(const PyCompositeList& /composites/, ModelProcessor* /rep_processor/) throw();
	// Representation(const GeometricObjectList& /object_list/) throw();
	Representation(const Representation& /representation/) throw();
	Size getTransparency() const throw();
	String getModelName() const throw();
	String getProperties() const throw();
	bool isValid() const throw();
	// CompositeList getComposites() const throw();
	const GeometricObjectList& getGeometricObjects() const throw();
	const ModelProcessor* getModelProcessor() const throw();
	// const Representation& operator = (const Representation& /representation/) throw();
	float getSurfaceDrawingPrecision() const throw();
	void clear() throw();
	//	void dump(std::ostream& s, Size depth) const throw();
	void insert(GeometricObject& /object/) throw();
	void setColorProcessor(ColorProcessor* /processor/) throw();
	void setColoringMethod(ColoringMethod /type/) throw();
	void setDrawingMode(DrawingMode /mode/) throw();
	void setDrawingPrecision(DrawingPrecision /precision/) throw();
	void setModelProcessor(ModelProcessor* /processor/) throw();
	void setModelType(ModelType /type/) throw();
	void setSurfaceDrawingPrecision(float /precision/) throw();
	void setTransparency(Size /value/) throw();
	void update(bool /rebuild/) throw();
	bool isHidden() const throw();
	void setHidden(bool state) throw();
	~Representation() throw();
};
