// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.43 — BALLView Refresh: SwatchButton implementation.
// Phase 999.48 — a11y: QAccessible name/description per Handover §8.2.
//

#include <BALL/VIEW/WIDGETS/swatchButton.h>

#include <QtGui/QMouseEvent>
#include <QtGui/QPainter>
#include <QtWidgets/QColorDialog>

namespace BALL
{
	namespace VIEW
	{

		SwatchButton::SwatchButton(const QColor& initial, QWidget* parent)
			: QAbstractButton(parent),
				color_(initial.isValid() ? initial : QColor(Qt::white)),
				dialog_enabled_(true)
		{
			setObjectName(QStringLiteral("swatchButton"));
			setCursor(Qt::PointingHandCursor);
			setFocusPolicy(Qt::StrongFocus);

			// Phase 999.48 §8.2 — a11y. QAbstractButton already exposes
			// QAccessible::Button via Qt's default factory; we just need
			// to keep the accessibleName in sync with the colour and add
			// a localized description for screen readers.
			setAccessibleName(tr("Color: %1").arg(color_.name(QColor::HexRgb)));
			setAccessibleDescription(tr("Color swatch. Activate to open the color picker."));
		}

		SwatchButton::SwatchButton(QWidget* parent)
			: SwatchButton(QColor(Qt::white), parent)
		{
		}

		SwatchButton::~SwatchButton() = default;

		QSize SwatchButton::sizeHint() const     { return QSize(48, 22); }
		QSize SwatchButton::minimumSizeHint() const { return QSize(28, 18); }

		void SwatchButton::setColor(const QColor& c)
		{
			if (!c.isValid()) return;
			if (c == color_) return;        // no spurious notifications
			color_ = c;
			// Phase 999.48 §8.2 — keep accessibleName synced with the
			// displayed colour so screen readers announce updates.
			setAccessibleName(tr("Color: %1").arg(color_.name(QColor::HexRgb)));
			update();
			Q_EMIT colorChanged(color_);
		}

		void SwatchButton::chooseColor()
		{
			QColor chosen = QColorDialog::getColor(color_, this, tr("Choose Colour"),
			                                       QColorDialog::ShowAlphaChannel);
			if (chosen.isValid())
			{
				setColor(chosen);
			}
		}

		void SwatchButton::paintEvent(QPaintEvent* /*e*/)
		{
			QPainter p(this);
			p.setRenderHint(QPainter::Antialiasing, true);

			QRectF r = QRectF(rect()).adjusted(0.5, 0.5, -0.5, -0.5);
			const qreal radius = 3.0;

			p.setBrush(color_);
			QColor border(0x80, 0x80, 0x80);
			if (hasFocus()) border = palette().highlight().color();
			p.setPen(QPen(border, 1.0));
			p.drawRoundedRect(r, radius, radius);
		}

		void SwatchButton::mousePressEvent(QMouseEvent* e)
		{
			QAbstractButton::mousePressEvent(e);
			if (e->button() == Qt::LeftButton && dialog_enabled_)
			{
				chooseColor();
			}
		}

	} // namespace VIEW
} // namespace BALL
