// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 04 — LabelController (read-only mirror).
//
// Source dialog: source/VIEW/DIALOGS/labelDialog.C (~227 LOC).
//

#ifndef BALL_VIEW_KERNEL_CONTROLLERS_LABELCONTROLLER_H
#define BALL_VIEW_KERNEL_CONTROLLERS_LABELCONTROLLER_H


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
// 999.59-03 per-controller contract-fixture activation symbol (Codex MEDIUM
// #7) — flips label_contract_test's parity block live (see
// test/contract/contractFixtureActivation.h). Defined ONLY because apply()
// below returns bool. §2a declared invalidation: SOFT refresh (a label change
// re-draws existing geometry; the primitive set is unchanged).
// ---------------------------------------------------------------------------
#define BALL_VIEW_LABEL_APPLY_BOOL 1

namespace BALL
{
	namespace VIEW
	{
		class Representation;

		/**
		 * Per-Representation Label settings (text content + font +
		 * placement). Cut over to the §2 `bool apply()` command contract in
		 * 999.59-03 (declared §2a invalidation: SOFT refresh — a label change
		 * leaves the primitive set intact).
		 */
		class BALL_VIEW_EXPORT LabelController : public Controller
		{
			Q_OBJECT

			Q_PROPERTY(QString labelText READ labelText WRITE setLabelText NOTIFY labelTextChanged)
			Q_PROPERTY(int labelType READ labelType WRITE setLabelType NOTIFY labelTypeChanged)
			Q_PROPERTY(int fontSize READ fontSize WRITE setFontSize NOTIFY fontSizeChanged)

			public:
				explicit LabelController(Representation* rep = nullptr,
				                         QObject* parent = nullptr);
				~LabelController() override;

				Representation* representation() const { return rep_; }
				void setRepresentation(Representation* rep);

				QString labelText() const { return label_text_; }
				int labelType() const     { return label_type_; }
				int fontSize() const      { return font_size_; }

				/// 999.59-03 — true while apply() is mutating (base depth counter).
				bool isApplying() const { return applying_depth_ > 0; }

				/**
				 * Build + insert the label Representation for the current
				 * molecular selection, command-shaped per §2. Returns true when
				 * a label was built, false when dropped (re-entry) or rejected
				 * (no MainControl / busy / empty selection). Overrides
				 * Controller::apply(); called directly (controller_->apply()).
				 * @return true if a label was built, false otherwise.
				 */
				bool apply() override;

				/**
				 * Reset the mirror to the owner's current state (999.64 reset
				 * path). Overrides Controller::reset(); delegates to revert().
				 */
				void reset() override;

			public Q_SLOTS:
				void revert();

				void setLabelText(const QString& t);
				void setLabelType(int t);
				void setFontSize(int s);

			Q_SIGNALS:
				void labelTextChanged(const QString& t);
				void labelTypeChanged(int t);
				void fontSizeChanged(int s);
				/// §2 step 5 — the ONE typed event apply() emits. Replaces the
				/// 999.44 migration-window stub signal (removed in the
				/// 999.59-03 cut-over).
				void applied();

			protected:
				/**
				 * §2a declared invalidation — SOFT refresh: a label build
				 * re-draws via MainControl::update(rep) without rebuilding the
				 * full scene. Overrides Controller::invalidateDeclared_().
				 */
				void invalidateDeclared_() override;

			private:
				/// §13-cookbook mutation body, moved out of apply(): builds the
				/// MODEL_LABEL Representation against the current selection and
				/// inserts it through MainControl. Sets last_built_rep_ so
				/// invalidateDeclared_() can refresh it. Precondition (a
				/// MainControl + non-empty selection) guaranteed by apply().
				bool applyInternal_();

				/// Capture-only reversible intent (§2 step 7; v2.0 UndoStack).
				void recordIntent_(const ApplyPayload& payload);

				Representation* rep_;
				QString label_text_;
				int     label_type_;
				int     font_size_;

				// The Representation built by the most recent applyInternal_();
				// invalidateDeclared_() refreshes it. Non-owning (MainControl
				// owns inserted Representations).
				Representation* last_built_rep_ = nullptr;

				ApplyPayload last_payload_;  // 999.59-03 — capture-only reversible intent.
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_KERNEL_CONTROLLERS_LABELCONTROLLER_H
