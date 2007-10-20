#ifndef BALL_VIEW_WIDGETS_SDWIDGET_H
#define BALL_VIEW_WIDGETS_SDWIDGET_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#include <BALL/KERNEL/system.h>
#include <QtGui/qwidget.h>

namespace BALL
{
	namespace VIEW
	{

		///
		class SDWidget 
			: public QWidget
		{
			public:

				///
				SDWidget(QWidget *parent = 0);

				///
				SDWidget(const System& system, QWidget *parent = 0, bool resize_to_parent = true);

				///
				void plot(const System& system, bool clear = true, bool create_sd = true);

				///
				System& getSystem() {return system_;};
				
				///
				const System& getSystem() const {return system_;};

				///
				void setSystem(const System& system) {system_ = system;};

				///
				void clear();

			protected:
				void paintEvent(QPaintEvent *event);

				QPointF getEndpoint_(QRectF& character_boundary, QPointF from, QPointF to);
				System system_;

				bool resize_to_parent_;
				bool clear_;
		};

	}
}

#endif
