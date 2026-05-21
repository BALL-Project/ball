// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 04 — LabelController implementation.
//

#include <BALL/VIEW/KERNEL/controllers/labelController.h>


#include <BALL/VIEW/KERNEL/representation.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/VIEW/MODELS/labelModel.h>
#include <BALL/CONCEPT/composite.h>
#include <BALL/COMMON/logStream.h>

#include <QtGui/QFont>

#include <list>

namespace BALL
{
	namespace VIEW
	{

		LabelController::LabelController(Representation* rep, QObject* parent)
			: QObject(parent), rep_(rep), label_type_(0), font_size_(12)
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

		void LabelController::apply()
		{
			// UFG-11 cut-over (Phase 999.51-02) — the Inspector now hosts a
			// LabelSection, so apply() functionally creates a label
			// Representation via the SAME backend the legacy LabelDialog uses
			// (LabelDialog::accept): a MODEL_LABEL Representation carrying a
			// LabelModel, set against the current molecular selection and
			// inserted through MainControl. The legacy LabelDialog remains
			// reachable (Display > Create > Label) and drives the SAME backend
			// (deletion deferred to 999.53).
			MainControl* mc = MainControl::getInstance(0);
			if (mc == nullptr)
			{
				Log.warn() << "[LabelController::apply] no MainControl available — skipping." << std::endl;
				return;
			}

			// Busy guard — mirrors ModelController/ClippingController. Defer
			// rather than racing a scene mutation while another job runs.
			if (mc->isBusy())
			{
				Log.info() << "[LabelController::apply] MainControl busy — deferring." << std::endl;
				return;
			}

			// A label targets the current molecular SELECTION (action-like,
			// not a per-Representation live edit). With no selection there is
			// nothing to label — return gracefully (do NOT crash), mirroring
			// LabelDialog::accept's empty-selection early return.
			std::list<Composite*> selection = mc->getMolecularControlSelection();
			if (selection.empty())
			{
				Log.info() << "[LabelController::apply] no molecular selection — nothing to label." << std::endl;
				return;
			}

			// Build the label Representation exactly as LabelDialog::accept does.
			Representation* rep = new Representation;
			rep->setProperty(Representation::PROPERTY__ALWAYS_FRONT);
			rep->setModelType(MODEL_LABEL);

			LabelModel* model = new LabelModel;
			model->setText(ascii(label_text_));

			// Map the mirrored int labelType_ back to the LabelModel::Mode
			// enum. The Inspector combobox items are ordered to match the
			// enum (0=ALL_ITEMS, 1=ALL_ATOMS, 2=ALL_RESIDUES, 3=ONE_LABEL).
			switch (label_type_)
			{
				case 1:  model->setMode(LabelModel::ALL_ATOMS);    break;
				case 2:  model->setMode(LabelModel::ALL_RESIDUES); break;
				case 3:  model->setMode(LabelModel::ONE_LABEL);    break;
				case 0:
				default: model->setMode(LabelModel::ALL_ITEMS);    break;
			}

			// Font from the mirrored point size (mirror LabelDialog's font_;
			// a default family is fine — the legacy default is Helvetica 12).
			QFont font(QStringLiteral("Helvetica"), font_size_);
			model->setFont(font);

			// Color uses the LabelModel default (the controller has no color
			// property; the legacy dialog seeds custom_color_ from INI).

			rep->setModelProcessor(model);

			// Process all objects in the selection list (mirror
			// LabelDialog::accept lines 162-170).
			std::list<Composite*>::const_iterator list_it = selection.begin();
			std::list<const Composite*> composites;
			for (; list_it != selection.end(); ++list_it)
			{
				composites.push_back(*list_it);
			}
			rep->setComposites(composites);

			mc->insert(*rep);
			mc->update(*rep);

			Q_EMIT appliedStub();
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

