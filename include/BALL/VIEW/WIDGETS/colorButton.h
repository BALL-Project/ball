#ifndef BALL_VIEW_WIDGETS_COLORBUTTON_H
#define BALL_VIEW_WIDGETS_COLORBUTTON_H

#ifndef BALL_COMMON_GLOBAL_
# include <BALL/COMMON/global.h>
#endif

#include <QtWidgets/QPushButton>
#include <QtGui/QColor>

class QPaintEvent;

namespace BALL
{
	namespace VIEW
	{
		class ColorRGBA;

		class BALL_VIEW_EXPORT ColorButton : public QPushButton
		{
			Q_OBJECT
			Q_PROPERTY (QColor color_ READ getColor WRITE setColor USER true)
			Q_PROPERTY (QColor default_color_ READ getDefaultColor WRITE setDefaultColor)

			public:
				explicit ColorButton(QWidget* parent);
				explicit ColorButton(const QColor& color, QWidget* parent);
				explicit ColorButton(const QColor& color, const QColor& default_color, QWidget* parent);
				explicit ColorButton(const ColorRGBA& color, QWidget* parent);
				explicit ColorButton(const ColorRGBA& color, const ColorRGBA& defaultColor, QWidget* parent);

				virtual ~ColorButton();

				void setColor(const QColor& color);
				void setColor(const ColorRGBA& color);
				const QColor& getColor() const;

				void setDefaultColor(const QColor& color);
				void setDefaultColor(const ColorRGBA& color);
				const QColor& getDefaultColor() const;

				void setEnableAlpha(bool enable);
				bool isAlphaEnabled() const;

				QSize sizeHint() const;

			public slots:
				void chooseColor();

			signals:
				void colorChanged(QColor color);

			protected:
				QColor color_;
				QColor default_color_;

				bool enable_alpha_;

				virtual void paintEvent(QPaintEvent* evt);

			private:
				void setup_();
		};
	}
}

#endif //BALL_VIEW_WIDGETS_COLORBUTTON_H
