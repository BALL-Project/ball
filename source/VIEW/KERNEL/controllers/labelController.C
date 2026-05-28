// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 04 — LabelController implementation.
//

#include <BALL/VIEW/KERNEL/controllers/labelController.h>


#include <BALL/VIEW/KERNEL/representation.h>
#include <BALL/VIEW/MODELS/representationBuilder.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/VIEW/MODELS/labelModel.h>
#include <BALL/VIEW/KERNEL/controllers/controllerApplyGuard.h>
#include <BALL/CONCEPT/composite.h>
#include <BALL/COMMON/logStream.h>
#include <BALL/CONCEPT/timeStamp.h>

#include <QtGui/QFont>

#include <list>
#include <sstream>

namespace BALL
{
	namespace VIEW
	{

		namespace
		{
			// Map the mirrored int labelType to the LabelModel::Mode enum. The
			// Inspector combobox items are ordered to match the enum
			// (0=ALL_ITEMS, 1=ALL_ATOMS, 2=ALL_RESIDUES, 3=ONE_LABEL).
			LabelModel::Mode labelModeFromInt_(int label_type)
			{
				switch (label_type)
				{
					case 1:  return LabelModel::ALL_ATOMS;
					case 2:  return LabelModel::ALL_RESIDUES;
					case 3:  return LabelModel::ONE_LABEL;
					case 0:
					default: return LabelModel::ALL_ITEMS;
				}
			}

			LabelModel* buildLabelModel_(const QString& text, int label_type, int font_size)
			{
				LabelModel* model = new LabelModel;
				model->setText(ascii(text));
				model->setMode(labelModeFromInt_(label_type));
				// Font from the mirrored point size (mirror LabelDialog's font_;
				// the legacy default is Helvetica 12).
				QFont font(QStringLiteral("Helvetica"), font_size);
				model->setFont(font);
				return model;
			}

			// Compact owner-state snapshot for the ApplyPayload (§2 step 3).
			String snapshotLabelState_(Representation* rep, const QString& text, int label_type)
			{
				std::ostringstream s;
				s << "text=" << ascii(text)
				  << ";type=" << label_type
				  << ";model=" << (rep != nullptr ? static_cast<int>(rep->getModelType()) : -1);
				return String(s.str());
			}
		}

		LabelController::LabelController(Representation* rep, QObject* parent)
			: Controller(parent), rep_(rep), label_type_(0), font_size_(12)
		{
		}

		LabelController::~LabelController() = default;

		void LabelController::setRepresentation(Representation* rep)
		{
			if (rep_ == rep) return;
			rep_ = rep;
			revert();
		}

		void LabelController::revert() { /* read-only mirror — defaults */ }

		bool LabelController::apply()
		{
			// 999.59-03 cut-over to the §2 `bool apply()` command contract.
			// Two owner paths, both command-shaped:
			//   - per-Representation: when a Representation is attached (the
			//     Inspector LabelSection / contract fixture path), the label
			//     model is written directly onto that rep (headless-testable,
			//     no MainControl needed).
			//   - selection-action: with no attached rep but a MainControl +
			//     molecular selection, build + insert a MODEL_LABEL
			//     Representation against the selection — the SAME backend the
			//     legacy LabelDialog::accept uses (legacy dialog still reachable).

			MainControl* mc = MainControl::getInstance(0);

			// ── 1. Preconditions (§2 step 1) ─────────────────────────────────
			if (mc != nullptr && mc->isBusy())
			{
				Log.info() << "[LabelController::apply] MainControl busy — deferring." << std::endl;
				return false;                    // rejected — busy
			}

			// The selection-action path needs a MainControl AND a non-empty
			// molecular selection; the per-rep path needs only an attached rep.
			if (rep_ == nullptr)
			{
				if (mc == nullptr)
				{
					Log.warn() << "[LabelController::apply] no Representation and no MainControl — skipping." << std::endl;
					return false;                // rejected — nothing to mutate
				}
				std::list<Composite*> selection = mc->getMolecularControlSelection();
				if (selection.empty())
				{
					Log.info() << "[LabelController::apply] no Representation and no molecular selection — nothing to label." << std::endl;
					return false;                // rejected — nothing to label
				}
			}

			// ── 2. Re-entrancy shield (§2 step 2 / §4) — nest-aware drop ─────
			if (applying_depth_ > 0) return false;          // dropped — re-entry
			ControllerApplyGuard apply_guard(applying_depth_);

			// ── 3. Capture reversible intent (§2 step 3) ─────────────────────
			ApplyPayload payload;
			payload.command_id = "label.setText";
			payload.before     = snapshotLabelState_(rep_, label_text_, label_type_);
			payload.target     = rep_;
			payload.t_us       = PreciseTime::now().getMicroSeconds();

			// ── 4. Mutate the single owner (§2 step 4) ───────────────────────
			applyInternal_();

			payload.after = snapshotLabelState_(rep_, label_text_, label_type_);

			// ── 5. Emit ONE typed event (§2 step 5) ──────────────────────────
			Q_EMIT applied();

			// ── 6. Request the DECLARED invalidation (§2 step 6 / §2a) ───────
			invalidateDeclared_();               // soft refresh

			// ── 7. Record reversible intent (§2 step 7) — capture only ───────
			recordIntent_(payload);

			return true;                          // mutated
		}

