#include <BALL/VIEW/WIDGETS/colorButton.h>
#include <BALL/VIEW/DATATYPE/colorRGBA.h>

#include <QtGui/QPaintEvent>
#include <QtWidgets>
#include <QtGui/QPainter>
#include <QtWidgets/QStyle>
#include <QtWidgets/QStyleOption>
#include <QtWidgets/QApplication>

namespace BALL
{
	namespace VIEW
	{

		ColorButton::ColorButton(QWidget* parent)
			: QPushButton(parent),
				enable_alpha_(false)
		{
			setup_();
		}

		ColorButton::ColorButton(const QColor& color, QWidget* parent)
			: QPushButton(parent),
			  color_(color),
			  default_color_(color),
				enable_alpha_(false)
		{
			setup_();
		}

		ColorButton::ColorButton(const QColor& color, const QColor& default_color, QWidget* parent)
			: QPushButton(parent),
			  color_(color),
				default_color_(default_color),
				enable_alpha_(false)
		{
			setup_();
		}

		ColorButton::ColorButton(const ColorRGBA& color, QWidget* parent)
			: QPushButton(parent),
			  color_(color.getQColor()),
			  default_color_(color.getQColor()),
				enable_alpha_(false)
		{
			setup_();
		}

		ColorButton::ColorButton(const ColorRGBA& color, const ColorRGBA& default_color, QWidget* parent)
			: QPushButton(parent),
			  color_(color.getQColor()),
				default_color_(default_color.getQColor()),
				enable_alpha_(false)
		{
			setup_();
		}

		ColorButton::~ColorButton()
		{
			setup_();
		}

		void ColorButton::setup_()
		{
			connect(this, SIGNAL(clicked()), this, SLOT(chooseColor()));
		}

		void ColorButton::chooseColor()
		{
			QColor tmp;
			if(enable_alpha_) {
				tmp = QColorDialog::getColor(color_, this, tr("Select color"), QColorDialog::ShowAlphaChannel);
			} else {
				tmp = QColorDialog::getColor(color_, this, tr("Choose color"));
			}

			if(tmp.isValid()) {
				color_ = tmp;
				emit colorChanged(color_);
			}
		}

		void ColorButton::setEnableAlpha(bool enable)
		{
			if(enable != enable_alpha_) {
				setColor(color_.rgb());
				enable_alpha_ = enable;
			}
		}

		bool ColorButton::isAlphaEnabled() const
		{
			return enable_alpha_;
		}

		void ColorButton::setColor(const QColor& color)
		{
			color_ = color;
			update();
		}

		void ColorButton::setColor(const ColorRGBA& color)
		{
			color_ = color.getQColor();
		}

		const QColor& ColorButton::getColor() const
		{
			return color_;
		}

		void ColorButton::setDefaultColor(const QColor& color)
		{
			default_color_ = color;
		}

		void ColorButton::setDefaultColor(const ColorRGBA& color)
		{
			default_color_ = color.getQColor();
		}

		const QColor& ColorButton::getDefaultColor() const
		{
			return default_color_;
		}

		//Stolen from KDE (KColorButton)
		void ColorButton::paintEvent( QPaintEvent* )
		{
			QPainter painter(this);

			//First, we need to draw the bevel.
			QStyleOptionButton butOpt;
			initStyleOption(&butOpt);
			style()->drawControl( QStyle::CE_PushButtonBevel, &butOpt, &painter, this );

			//OK, now we can muck around with drawing out pretty little color box
			//First, sort out where it goes
			QRect labelRect = style()->subElementRect( QStyle::SE_PushButtonContents,
					&butOpt, this );
			int shift = style()->pixelMetric( QStyle::PM_ButtonMargin );
			labelRect.adjust(shift, shift, -shift, -shift);
			int x, y, w, h;
			labelRect.getRect(&x, &y, &w, &h);

			if (isChecked() || isDown()) {
				x += style()->pixelMetric( QStyle::PM_ButtonShiftHorizontal );
				y += style()->pixelMetric( QStyle::PM_ButtonShiftVertical   );
			}

			QColor fillCol = isEnabled() ? color_ : palette().color(backgroundRole());
			qDrawShadePanel( &painter, x, y, w, h, palette(), true, 1, NULL);
			if ( fillCol.isValid() )
				painter.fillRect( x+1, y+1, w-2, h-2, fillCol );

			if ( hasFocus() ) {
				QRect focusRect = style()->subElementRect( QStyle::SE_PushButtonFocusRect, &butOpt, this );
				QStyleOptionFocusRect focusOpt;
				focusOpt.init(this);
				focusOpt.rect            = focusRect;
				focusOpt.backgroundColor = palette().background().color();
				style()->drawPrimitive( QStyle::PE_FrameFocusRect, &focusOpt, &painter, this );
			}
		}

		//Stolen from KDE (KColorButton)
		QSize ColorButton::sizeHint() const
		{
				QStyleOptionButton opt;
				initStyleOption(&opt);
				return style()->sizeFromContents(QStyle::CT_PushButton, &opt, QSize(40, 15), this).
						expandedTo(QApplication::globalStrut());
		}

	}
}
