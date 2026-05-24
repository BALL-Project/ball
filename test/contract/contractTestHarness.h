// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.60 (v1.7.x-25) — Contract-test harness.
//
// The cross-surface contract-test harness defined by
// ARCHITECTURE-CONTRACT.md §11. It boots a headless Qt environment
// (matching test/tabOrder_test.C discipline), exposes the four mutation
// surfaces the contract recognises (Inspector / toolbar / menu / Python),
// and produces a comparable owner snapshot so a per-controller parity
// fixture can assert that all surfaces leave the owner in a bit-equal
// state.
//
// SEQUENCING NOTE (Codex MEDIUM #7 — harness FIRST). This harness and the
// compile-fail infra (owner_narrowing_test.C) land BEFORE 999.59 cuts the
// controllers over to the §2 `bool apply()` contract. Today every
// Controller::apply() returns `void` and CommandRegistry is compiled out
// (BALL_UI_V2 was removed, so the registered-command-body surface §8 does
// not yet exist). Therefore:
//
//   * The toolbar/menu surface helpers are STUBS that report
//     CONTRACT_SURFACE_UNAVAILABLE until the §8 command-body migration
//     lands in 999.59-03. surfaceAvailable() reports which surfaces are
//     live in the current build.
//   * The per-controller parity fixtures ship DISABLED_ / expected-fail
//     (see CONTRACT_FIXTURE_DISABLED below) and are flipped green by the
//     999.59-NN plan that cuts each controller over.
//
// Header-only enums + the fixture base declaration; the boot/snapshot
// implementation lives in contractTestHarness.C so it stays out of the
// per-fixture TUs.
//

#ifndef BALL_TEST_CONTRACT_CONTRACTTESTHARNESS_H
#define BALL_TEST_CONTRACT_CONTRACTTESTHARNESS_H

#include <BALL/COMMON/global.h>

#include <QtCore/QString>

#include <string>
#include <vector>

namespace BALL
{
	class System;

	namespace VIEW
	{
		class Stage;
		class Representation;
	}
}

namespace BALL
{
	namespace VIEW
	{
		namespace Contract
		{

			/**
			 * The four mutation surfaces the architecture contract
			 * recognises (ARCHITECTURE-CONTRACT.md §1). A parity test
			 * runs the same logical operation through each available
			 * surface and asserts identical owner postconditions.
			 */
			enum class SurfaceKind
			{
				Inspector,   ///< Inspector section public binding methods + apply().
				Toolbar,     ///< Registered CommandRegistry command body (§8) the toolbar action resolves to.
				Menu,        ///< Same registered body, reached via the menu entry.
				Python       ///< SIP-exposed controller method from controllers.sip (skipped when PyBALL is OFF).
			};

			/** Human-readable surface name (for STATUS/diagnostic output). */
			BALL_VIEW_EXPORT const char* surfaceName(SurfaceKind kind);

			/**
			 * Whether a surface can actually be exercised in the current
			 * build. Harness-first reality (2026-05-22):
			 *
			 *   - Inspector : available (controllers exist in-tree).
			 *   - Toolbar   : UNAVAILABLE until 999.59-03 migrates the
			 *                 command body to a registered CommandRegistry
			 *                 std::function (CommandRegistry is compiled
			 *                 out today — BALL_UI_V2 was removed).
			 *   - Menu      : UNAVAILABLE (same reason as Toolbar).
			 *   - Python    : UNAVAILABLE unless BALL_HAS_PYBALL is ON and
			 *                 controllers.sip (999.59-06) is built.
			 *
			 * A parity fixture consults this to decide whether to run a
			 * surface or report it as a documented skip. As 999.59 lands,
			 * the corresponding surface flips to available and the fixture
			 * is enabled.
			 */
			BALL_VIEW_EXPORT bool surfaceAvailable(SurfaceKind kind);