		bool LabelController::applyInternal_()
		{
			// §13 cookbook step 2 — mutation body, moved out of apply().
			// Precondition (rep_ != nullptr) OR (MainControl + selection)
			// guaranteed by apply().
			if (rep_ != nullptr)
			{
				// Per-Representation path: write the label model onto the
				// attached rep directly. Headless-testable (no MainControl).
				rep_->setProperty(Representation::PROPERTY__ALWAYS_FRONT);
				// §3c — Representation mutation flows through RepresentationBuilder.
				RepresentationBuilder::setModelType(*rep_, MODEL_LABEL);
				RepresentationBuilder::setModelProcessor(*rep_, buildLabelModel_(label_text_, label_type_, font_size_));
				last_built_rep_ = rep_;
				return true;
			}

			// Selection-action path: build + insert a label Representation
			// exactly as LabelDialog::accept does.
			MainControl* mc = MainControl::getInstance(0);
			std::list<Composite*> selection = mc->getMolecularControlSelection();

			Representation* rep = new Representation;
			rep->setProperty(Representation::PROPERTY__ALWAYS_FRONT);
			// §3c — Representation mutation flows through RepresentationBuilder.
			RepresentationBuilder::setModelType(*rep, MODEL_LABEL);
			RepresentationBuilder::setModelProcessor(*rep, buildLabelModel_(label_text_, label_type_, font_size_));

			// Process all objects in the selection list (mirror
			// LabelDialog::accept lines 162-170).
			std::list<Composite*>::const_iterator list_it = selection.begin();
			std::list<const Composite*> composites;
			for (; list_it != selection.end(); ++list_it)
			{
				composites.push_back(*list_it);
			}
			RepresentationBuilder::setComposites(*rep, composites);

			mc->insert(*rep);
			last_built_rep_ = rep;
			return true;
		}

		void LabelController::invalidateDeclared_()
		{
			// §2a Label row — SOFT refresh. A label build re-draws via
			// MainControl::update(rep) without rebuilding the full scene; the
			// primitive set of other reps is unchanged. Headless (no
			// MainControl) there is nothing to refresh.
			MainControl* mc = MainControl::getInstance(0);
			if (mc != nullptr && last_built_rep_ != nullptr)
			{
				mc->update(*last_built_rep_);
			}
		}

		void LabelController::reset()
		{
			// §2 reset path (999.64 consumes) — single-pass re-sync from owner.
			revert();
		}

		void LabelController::recordIntent_(const ApplyPayload& payload)
		{
			// v1.7.4 — capture only (no UndoStack yet, §2 step 7).
			last_payload_ = payload;
		}

		void LabelController::setLabelText(const QString& t)
		{
			if (t == label_text_) return;
			label_text_ = t;
			Q_EMIT labelTextChanged(t);
		}

		void LabelController::setLabelType(int t)
		{
			if (t == label_type_) return;
			label_type_ = t;
			Q_EMIT labelTypeChanged(t);
		}

		void LabelController::setFontSize(int s)
		{
			if (s == font_size_) return;
			font_size_ = s;
			Q_EMIT fontSizeChanged(s);
		}

	} // namespace VIEW
} // namespace BALL

