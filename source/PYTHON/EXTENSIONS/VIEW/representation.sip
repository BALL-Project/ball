class Representation
	: PropertyManager
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/representation.h>
	// 999.59-06 — the deprecation shims below forward the legacy setters to
	// the command-shaped controllers (the owner setters were privatized in
	// 999.59-04). Removed in v1.8.
	#include <BALL/VIEW/KERNEL/controllers/modelController.h>
	#include <BALL/VIEW/KERNEL/controllers/coloringController.h>
	#include <BALL/VIEW/KERNEL/controllers/materialController.h>
	#include <list>
	using std::list;
%End
	public:

	enum Properties
	{
		PROPERTY__ALWAYS_FRONT = 0,
		PROPERTY__IS_COORDINATE_SYSTEM
	};

	Representation() throw();
	Representation(ModelType, DrawingPrecision, DrawingMode) throw();
	Representation(const Representation&) throw();
	~Representation() throw();

	void clear() throw();
	bool isHidden() const throw();
	DrawingPrecision getDrawingPrecision() const throw();
	float getSurfaceDrawingPrecision() const throw();
 	DrawingMode getDrawingMode() const throw();
	Size getTransparency() const throw();
	list<GeometricObject*>& getGeometricObjects() throw();
	void setGeometricObjects(list<GeometricObject*>&) throw();
	void insert(GeometricObject& /Transfer/) throw();

	PyCompositeList getComposites() const throw();
%MethodCode
	PyCompositeList& my_list = *new PyCompositeList;
	std::list<const Composite*> org_list = sipCpp->getComposites();
	std::list<const Composite*>::iterator it = org_list.begin();
	for (; it != org_list.end(); it++)
	{
		my_list.push_back((Composite*)*it);
	}
	sipRes = &my_list;
%End

	// ---------------------------------------------------------------------
	// 999.59-06 (v1.7.4) deprecation shims. The narrowed owner setters
	// (setDrawingMode / setTransparency / setModelType / setColoringMethod
	// and the lower-level setComposites / setModelProcessor / ... ) were
	// privatized in 999.59-04 — they no longer exist on the public C++
	// surface, so SIP can no longer bind them directly. The shims below keep
	// the legacy method path working for ONE release by forwarding to the
	// command-shaped controller (controllers.sip) and emitting a
	// DeprecationWarning. They are REMOVED in v1.8; new code uses the
	// controller surface (e.g. ColoringController). The lower-level setters
	// with no common script path were dropped outright.
	// ---------------------------------------------------------------------

	void setModelType(ModelType);
%MethodCode
	PyErr_WarnEx(PyExc_DeprecationWarning,
		"Representation.setModelType() is deprecated; use "
		"ModelController().setRepresentation(rep); setModelType(t); apply(). "
		"Removed in v1.8.", 1);
	BALL::VIEW::ModelController _c;
	_c.setRepresentation(sipCpp);
	_c.setModelType((int)a0);
	_c.apply();
%End

	void setDrawingMode(DrawingMode);
%MethodCode
	PyErr_WarnEx(PyExc_DeprecationWarning,
		"Representation.setDrawingMode() is deprecated; use "
		"ModelController().setRepresentation(rep); setDrawingMode(m); apply(). "
		"Removed in v1.8.", 1);
	BALL::VIEW::ModelController _c;
	_c.setRepresentation(sipCpp);
	_c.setDrawingMode((int)a0);
	_c.apply();
%End

	void setTransparency(Size);
%MethodCode
	PyErr_WarnEx(PyExc_DeprecationWarning,
		"Representation.setTransparency() is deprecated; use "
		"MaterialController().setRepresentation(rep); setTransparency(v); apply(). "
		"Removed in v1.8.", 1);
	BALL::VIEW::MaterialController _c;
	_c.setRepresentation(sipCpp);
	_c.setTransparency((int)a0);
	_c.apply();
%End

	void setColoringMethod(ColoringMethod);
%MethodCode
	PyErr_WarnEx(PyExc_DeprecationWarning,
		"Representation.setColoringMethod() is deprecated; use "
		"ColoringController().setRepresentation(rep); setColoringMethod(m); apply(). "
		"Removed in v1.8.", 1);
	BALL::VIEW::ColoringController _c;
	_c.setRepresentation(sipCpp);
	_c.setColoringMethod((int)a0);
	_c.apply();
%End

	ModelType getModelType() const throw();
	ColoringMethod getColoringMethod() const throw();

	void enableModelUpdate(bool);
	void enableColoringUpdate(bool);
	bool modelUpdateEnabled() const;
	bool coloringUpdateEnabled() const;

	String getProperties() const throw();
	bool isValid() const throw();
	void update(bool) throw();
	void clearGeometricObjects() throw();
	const PreciseTime& getModelBuildTime() const throw();
	bool needsUpdate() const throw();
	void setNeedsUpdate() throw();
	String toString() const throw();
	String getName() const;
	void setName(const String&);
};
