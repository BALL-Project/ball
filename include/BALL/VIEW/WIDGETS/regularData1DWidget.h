// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: regularData1DWidget.h,v 1.3 2003/09/04 23:14:15 amoll Exp $
//

#ifndef BALL_VIEW_WIDGET_REGULARDATA1DWIDGET_H
#define BALL_VIEW_WIDGET_REGULARDATA1DWIDGET_H

#ifndef BALL_VIEW_KERNEL_MESSAGE_H
# include <BALL/VIEW/KERNEL/message.h>
#endif

#ifndef BALL_VIEW_KERNEL_MODULARWIDGET_H
# include <BALL/VIEW/KERNEL/modularWidget.h>
#endif

#ifndef BALL_DATATYPE_REGULARDATA1D_H
# include <BALL/DATATYPE/regularData1D.h>
#endif

#include <qscrollview.h>
#include <qpixmap.h>
#include <qlabel.h>
#include <qlayout.h>

namespace BALL
{
	namespace VIEW
	{
		class MainControl;

		/**  New RegularData1D Message Classic.
				\ingroup ViewWidgets
		 */
		class UpdateRegularData1DMessage 
			: public Message
		{
		 public:
			 
			/**  @name Constructors and Destructors
			 */
			//@{
			
			///
			UpdateRegularData1DMessage();

			///
			UpdateRegularData1DMessage(const UpdateRegularData1DMessage& message);

			///
			virtual ~UpdateRegularData1DMessage()
				throw();

			///
			void setData(RegularData1D* data)
				throw() { data_ = data;}

			///
			RegularData1D* getData() 
				throw() { return data_;}

		 protected:

			RegularData1D* data_;
			
			//@}
		};


		/** Widget used to display one-dimensional data.
		 \ingroup ViewGuiWidgets
		 */
		class RegularData1DWidget
			: public QScrollView,
				public ModularWidget
		{
			Q_OBJECT
				
			public:
				
			BALL_EMBEDDABLE(RegularData1DWidget)

			/** Constructors and Destructors
			 */
			//@{
			
			/// Detailed constructor
			RegularData1DWidget(RegularData1D* data, QWidget* parent = 0)
				throw();

			/// Copy constructor
			RegularData1DWidget(const RegularData1DWidget& widget)
				throw();

			/// Destructor
			virtual ~RegularData1DWidget()
				throw();
			
			//@}
			/** Assignment
			 */
			//@{

			/// Assignment operator
			const RegularData1DWidget& operator = (const RegularData1DWidget &widget)
				throw();

			/// Clear method
			virtual void clear()
				throw();

			void setData(RegularData1D* data)
			{ 
				data_ = data;
			}
			
			//@}
			/** @name Predicates
			 */
			//@{

			/** Equality operator. 
					Two spectra are equal by definition if they point to the same data.
			 */
			bool operator == (const RegularData1DWidget &widget) const
				throw();

			//@}

			void onNotify(Message *message)
				throw();

			public slots:
			
			/** @name ModularWidget related methods.
			 */
			//@{
			
			/** Setup the menu entries.
					RegularData1DWidget creates an entry in Tools|RegularData1DWidget and connects
					the entry to createPlot()
			*/
			virtual void initializeWidget(MainControl& main_control);

			/** Remove menu entries.
			 */
			virtual void finalizeWidget(MainControl& main_control);
			
			///
			void setXAxisText(const String& text)
				throw();
			
			///
			void setYAxisText(const String& text)
				throw();

			///
			const String& getXAxisText() const
				throw();

			///
			const String& getYAxisText() const
				throw();

			///
			void setBackgroundColor(const QColor& color)
				throw();

			///
			const QColor& getBackgroundColor() const
				throw();
			
			///
			void setDiagramColor(const QColor& color)
				throw();

			///
			const QColor& getDiagramColor() const
				throw();

			///
			void setLineStyle(bool state)
				throw();

			///
			bool isLineStyle() const
				throw();

			//@}

			public slots:

			///
			void createPlot();

		 protected:
			
			QGridLayout layout_;
			QPixmap pm_;
			QLabel diagram_;
			QLabel x_label_;
			QLabel y_label_;
			QLabel dummy_label_;
			RegularData1D* data_;
			Size width_, height_;
			String x_axis_text_, y_axis_text_;
			QColor background_color_;
			QColor diagram_color_;
			bool line_style_;
		};
	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_WIDGET_REGULARDATA1DWIDGET_H
