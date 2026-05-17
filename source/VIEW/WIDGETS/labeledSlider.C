// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.43 — BALLView Refresh: LabeledSlider implementation.
// Phase 999.48 — a11y: QAccessible name/description per Handover §8.2.
//

#include <BALL/VIEW/WIDGETS/labeledSlider.h>
#include <BALL/VIEW/KERNEL/theme/iconRegistry.h>

#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QToolButton>

namespace BALL
{
	namespace VIEW
	{

		LabeledSlider::LabeledSlider(int minimum, int maximum, int defaultValue,
		                             const QString& units, QWidget* parent)
			: QWidget(parent),
				default_value_(defaultValue),
				syncing_(false),
				slider_(nullptr),
				spin_(nullptr),
				units_label_(nullptr),
				reset_button_(nullptr)
		{
			setObjectName(QStringLiteral("labeledSlider"));

			slider_ = new QSlider(Qt::Horizontal, this);
			slider_->setObjectName(QStringLiteral("labeledSliderSlider"));
			slider_->setRange(minimum, maximum);
			slider_->setValue(defaultValue);

			spin_ = new QSpinBox(this);
			spin_->setObjectName(QStringLiteral("labeledSliderSpin"));
			spin_->setRange(minimum, maximum);
			spin_->setValue(defaultValue);
			spin_->setButtonSymbols(QAbstractSpinBox::PlusMinus);

			units_label_ = new QLabel(units, this);
			units_label_->setObjectName(QStringLiteral("labeledSliderUnits"));
			units_label_->setVisible(!units.isEmpty());

			reset_button_ = new QToolButton(this);
			reset_button_->setObjectName(QStringLiteral("labeledSliderReset"));
			reset_button_->setAutoRaise(true);
			reset_button_->setFocusPolicy(Qt::NoFocus);
			reset_button_->setToolTip(tr("Reset to default"));
			reset_button_->setIconSize(QSize(14, 14));
			QIcon reset_icon = Icons::get("actions/reset");
			if (reset_icon.isNull())
			{
				// Fallback glyph if the SVG isn't bundled (OFF path).
				reset_button_->setText(QStringLiteral("↺"));
			}
			else
			{
				reset_button_->setIcon(reset_icon);
			}

			QHBoxLayout* row = new QHBoxLayout(this);
			row->setContentsMargins(0, 0, 0, 0);
			row->setSpacing(8);
			row->addWidget(slider_, /*stretch=*/1);
			row->addWidget(spin_);
			row->addWidget(units_label_);
			row->addWidget(reset_button_);

			connect(slider_, &QSlider::valueChanged, this, &LabeledSlider::onSliderChanged_);
			connect(spin_, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
			        this, &LabeledSlider::onSpinChanged_);
			connect(reset_button_, &QToolButton::clicked, this, &LabeledSlider::reset);

			// Phase 999.48 §8.2 — a11y. QSlider already exposes
			// QAccessible::Slider via Qt's default factory; for the
			// composite container we mirror the units string into the
			// accessibleName so screen readers announce e.g. "Opacity, 50%".
			// Subclasses (in calling code) override accessibleName per
			// instance via setAccessibleName().
			if (!units.isEmpty())
			{
				slider_->setAccessibleName(units);
				spin_->setAccessibleName(units);
			}
		}

		LabeledSlider::~LabeledSlider() = default;

		int LabeledSlider::value() const { return slider_ ? slider_->value() : 0; }
		int LabeledSlider::minimum() const { return slider_ ? slider_->minimum() : 0; }
		int LabeledSlider::maximum() const { return slider_ ? slider_->maximum() : 0; }
		QString LabeledSlider::units() const { return units_label_ ? units_label_->text() : QString(); }

		void LabeledSlider::setResetVisible(bool visible)
		{
			if (reset_button_) reset_button_->setVisible(visible);
		}

		void LabeledSlider::setValue(int v)
		{
			if (!slider_) return;
			// Clamp to widget range so external callers can't push the
			// internal widgets into inconsistent state.
			int clamped = qBound(slider_->minimum(), v, slider_->maximum());
			if (clamped == slider_->value()) return;
			syncing_ = true;
			slider_->setValue(clamped);
			spin_->setValue(clamped);
			syncing_ = false;
			Q_EMIT valueChanged(clamped);
		}

		void LabeledSlider::reset()
		{
			setValue(default_value_);
		}

		void LabeledSlider::onSliderChanged_(int v)
		{
			if (syncing_) return;
			syncing_ = true;
			spin_->setValue(v);
			syncing_ = false;
			Q_EMIT valueChanged(v);
		}

		void LabeledSlider::onSpinChanged_(int v)
		{
			if (syncing_) return;
			syncing_ = true;
			slider_->setValue(v);
			syncing_ = false;
			Q_EMIT valueChanged(v);
		}

	} // namespace VIEW
} // namespace BALL
