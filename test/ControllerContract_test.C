// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// v1.7.x-25 — Controller command-contract tests.
//
// These are the regression safety-net for the v1.7.x-24 controller
// cut-over. Each test asserts the *contract* of a presentation
// Controller: setting a mirrored property and calling apply() must
// persist that state to the single owner (the Stage), and a freshly
// constructed Controller's revert() (run in its constructor) must read
// the same value back. set -> apply -> (fresh controller) revert ==
// original. This proves the Inspector mutation path actually reaches the
// owner and is consistent by construction — exactly the property the
// cut-over must not break.
//
// Scope: the four Stage-attached controllers, which are cleanly headless-
// testable (their apply() guards the Scene/MainControl singletons, which
// are absent in a unit test, so only the Stage is mutated). The
// Representation-attached controllers (Model/Coloring/Material) need a
// Representation + DisplayProperties fixture and are covered separately.
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

#include <BALL/VIEW/KERNEL/stage.h>
#include <BALL/VIEW/KERNEL/representation.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/VIEW/KERNEL/controllers/stageController.h>
#include <BALL/VIEW/KERNEL/controllers/cameraController.h>
#include <BALL/VIEW/KERNEL/controllers/stereoController.h>
#include <BALL/VIEW/KERNEL/controllers/lightController.h>
#include <BALL/VIEW/KERNEL/controllers/modelController.h>
#include <BALL/VIEW/KERNEL/controllers/coloringController.h>
#include <BALL/VIEW/DATATYPE/colorRGBA.h>

#include <QtGui/QColor>
#include <QtGui/QVector3D>

using namespace BALL;
using namespace BALL::VIEW;

START_TEST(ControllerContract)

using namespace BALL::VIEW;

CHECK(StageController::apply() persists float/bool state to the Stage)
	Stage stage;
	StageController c(&stage, nullptr);
	c.setFogIntensity(0.42f);
	c.setShowCoordinateSystem(true);
	c.setEyeDistance(1.5f);
	c.setFocalDistance(7.25f);
	c.apply();

	// A fresh controller's ctor calls revert(), pulling from the Stage.
	StageController back(&stage, nullptr);
	TEST_REAL_EQUAL(back.fogIntensity(), 0.42f)
	TEST_EQUAL(back.showCoordinateSystem(), true)
	TEST_REAL_EQUAL(back.eyeDistance(), 1.5f)
	TEST_REAL_EQUAL(back.focalDistance(), 7.25f)
RESULT

CHECK(StageController::apply() persists the background colour to the Stage)
	Stage stage;
	StageController c(&stage, nullptr);
	c.setBackgroundColor(QColor(0, 128, 255));
	c.apply();
	// Assert on the owner's ColorRGBA (0..1) with tolerance — the QColor
	// round-trip truncates, but the stored channels are exact-ish floats.
	const ColorRGBA& bg = stage.getBackgroundColor();
	TEST_REAL_EQUAL(static_cast<float>(bg.getRed()),   0.0f)
	TEST_REAL_EQUAL(static_cast<float>(bg.getGreen()), 128.0f / 255.0f)
	TEST_REAL_EQUAL(static_cast<float>(bg.getBlue()),  255.0f / 255.0f)
RESULT

// v1.7.x-24/25 — the re-entrancy guard contract. The guard is the
// v1.7.x-13 freeze fix: a notification emitted mid-apply() must NOT be
// able to synchronously re-run the mutation. apply() emits appliedStub()
// at its tail *while the ControllerApplyGuard is still held*, so a slot
// connected there observes isApplying() == true and any apply() it calls
// re-enters and must early-return. These checks lock that in — without
// them the guard could silently regress (the cut-over would still pass the
// round-trip checks above, but the freeze would be back).
CHECK(StageController::apply() is shielded against re-entrancy (v1.7.x-13 guard))
	Stage stage;
	StageController c(&stage, nullptr);
	c.setFogIntensity(0.42f);

	// From inside apply() (via appliedStub), confirm the guard is held, then
	// attempt a re-entrant apply() with a DIFFERENT fog value. The guard must
	// make that nested call a no-op, so the Stage keeps the value the outer
	// apply() already pushed (0.42), not the 0.99 staged inside the slot.
	bool guard_held_mid_apply = false;
	bool attempted_reentry = false;
	QObject::connect(&c, &StageController::appliedStub, &c, [&]() {
		if (attempted_reentry) return;          // one-shot
		attempted_reentry = true;
		guard_held_mid_apply = c.isApplying();
		c.setFogIntensity(0.99f);               // change the mirror only
		c.apply();                              // re-entrant => must be shielded
	});

	c.apply();

	TEST_EQUAL(guard_held_mid_apply, true)            // guard active during the emit
	TEST_EQUAL(c.isApplying(), false)                 // RAII cleared it on return
	TEST_REAL_EQUAL(stage.getFogIntensity(), 0.42f)   // shielded nested apply() pushed nothing

	// The controller is not wedged: a later, non-reentrant apply() pushes the
	// pending mirror value (0.99) normally.
	c.apply();
	TEST_REAL_EQUAL(stage.getFogIntensity(), 0.99f)
RESULT

