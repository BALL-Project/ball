// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 04 — ModelController (read-only mirror).
//
// Source dialog: source/VIEW/DIALOGS/modelSettingsDialog.C (~385 LOC).
//
// Phase 999.59-02 (v1.7.x-24) — cut over to the §2 `bool apply()` command
// contract (ARCHITECTURE-CONTRACT.md §2 / §2a / §13). ModelController now
// inherits the base Controller and overrides bool apply() in the
// validate → guard → capture → applyInternal_ → emit-one → invalidateDeclared_
// → recordIntent_ shape. Its §2a declared invalidation is a display-list
// REBUILD (representation_->update(true)) — Model is the one Representation-
// domain controller that rebuilds because a model-processor swap changes the
// primitive set (a soft refresh would re-draw stale geometry).
//

#ifndef BALL_VIEW_KERNEL_CONTROLLERS_MODELCONTROLLER_H
#define BALL_VIEW_KERNEL_CONTROLLERS_MODELCONTROLLER_H


#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifndef BALL_VIEW_KERNEL_CONTROLLERS_CONTROLLER_H
# include <BALL/VIEW/KERNEL/controllers/controller.h>
#endif

#ifndef BALL_VIEW_KERNEL_CONTROLLERS_APPLYPAYLOAD_H
# include <BALL/VIEW/KERNEL/controllers/applyPayload.h>
#endif

#include <QtCore/QObject>
#include <QtCore/QString>

// ---------------------------------------------------------------------------
// 999.59-02 per-controller contract-fixture activation symbol (Codex MEDIUM
// #7). Defining this tells the 999.60 contract-test harness that
// ModelController has been cut over to the §2 `bool apply()` shape, so the
// model_contract_test cross-surface parity block flips from STATUS-skip to
// live (see test/contract/contractFixtureActivation.h). Defined ONLY because
// apply() below returns bool — do NOT define for a controller still on void.
// ---------------------------------------------------------------------------
#define BALL_VIEW_MODEL_APPLY_BOOL 1

namespace BALL
{
	namespace VIEW
	{
		class Representation;

		/**
		 * Presentation controller for per-Representation Model
		 * settings: model type / drawing precision / drawing mode /
		 * transparency. Cut over to the §2 `bool apply()` command
		 * contract in 999.59-02 (declared §2a invalidation: display-list
		 * REBUILD — a model-processor swap changes the primitive set).
		 */
		class BALL_VIEW_EXPORT ModelController : public Controller
		{
			Q_OBJECT

			Q_PROPERTY(int modelType READ modelType WRITE setModelType NOTIFY modelTypeChanged)
			Q_PROPERTY(int drawingMode READ drawingMode WRITE setDrawingMode NOTIFY drawingModeChanged)
			Q_PROPERTY(int drawingPrecision READ drawingPrecision WRITE setDrawingPrecision NOTIFY drawingPrecisionChanged)
			Q_PROPERTY(int transparency READ transparency WRITE setTransparency NOTIFY transparencyChanged)
			Q_PROPERTY(float ballRadius READ ballRadius WRITE setBallRadius NOTIFY ballRadiusChanged)
			Q_PROPERTY(float ballStickBondRadius READ ballStickBondRadius WRITE setBallStickBondRadius NOTIFY ballStickBondRadiusChanged)
			Q_PROPERTY(bool dashedBonds READ dashedBonds WRITE setDashedBonds NOTIFY dashedBondsChanged)
			Q_PROPERTY(float stickRadius READ stickRadius WRITE setStickRadius NOTIFY stickRadiusChanged)
			Q_PROPERTY(float surfaceProbeRadius READ surfaceProbeRadius WRITE setSurfaceProbeRadius NOTIFY surfaceProbeRadiusChanged)
			Q_PROPERTY(float cartoonTubeRadius READ cartoonTubeRadius WRITE setCartoonTubeRadius NOTIFY cartoonTubeRadiusChanged)

			public:
				explicit ModelController(Representation* rep = nullptr,
				                         QObject* parent = nullptr);
				~ModelController() override;

				Representation* representation() const { return rep_; }
				void setRepresentation(Representation* rep);