			/**
			 * A diff-friendly POD snapshot of the owner state, per
			 * ARCHITECTURE-CONTRACT.md §11a / 60.1. A parity fixture
			 * snapshots the owner after each surface's apply() and asserts
			 * the snapshots are equal.
			 *
			 * The serialised form is a deterministic string blob composed
			 * of the owner's observable getters (Stage scene state +
			 * Representation per-rep state + the active Composite selection
			 * set). String form keeps it cheap to EXPECT_EQ and to print on
			 * mismatch without a bespoke diff routine.
			 */
			struct BALL_VIEW_EXPORT OwnerSnapshot
			{
				std::string stage_blob;       ///< Stage::serialize()-equivalent (bg/fog/camera/eye/focal/...).
				std::string representation_blob; ///< Representation per-rep state (model/coloring/material/...).
				std::string selection_blob;   ///< Composite::isSelected() set, canonicalised.

				bool operator==(const OwnerSnapshot& o) const
				{
					return stage_blob == o.stage_blob
					    && representation_blob == o.representation_blob
					    && selection_blob == o.selection_blob;
				}
				bool operator!=(const OwnerSnapshot& o) const { return !(*this == o); }

				/** Concatenated blob — handy for STATUS/printing on mismatch. */
				std::string str() const
				{
					return "stage{" + stage_blob + "} rep{" + representation_blob
					     + "} sel{" + selection_blob + "}";
				}
			};

			/**
			 * Contract-test harness fixture base.
			 *
			 * Lifecycle mirrors test/tabOrder_test.C: the QApplication is
			 * forced onto the `offscreen` QPA platform so the harness is
			 * headless on CI runners with no display. setUp() constructs an
			 * empty Stage + Representation (the headless owners the
			 * Controllers mutate) and, when BALL_DATA_PATH points at a
			 * usable sample, loads it into a System for the selection
			 * surface; tearDown() releases them.
			 *
			 * The harness deliberately does NOT depend on a constructed
			 * Mainframe/MainControl — those are not headless-stable on every
			 * runner (tabOrder_test documents the Windows fragility). The
			 * Stage/Representation owners are sufficient for the §2 apply()
			 * postcondition assertions; the full Inspector/Scene wiring is a
			 * v1.7.5 concern (per the plan's "Out" list).
			 */
			class BALL_VIEW_EXPORT ContractTestHarness
			{
				public:
					ContractTestHarness();
					~ContractTestHarness();

					/** Construct the headless owners + (optionally) load a sample. */
					void setUp();

					/** Release owners + loaded system. */
					void tearDown();

					/** The Stage owner the scene-state Controllers mutate. */
					Stage* stage() { return stage_; }

					/** The Representation owner the per-rep Controllers mutate. */
					Representation* representation() { return representation_; }

					/** The loaded sample system, or nullptr if none was loaded. */
					System* system() { return system_; }

					/** True once a CI-stable sample (1BNA.pdb) loaded successfully. */
					bool sampleLoaded() const { return sample_loaded_; }

					/**
					 * Capture the current owner state as a comparable POD.
					 * Pulls from the Stage + Representation getters and the
					 * active selection set.
					 */
					OwnerSnapshot snapshotOwner() const;

				private:
					Stage*          stage_;
					Representation* representation_;
					System*         system_;
					bool            sample_loaded_;

					ContractTestHarness(const ContractTestHarness&) = delete;
					ContractTestHarness& operator=(const ContractTestHarness&) = delete;
			};

		} // namespace Contract
	} // namespace VIEW
} // namespace BALL

// ---------------------------------------------------------------------------
// Fixture enable/disable switch (harness-first sequencing).
//
// CONTRACT_FIXTURE_DISABLED expands to 1 while the 999.59 `bool apply()`
// command contract has NOT landed, so the per-controller parity fixtures
// and the bool-apply re-entrancy assertions ship as documented skips
// (they compile, register, and run, but report STATUS-skip instead of
// failing). Each 999.59-NN cut-over flips its controller's fixture green
// by removing the guard / changing this to 0 once `bool apply()` exists.
//
// Detection: 999.59-01 introduces a feature macro the controllers define
// once apply() returns bool. Until that macro exists, fixtures are
// disabled. The macro name is fixed here so 999.59-01 only has to define
// it.
// ---------------------------------------------------------------------------
#if defined(BALL_VIEW_CONTROLLER_APPLY_RETURNS_BOOL)
#	define CONTRACT_FIXTURE_DISABLED 0
#else
#	define CONTRACT_FIXTURE_DISABLED 1
#endif

#endif // BALL_TEST_CONTRACT_CONTRACTTESTHARNESS_H
