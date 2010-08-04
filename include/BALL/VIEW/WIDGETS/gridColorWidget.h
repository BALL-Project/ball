#ifndef BALL_VIEW_WIDGETS_GRIDCOLORWIDGET_H
#define BALL_VIEW_WIDGETS_GRIDCOLORWIDGET_H

#include <BALL/VIEW/UIC/ui_gridColorWidget.h>

namespace BALL
{
	namespace VIEW
	{
		class GridColorWidget : public QWidget
		{
			Q_OBJECT

			public:
				GridColorWidget(QWidget* parent);

				//Accessors
				void setMinValue(float minValue);
				float getMinValue() const;

				void setMidValue(float midValue);
				float getMidValue() const;

				void setMaxValue(float maxValue);
				float getMaxValue() const;

				void setTransparencyEnabled(bool enabled);
				bool isTransparencyEnabled() const;

				void setNormalizationEnabled(bool enabled);
				bool isNormalizationEnabled() const;

				ColorRGBA getMinMinColor() const;
				ColorRGBA getMinColor() const;
				ColorRGBA getMidColor() const;
				ColorRGBA getMaxColor() const;
				ColorRGBA getMaxMaxColor() const;

				int getNumLevels() const;

			signals:
				void autoScaleRequested();

			protected slots:
				void transparencyToggled(bool enabled);
				void normalizationToggled(bool enabled);
				void validateText(QString str);

				void minMinColorChanged(QColor color);
				void minColorChanged(QColor color);
				void midColorChanged(QColor color);
				void maxColorChanged(QColor color);
				void maxMaxColorChanged(QColor color);

				void minMinAlphaChanged(int value);
				void minAlphaChanged(int value);
				void midAlphaChanged(int value);
				void maxAlphaChanged(int value);
				void maxMaxAlphaChanged(int value);

			protected:
				void setTransparency_(int value, ColorButton* button);
				void setTransparency_(const QColor& color, QSpinBox* box);
				Ui::GridColorWidget ui;
		};

	}
}

#endif //BALL_VIEW_WIDGETS_GRIDCOLORWIDGET_H
