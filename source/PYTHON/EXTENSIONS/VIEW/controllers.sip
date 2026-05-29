/////////////////////////////////////////////////////////////////////////
// VIEW command-shaped Controllers (Phase 999.59-06, v1.7.4)            //
//                                                                       //
// PyBALL binding surface for the 9 command-shaped Controllers           //
// (ARCHITECTURE-CONTRACT.md §2 / §3d). 999.59-04 privatized the owner    //
// (Stage / Representation) setters, so the bindings now expose the       //
// Controllers — NOT the owners. A script attaches a controller to its    //
// owner, stages typed mutations, then calls apply():                     //
//                                                                        //
//   c = ColoringController()                                             //
//   c.setRepresentation(rep)                                             //
//   c.setColoringMethod(method)                                          //
//   c.apply()              # validate -> guard -> mutate -> notify        //
//                                                                        //
// NOTE: SIP is disabled in the modernization build (CLAUDE.md). These    //
// declarations mirror the real C++ public surface so the binding is      //
// honest + lint-checkable today; full regeneration + runtime coverage    //
// ride with the v2.1 PyBALL milestone (Phase 6 generator bake-off).      //
/////////////////////////////////////////////////////////////////////////

class Controller : QObject
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/controllers/controller.h>
%End
	public:
		Controller(QObject* /TransferThis/ = 0);
		virtual bool apply();
		virtual void reset();
};

class ModelController : Controller
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/controllers/modelController.h>
%End
	public:
		ModelController(QObject* /TransferThis/ = 0);
		void setRepresentation(Representation*);
		bool apply();
		void reset();
		void setModelType(int);
		void setDrawingMode(int);
		void setDrawingPrecision(int);
		void setTransparency(int);
};

class ColoringController : Controller
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/controllers/coloringController.h>
%End
	public:
		ColoringController(QObject* /TransferThis/ = 0);
		void setRepresentation(Representation*);
		bool apply();
		void reset();
		void setColoringMethod(int);
		void setValueMin(float);
		void setValueMax(float);
};

class MaterialController : Controller
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/controllers/materialController.h>
%End
	public:
		MaterialController(QObject* /TransferThis/ = 0);
		void setRepresentation(Representation*);
		bool apply();
		void reset();
		void setTransparency(int);
};

class LabelController : Controller
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/controllers/labelController.h>
%End
	public:
		LabelController(QObject* /TransferThis/ = 0);
		void setRepresentation(Representation*);
		bool apply();
		void reset();
		void setLabelType(int);
		void setFontSize(int);
};

class ClippingController : Controller
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/controllers/clippingController.h>
%End
	public:
		ClippingController(QObject* /TransferThis/ = 0);
		bool apply();
		void reset();
		void setEnabled(bool);
		void setOffset(float);
		void setCapped(bool);
};

class StageController : Controller
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/controllers/stageController.h>
%End
	public:
		StageController(QObject* /TransferThis/ = 0);
		void setStage(Stage*);
		bool apply();
		void reset();
		void setShowCoordinateSystem(bool);
		void setFogIntensity(float);
		void setEyeDistance(float);
		void setFocalDistance(float);
		void setPerspectiveProjection(bool);
		void setShowLightSources(bool);
		void setAnimationSmoothness(float);
};

class CameraController : Controller
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/controllers/cameraController.h>
%End
	public:
		CameraController(QObject* /TransferThis/ = 0);
		void setStage(Stage*);
		bool apply();
		void reset();
};

class LightController : Controller
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/controllers/lightController.h>
%End
	public:
		LightController(QObject* /TransferThis/ = 0);
		void setStage(Stage*);
		bool apply();
		void reset();
		void setAmbientIntensity(float);
};

class StereoController : Controller
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/controllers/stereoController.h>
%End
	public:
		StereoController(QObject* /TransferThis/ = 0);
		void setStage(Stage*);
		bool apply();
		void reset();
		void setStereoEnabled(bool);
		void setEyeDistance(float);
		void setFocalDistance(float);
		void setSwapSideBySide(bool);
};