				int modelType() const        { return model_type_; }
				int drawingMode() const      { return drawing_mode_; }
				int drawingPrecision() const { return drawing_precision_; }
				int transparency() const     { return transparency_; }
				// 999.59-02 — re-entrancy state now lives in the base depth
				// counter (applying_depth_); isApplying() reflects it for the
				// existing Inspector/diagnostic callers and the §11b tests.
				bool isApplying() const      { return applying_depth_ > 0; }

				float ballRadius() const          { return ball_radius_; }
				float ballStickBondRadius() const { return ball_stick_bond_radius_; }
				bool  dashedBonds() const         { return dashed_bonds_; }
				float stickRadius() const         { return stick_radius_; }
				float surfaceProbeRadius() const  { return surface_probe_radius_; }
				float cartoonTubeRadius() const   { return cartoon_tube_radius_; }

				/**
				 * Apply the staged model settings to the attached
				 * Representation, command-shaped per §2. Returns true when the
				 * Representation was mutated, false when dropped (re-entry,
				 * applying_depth_ > 0) or rejected (no Representation / busy).
				 * Overrides Controller::apply(). Inspector sections call this
				 * directly (controller_->apply()) — the bool is the resync
				 * signal; it is NOT a string-based Qt slot, so the bool return
				 * is binding-surface-compatible with the prior void apply().
				 * @return true if the owner was mutated, false otherwise.
				 */
				bool apply() override;

				/**
				 * Reset the controller's mirror to the owner's current state
				 * (the 999.64 single-event reset path). Overrides
				 * Controller::reset(); delegates to revert(), which pulls the
				 * Representation's live model/drawing/transparency state back
				 * into the mirror and emits the change notifications.
				 */
				void reset() override;

			public Q_SLOTS:
				void revert();

				void setModelType(int t);
				void setDrawingMode(int m);
				void setDrawingPrecision(int p);
				void setTransparency(int t);

				void setBallRadius(float v);
				void setBallStickBondRadius(float v);
				void setDashedBonds(bool v);
				void setStickRadius(float v);
				void setSurfaceProbeRadius(float v);
				void setCartoonTubeRadius(float v);

			Q_SIGNALS:
				void modelTypeChanged(int t);
				void drawingModeChanged(int m);
				void drawingPrecisionChanged(int p);
				void transparencyChanged(int t);
				void ballRadiusChanged(float v);
				void ballStickBondRadiusChanged(float v);
				void dashedBondsChanged(bool v);
				void stickRadiusChanged(float v);
				void surfaceProbeRadiusChanged(float v);
				void cartoonTubeRadiusChanged(float v);
				void appliedStub();

			protected:
				/**
				 * §2a declared invalidation — display-list REBUILD.
				 * Overrides Controller::invalidateDeclared_() to call
				 * representation_->update(true). Model is the ONE
				 * Representation-domain controller that rebuilds: a
				 * model-processor swap (e.g. Stick → Surface) changes the
				 * primitive set itself, so a soft refresh (update(false))
				 * would re-draw stale geometry. NEVER a blanket full-scene
				 * rebuild (the v1.7.x-13 freeze).
				 */
				void invalidateDeclared_() override;

			private:
				/**
				 * The §13-cookbook mutation body: builds the model processor
				 * from the mirrored fields and writes the model/drawing/
				 * transparency state onto rep_. Called between the §2 guard
				 * and the emit step. Returns true when it mutated rep_.
				 * Precondition: rep_ != nullptr (apply() guarantees it).
				 */
				bool applyInternal_();

				/**
				 * Record the reversible-intent payload (§2 step 7). v1.7.4 is
				 * capture-only — store the last payload; v2.0 pushes it onto
				 * the UndoStack here.
				 */

				Representation* rep_;
				int model_type_;
				int drawing_mode_;
				int drawing_precision_;
				int transparency_;
				float ball_radius_;
				float ball_stick_bond_radius_;
				bool  dashed_bonds_;
				float stick_radius_;
				float surface_probe_radius_;
				float cartoon_tube_radius_;
				bool  params_dirty_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_KERNEL_CONTROLLERS_MODELCONTROLLER_H
