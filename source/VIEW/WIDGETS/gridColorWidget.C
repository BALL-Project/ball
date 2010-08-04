#include <BALL/VIEW/WIDGETS/gridColorWidget.h>

#include <BALL/VIEW/DATATYPE/colorRGBA.h>

#include <QtGui/QDoubleValidator>

namespace BALL
{
	namespace VIEW
	{

		BALL::VIEW::GridColorWidget::GridColorWidget(QWidget* parent)
			: QWidget(parent)
		{
			ui.setupUi(this);

			ui.min_min_button->setColor(QColor(255, 0, 0));
			ui.min_button->setColor(QColor(255, 0, 0));
			ui.mid_button->setColor(QColor(255, 255, 255));
			ui.max_button->setColor(QColor(0, 0, 255));
			ui.max_max_button->setColor(QColor(0, 0, 255));

			QDoubleValidator* val = new QDoubleValidator(this);
			ui.min_box->setValidator(val);
			ui.mid_box->setValidator(val);
			ui.max_box->setValidator(val);

			connect(ui.autoscale, SIGNAL(clicked()), this, SIGNAL(autoScaleRequested()));
		}

		//Min, Mid and Max
		void GridColorWidget::setMinValue(float minValue)
		{
			ui.min_box->setText(QString::number(minValue));
		}

		float GridColorWidget::getMinValue() const
		{
			return ui.min_box->text().toFloat();
		}

		void GridColorWidget::setMidValue(float midValue)
		{
			ui.mid_box->setText(QString::number(midValue));
		}

		float GridColorWidget::getMidValue() const
		{
			return ui.mid_box->text().toFloat();
		}

		void GridColorWidget::setMaxValue(float maxValue)
		{
			ui.max_box->setText(QString::number(maxValue));
		}

		float GridColorWidget::getMaxValue() const
		{
			return ui.max_box->text().toFloat();
		}

		//Transparency
		void GridColorWidget::setTransparencyEnabled(bool state)
		{
			ui.grid_transparency->setChecked(state);
		}

		bool GridColorWidget::isTransparencyEnabled() const
		{
			return ui.grid_transparency->isChecked();
		}

		void GridColorWidget::setNormalizationEnabled(bool state)
		{
			ui.normalization->setChecked(state);
		}

		bool GridColorWidget::isNormalizationEnabled() const
		{
			return ui.normalization->isChecked();
		}

		ColorRGBA GridColorWidget::getMinMinColor() const
		{
			return ui.min_min_button->getColor();
		}

		ColorRGBA GridColorWidget::getMinColor() const
		{
			return ui.min_button->getColor();
		}

		ColorRGBA GridColorWidget::getMidColor() const
		{
			return ui.mid_button->getColor();
		}

		ColorRGBA GridColorWidget::getMaxColor() const
		{
			return ui.max_button->getColor();
		}

		ColorRGBA GridColorWidget::getMaxMaxColor() const
		{
			return ui.max_max_button->getColor();
		}

		int GridColorWidget::getNumLevels() const
		{
			return ui.levels_box->value();
		}

		void GridColorWidget::transparencyToggled(bool enabled)
		{
			ui.min_min_alpha->setEnabled(enabled);
			ui.min_alpha->setEnabled(enabled);
			ui.mid_alpha->setEnabled(enabled);
			ui.max_alpha->setEnabled(enabled);
			ui.max_max_alpha->setEnabled(enabled);

			ui.min_min_button->setEnableAlpha(enabled);
			ui.min_button->setEnableAlpha(enabled);
			ui.mid_button->setEnableAlpha(enabled);
			ui.max_button->setEnableAlpha(enabled);
			ui.max_max_button->setEnableAlpha(enabled);

			if(enabled) {
				setTransparency_(ui.min_min_alpha->value(), ui.min_min_button);
				setTransparency_(ui.min_alpha->value(), ui.min_button);
				setTransparency_(ui.mid_alpha->value(), ui.mid_button);
				setTransparency_(ui.max_alpha->value(), ui.max_button);
				setTransparency_(ui.max_max_alpha->value(), ui.max_max_button);
			}
		}

		void GridColorWidget::normalizationToggled(bool enabled)
		{
			if(enabled)
			{
				setMinValue(0.0f);
				setMidValue(0.5f);
				setMaxValue(1.0f);
			}
		}

		void GridColorWidget::validateText(QString str)
		{
			if(str == "")
			{
				QLineEdit* s = qobject_cast<QLineEdit*>(sender());
				if(s)
				{
					s->setText(QString::number(0.0f));
				}
			}
		}

		void GridColorWidget::minMinColorChanged(QColor color)
		{
			setTransparency_(color, ui.min_min_alpha);
		}

		void GridColorWidget::minColorChanged(QColor color)
		{
			setTransparency_(color, ui.min_alpha);
		}

		void GridColorWidget::midColorChanged(QColor color)
		{
			setTransparency_(color, ui.mid_alpha);
		}

		void GridColorWidget::maxColorChanged(QColor color)
		{
			setTransparency_(color, ui.max_alpha);
		}

		void GridColorWidget::maxMaxColorChanged(QColor color)
		{
			setTransparency_(color, ui.max_max_alpha);
		}

		void GridColorWidget::minMinAlphaChanged(int value)
		{
			setTransparency_(value, ui.min_min_button);
		}

		void GridColorWidget::minAlphaChanged(int value)
		{
			setTransparency_(value, ui.min_button);
		}

		void GridColorWidget::midAlphaChanged(int value)
		{
			setTransparency_(value, ui.mid_button);
		}

		void GridColorWidget::maxAlphaChanged(int value)
		{
			setTransparency_(value, ui.max_button);
		}

		void GridColorWidget::maxMaxAlphaChanged(int value)
		{
			setTransparency_(value, ui.max_max_button);
		}

		void GridColorWidget::setTransparency_(int value, ColorButton* button)
		{
			if(isTransparencyEnabled())
			{
				QColor tmp = button->getColor();
				tmp.setAlpha(value);
				button->setColor(tmp);
			}
		}

		void GridColorWidget::setTransparency_(const QColor& color, QSpinBox* box)
		{
			if(isTransparencyEnabled())
			{
				box->setValue(color.alpha());
			}
		}

	}
}
