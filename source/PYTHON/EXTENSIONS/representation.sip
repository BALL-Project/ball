// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: representation.sip,v 1.3 2003/11/25 02:09:47 amoll Exp $
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
	// CompositeSet& getComposites() throw();
	// CompositesConstIterator begin() const throw();
	// CompositesConstIterator end() const throw();
	// CompositesIterator begin() throw();
	// CompositesIterator end() throw();
	// GeometricObjectList& getGeometricObjects() throw();
//	ColoringMethod getColoringMethod() const throw();
	Index getDrawingMode() const throw();
	Index getDrawingPrecision() const throw();
//	Index getModelType() const throw();
	ModelProcessor* getModelProcessor() throw();
	Representation() throw();
//	Representation(Index /model_type/, Index /drawing_precision/, Index /drawing_mode/) throw();
	// Representation(const CompositeSet& /composites/, ModelProcessor* /rep_processor/) throw();
	// Representation(const GeometricObjectList& /object_list/) throw();
	Representation(const Representation& /representation/) throw();
	Size getTransparency() const throw();
	String getModelName() const throw();
	String getProperties() const throw();
	bool isValid() const throw();
	// const CompositeSet& getComposites() const throw();
	// const GeometricObjectList& getGeometricObjects() const throw();
	const ModelProcessor* getModelProcessor() const throw();
	// const Representation& operator = (const Representation& /representation/) throw();
	float getSurfaceDrawingPrecision() const throw();
	void clear() throw();
	//	void dump(std::ostream& s, Size depth) const throw();
	void insert(GeometricObject& /object/) throw();
	void setColorProcessor(ColorProcessor* /processor/) throw();
//	void setColoringMethod(ColoringMethod /type/) throw();
	void setDrawingMode(Index /mode/) throw();
	void setDrawingPrecision(Index /precision/) throw();
	void setModelProcessor(ModelProcessor* /processor/) throw();
//	void setModelType(Index /type/) throw();
	void setSurfaceDrawingPrecision(float /precision/) throw();
	void setTransparency(Size /value/) throw();
	void update(bool /rebuild/) throw();
	~Representation() throw();
};
