// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.43 — BALLView Refresh: SwatchButton shared widget.
//
// Color-swatch button. Click → QColorDialog → if accepted, the
// swatch updates and `colorChanged(QColor)` fires. Drawn as a
// rounded-rect filled with the current colour plus a 1px hairline
// border in mid-grey; QSS-targetable.
//

#ifndef BALL_VIEW_WIDGETS_SWATCHBUTTON_H
#define BALL_VIEW_WIDGETS_SWATCHBUTTON_H

#ifndef BALL_COMMON_GLOBAL_
# include <BALL/COMMON/global.h>
#endif

#include <QtGui/QColor>
#include <QtWidgets/QAbstractButton>

namespace BALL
{
	namespace VIEW
	{
		/**
		 * Color swatch button — replacement for ColorButton that
		 * respects theme contrast (does NOT bake the colour into a
		 * QPalette so dark/light theming inherits cleanly).
		 *
		 * QSS objectName: `swatchButton`.
		 */
		class BALL_VIEW_EXPORT SwatchButton : public QAbstractButton
		{
			Q_OBJECT
			Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged USER true)

			public:
				explicit SwatchButton(const QColor& initial, QWidget* parent = nullptr);
				explicit SwatchButton(QWidget* parent = nullptr);
				~SwatchButton() override;

				QColor color() const { return color_; }

				QSize sizeHint() const override;
				QSize minimumSizeHint() const override;

				/** Whether clicks open the QColorDialog (default true). */
				void setDialogEnabled(bool enabled) { dialog_enabled_ = enabled; }
				bool isDialogEnabled() const { return dialog_enabled_; }

			public Q_SLOTS:
				void setColor(const QColor& c);
				void chooseColor();

			Q_SIGNALS:
				void colorChanged(QColor color);

			protected:
				void paintEvent(QPaintEvent* e) override;
				void mousePressEvent(QMouseEvent* e) override;

			private:
				QColor color_;
				bool dialog_enabled_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_WIDGETS_SWATCHBUTTON_H