CHECK(CameraController::apply() persists viewpoint + look-at to the Stage camera)
	Stage stage;
	CameraController c(&stage);
	c.setPosition(QVector3D(1.0f, 2.0f, 3.0f));
	c.setLookAt(QVector3D(4.0f, 5.0f, 6.0f));   // distinct => not degenerate
	c.apply();

	CameraController back(&stage);
	TEST_REAL_EQUAL(back.position().x(), 1.0f)
	TEST_REAL_EQUAL(back.position().y(), 2.0f)
	TEST_REAL_EQUAL(back.position().z(), 3.0f)
	TEST_REAL_EQUAL(back.lookAt().x(),   4.0f)
	TEST_REAL_EQUAL(back.lookAt().y(),   5.0f)
	TEST_REAL_EQUAL(back.lookAt().z(),   6.0f)
RESULT

CHECK(CameraController::apply() engages the re-entrancy guard)
	Stage stage;
	CameraController c(&stage);
	c.setPosition(QVector3D(1.0f, 2.0f, 3.0f));
	c.setLookAt(QVector3D(4.0f, 5.0f, 6.0f));
	bool guard_held_mid_apply = false;
	QObject::connect(&c, &CameraController::appliedStub, &c, [&]() {
		guard_held_mid_apply = c.isApplying();
	});
	c.apply();
	TEST_EQUAL(guard_held_mid_apply, true)   // guard set for the duration of apply()
	TEST_EQUAL(c.isApplying(), false)        // and RAII-cleared on return
RESULT

CHECK(StereoController::apply() persists eye/focal/swap to the Stage)
	Stage stage;
	StereoController c(&stage);
	c.setEyeDistance(2.0f);
	c.setFocalDistance(9.0f);
	c.setSwapSideBySide(true);
	c.apply();

	StereoController back(&stage);
	TEST_REAL_EQUAL(back.eyeDistance(), 2.0f)
	TEST_REAL_EQUAL(back.focalDistance(), 9.0f)
	TEST_EQUAL(back.swapSideBySide(), true)
RESULT

CHECK(StereoController::apply() engages the re-entrancy guard)
	Stage stage;
	StereoController c(&stage);
	c.setEyeDistance(2.0f);
	bool guard_held_mid_apply = false;
	QObject::connect(&c, &StereoController::appliedStub, &c, [&]() {
		guard_held_mid_apply = c.isApplying();
	});
	c.apply();
	TEST_EQUAL(guard_held_mid_apply, true)
	TEST_EQUAL(c.isApplying(), false)
RESULT

CHECK(LightController::apply() persists the ambient intensity to the Stage)
	Stage stage;
	LightController c(&stage);
	c.setAmbientIntensity(0.6f);
	c.apply();

	LightController back(&stage);
	TEST_REAL_EQUAL(back.ambientIntensity(), 0.6f)
RESULT

CHECK(LightController::apply() engages the re-entrancy guard)
	Stage stage;
	LightController c(&stage);
	c.setAmbientIntensity(0.6f);
	bool guard_held_mid_apply = false;
	QObject::connect(&c, &LightController::appliedStub, &c, [&]() {
		guard_held_mid_apply = c.isApplying();
	});
	c.apply();
	TEST_EQUAL(guard_held_mid_apply, true)
	TEST_EQUAL(c.isApplying(), false)
RESULT

// Representation-attached controllers. These controllers' apply() are
// Scene-independent: DisplayProperties::getInstance(0) is null in this
// headless test, so the per-method PROCESSOR-factory branch is skipped,
// but rep_->setModelType / setColoringMethod / setTransparency still run
// and Representation::update() is safe with no model_processor_/composites.
CHECK(ModelController::apply() persists model/precision/transparency to the Representation)
	Representation rep;
	ModelController c(&rep);
	c.setModelType(MODEL_VDW);
	c.setDrawingPrecision(2);
	c.setTransparency(64);
	c.apply();

	ModelController back(&rep);
	TEST_EQUAL(back.modelType(), MODEL_VDW)
	TEST_EQUAL(back.drawingPrecision(), 2)
	TEST_EQUAL(back.transparency(), 64)
RESULT

CHECK(ModelController::apply() engages the re-entrancy guard)
	Representation rep;
	ModelController c(&rep);
	c.setModelType(MODEL_VDW);
	bool guard_held_mid_apply = false;
	QObject::connect(&c, &ModelController::appliedStub, &c, [&]() {
		guard_held_mid_apply = c.isApplying();
	});
	c.apply();
	TEST_EQUAL(guard_held_mid_apply, true)
	TEST_EQUAL(c.isApplying(), false)
RESULT

CHECK(ColoringController::apply() persists the coloring method to the Representation)
	Representation rep;
	ColoringController c(&rep);
	c.setColoringMethod(COLORING_CHAIN);
	c.apply();

	ColoringController back(&rep);
	TEST_EQUAL(back.coloringMethod(), COLORING_CHAIN)
RESULT

CHECK(ColoringController::apply() engages the re-entrancy guard)
	Representation rep;
	ColoringController c(&rep);
	c.setColoringMethod(COLORING_CHAIN);
	bool guard_held_mid_apply = false;
	QObject::connect(&c, &ColoringController::appliedStub, &c, [&]() {
		guard_held_mid_apply = c.isApplying();
	});
	c.apply();
	TEST_EQUAL(guard_held_mid_apply, true)
	TEST_EQUAL(c.isApplying(), false)
RESULT

// MaterialController is NOT checked here: its apply() bails when
// Scene::getInstance(0) is null (material is per-rep state on the
// Scene/Renderer, not on the Representation), so it is not headless-
// testable — it is covered by the GUI path only.

END_TEST
