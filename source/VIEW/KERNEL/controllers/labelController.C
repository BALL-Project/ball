// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 04 — LabelController implementation.
//

#include <BALL/VIEW/KERNEL/controllers/labelController.h>


#include <BALL/COMMON/logStream.h>

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
			Log.info() << "[LabelController::apply] STUB — legacy LabelDialog "
				"owns mutation. text='" << label_text_.toUtf8().constData()
				<< "' type=" << label_type_
				<< " fontSize=" << font_size_ << std::endl;
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

