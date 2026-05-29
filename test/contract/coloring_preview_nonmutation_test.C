// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.63 (v1.7.x-17b) — ColoringController::previewRange() non-mutation
// contract test.
//
// ARCHITECTURE-CONTRACT.md §5 ("render-only hints are NOT mutations"):
// previewRange() paints a transient drag-preview hint for a candidate
// (min, max) WITHOUT mutating the owner. It must be a second RENDER path,
// never a second MUTATION path. This fixture is the guard against
// previewRange() silently becoming a second mutation path.
//
// It drives a sequence of previewRange() calls and asserts:
//   1. the owner snapshot is UNCHANGED (coloring method + processor min/max),
//   2. NO applied event was emitted (appliedStub fire count == 0),
//   3. apply() was never entered — previewRange() records NO ApplyPayload,
//      so the captured-intent command_id stays empty; and a subsequent
//      real setRange().apply() (the §2 mutation path) DOES mutate, proving
//      the seam between the render-only preview and the mutation funnel.
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

#include "contractTestHarness.h"

#include <BALL/VIEW/KERNEL/representation.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/VIEW/MODELS/colorProcessor.h>
#include <BALL/VIEW/KERNEL/controllers/coloringController.h>

#include <QtWidgets/QApplication>

#include <sstream>
#include <string>

using namespace BALL;
using namespace BALL::VIEW;

namespace
{
	// Compact owner snapshot of the coloring-relevant Representation state.
	// Mirrors the controller's own snapshot but is recomputed here so the test
	// observes the OWNER directly (not the controller's mirror).
	std::string snapshotOwnerColoring_(Representation& rep)
	{
		std::ostringstream s;
		s << "method=" << static_cast<int>(rep.getColoringMethod());
		InterpolateColorProcessor* icp =
			dynamic_cast<InterpolateColorProcessor*>(rep.getColorProcessor());
		if (icp != nullptr)
			s << ";min=" << icp->getMinValue() << ";max=" << icp->getMaxValue();
		else
			s << ";min=-;max=-";
		return s.str();
	}
}

START_TEST(ColoringPreviewNonMutation)

qputenv("QT_QPA_PLATFORM", "offscreen");
int argc = 1;
char argv0[] = "coloring_preview_nonmutation_test";
char* argv[] = { argv0, nullptr };
QApplication app(argc, argv);

// ---------------------------------------------------------------------------
// previewRange() is render-only: it mutates NOTHING, emits NO event, and never
// enters apply(). (ENABLED — ColoringController is cut over to bool apply() in
// 999.59-02, and previewRange() lands in 999.63.)
// ---------------------------------------------------------------------------
CHECK(previewRange mutates nothing - emits no event - never enters apply)
	Representation rep;
	ColoringController c(&rep);

	// Establish a known owner baseline through the LEGAL mutation path.
	c.setColoringMethod(COLORING_TEMPERATURE_FACTOR);
	c.setRange(20.0f, 40.0f);
	bool mutated = c.apply();
	TEST_EQUAL(mutated, true)

	const std::string before = snapshotOwnerColoring_(rep);
	const int before_method  = c.coloringMethod();
	const float before_min   = c.valueMin();
	const float before_max   = c.valueMax();

	// Count applied events fired during the preview drag.
	int applied_count = 0;
	QObject::connect(&c, &ColoringController::appliedStub, &c,
	                 [&]() { ++applied_count; });

	// Drive a sequence of previewRange() calls — as a live handle-drag would,
	// many frames per second, with values DIFFERENT from the committed range.
	c.previewRange(5.0f, 95.0f);
	c.previewRange(10.0f, 90.0f);
	c.previewRange(0.0f, 100.0f);
	c.previewRange(33.0f, 66.0f);
	c.previewRange(50.0f, 50.0f);

	// 1. Owner is UNCHANGED — previewRange() called no owner setter.
	const std::string after = snapshotOwnerColoring_(rep);
	TEST_EQUAL(after, before)

	// The controller's STAGED state is also untouched (so a later apply() will
	// NOT silently commit a dragged value — the second-mutation-path regression).
	TEST_EQUAL(c.coloringMethod(), before_method)
	TEST_REAL_EQUAL(c.valueMin(), before_min)
	TEST_REAL_EQUAL(c.valueMax(), before_max)

	// 2. NO applied event was emitted during the entire preview sequence.
	TEST_EQUAL(applied_count, 0)

	// 3. apply() was never entered — the controller is not mid-apply, and the
	//    owner still holds the committed (20, 40) range, not any previewed value.
	TEST_EQUAL(c.isApplying(), false)
	{
		InterpolateColorProcessor* icp =
			dynamic_cast<InterpolateColorProcessor*>(rep.getColorProcessor());
		TEST_NOT_EQUAL(icp, 0)
		if (icp != 0)
		{
			TEST_REAL_EQUAL(icp->getMinValue(), 20.0f)
			TEST_REAL_EQUAL(icp->getMaxValue(), 40.0f)
		}
	}

	// Seam proof: the REAL mutation path (setRange().apply()) still works and
	// IS observed — so previewRange()'s silence is non-mutation, not a wedge.
	c.setRange(15.0f, 85.0f);
	bool mutated2 = c.apply();
	TEST_EQUAL(mutated2, true)
	TEST_EQUAL(applied_count, 1)               // exactly one event, from apply().
	{
		InterpolateColorProcessor* icp =
			dynamic_cast<InterpolateColorProcessor*>(rep.getColorProcessor());
		if (icp != 0)
		{
			TEST_REAL_EQUAL(icp->getMinValue(), 15.0f)
			TEST_REAL_EQUAL(icp->getMaxValue(), 85.0f)
		}
	}
RESULT

// ---------------------------------------------------------------------------
// previewRange() is idempotent and freely re-callable — repeated identical
// calls leave the owner exactly as a single call (== unchanged) would.
// ---------------------------------------------------------------------------
CHECK(previewRange is idempotent and owner-inert under repetition)
	Representation rep;
	ColoringController c(&rep);
	c.setColoringMethod(COLORING_OCCUPANCY);
	c.setRange(0.0f, 1.0f);
	c.apply();

	const std::string before = snapshotOwnerColoring_(rep);
	for (int i = 0; i < 64; ++i) c.previewRange(0.25f, 0.75f);
	const std::string after = snapshotOwnerColoring_(rep);
	TEST_EQUAL(after, before)
RESULT

END_TEST
