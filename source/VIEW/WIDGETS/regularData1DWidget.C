// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: regularData1DWidget.C,v 1.2 2003/08/26 16:07:03 amoll Exp $
//

#include <BALL/VIEW/WIDGETS/regularData1DWidget.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <qpointarray.h>
#include <qpainter.h>
#include <qwmatrix.h>

namespace BALL
{
	namespace VIEW
	{

		UpdateRegularData1DMessage::UpdateRegularData1DMessage()
			: Message(),
				data_(0)
		{
		}

		UpdateRegularData1DMessage::UpdateRegularData1DMessage(const UpdateRegularData1DMessage& message)
			: Message(message),
				data_(message.data_)
		{
		}

		UpdateRegularData1DMessage::~UpdateRegularData1DMessage()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
			Log.error() << "Destructing object " << (void *)this
									<< " of class " << RTTI::getName<UpdateRegularData1DMessage>() << std::endl;
			#endif
		}

		RegularData1DWidget::RegularData1DWidget(RegularData1D* data, QWidget *parent) 
			throw()
			: QScrollView(parent, "1D Diagram"), 
				ModularWidget("RegularData1DWidget"), 
				layout_(viewport(),2,2),
				pm_(),
				diagram_(viewport()),
				x_label_(viewport()),
				y_label_(viewport()),
				dummy_label_(viewport()),
				data_(data),
				width_(200),
				height_(200),
				background_color_(QColor(white)),
				diagram_color_(QColor(red)),
				line_style_(true)
		{
			setCaption("RegularData1DWidget");
			ModularWidget::registerWidget(this);

			layout_.addWidget(&y_label_, 0, 0);
			layout_.addWidget(&diagram_, 0, 1);
			layout_.addWidget(&dummy_label_, 1, 0);
			layout_.addWidget(&x_label_, 1, 1);
			layout_.setColStretch(1, 5);
			layout_.setRowStretch(0, 5);

			x_label_.setAlignment(AlignRight);
			y_label_.setAlignment(AlignTop);

			diagram_.resize(width_,height_);
			diagram_.setScaledContents(true);  
			diagram_.setFrameShape(QFrame::Box);  
			diagram_.setLineWidth(1);
		}

		RegularData1DWidget::~RegularData1DWidget()
			throw()
		{
		}

		void RegularData1DWidget::initializeWidget(MainControl& main_control)
		{
			main_control.insertMenuEntry(MainControl::TOOLS, "&1D-NMR", this, SLOT(createPlot()));
		}

		void RegularData1DWidget::finalizeWidget(MainControl& main_control)
		{
			main_control.removeMenuEntry(MainControl::TOOLS, "&1D-NMR", this, SLOT(createPlot()));
		}

		void RegularData1DWidget::onNotify(Message* message)
			throw()
		{
			if ( data_ == 0 ||
					!RTTI::isKindOf<UpdateRegularData1DMessage>(*message) ||
					RTTI::castTo<UpdateRegularData1DMessage>(*message)->getData() != data_)
			{
				return;
			}

			createPlot();
		}

		const RegularData1DWidget& RegularData1DWidget::operator = (const RegularData1DWidget &widget)
			throw()
		{
			data_ = widget.data_;
			return *this;
		}

		void RegularData1DWidget::clear()
			throw()
		{
			data_ = 0;
		}

		bool RegularData1DWidget::operator == (const RegularData1DWidget &widget) const
			throw()
		{
			return (data_ == widget.data_);
		}

		void RegularData1DWidget::createPlot()
		{
			if (data_->size() == 0) return;
		Log.error() << "createPlot " << data_->size() << std::endl;
			x_label_.setBackgroundColor(background_color_);
			y_label_.setBackgroundColor(background_color_);
			diagram_.setBackgroundColor(background_color_);
			dummy_label_.setBackgroundColor(background_color_);
			setPaletteBackgroundColor(background_color_);

			//pm_.resize(width_, height_);
			pm_.resize(data_->size(), height_);
			pm_.fill(background_color_);
			resizeContents(data_->size() + 20, height_ + 30);
			resize(width_, height_+50);

			QPainter p( &pm_ );
			p.setPen(diagram_color_);
			for (Size i = 0; i < data_->size(); i++)
			{
				if (line_style_)
				{
					p.drawLine(i,height_-(Size)(*data_)[i], i, height_);
				}
				else
				{
					p.drawPoint(i,(Position)(*data_)[i]);
				}
			}

			Log.error() <<std::endl << (*data_)[width_/2] << std::endl; 
		//  p.drawLine(1,height_-1,width_-1, height_-1);
		//  p.drawLine(1,1,1, height_-1);

			p.end();
			diagram_.setPixmap(pm_);
			x_label_.setText(x_axis_text_.c_str());
			y_label_.setText(y_axis_text_.c_str());
			repaint();
		}

		void RegularData1DWidget::setXAxisText(const String& text)
			throw()
		{
			x_axis_text_ = text;
		}

		void RegularData1DWidget::setYAxisText(const String& text)
			throw()
		{
			y_axis_text_ = text;
		}

		const String& RegularData1DWidget::getXAxisText() const
			throw()
		{
			return x_axis_text_;
		}

		const String& RegularData1DWidget::getYAxisText() const
			throw()
		{
			return y_axis_text_;
		}

		void RegularData1DWidget::setBackgroundColor(const QColor& color)
			throw()
		{
			background_color_ = color;
		}

		const QColor& RegularData1DWidget::getBackgroundColor() const
			throw()
		{
			return background_color_;
		}

		void RegularData1DWidget::setDiagramColor(const QColor& color)
			throw()
		{
			diagram_color_ = color;
		}

		const QColor& RegularData1DWidget::getDiagramColor() const
			throw()
		{
			return diagram_color_;
		}

		void RegularData1DWidget::setLineStyle(bool state)
			throw()
		{
			line_style_ = state;
		}

		bool RegularData1DWidget::isLineStyle() const
			throw()
		{
			return line_style_;
		}

} // namespace VIEW
} // namespace BALL
