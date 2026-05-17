// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.43 — BALLView Refresh: LabeledSlider shared widget.
//
// Composite control: [QSlider | QSpinBox | unit-suffix label |
// reset button]. Slider and spinbox are bi-directionally synced.
// Reset returns to the default value supplied at construction.
//

#ifndef BALL_VIEW_WIDGETS_LABELEDSLIDER_H
#define BALL_VIEW_WIDGETS_LABELEDSLIDER_H

#ifndef BALL_COMMON_GLOBAL_
# include <BALL/COMMON/global.h>
#endif

#include <QtCore/QString>
#include <QtWidgets/QWidget>

class QLabel;
class QSlider;
class QSpinBox;
class QToolButton;

namespace BALL
{
	namespace VIEW
	{
		/**
		 * Composite slider with synchronized numeric input and reset
		 * button. Used dozens of places across the BALLView dialogs
		 * (line widths, opacities, sampling counts, etc.).
		 *
		 * `valueChanged(int)` fires exactly once per logical change —
		 * the internal slider↔spinbox sync is guarded against
		 * feedback loops.
		 *
		 * QSS hooks: labeledSlider (root), labeledSliderSlider,
		 * labeledSliderSpin, labeledSliderUnits, labeledSliderReset.
		 */
		class BALL_VIEW_EXPORT LabeledSlider : public QWidget
		{
			Q_OBJECT

			public:
				LabeledSlider(int minimum, int maximum, int defaultValue,
				              const QString& units = QString(),
				              QWidget* parent = nullptr);
				~LabeledSlider() override;

				int value() const;
				int minimum() const;
				int maximum() const;
				int defaultValue() const { return default_value_; }
				QString units() const;

				/** Show/hide the reset button (visible by default). */
				void setResetVisible(bool visible);

			public Q_SLOTS:
				void setValue(int v);
				void reset();

			Q_SIGNALS:
				void valueChanged(int v);

			private Q_SLOTS:
				void onSliderChanged_(int v);
				void onSpinChanged_(int v);

			private:
				int default_value_;
				bool syncing_;             // re-entry guard for slider↔spinbox loop
				QSlider* slider_;
				QSpinBox* spin_;
				QLabel* units_label_;
				QToolButton* reset_button_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_WIDGETS_LABELEDSLIDER_H
