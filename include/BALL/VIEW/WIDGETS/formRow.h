// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.43 — BALLView Refresh: FormRow shared widget.
//
// Row helper that lays out a label + control in the standard
// design-system column geometry. Optional inline hint below the
// control. Replaces ad-hoc QHBoxLayout(label, control) patterns
// scattered through preferences dialogs.
//

#ifndef BALL_VIEW_WIDGETS_FORMROW_H
#define BALL_VIEW_WIDGETS_FORMROW_H

#ifndef BALL_COMMON_GLOBAL_
# include <BALL/COMMON/global.h>
#endif

#include <QtCore/QString>
#include <QtWidgets/QWidget>

class QLabel;
class QVBoxLayout;

namespace BALL
{
	namespace VIEW
	{
		/**
		 * Form-row layout primitive: [label | control] horizontally,
		 * with an optional hint line below. Label column has a fixed
		 * min-width (140 px) and right-aligned text; the control fills
		 * the remaining horizontal space.
		 *
		 * The control widget is reparented to this FormRow on
		 * construction (the FormRow takes ownership in the Qt
		 * parent-child sense).
		 *
		 * QSS objectNames: `formRow` (root), `formRowLabel`,
		 * `formRowHint`.
		 */
		class BALL_VIEW_EXPORT FormRow : public QWidget
		{
			Q_OBJECT

			public:
				/** Default label-column min-width in device-independent pixels.
				 * UFG-21 (rc4 validation) — dropped 140→90 so the value
				 * column has room for slider + spin + units + reset
				 * glyph at the InspectorDock's 280px minimum width. At
				 * 90px the label still fits "Transparency" / "Specular"
				 * / "Shininess" / "Ambient" etc. without ellipsis.
				 */
				static constexpr int LABEL_MIN_WIDTH = 90;

				/**
				 * Build a row. `control` must be non-null and is reparented
				 * to this FormRow. If `control` is null the row renders only
				 * the label (still valid; supports section-divider rows).
				 */
				FormRow(const QString& label, QWidget* control, QWidget* parent = nullptr);
				~FormRow() override;

				/** Accessor for the label widget (for further QSS tweaks). */
				QLabel* labelWidget() const { return label_; }

				/** Accessor for the control widget (may be null). */
				QWidget* control() const { return control_; }

				/**
				 * Set/clear the optional hint line below the row. Passing
				 * an empty string removes the hint widget.
				 */
				void setHint(const QString& hint);
				void clearHint();

				/** Current hint text (empty if no hint). */
				QString hint() const;

			private:
				QLabel* label_;
				QWidget* control_;
				QLabel* hint_label_;       // lazily created when setHint() is first called
				QVBoxLayout* root_layout_; // outer vertical; row + optional hint

				void ensureHintLabel_();
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_WIDGETS_FORMROW_H
